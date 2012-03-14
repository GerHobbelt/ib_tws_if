
// Copyright (c) 2011 Ger Hobbelt
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

/*
 * Mongoose-based server app which interfaces with a TWS station and displays data in HTML form,
 * suitable for loading by, for example, 64-bit Excel 2010, using web queries.
 */



#include "tws_comm_thread.h"

#include "tws_stocklist_loading_thread.h"
#include "tws_database_io.h"






#define TWS_CONNECT_RETRY_DELAY     10 // unit: seconds




static void process_one_queued_tier2_request(struct my_tws_io_info *info);


/*
replace TWSAPI debug printf call.
*/
void tws_debug_printf(void *opaque, const char *fmt, ...)
{
	struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;
	va_list ap;

	va_start(ap, fmt);
	mg_vlog((info ? info->conn : NULL), "debug", fmt, ap);
	va_end(ap);
}


void init_tws_thread_exch(struct tws_thread_exch **ptr_ref)
{
    struct tws_thread_exch *ptr = (struct tws_thread_exch *)calloc(1, sizeof(*ptr));

    pthread_mutex_init(&ptr->tws_queue_mutex, NULL);
    pthread_mutex_init(&ptr->tws_ib_send_mutex, NULL);
    pthread_mutex_init(&ptr->tws_rx_mutex, NULL);
    pthread_cond_init(&ptr->tws_rx_signal, NULL);
    pthread_mutex_init(&ptr->tws_tx_mutex, NULL);
    pthread_cond_init(&ptr->tws_tx_signal, NULL);

    // set up the queue:
    ptr->work_queue_size = 16;
    ptr->work_queue_fill = 0;
    ptr->work_queue_poppos = 0;
    ptr->work_queue_diane = 0;
    ptr->work_queue = (tier2_queue_item_t *)calloc(ptr->work_queue_size, sizeof(ptr->work_queue[0]));

    // init libxml, which is used to process the XML messages coming from TWS on various occassions:
    xmlInitParser();
    // xmlRegisterInputCallbacks(xmlTWSmatch, xmlTWSopen, xmlTWSread, xmlTWSclose);

    *ptr_ref = ptr;
}

void destroy_tws_thread_exch(struct tws_thread_exch **ptr_ref)
{
    if (*ptr_ref)
    {
        struct tws_thread_exch *ptr = *ptr_ref;
        size_t i;

        pthread_cond_destroy(&ptr->tws_rx_signal);
        pthread_cond_destroy(&ptr->tws_tx_signal);
        pthread_mutex_destroy(&ptr->tws_rx_mutex);
        pthread_mutex_destroy(&ptr->tws_tx_mutex);
        pthread_mutex_destroy(&ptr->tws_ib_send_mutex);
        pthread_mutex_destroy(&ptr->tws_queue_mutex);

        // TODO: signal all pending requests as ABORTED:
        for (i = 0; i < ptr->work_queue_size; i++)
        {
            tier2_queue_item_t *elem = ptr->work_queue + i;

			elem->request_state = TIER2_ABORTED;
			elem->exec_repeat_count = -1;

            // TODO: wait for the front-ends to recognize this change of affairs.
        }

        free(ptr->work_queue);
        free(ptr);

        xmlCleanupParser();
        xmlMemoryDump();

        *ptr_ref = NULL;
    }
}









static int tws_transmit_func(void *arg, const void *buf, unsigned int buflen)
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)arg;

    if (info->conn)
    {
        return mg_write(info->conn, buf, buflen);
    }
    return -1;
}

static int tws_receive_func(void *arg, void *buf, unsigned int max_bufsize)
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)arg;
    struct tws_thread_exch *exch = info->tws_cfg->exch;

    if (info->conn)
    {
        // check whether there's anything available:
        fd_set read_set;
        struct timeval tv;
        int max_fd;

        assert(exch != NULL);

        tv.tv_sec = info->tws_cfg->backend_poll_period / 1000;
        tv.tv_usec = (info->tws_cfg->backend_poll_period % 1000) * 1000;

        while (mg_get_stop_flag(mg_get_context(info->conn)) == 0)
        {
            struct timeval tv2 = tv;

            FD_ZERO(&read_set);
            max_fd = -1;

            // Add listening sockets to the read set
            mg_FD_SET(mg_get_client_socket(info->conn), &read_set, &max_fd);

            if (select(max_fd + 1, &read_set, NULL, NULL, &tv2) < 0)
            {
                break;
            }
            else
            {
                if (mg_FD_ISSET(mg_get_client_socket(info->conn), &read_set))
                {
                    /*
                    Mongoose mg_read() does NOT fetch any pending data from the TCP/IP stack when the 'content length' isn't set yet.

                    We, however, desire to load an unknown and arbitrary amount of data here to fill a buffer and our protocol doesn't
                    have somthing like a 'content length' to guide us along, so we'll have to use another method to make sure
                    the read operation actually delivers DATA!
                    */
                    // conn->content_len = MAX_INT;
                    break;
                }

                /*
                When there's no pending incoming data from TWS itself, we'll be running around in this loop while waiting for
                more data to arrive. Meanwhile, we can process queued requests from the front-end now:
                */
				process_one_queued_tier2_request(info);
            }
        }

        return mg_pull(info->conn, buf, max_bufsize);
        // return mg_read(info->conn, buf, max_bufsize);
    }
    return -1;
}

/* 'flush()' marks the end of the outgoing message: it should be transmitted ASAP */
static int tws_flush_func(void *arg)
{
    //struct my_tws_io_info *info = (struct my_tws_io_info *)arg;

    return 0;
}

/* open callback is invoked when tws_connect is invoked and no connection has been established yet (tws_connected() == false); return 0 on success; a twsclient_error_codes error code on failure. */
static int tws_open_func(void *arg)
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)arg;
    struct tws_conn_cfg *tws_cfg = info->tws_cfg;
    struct mg_context *ctx = info->ctx;
    struct mg_connection *conn = mg_connect_to_host(ctx, tws_cfg->ip_address, tws_cfg->port, 0);

    if (conn != NULL)
    {
		struct socket *sock = mg_get_client_socket(conn);

        // Disable Nagle - act a la telnet:
        mg_set_nodelay_mode(sock, 1);

		// enable keepalive + rx/tx timeouts:
		mg_set_socket_keepalive(sock, 1);
		mg_set_socket_timeout(sock, 10);
    }

    info->conn = conn;

    return (conn ? 0 : NOT_CONNECTED);
}


/* close callback is invoked on error or when tws_disconnect is invoked */
static int tws_close_func(void *arg)
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)arg;

    if (info->conn)
    {
        mg_close_connection(info->conn);

        free(info->conn);
        info->conn = NULL;
    }

    return 0;
}


static const char *tws_errcode2str(int errcode)
{
    const struct twsclient_errmsg *einfo = tws_strerror(errcode);

    return einfo->err_msg;
}






static __inline ham_u16_t strcost(const char *src)
{
	if (src)
	{
		return (ham_u16_t)strlen(src) + 1;
	}
	return 0;
}

static __inline void strserialize(char *dst, const char *src)
{
	if (src)
	{
		strcpy(dst, src);
	}
}

static ham_u16_t serialize_tws_contract_summary_phase1(trser_contract_summary_t *dst, ham_u16_t str_index, const tr_contract_t *src)
{
	int i;

	// serialize strings as offsets into 'tmp/dst' while determining the total bytes required for the real output:
	dst->has_c_undercomp = (src->c_undercomp != NULL);
	if (dst->has_c_undercomp)
	{
		under_comp_t *u = src->c_undercomp;

		dst->c_undercomp.u_conid = u->u_conid;
		dst->c_undercomp.u_delta = u->u_delta;
		dst->c_undercomp.u_price = u->u_price;
	}
    dst->c_strike = src->c_strike;
    
	dst->c_symbol = str_index;
	str_index += strcost(src->c_symbol);
    dst->c_sectype = str_index;
	str_index += strcost(src->c_sectype);
    dst->c_exchange = str_index;
	str_index += strcost(src->c_exchange);
    dst->c_primary_exch = str_index;
	str_index += strcost(src->c_primary_exch);
    dst->c_expiry = str_index;
	str_index += strcost(src->c_expiry);
    dst->c_currency = str_index;
	str_index += strcost(src->c_currency);
    dst->c_right = str_index;
	str_index += strcost(src->c_right);
    dst->c_local_symbol = str_index;
	str_index += strcost(src->c_local_symbol);
    dst->c_multiplier = str_index;
	str_index += strcost(src->c_multiplier);
    dst->c_combolegs_descrip = str_index;
	str_index += strcost(src->c_combolegs_descrip);
    dst->c_secid_type = str_index;
	str_index += strcost(src->c_secid_type);
    dst->c_secid = str_index;
	str_index += strcost(src->c_secid);

	dst->c_num_combolegs = src->c_num_combolegs;
	assert(src->c_num_combolegs <= ARRAY_SIZE(dst->c_comboleg));

	for (i = src->c_num_combolegs - 1; i >= 0; i--)
	{
		tr_comboleg_t *cls = src->c_comboleg + i;
		trser_comboleg_t *cld = dst->c_comboleg + i;

		cld->co_action = str_index;
		str_index += strcost(cls->co_action);
		cld->co_exchange = str_index;
		str_index += strcost(cls->co_exchange);
		cld->co_designated_location = str_index;
		str_index += strcost(cls->co_designated_location);

		cld->co_open_close = cls->co_open_close;
		cld->co_conid = cls->co_conid;
		cld->co_ratio = cls->co_ratio;
		cld->co_short_sale_slot = cls->co_short_sale_slot;
		cld->co_exempt_code = cls->co_exempt_code;
	}

	dst->c_conid = src->c_conid;
	dst->c_include_expired = src->c_include_expired;

	return str_index;
}

static void serialize_tws_contract_summary_phase2(char *dst_data, trser_contract_summary_t *dst, const tr_contract_t *src)
{
	int i;

	assert(src->c_num_combolegs <= ARRAY_SIZE(dst->c_comboleg));

	strserialize(dst_data + dst->c_symbol, src->c_symbol);
    strserialize(dst_data + dst->c_sectype, src->c_sectype);
    strserialize(dst_data + dst->c_exchange, src->c_exchange);
    strserialize(dst_data + dst->c_primary_exch, src->c_primary_exch);
    strserialize(dst_data + dst->c_expiry, src->c_expiry);
    strserialize(dst_data + dst->c_currency, src->c_currency);
    strserialize(dst_data + dst->c_right, src->c_right);
    strserialize(dst_data + dst->c_local_symbol, src->c_local_symbol);
    strserialize(dst_data + dst->c_multiplier, src->c_multiplier);
    strserialize(dst_data + dst->c_combolegs_descrip, src->c_combolegs_descrip);
    strserialize(dst_data + dst->c_secid_type, src->c_secid_type);
    strserialize(dst_data + dst->c_secid, src->c_secid);

	for (i = src->c_num_combolegs - 1; i >= 0; i--)
	{
		tr_comboleg_t *cls = src->c_comboleg + i;
		trser_comboleg_t *cld = dst->c_comboleg + i;

		strserialize(dst_data + cld->co_action, cls->co_action);
		strserialize(dst_data + cld->co_exchange, cls->co_exchange);
		strserialize(dst_data + cld->co_designated_location, cls->co_designated_location);
	}
}

trser_contract_t *serialize_tws_contract(const tr_contract_t *src)
{
	trser_contract_t *dst;
	trser_contract_summary_t tmp = {0};
	size_t cost = sizeof(*dst);
	ham_u16_t str_index;

	// serialize strings as offsets into 'tmp' while determining the total bytes required for the real output:
	if (src->c_num_combolegs > ARRAY_SIZE(tmp.c_comboleg))
		return NULL;

	str_index = serialize_tws_contract_summary_phase1(&tmp, 0, src);

	cost += str_index;
	dst = (trser_contract_t *)malloc(cost);
	if (!dst)
		return NULL;

	dst->c_contract = tmp;
	dst->struct_size = cost;

	serialize_tws_contract_summary_phase2(dst->data, &tmp, src);

	return dst;
}

trser_contract_details_t *serialize_tws_contract_details(const tr_contract_details_t *src)
{
	trser_contract_details_t *dst;
	trser_contract_details_t tmp = {0};
	size_t cost = sizeof(*dst);
	ham_u16_t str_index;

    tmp.d_mintick = src->d_mintick;
    tmp.d_coupon = src->d_coupon;

	if (src->d_summary.c_num_combolegs > ARRAY_SIZE(tmp.d_summary.c_comboleg))
		return NULL;

	str_index = serialize_tws_contract_summary_phase1(&tmp.d_summary, 0, &src->d_summary);

    tmp.d_market_name = str_index;
	str_index += strcost(src->d_market_name);
    tmp.d_trading_class = str_index;
	str_index += strcost(src->d_trading_class);
	tmp.d_order_types = str_index;
	str_index += strcost(src->d_order_types);
	tmp.d_valid_exchanges = str_index;
	str_index += strcost(src->d_valid_exchanges);
	tmp.d_cusip = str_index;
	str_index += strcost(src->d_cusip);
	tmp.d_maturity = str_index;
	str_index += strcost(src->d_maturity);
	tmp.d_issue_date = str_index;
	str_index += strcost(src->d_issue_date);
	tmp.d_ratings = str_index;
	str_index += strcost(src->d_ratings);
	tmp.d_bond_type = str_index;
	str_index += strcost(src->d_bond_type);
	tmp.d_coupon_type = str_index;
	str_index += strcost(src->d_coupon_type);
	tmp.d_desc_append = str_index;
	str_index += strcost(src->d_desc_append);
	tmp.d_next_option_date = str_index;
	str_index += strcost(src->d_next_option_date);
	tmp.d_next_option_type = str_index;
	str_index += strcost(src->d_next_option_type);
	tmp.d_notes = str_index;
	str_index += strcost(src->d_notes);
	tmp.d_long_name = str_index;
	str_index += strcost(src->d_long_name);
	tmp.d_contract_month = str_index;
	str_index += strcost(src->d_contract_month);
	tmp.d_industry = str_index;
	str_index += strcost(src->d_industry);
	tmp.d_category = str_index;
	str_index += strcost(src->d_category);
	tmp.d_subcategory = str_index;
	str_index += strcost(src->d_subcategory);
	tmp.d_timezone_id = str_index;
	str_index += strcost(src->d_timezone_id);
	tmp.d_trading_hours = str_index;
	str_index += strcost(src->d_trading_hours);
	tmp.d_liquid_hours = str_index;
	str_index += strcost(src->d_liquid_hours);
	
    tmp.d_price_magnifier = src->d_price_magnifier;
    tmp.d_under_conid = src->d_under_conid;
    tmp.d_convertible = src->d_convertible;
    tmp.d_callable = src->d_callable;
    tmp.d_putable = src->d_putable;
    tmp.d_next_option_partial = src->d_next_option_partial;



	cost += str_index;
	dst = (trser_contract_details_t *)malloc(cost);
	if (!dst)
		return NULL;

	*dst = tmp;
	dst->struct_size = cost;

	serialize_tws_contract_summary_phase2(dst->data, &tmp.d_summary, &src->d_summary);

    strserialize(dst->data + tmp.d_market_name, src->d_market_name);
    strserialize(dst->data + tmp.d_trading_class, src->d_trading_class);
	strserialize(dst->data + tmp.d_order_types, src->d_order_types);
	strserialize(dst->data + tmp.d_valid_exchanges, src->d_valid_exchanges);
	strserialize(dst->data + tmp.d_cusip, src->d_cusip);
	strserialize(dst->data + tmp.d_maturity, src->d_maturity);
	strserialize(dst->data + tmp.d_issue_date, src->d_issue_date);
	strserialize(dst->data + tmp.d_ratings, src->d_ratings);
	strserialize(dst->data + tmp.d_bond_type, src->d_bond_type);
	strserialize(dst->data + tmp.d_coupon_type, src->d_coupon_type);
	strserialize(dst->data + tmp.d_desc_append, src->d_desc_append);
	strserialize(dst->data + tmp.d_next_option_date, src->d_next_option_date);
	strserialize(dst->data + tmp.d_next_option_type, src->d_next_option_type);
	strserialize(dst->data + tmp.d_notes, src->d_notes);
	strserialize(dst->data + tmp.d_long_name, src->d_long_name);
	strserialize(dst->data + tmp.d_contract_month, src->d_contract_month);
	strserialize(dst->data + tmp.d_industry, src->d_industry);
	strserialize(dst->data + tmp.d_category, src->d_category);
	strserialize(dst->data + tmp.d_subcategory, src->d_subcategory);
	strserialize(dst->data + tmp.d_timezone_id, src->d_timezone_id);
	strserialize(dst->data + tmp.d_trading_hours, src->d_trading_hours);
	strserialize(dst->data + tmp.d_liquid_hours, src->d_liquid_hours);
	

	return dst;
}

trser_order_t *serialize_tws_order(const tr_order_t *src)
{
	trser_order_t *dst;
	trser_order_t tmp = {0};
	size_t cost = sizeof(*dst);
	ham_u16_t str_index = 0;
	int i;

    tmp.o_discretionary_amt = src->o_discretionary_amt;
    tmp.o_lmt_price = src->o_lmt_price;
    tmp.o_aux_price = src->o_aux_price;
    tmp.o_percent_offset = src->o_percent_offset;
    tmp.o_nbbo_price_cap = src->o_nbbo_price_cap;
    tmp.o_starting_price = src->o_starting_price;
    tmp.o_stock_ref_price = src->o_stock_ref_price;
    tmp.o_delta = src->o_delta;
    tmp.o_stock_range_lower = src->o_stock_range_lower;
    tmp.o_stock_range_upper = src->o_stock_range_upper;
    tmp.o_volatility = src->o_volatility;
    tmp.o_delta_neutral_aux_price = src->o_delta_neutral_aux_price;
    tmp.o_trail_stop_price = src->o_trail_stop_price;
	tmp.o_trailing_percent = src->o_trailing_percent;
    tmp.o_basis_points = src->o_basis_points;
    tmp.o_scale_price_increment = src->o_scale_price_increment;
	tmp.o_scale_price_adjust_value = src->o_scale_price_adjust_value;
	tmp.o_scale_profit_offset = src->o_scale_profit_offset;

    tmp.o_algo_strategy = str_index;
	str_index += strcost(src->o_algo_strategy);
	tmp.o_good_after_time = str_index;
	str_index += strcost(src->o_good_after_time);
	tmp.o_good_till_date = str_index;
	str_index += strcost(src->o_good_till_date);
	tmp.o_fagroup = str_index;
	str_index += strcost(src->o_fagroup);
	tmp.o_famethod = str_index;
	str_index += strcost(src->o_famethod);
	tmp.o_fapercentage = str_index;
	str_index += strcost(src->o_fapercentage);
	tmp.o_faprofile = str_index;
	str_index += strcost(src->o_faprofile);
	tmp.o_action = str_index;
	str_index += strcost(src->o_action);
	tmp.o_order_type = str_index;
	str_index += strcost(src->o_order_type);
	tmp.o_tif = str_index;
	str_index += strcost(src->o_tif);
	tmp.o_oca_group = str_index;
	str_index += strcost(src->o_oca_group);
	tmp.o_account = str_index;
	str_index += strcost(src->o_account);
	tmp.o_open_close = str_index;
	str_index += strcost(src->o_open_close);
	tmp.o_orderref = str_index;
	str_index += strcost(src->o_orderref);
	tmp.o_designated_location = str_index;
	str_index += strcost(src->o_designated_location);
	tmp.o_rule80a = str_index;
	str_index += strcost(src->o_rule80a);
	tmp.o_settling_firm = str_index;
	str_index += strcost(src->o_settling_firm);
	tmp.o_delta_neutral_order_type = str_index;
	str_index += strcost(src->o_delta_neutral_order_type);
	tmp.o_clearing_account = str_index;
	str_index += strcost(src->o_clearing_account);
	tmp.o_clearing_intent = str_index;
	str_index += strcost(src->o_clearing_intent);
	tmp.o_hedge_type = str_index;
	str_index += strcost(src->o_hedge_type);
	tmp.o_hedge_param = str_index;
	str_index += strcost(src->o_hedge_param);
	tmp.o_delta_neutral_settling_firm = str_index;
	str_index += strcost(src->o_delta_neutral_settling_firm);
	tmp.o_delta_neutral_clearing_account = str_index;
	str_index += strcost(src->o_delta_neutral_clearing_account);
	tmp.o_delta_neutral_clearing_intent = str_index;
	str_index += strcost(src->o_delta_neutral_clearing_intent);
	
	if (src->o_algo_params_count > ARRAY_SIZE(tmp.o_algo_params))
		return NULL;
	tmp.o_algo_params_count = src->o_algo_params_count;
	for (i = tmp.o_algo_params_count - 1; i >= 0; i--)
	{
		trser_tag_value_t *d = tmp.o_algo_params + i;
		tr_tag_value_t *s = src->o_algo_params + i;

	    d->t_tag = str_index;
		str_index += strcost(s->t_tag);
		d->t_val = str_index;
		str_index += strcost(s->t_val);
	}

	if (src->o_smart_combo_routing_params_count > ARRAY_SIZE(tmp.o_smart_combo_routing_params))
		return NULL;
	tmp.o_smart_combo_routing_params_count = src->o_smart_combo_routing_params_count;
	for (i = tmp.o_smart_combo_routing_params_count - 1; i >= 0; i--)
	{
		trser_tag_value_t *d = tmp.o_smart_combo_routing_params + i;
		tr_tag_value_t *s = src->o_smart_combo_routing_params + i;

		d->t_tag = str_index;
		str_index += strcost(s->t_tag);
		d->t_val = str_index;
		str_index += strcost(s->t_val);
	}

	if (src->o_combo_legs_count > ARRAY_SIZE(tmp.o_combo_legs))
		return NULL;
	tmp.o_combo_legs_count = src->o_combo_legs_count;
	for (i = tmp.o_combo_legs_count - 1; i >= 0; i--)
	{
		trser_order_comboleg_t *d = tmp.o_combo_legs + i;
		tr_order_combo_leg_t *s = src->o_combo_legs + i;

		d->cl_price = s->cl_price;
	}

	tmp.o_orderid = src->o_orderid;
    tmp.o_total_quantity = src->o_total_quantity;
    tmp.o_origin = src->o_origin;
    tmp.o_clientid = src->o_clientid;
    tmp.o_permid = src->o_permid;
    tmp.o_parentid = src->o_parentid;
    tmp.o_display_size = src->o_display_size;
    tmp.o_trigger_method = src->o_trigger_method;
    tmp.o_min_qty = src->o_min_qty;
    tmp.o_volatility_type = src->o_volatility_type;
    tmp.o_reference_price_type = src->o_reference_price_type;
    tmp.o_basis_points_type = src->o_basis_points_type;
    tmp.o_scale_subs_level_size = src->o_scale_subs_level_size;
    tmp.o_scale_init_level_size = src->o_scale_init_level_size;
	tmp.o_scale_price_adjust_interval = src->o_scale_price_adjust_interval;
	tmp.o_scale_init_position = src->o_scale_init_position;
	tmp.o_scale_init_fill_qty = src->o_scale_init_fill_qty;
    tmp.o_exempt_code = src->o_exempt_code;
	tmp.o_delta_neutral_con_id = src->o_delta_neutral_con_id;
    tmp.o_oca_type = src->o_oca_type;
    tmp.o_auction_strategy = src->o_auction_strategy;
    tmp.o_short_sale_slot = src->o_short_sale_slot;
    tmp.o_override_percentage_constraints = src->o_override_percentage_constraints;
    tmp.o_firm_quote_only = src->o_firm_quote_only;
    tmp.o_etrade_only = src->o_etrade_only;
    tmp.o_all_or_none = src->o_all_or_none;
    tmp.o_outside_rth = src->o_outside_rth;
    tmp.o_hidden = src->o_hidden;
    tmp.o_transmit = src->o_transmit;
    tmp.o_block_order = src->o_block_order;
    tmp.o_sweep_to_fill = src->o_sweep_to_fill;
    tmp.o_continuous_update = src->o_continuous_update;
    tmp.o_whatif = src->o_whatif;
    tmp.o_not_held = src->o_not_held;
	tmp.o_opt_out_smart_routing = src->o_opt_out_smart_routing;
	tmp.o_scale_auto_reset = src->o_scale_auto_reset;
	tmp.o_scale_random_percent = src->o_scale_random_percent;


	cost += str_index;
	dst = (trser_order_t *)malloc(cost);
	if (!dst)
		return NULL;

	*dst = tmp;
	dst->struct_size = cost;

    strserialize(dst->data + tmp.o_algo_strategy, src->o_algo_strategy);
	strserialize(dst->data + tmp.o_good_after_time, src->o_good_after_time);
	strserialize(dst->data + tmp.o_good_till_date, src->o_good_till_date);
	strserialize(dst->data + tmp.o_fagroup, src->o_fagroup);
	strserialize(dst->data + tmp.o_famethod, src->o_famethod);
	strserialize(dst->data + tmp.o_fapercentage, src->o_fapercentage);
	strserialize(dst->data + tmp.o_faprofile, src->o_faprofile);
	strserialize(dst->data + tmp.o_action, src->o_action);
	strserialize(dst->data + tmp.o_order_type, src->o_order_type);
	strserialize(dst->data + tmp.o_tif, src->o_tif);
	strserialize(dst->data + tmp.o_oca_group, src->o_oca_group);
	strserialize(dst->data + tmp.o_account, src->o_account);
	strserialize(dst->data + tmp.o_open_close, src->o_open_close);
	strserialize(dst->data + tmp.o_orderref, src->o_orderref);
	strserialize(dst->data + tmp.o_designated_location, src->o_designated_location);
	strserialize(dst->data + tmp.o_rule80a, src->o_rule80a);
	strserialize(dst->data + tmp.o_settling_firm, src->o_settling_firm);
	strserialize(dst->data + tmp.o_delta_neutral_order_type, src->o_delta_neutral_order_type);
	strserialize(dst->data + tmp.o_clearing_account, src->o_clearing_account);
	strserialize(dst->data + tmp.o_clearing_intent, src->o_clearing_intent);
	strserialize(dst->data + tmp.o_hedge_type, src->o_hedge_type);
	strserialize(dst->data + tmp.o_hedge_param, src->o_hedge_param);
	strserialize(dst->data + tmp.o_delta_neutral_settling_firm, src->o_delta_neutral_settling_firm);
	strserialize(dst->data + tmp.o_delta_neutral_clearing_account, src->o_delta_neutral_clearing_account);
	strserialize(dst->data + tmp.o_delta_neutral_clearing_intent, src->o_delta_neutral_clearing_intent);
	
	for (i = tmp.o_algo_params_count - 1; i >= 0; i--)
	{
		trser_tag_value_t *d = tmp.o_algo_params + i;
		tr_tag_value_t *s = src->o_algo_params + i;

	    strserialize(dst->data + d->t_tag, s->t_tag);
		strserialize(dst->data + d->t_val, s->t_val);
	}
    
	for (i = tmp.o_smart_combo_routing_params_count - 1; i >= 0; i--)
	{
		trser_tag_value_t *d = tmp.o_smart_combo_routing_params + i;
		tr_tag_value_t *s = src->o_smart_combo_routing_params + i;

		strserialize(dst->data + d->t_tag, s->t_tag);
		strserialize(dst->data + d->t_val, s->t_val);
	}

	return dst;
}

trser_order_status_t *serialize_tws_order_status(const tr_order_status_t *src)
{
	trser_order_status_t *dst;
	trser_order_status_t tmp = {0};
	size_t cost = sizeof(*dst);
	ham_u16_t str_index = 0;

    tmp.ost_commission = src->ost_commission;
    tmp.ost_min_commission = src->ost_min_commission;
    tmp.ost_max_commission = src->ost_max_commission;

    tmp.ost_status = str_index;
	str_index += strcost(src->ost_status);
	tmp.ost_init_margin = str_index;
	str_index += strcost(src->ost_init_margin);
	tmp.ost_maint_margin = str_index;
	str_index += strcost(src->ost_maint_margin);
	tmp.ost_equity_with_loan = str_index;
	str_index += strcost(src->ost_equity_with_loan);
	tmp.ost_commission_currency = str_index;
	str_index += strcost(src->ost_commission_currency);
	tmp.ost_warning_text = str_index;
	str_index += strcost(src->ost_warning_text);
	


	cost += str_index;
	dst = (trser_order_status_t *)malloc(cost);
	if (!dst)
		return NULL;

	*dst = tmp;
	dst->struct_size = cost;

    strserialize(dst->data + tmp.ost_status, src->ost_status);
	strserialize(dst->data + tmp.ost_init_margin, src->ost_init_margin);
	strserialize(dst->data + tmp.ost_maint_margin, src->ost_maint_margin);
	strserialize(dst->data + tmp.ost_equity_with_loan, src->ost_equity_with_loan);
	strserialize(dst->data + tmp.ost_commission_currency, src->ost_commission_currency);
	strserialize(dst->data + tmp.ost_warning_text, src->ost_warning_text);

	return dst;
}

trser_execution_t *serialize_tws_execution(const tr_execution_t *src)
{
	trser_execution_t *dst;
	trser_execution_t tmp = {0};
	size_t cost = sizeof(*dst);
	ham_u16_t str_index = 0;

    tmp.e_price = src->e_price;
    tmp.e_avg_price = src->e_avg_price;
    
	tmp.e_execid = str_index;
	str_index += strcost(src->e_execid);
	tmp.e_time = str_index;
	str_index += strcost(src->e_time);
	tmp.e_acct_number = str_index;
	str_index += strcost(src->e_acct_number);
	tmp.e_exchange = str_index;
	str_index += strcost(src->e_exchange);
	tmp.e_side = str_index;
	str_index += strcost(src->e_side);
	tmp.e_orderref = str_index;
	str_index += strcost(src->e_orderref);
	
	tmp.e_shares = src->e_shares;
    tmp.e_permid = src->e_permid;
    tmp.e_clientid = src->e_clientid;
    tmp.e_liquidation = src->e_liquidation;
    tmp.e_orderid = src->e_orderid;
    tmp.e_cum_qty = src->e_cum_qty;



	cost += str_index;
	dst = (trser_execution_t *)malloc(cost);
	if (!dst)
		return NULL;

	*dst = tmp;
	dst->struct_size = cost;

	strserialize(dst->data + tmp.e_execid, src->e_execid);
	strserialize(dst->data + tmp.e_time, src->e_time);
	strserialize(dst->data + tmp.e_acct_number, src->e_acct_number);
	strserialize(dst->data + tmp.e_exchange, src->e_exchange);
	strserialize(dst->data + tmp.e_side, src->e_side);
	strserialize(dst->data + tmp.e_orderref, src->e_orderref);
	
	return dst;
}

trser_exec_filter_t *serialize_tws_exec_filter(const tr_exec_filter_t *src)
{
	trser_exec_filter_t *dst;
	trser_exec_filter_t tmp = {0};
	size_t cost = sizeof(*dst);
	ham_u16_t str_index = 0;

    tmp.f_acctcode = str_index;
	str_index += strcost(src->f_acctcode);
	tmp.f_time = str_index;
	str_index += strcost(src->f_time);
	tmp.f_symbol = str_index;
	str_index += strcost(src->f_symbol);
	tmp.f_sectype = str_index;
	str_index += strcost(src->f_sectype);
	tmp.f_exchange = str_index;
	str_index += strcost(src->f_exchange);
	tmp.f_side = str_index;
	str_index += strcost(src->f_side);
	
    tmp.f_clientid = src->f_clientid;



	cost += str_index;
	dst = (trser_exec_filter_t *)malloc(cost);
	if (!dst)
		return NULL;

	*dst = tmp;
	dst->struct_size = cost;

    strserialize(dst->data + tmp.f_acctcode, src->f_acctcode);
	strserialize(dst->data + tmp.f_time, src->f_time);
	strserialize(dst->data + tmp.f_symbol, src->f_symbol);
	strserialize(dst->data + tmp.f_sectype, src->f_sectype);
	strserialize(dst->data + tmp.f_exchange, src->f_exchange);
	strserialize(dst->data + tmp.f_side, src->f_side);
	
	return dst;
}

trser_scanner_subscription_t *serialize_tws_scanner_subscription(const tr_scanner_subscription_t *src)
{
	trser_scanner_subscription_t *dst;
	trser_scanner_subscription_t tmp = {0};
	size_t cost = sizeof(*dst);
	ham_u16_t str_index = 0;

    tmp.scan_above_price = src->scan_above_price;
    tmp.scan_below_price = src->scan_below_price;
    tmp.scan_coupon_rate_above = src->scan_coupon_rate_above;
    tmp.scan_coupon_rate_below = src->scan_coupon_rate_below;
    tmp.scan_market_cap_above = src->scan_market_cap_above;
    tmp.scan_market_cap_below = src->scan_market_cap_below;
    
	tmp.scan_exclude_convertible = str_index;
	str_index += strcost(src->scan_exclude_convertible);
	tmp.scan_instrument = str_index;
	str_index += strcost(src->scan_instrument);
	tmp.scan_location_code = str_index;
	str_index += strcost(src->scan_location_code);
	tmp.scan_maturity_date_above = str_index;
	str_index += strcost(src->scan_maturity_date_above);
	tmp.scan_maturity_date_below = str_index;
	str_index += strcost(src->scan_maturity_date_below);
	tmp.scan_moody_rating_above = str_index;
	str_index += strcost(src->scan_moody_rating_above);
	tmp.scan_moody_rating_below = str_index;
	str_index += strcost(src->scan_moody_rating_below);
	tmp.scan_code = str_index;
	str_index += strcost(src->scan_code);
	tmp.scan_sp_rating_above = str_index;
	str_index += strcost(src->scan_sp_rating_above);
	tmp.scan_sp_rating_below = str_index;
	str_index += strcost(src->scan_sp_rating_below);
	tmp.scan_scanner_setting_pairs = str_index;
	str_index += strcost(src->scan_scanner_setting_pairs);
	tmp.scan_stock_type_filter = str_index;
	str_index += strcost(src->scan_stock_type_filter);
	
	tmp.scan_above_volume = src->scan_above_volume;
    tmp.scan_number_of_rows = src->scan_number_of_rows;
    tmp.scan_average_option_volume_above = src->scan_average_option_volume_above;



	cost += str_index;
	dst = (trser_scanner_subscription_t *)malloc(cost);
	if (!dst)
		return NULL;

	*dst = tmp;
	dst->struct_size = cost;

	strserialize(dst->data + tmp.scan_exclude_convertible, src->scan_exclude_convertible);
	strserialize(dst->data + tmp.scan_instrument, src->scan_instrument);
	strserialize(dst->data + tmp.scan_location_code, src->scan_location_code);
	strserialize(dst->data + tmp.scan_maturity_date_above, src->scan_maturity_date_above);
	strserialize(dst->data + tmp.scan_maturity_date_below, src->scan_maturity_date_below);
	strserialize(dst->data + tmp.scan_moody_rating_above, src->scan_moody_rating_above);
	strserialize(dst->data + tmp.scan_moody_rating_below, src->scan_moody_rating_below);
	strserialize(dst->data + tmp.scan_code, src->scan_code);
	strserialize(dst->data + tmp.scan_sp_rating_above, src->scan_sp_rating_above);
	strserialize(dst->data + tmp.scan_sp_rating_below, src->scan_sp_rating_below);
	strserialize(dst->data + tmp.scan_scanner_setting_pairs, src->scan_scanner_setting_pairs);
	strserialize(dst->data + tmp.scan_stock_type_filter, src->scan_stock_type_filter);
	
	return dst;
}




trser_commission_report_t *serialize_tws_commission_report(const tr_commission_report_t *src)
{
	trser_commission_report_t *dst;
	trser_commission_report_t tmp = {0};
	size_t cost = sizeof(*dst);
	ham_u16_t str_index = 0;

	tmp.cr_exec_id = str_index;
	str_index += strcost(src->cr_exec_id);
	tmp.cr_currency = str_index;
	str_index += strcost(src->cr_currency);

	tmp.cr_commission = src->cr_commission;
	tmp.cr_realized_pnl = src->cr_realized_pnl;
	tmp.cr_yield = src->cr_yield;
	tmp.cr_yield_redemption_date = src->cr_yield_redemption_date;
	


	cost += str_index;
	dst = (trser_commission_report_t *)malloc(cost);
	if (!dst)
		return NULL;

	*dst = tmp;
	dst->struct_size = cost;

	strserialize(dst->data + tmp.cr_exec_id, src->cr_exec_id);
	strserialize(dst->data + tmp.cr_currency, src->cr_currency);

	return dst;
}










static int queue_request(struct tws_conn_cfg *tws_cfg, const tier2_queue_item_t *cmd)
{
	struct tws_thread_exch *exch = tws_cfg->exch;
	struct tier2_queue_item *item;
	int i;

	for (i = (exch->work_queue_diane + 1) % exch->work_queue_size; 
		 i != exch->work_queue_diane; 
		 i = (i + 1) % exch->work_queue_size)
	{
		item = exch->work_queue + i;
		if (item->exec_repeat_count < 0
			|| (item->exec_repeat_count == 0
				&& item->request_state <= TIER2_MUST_EXEC_COMMAND))
		{
			goto got_a_hit;
		}
	}

	// when we get here it means the queue is full: enlarge the queue!
	i = exch->work_queue_size;
	exch->work_queue_size++;
	exch->work_queue = (tier2_queue_item_t *)realloc(exch->work_queue, exch->work_queue_size * sizeof(exch->work_queue[0]));
	item = exch->work_queue + i;

got_a_hit:
	exch->work_queue_diane = i; // (i + 1) % exch->work_queue_size;

	// make sure the exec count is at least 1:
	*item = *cmd;
	if (item->exec_repeat_count <= 0)
	{
		item->exec_repeat_count = 1;
	}
	exch->work_queue_fill++;
	item->request_state = TIER2_MUST_EXEC_COMMAND;

	return i;
}



int tier2_send_request(struct tws_conn_cfg *tws_cfg, const tier2_queue_item_t *cmd)
{
	struct tws_thread_exch *exch = tws_cfg->exch;
	struct my_tws_io_info *info = tws_cfg->tws_thread_info;

	assert(info);
	assert(exch);
	
	pthread_mutex_lock(&exch->tws_queue_mutex);

	{
		int i;
		struct tier2_queue_item *item;

		i = queue_request(tws_cfg, cmd);
		item = exch->work_queue + i;
		item->request_state = TIER2_NO_RESPONSE_YET;
	}

	pthread_mutex_unlock(&exch->tws_queue_mutex);

	switch (cmd->command_code)
	{
	default:
		assert(0);
		break;

	case TIER2_REQUEST_TWS_SCANNER_PARAMETERS:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_req_scanner_parameters(info->tws_handle);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_REQUEST_TWS_SCANNER_SUBSCRIPTION:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_req_scanner_subscription(info->tws_handle, cmd->command_params.scanner_subscription.ticker_id, cmd->command_params.scanner_subscription.s);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_CANCEL_TWS_SCANNER_SUBSCRIPTION:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_cancel_scanner_subscription(info->tws_handle, cmd->command_params.ticker_id);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_REQUEST_TWS_MKT_DATA:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_req_mkt_data(info->tws_handle, cmd->command_params.market_data.ticker_id, cmd->command_params.market_data.contract, cmd->command_params.market_data.generic_tick_list, cmd->command_params.market_data.snapshot);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_REQUEST_TWS_MARKET_DATA_TYPE:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_req_market_data_type(info->tws_handle, cmd->command_params.market_data_type);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_REQUEST_TWS_HISTORICAL_DATA:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_req_historical_data(info->tws_handle, cmd->command_params.historical_data.ticker_id, cmd->command_params.historical_data.contract, cmd->command_params.historical_data.end_date_time, cmd->command_params.historical_data.duration_str, cmd->command_params.historical_data.bar_size_setting, cmd->command_params.historical_data.what_to_show, cmd->command_params.historical_data.use_rth, cmd->command_params.historical_data.format_date);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_CANCEL_TWS_HISTORICAL_DATA:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_cancel_historical_data(info->tws_handle, cmd->command_params.ticker_id);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_CANCEL_TWS_MKT_DATA:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_cancel_mkt_data(info->tws_handle, cmd->command_params.ticker_id);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_TWS_EXERCISE_OPTIONS:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_exercise_options(info->tws_handle, cmd->command_params.exec_options.ticker_id, cmd->command_params.exec_options.contract, cmd->command_params.exec_options.exercise_action, cmd->command_params.exec_options.exercise_quantity, cmd->command_params.exec_options.account, cmd->command_params.exec_options.exc_override);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_TWS_PLACE_ORDER:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_place_order(info->tws_handle, cmd->command_params.order.id, cmd->command_params.order.contract, cmd->command_params.order.order);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_CANCEL_TWS_ORDER:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_cancel_order(info->tws_handle, cmd->command_params.order_id);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_REQUEST_TWS_OPEN_ORDERS:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_req_open_orders(info->tws_handle);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_REQUEST_TWS_ACCOUNT_UPDATES:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_req_account_updates(info->tws_handle, cmd->command_params.account_update.subscribe, cmd->command_params.account_update.acct_code);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_REQUEST_TWS_EXECUTIONS:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_req_executions(info->tws_handle, cmd->command_params.req_executions.reqid, cmd->command_params.req_executions.filter);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_REQUEST_TWS_IDS:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_req_ids(info->tws_handle, cmd->command_params.numids);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_REQUEST_TWS_CONTRACT_DETAILS:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_req_contract_details(info->tws_handle, cmd->command_params.contract_details.reqid, cmd->command_params.contract_details.contract);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_REQUEST_TWS_MKT_DEPTH:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_req_mkt_depth(info->tws_handle, cmd->command_params.market_depth.ticker_id, cmd->command_params.market_depth.contract, cmd->command_params.market_depth.num_rows);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_CANCEL_TWS_MKT_DEPTH:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_cancel_mkt_depth(info->tws_handle, cmd->command_params.ticker_id);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_REQUEST_TWS_NEWS_BULLETINS:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_req_news_bulletins(info->tws_handle, cmd->command_params.allmsgs);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_CANCEL_TWS_NEWS_BULLETINS:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_cancel_news_bulletins(info->tws_handle);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_SET_TWS_SERVER_LOG_LEVEL:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_set_server_log_level(info->tws_handle, cmd->command_params.log_level);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_REQUEST_TWS_AUTO_OPEN_ORDERS:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_req_auto_open_orders(info->tws_handle, cmd->command_params.auto_bind);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_REQUEST_TWS_ALL_OPEN_ORDERS:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_req_all_open_orders(info->tws_handle);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_REQUEST_TWS_MANAGED_ACCTS:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_req_managed_accts(info->tws_handle);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_TWS_REQUEST_FA:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_request_fa(info->tws_handle, cmd->command_params.fa_data_type);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_TWS_REPLACE_FA:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_replace_fa(info->tws_handle, cmd->command_params.replace_fa.fa_data_type, cmd->command_params.replace_fa.xml);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_REQUEST_TWS_CURRENT_TIME:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_req_current_time(info->tws_handle);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_REQUEST_TWS_FUNDAMENTAL_DATA:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_req_fundamental_data(info->tws_handle, cmd->command_params.fundamental_data.reqid, cmd->command_params.fundamental_data.contract, cmd->command_params.fundamental_data.report_type);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_CANCEL_TWS_FUNDAMENTAL_DATA:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_cancel_fundamental_data(info->tws_handle, cmd->command_params.reqid);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_TWS_CALCULATE_IMPLIED_VOLATILITY:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_calculate_implied_volatility(info->tws_handle, cmd->command_params.implied_vola.reqid, cmd->command_params.implied_vola.contract, cmd->command_params.implied_vola.option_price, cmd->command_params.implied_vola.under_price);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_CANCEL_TWS_CALCULATE_IMPLIED_VOLATILITY:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_cancel_calculate_implied_volatility(info->tws_handle, cmd->command_params.reqid);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_TWS_CALCULATE_OPTION_PRICE:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_calculate_option_price(info->tws_handle, cmd->command_params.option_price.reqid, cmd->command_params.option_price.contract, cmd->command_params.option_price.volatility, cmd->command_params.option_price.under_price);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_CANCEL_TWS_CALCULATE_OPTION_PRICE:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_cancel_calculate_option_price(info->tws_handle, cmd->command_params.reqid);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_REQUEST_TWS_GLOBAL_CANCEL:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_req_global_cancel(info->tws_handle);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_TWS_REQUEST_REALTIME_BARS:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_request_realtime_bars(info->tws_handle, cmd->command_params.rt_bars.ticker_id, cmd->command_params.rt_bars.contract, cmd->command_params.rt_bars.bar_size, cmd->command_params.rt_bars.what_to_show, cmd->command_params.rt_bars.use_Rth);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	case TIER2_CANCEL_TWS_REALTIME_BARS:
		pthread_mutex_lock(&exch->tws_tx_mutex);
		tws_cancel_realtime_bars(info->tws_handle, cmd->command_params.ticker_id);
		pthread_mutex_unlock(&exch->tws_tx_mutex);
		break;

	/* auxilliary routines */

	case TIER2_REQUEST_TWS_SERVER_VERSION:
		tws_server_version(info->tws_handle);
		break;

	case TIER2_REQUEST_TWS_CONNECTION_TIME:
		tws_connection_time(info->tws_handle);
		break;
	}

	return 0;
}

int tier2_abort_request(struct tws_conn_cfg *tws_cfg, const tier2_queue_item_t *cmd)
{
	struct tws_thread_exch *exch = tws_cfg->exch;
	int i = cmd - exch->work_queue;
	struct tier2_queue_item *item;

	if (i < 0 || i >= exch->work_queue_size)
	{
		assert(0);
		return -1;
	}

	item = exch->work_queue + i;
	item->exec_repeat_count = -1;
	item->request_state = TIER2_ABORTED;

	return 0;
}



/*
Check which queued request has become active (round robin!) and 
copy the queue entry to the referenced location. Return 0 when there is no
pending entry, 1 if there is.
*/
int tier2_pop_request(struct tws_conn_cfg *tws_cfg, tier2_queue_item_t *dst)
{
	time_t timestamp = time(NULL);
	struct tws_thread_exch *exch = tws_cfg->exch;
	int i;

	for (i = (exch->work_queue_poppos + 1) % exch->work_queue_size; 
		 i != exch->work_queue_poppos; 
		 i = (i + 1) % exch->work_queue_size)
	{
		struct tier2_queue_item *item = exch->work_queue + i;
		if (item->exec_repeat_count > 0
			&& (!item->activation_time 
				|| item->activation_time >= timestamp))
		{
			// round robin polling of the queue:
			exch->work_queue_poppos = (exch->work_queue_poppos + 1) % exch->work_queue_size;

			// update the periodical when it's a repeat performance request:
			if (--item->exec_repeat_count > 0)
			{
				item->activation_time += item->exec_time_interval;

				// and cope with the situation where the original request had an activation timestamp of zero or pickup was much delayed
				if (item->activation_time < timestamp)
				{
					item->activation_time = timestamp + item->exec_time_interval;
				}
			}
			else
			{
				assert(exch->work_queue_fill > 0);
				exch->work_queue_fill--;
			}
			//item->request_state = TIER2_NO_RESPONSE_YET;

			*dst = *item;
			return 1;
		}
	}
	return 0;
}



// utility calls:
unsigned int tier2_queue_depth(struct tws_conn_cfg *tws_cfg)
{
	const struct tws_thread_exch *exch = tws_cfg->exch;
	unsigned int rv;
	rv = exch->work_queue_fill;
	return rv;
}



/*
Check whether there are any queued requests which are 'activated' and if there
are, process one.
*/
static void process_one_queued_tier2_request(struct my_tws_io_info *info)
{
	struct tws_conn_cfg *tws_cfg = info->tws_cfg;
	tier2_queue_item_t cmd;
	
	if (tier2_pop_request(tws_cfg, &cmd) > 0)
	{


	}
}







#if 0
				pthread_mutex_lock(&exch->tws_exch_mutex);

                // block & wait until we can go and submit the request:
                while (mg_get_stop_flag(ctx) == 0 && ETIMEOUT == pthread_cond_timedwait(&exch->tws_tx_signal, &exch->tws_exch_mutex, &poll_time))
                    ;

                // send the request
                mg_cry(conn, "frontend request handler for url '%s': command count = %d", request_info->uri, exch->command);

                exch->command++;


                while (mg_get_stop_flag(ctx) == 0 && ETIMEOUT == pthread_cond_timedwait(&exch->tws_rx_signal, &exch->tws_exch_mutex, &poll_time))
                    ;

                // receive the response
                mg_cry(conn, "frontend request handler for url '%s': response count = %d", request_info->uri, exch->response);











                pthread_mutex_lock(&exch->tws_exch_mutex);
                if (0 != pthread_cond_signal(&exch->tws_tx_signal))
                {
                    pthread_mutex_unlock(&exch->tws_exch_mutex);
                }
                else
                {
                    int cmd;

                    // process incoming TX command/request from one of the front-end threads:
                    if (exch->command != 0)
                    {
                        mg_cry(info->conn, "backend recv waiter: command count = %d", exch->command);
                    }

                    // copy command to local storage???
                    cmd = (exch->command == 1);

                    pthread_mutex_unlock(&exch->tws_exch_mutex);

                    // fire command at TWS itself:
                    if (cmd)
                    {
                        tws_req_current_time(info->tws_handle);

                        pthread_mutex_lock(&exch->tws_exch_mutex);
                        exch->command--;
                        pthread_mutex_unlock(&exch->tws_exch_mutex);
                    }
                }
#endif





/*
Connect to the TWS machine; when this fails or when the connection breaks, we retry after a while.

Send all the queued requests to the TWS node and process the responses; signal the origin for each
request when their response has arrived.
*/
void tws_worker_thread(struct mg_context *ctx)
{
    int tws_app_is_down = 0;

    // retry connecting to TWS as long as the server itself hasn't been stopped!
    while (mg_get_stop_flag(ctx) == 0)
    {
        struct tws_conn_cfg *tws_cfg = (struct tws_conn_cfg *)mg_get_user_data(ctx)->user_data;
		struct tws_thread_exch *exch = tws_cfg->exch;
        struct my_tws_io_info info = {0};
        int err;
		int abortus_provocatus = 0;

		tws_cfg->tws_thread_info = &info;

        info.tws_cfg = tws_cfg;
        info.ctx = ctx;

		info.tws_handle = tws_create(&info, tws_transmit_func, tws_receive_func, tws_flush_func, tws_open_func, tws_close_func);
        if (info.tws_handle)
        {
            err = tws_connect(info.tws_handle, tws_cfg->our_id_code);
            if (err)
            {
                mg_cry4ctx(ctx, "tws connect returned error: %s", tws_errcode2str(err));

                if (!tws_app_is_down)
                {
                    mg_cry4ctx(ctx, "Cannot establish a connection with the TWS application. Retrying every %d seconds...", TWS_CONNECT_RETRY_DELAY);
                    tws_app_is_down++;
                }
            }
            else
            {
                // reset the counter as we have a valid/working connection again now:
                tws_app_is_down = 0;

				// make sure the databases have been opened and are ready to receive data:
				err = ib_open_databases(&info);
				if (err)
				{
					mg_cry4ctx(ctx, "FATAL ERROR: Cannot access the database: %d (%s)\n", err, ib_strerror(err));
					abortus_provocatus = 1;
					goto fail_dramatically;
				}

                // request the valid set of scanner parameters first: this will trigger the requesting of several market scans from the msg receive handler:
				pthread_mutex_lock(&exch->tws_tx_mutex);
                tws_req_scanner_parameters(info.tws_handle);
				pthread_mutex_unlock(&exch->tws_tx_mutex);

                while (mg_get_stop_flag(ctx) == 0 && tws_connected(info.tws_handle))
                {
                    // process another message: reset the message-internal counter(s):
                    info.row_count = 0;

                    if (0 != tws_event_process(info.tws_handle))
                    {
                        break;
                    }

					/*
					Meanwhile, we also process queued requests from the front-end at a 1:1 mix ratio vs. incoming messages from TWS/IB:
					*/
					process_one_queued_tier2_request(&info);
                }
            }
        }
        else
        {
            mg_cry4ctx(ctx, "FATAL ERROR: memory error in tws_create: aborting thread\n");
			abortus_provocatus = 1;
        }

fail_dramatically:
        tws_disconnect(info.tws_handle);
		ib_close_databases(&info);
        tws_destroy(info.tws_handle);
		info.tws_handle = NULL;
		tws_cfg->tws_thread_info = NULL;
		if (abortus_provocatus)
		{
			mg_signal_stop(ctx);
			break;
		}

        // wait N seconds before retrying to connect to TWS:
        if (mg_get_stop_flag(ctx) == 0)
        {
            sleep(TWS_CONNECT_RETRY_DELAY);
        }
    }

    mg_signal_mgr_this_thread_is_done(ctx);

    DEBUG_TRACE(("exiting"));
}






