
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

#include "system-includes.h"

#include "tws_comm_thread.h"
#include "tws_stocklist_loading_thread.h"





void event_tick_price(void *opaque, int ticker_id, tr_tick_type_t field, double price, int can_auto_execute)
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "info", "tick_price: opaque=%p, ticker_id=%d, type=%d, price=%.2lf, can_auto=%d",
        opaque, ticker_id, (int)field, price, can_auto_execute);
}

void event_tick_size(void *opaque, int ticker_id, tr_tick_type_t type, int size)
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "info", "tick_size: opaque=%p, ticker_id=%d, type=%d, size=%d",
        opaque, ticker_id, (int)type, size);
}

void event_tick_option_computation(void *opaque, int ticker_id, tr_tick_type_t type, double implied_vol, double delta, double opt_price, double pv_dividend, double gamma, double vega, double theta, double und_price)
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "info", "tick option computation: opaque=%p, ticker_id=%d, type=%d, implied_vol=%f, delta=%f, opt_price=%f, pv_dividend=%f, gamma=%f, vega=%f, theta=%f, und_price=%f",
        opaque, ticker_id, (int)type, implied_vol, delta, opt_price, pv_dividend, gamma, vega, theta, und_price);
}

void event_tick_generic(void *opaque, int ticker_id, tr_tick_type_t type, double value)
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "info", "tick_generic: opaque=%p, ticker_id=%d, type=%d, ...", opaque, ticker_id, type);
}

void event_tick_string(void *opaque, int ticker_id, tr_tick_type_t type, const char value[])
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "info", "tick_string: opaque=%p, ticker_id=%d, type=%d, ...", opaque, ticker_id, type);
}

void event_tick_efp(void *opaque, int ticker_id, tr_tick_type_t tick_type, double basis_points, const char formatted_basis_points[], double implied_futures_price, int hold_days, const char future_expiry[], double dividend_impact, double dividends_to_expiry)
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "info", "tick_efp: opaque=%p, ticker_id=%d, type=%d, ...", opaque, ticker_id, tick_type);
}

void event_order_status(void *opaque, int order_id, const char status[],
                        int filled, int remaining, double avg_fill_price, int perm_id,
                        int parent_id, double last_fill_price, int client_id, const char why_held[])
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "info", "order_status: opaque=%p, order_id=%d, filled=%d remaining %d, avg_fill_price=%lf, last_fill_price=%lf, why_held=%s", opaque, order_id, filled, remaining, avg_fill_price, last_fill_price, why_held);
}

void event_open_order(void *opaque, int order_id, const tr_contract_t *contract, const tr_order_t *order, const tr_order_status_t *ostatus)
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

    /* commission values might be DBL_MAX */
    if(fabs(ostatus->ost_commission - DBL_MAX) < DBL_EPSILON)
	{
		mg_log(info->conn, "info", "open_order: commission not reported");
	}
    else
    {
		mg_log(info->conn, "info", "open_order: commission for %d was %.4lf", order_id, ostatus->ost_commission);
	}
	mg_log(info->conn, "info", "open_order: opaque=%p, order_id=%d, sym=%s", opaque, order_id, contract->c_symbol);
}

void event_update_account_value(void *opaque, const char key[], const char val[],
                                const char currency[], const char account_name[])
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "info", "update_account_value: %p, key=%s val=%s, currency=%s, name=%s",
        opaque, key, val, currency, account_name);
}

void event_update_portfolio(void *opaque, const tr_contract_t *contract, int position,
                            double mkt_price, double mkt_value, double average_cost,
                            double unrealized_pnl, double realized_pnl, const char account_name[])
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "info", "update_portfolio: %p, sym=%s, position=%d, mkt_price=%.4lf, mkt_value=%.4lf, avg_cost=%.4lf, unrealized_pnl=%.4lf, realized pnl=%.4lf name=%s",
        opaque, contract->c_symbol, position, mkt_price, mkt_value, average_cost, unrealized_pnl, realized_pnl, account_name);
}

void event_update_account_time(void *opaque, const char time_stamp[])
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "info", "update_account_time: opaque=%p, ...", opaque);
}

void event_next_valid_id(void *opaque, int order_id)
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

    /* invoked once at connection establishment
     * the scope of this variable is not program wide, instance wide,
     * or even system wide. It has the same scope as the TWS account itself.
     * It appears to be persistent across TWS restarts.
     * Well behaved human and automatic TWS clients shall increment
     * this order_id atomically and cooperatively
     */
    tws_setNextOrderId(info, order_id);

	mg_log(info->conn, "info", "next_valid_id for order placement %d", order_id);
}

void event_contract_details(void *opaque, int req_id, const tr_contract_details_t *cd)
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "info", "contract_details: opaque=%p, ...", opaque);
	mg_log(info->conn, "info", "contract details: sym=%s, sectype=%s, expiry=%s, strike=%.3lf, right=%s, exch=%s, primary exch=%s, currency=%s, multiplier=%s, local_sym=%s, market_name=%s, trading_class=%s, conid=%d",
        cd->d_summary.c_symbol, cd->d_summary.c_sectype, cd->d_summary.c_expiry, cd->d_summary.c_strike, cd->d_summary.c_right, cd->d_summary.c_exchange, cd->d_summary.c_primary_exch,
        cd->d_summary.c_currency, cd->d_summary.c_multiplier, cd->d_summary.c_local_symbol, cd->d_market_name, cd->d_trading_class, cd->d_summary.c_conid);
	mg_log(info->conn, "info", "contract details: min.tick: %f, coupon: %f, order types: %s, valid exch: %s, cusip: %s, maturity: %s, issue_date: %s, ratings: %s, bond_type: %s, "
        "coupon_type: %s, notes: %s, long name: %s, industry: %s, category: %s, subcategory: %s, timezone: %s, trading hours: %s, liquid hours: %s, price_magnifier: %d, "
        "under_conid: %d",
        cd->d_mintick, cd->d_coupon, cd->d_order_types, cd->d_valid_exchanges, cd->d_cusip, cd->d_maturity, cd->d_issue_date, cd->d_ratings, cd->d_bond_type,
        cd->d_coupon_type, cd->d_notes, cd->d_long_name, cd->d_industry, cd->d_category, cd->d_subcategory, cd->d_timezone_id, cd->d_trading_hours, cd->d_liquid_hours,
        cd->d_price_magnifier, cd->d_under_conid);

	ib_cache_ticker_info(cd);
}

void event_bond_contract_details(void *opaque, int req_id, const tr_contract_details_t *cd)
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "info", "bond_contract_details: opaque=%p, ...", opaque);
	mg_log(info->conn, "info", "bond contract details: sym=%s, sectype=%s, expiry=%s, strike=%.3lf, right=%s, exch=%s, primary exch=%s, currency=%s, multiplier=%s, local_sym=%s, market_name=%s, trading_class=%s, conid=%d",
        cd->d_summary.c_symbol, cd->d_summary.c_sectype, cd->d_summary.c_expiry, cd->d_summary.c_strike, cd->d_summary.c_right, cd->d_summary.c_exchange, cd->d_summary.c_primary_exch,
        cd->d_summary.c_currency, cd->d_summary.c_multiplier, cd->d_summary.c_local_symbol, cd->d_market_name, cd->d_trading_class, cd->d_summary.c_conid);
	mg_log(info->conn, "info", "bond contract details: min.tick: %f, coupon: %f, order types: %s, valid exch: %s, cusip: %s, maturity: %s, issue_date: %s, ratings: %s, bond_type: %s, "
        "coupon_type: %s, notes: %s, long name: %s, industry: %s, category: %s, subcategory: %s, timezone: %s, trading hours: %s, liquid hours: %s, price_magnifier: %d, "
        "under_conid: %d",
        cd->d_mintick, cd->d_coupon, cd->d_order_types, cd->d_valid_exchanges, cd->d_cusip, cd->d_maturity, cd->d_issue_date, cd->d_ratings, cd->d_bond_type,
        cd->d_coupon_type, cd->d_notes, cd->d_long_name, cd->d_industry, cd->d_category, cd->d_subcategory, cd->d_timezone_id, cd->d_trading_hours, cd->d_liquid_hours,
        cd->d_price_magnifier, cd->d_under_conid);

	ib_cache_ticker_info(cd);
}

void event_exec_details(void *opaque, int order_id, const tr_contract_t *contract, const tr_execution_t *execution)
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "info", "exec_details: opaque=%p, ...", opaque);
}

void event_error(void *opaque, int ticker_id, int error_code, const char error_string[])
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "error", "opaque=%p, id=%d, error_code=%d, msg=%s", opaque, ticker_id, error_code, error_string);

    /*
    some scanner subscription requests may trigger error responses, such as 'duplicate subscription'
    and we need to keep the scanner subscription active list in proper order, so we check whether the related
    ticker_id is one of our active scanner subscription queue items and when it is, we ditch that one
    and replace it by another pending scanner subscription request.
    */
    cancel_tws_scanner_subscription(info, ticker_id);
}

void event_update_mkt_depth(void *opaque, int ticker_id, int position, int operation, int side, double price, int size)
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "info", "update_mkt_depth: opaque=%p, ticker_id=%d, ...", opaque, ticker_id);
}

void event_update_mkt_depth_l2(void *opaque, int ticker_id, int position, char *market_maker, int operation, int side, double price, int size)
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "info", "update_mkt_depth_l2: opaque=%p, ticker_id=%d, ...", opaque, ticker_id);
}

void event_update_news_bulletin(void *opaque, int msgid, int msg_type, const char news_msg[], const char origin_exch[])
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "info", "update_news_bulletin: opaque=%p, ...", opaque);
}

void event_managed_accounts(void *opaque, const char accounts_list[])
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "info", "managed_accounts: opaque=%p, ...", opaque);
}

void event_receive_fa(void *opaque, int fa_data_type, const char cxml[])
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "info", "receive_fa: opaque=%p, fa_data_type=%d, xml='%s'", opaque, fa_data_type, cxml);
}

void event_historical_data(void *opaque, int reqid, const char date[], double open, double high, double low, double close, int volume, int bar_count, double wap, int has_gaps)
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "info", "historical: opaque=%p, reqid=%d, date=%s, %.3lf, %.3lf, %.3lf, %.3lf, %d, wap=%.3lf, has_gaps=%d", opaque, reqid, date, open, high, low, close, volume, wap, has_gaps);
}

void event_historical_data_end(void *opaque, int reqid, const char completion_from[], const char completion_to[])
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "info", "historical list end: opaque=%p, reqid=%d, from date=%s, to date=%s", opaque, reqid, completion_from, completion_to);
}

/*
As TWS limits the number of active scanner requests to 10, we might delegate the processing of this message to a separate thread,
but that would mean we'd need inter-thread messaging back & forth, at least for the outgoing request messages (to TWS) and
the scanner reports returning from TWS.

The alternative is to process this message and queue all the scanner subscription requests generated from it in a separate
queue which is slowly depleted as scanner reports come in and the code in that handler decides to UNsubscribe a given
scanner report.

By using the queue the incoming TWS message traffic processing is kept as simple as possible; the scanner subscribe msg queue
keeps this part essentially a single-thread process.
*/
void event_scanner_parameters(void *opaque, const char xml[])
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;
    int rv;
    xmlTextReaderPtr reader;
    xmlNodePtr tree;
    xmlDocPtr doc;
    const char *URL = "tws://dummy";

    mg_log(info->conn, "info", "INFO: dumping TWS scanner parameters to log file:");
    mg_log(info->conn, "info", "XML: %s", xml);

	mg_log(info->conn, "info", "scanner_parameters: opaque=%p, xml:(len=%d)", opaque, (int)strlen(xml));

	ib_store_scanner_parameters_xml(info, xml);

    doc = xmlReadDoc((const xmlChar *)xml, URL, "UTF-8", XML_PARSE_NOENT | XML_PARSE_NONET | XML_PARSE_NOCDATA);

    if (doc)
    {
        xmlXPathObjectPtr usable_locations;
        xmlXPathContextPtr ctxt = NULL;

        ctxt = xmlXPathNewContext(doc);
        if (ctxt)
        {
            FILE *fp;

            ctxt->node = xmlDocGetRootElement(doc);

            xmlXPathRegisterFunc(ctxt, (const xmlChar *)"contains-any-of", xmlXPathContainsAnyOfFunction);

            usable_locations = xmlXPathEvalExpression((const xmlChar *)"//LocationTree//Location[not(contains(access, 'restricted')) and not(LocationTree)]", ctxt);

#ifdef LIBXML_DEBUG_ENABLED
            mg_write2log(info->conn, NULL, time(NULL), "info", "INFO: dumping XPath result to log file:");
            fp = mg_fopen(mg_get_default_logfile_path(info->conn), "a+");

            if (fp != NULL)
            {
                mg_flockfile(fp);

                xmlXPathDebugDumpObject(fp, usable_locations, 0);

                fflush(fp);
                mg_funlockfile(fp);
                if (fp != stderr)
                {
                    fclose(fp);
                }
            }
#endif

            if (usable_locations == NULL)
            {
                mg_cry(info->conn, "No usable entries found in the TWS scanner report. IB_TWS_SRV will NOT be collecting any stock data!");
            }
            else if (usable_locations->type != XPATH_NODESET)
            {
                mg_cry(info->conn, "Unexpected XPath result type %d while searching the TWS scanner report. IB_TWS_SRV will NOT be collecting any stock data!", (int)usable_locations->type);
            }
            else if (usable_locations->nodesetval != NULL)
            {
                xmlNodeSetPtr nodeset = usable_locations->nodesetval;
                xmlXPathObjectPtr usable_scanners;
                int i;

                for (i = 0; i < nodeset->nodeNr; i++)
                {
                    xmlNodePtr location = nodeset->nodeTab[i];
                    xmlNodePtr child = xmlFirstElementChild(location);

                    while (child && xmlStrcmp(child->name, (const xmlChar *)"instruments"))
                    {
                        child = xmlNextElementSibling(child);
                    }

                    if (child)
                    {
                        xmlXPathObjectPtr instruments = xmlXPathNewNodeSetList(xmlXPathNodeSetMerge(NULL, xmlXPathNodeSetCreate(child)));
                        //xmlXPathObjectPtr instruments = xmlXPathNewNodeSet(child);   //<-- b0rks in RegisterVariable below as that one expects to free the specified nodeset using xmlXPathFreeObject()!

                        xmlXPathRegisterVariable(ctxt, (const xmlChar *)"loc_instruments", instruments);

                        usable_scanners = xmlXPathEvalExpression((const xmlChar *)"//ScanTypeList/ScanType[contains-any-of(instruments, $loc_instruments) and not(starts-with(access, 'restricted')) and not(contains(access, 'disabled'))]", ctxt);

                        // fire a couple of useful market scanner report requests for each node
                        request_range_of_interesting_market_scans(info, location, usable_scanners);

                        //xmlXPathFreeObject(instruments);
                        //xmlXPathFreeNodeSet();
                        //xmlXPathFreeNodeSetList(instruments);
                    }
                }
            }

            xmlXPathFreeObject(usable_locations);
            xmlXPathFreeContext(ctxt);
        }
    }

    xmlFreeDoc(doc);

#if 0
    // process the XML response using libxml:
    xmlResetLastError();
    reader = xmlReaderForDoc((const xmlChar *)xml, URL, "UTF-8", XML_PARSE_NOENT | XML_PARSE_NONET | XML_PARSE_NOCDATA);

    rv = xmlTextReaderRead(reader);

    // signal the xmlreader to 'preserve' all XML nodes in there; we'll free them when their time is due...
    doc = xmlTextReaderCurrentDoc(reader);

    tree = xmlTextReaderExpand(reader);

    doc = xmlTextReaderCurrentDoc(reader);

    xmlFreeTextReader(reader);

    xmlFreeDoc(doc);
#endif
}

void event_scanner_data(void *opaque, int ticker_id, int rank, tr_contract_details_t *cd, const char distance[], const char benchmark[], const char projection[], const char legs_str[])
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

    // we CAN receive responses for already canceled subscriptions, e.g. when an error report triggered a (pending in the TCP pipeline) subscription cancel request:
    if (!is_active_tws_scanner_subscription(info, ticker_id))
    {
        // only report this for the first item; no need to keep repeating ourselves.
        if (rank == 1)
        {
			mg_log(info->conn, "info", "scanner_data: opaque=%p, ticker_id=%d -- *INFO* -- receiving scanner data for a cancelled subscription. The cancel request and this report will probably have crossed along the way.",
                opaque, ticker_id);
        }
        return;
    }

    info->row_count++;

	mg_log(info->conn, "info", "scanner_data: opaque=%p, ticker_id=%d, rank=%d, distance=%s, benchmark=%s, projection=%s",
        opaque, ticker_id, rank, distance, benchmark, projection);
	mg_log(info->conn, "info", "scanner_data details: sym=%s, sectype=%s, expiry=%s, strike=%.3lf, right=%s, exch=%s, primary exch=%s, currency=%s, multiplier=%s, local_sym=%s, market_name=%s, trading_class=%s, conid=%d",
        cd->d_summary.c_symbol, cd->d_summary.c_sectype, cd->d_summary.c_expiry, cd->d_summary.c_strike, cd->d_summary.c_right, cd->d_summary.c_exchange, cd->d_summary.c_primary_exch,
        cd->d_summary.c_currency, cd->d_summary.c_multiplier, cd->d_summary.c_local_symbol, cd->d_market_name, cd->d_trading_class, cd->d_summary.c_conid);
	mg_log(info->conn, "info", "scanner_data details: min.tick: %f, coupon: %f, order types: %s, valid exch: %s, cusip: %s, maturity: %s, issue_date: %s, ratings: %s, bond_type: %s, "
        "coupon_type: %s, notes: %s, long name: %s, industry: %s, category: %s, subcategory: %s, timezone: %s, trading hours: %s, liquid hours: %s, price_magnifier: %d, "
        "under_conid: %d",
        cd->d_mintick, cd->d_coupon, cd->d_order_types, cd->d_valid_exchanges, cd->d_cusip, cd->d_maturity, cd->d_issue_date, cd->d_ratings, cd->d_bond_type,
        cd->d_coupon_type, cd->d_notes, cd->d_long_name, cd->d_industry, cd->d_category, cd->d_subcategory, cd->d_timezone_id, cd->d_trading_hours, cd->d_liquid_hours,
        cd->d_price_magnifier, cd->d_under_conid);

    /* it seems that NYSE traded stocks have different market_name and trading_class from NASDAQ */


    // now also request the contract details so that we'll receive the extended info for this stock:
    request_contract_details_from_tws(info, cd);
}

void event_scanner_data_end(void *opaque, int ticker_id)
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "info", "scanner_data_end: opaque=%p, ticker_id=%d", opaque, ticker_id);

    // always unsubsubscribe a scanner report when it didn't deliver any rows:
    if (info->row_count == 0)
    {
        cancel_tws_scanner_subscription(info, ticker_id);
    }
}

void event_current_time(void *opaque, long time)
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;
    struct tws_thread_exch *exch = info->tws_cfg->exch;
    char tbuf[40];
    time_t timestamp = (time_t)time;

    strftime(tbuf, sizeof(tbuf), "[%Y%m%dT%H%M%S] ", gmtime(&timestamp));

	mg_log(info->conn, "info", "current_time: opaque=%p, time=%ld ~ '%s'", opaque, time, tbuf);

    /*
     * Pass response on to front-end now:
     */
#if 0
	pthread_mutex_lock(&exch->tws_exch_mutex);
    if (0 != pthread_cond_signal(&exch->tws_rx_signal))
    {
        pthread_mutex_unlock(&exch->tws_exch_mutex);
    }
    else
    {
        // process incoming TX command/request from one of the front-end threads:
        mg_cry(info->conn, "backend CURRENT TIME message handler: response count = %d", exch->response);

        exch->response++;
        exch->current_time = time;

        pthread_mutex_unlock(&exch->tws_exch_mutex);
    }
#endif
}

void event_realtime_bar(void *opaque, int reqid, long time, double open, double high, double low, double close, long volume, double wap, int count)
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "info", "realtime_bar: %p reqid=%d time=%ld, ohlc=%.4lf/%.4lf/%.4lf/%.4lf, vol=%ld, wap=%.4lf, count=%d", opaque, reqid, time, open, high, low, close, volume, wap, count);
}

void event_fundamental_data(void *opaque, int reqid, const char data[])
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "info", "fundamental_data: opaque=%p, reqid=%d, ...", opaque, reqid);
}

void event_contract_details_end(void *opaque, int reqid)
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "info", "contract_details_end: opaque=%p, ...", opaque);
}

void event_open_order_end(void *opaque)
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "info", "open_order_end: opaque=%p", opaque);
}

void event_delta_neutral_validation(void *opaque, int reqid, under_comp_t *und)
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "info", "delta_neutral_validation: opaque=%p, reqid=%d, ...", opaque, reqid);
}

void event_acct_download_end(void *opaque, char acct_name[])
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "info", "acct_download_end: opaque=%p, account name='%s'", opaque, acct_name);
}

void event_exec_details_end(void *opaque, int reqid)
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "info", "exec_details_end: opaque=%p, reqid=%d", opaque, reqid);
}

void event_tick_snapshot_end(void *opaque, int reqid)
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "info", "tick_snapshot_end: opaque=%p, reqid=%d", opaque, reqid);
}

void event_market_data_type(void *opaque, int reqid, market_data_type_t data_type)
{
	struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "info", "market_data_type: opaque=%p, reqid=%d, data_type=%d\n", opaque, reqid, (int)data_type);
}

void event_commission_report(void *opaque, tr_commission_report_t *report)
{
	struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

	mg_log(info->conn, "info", "commission_report: opaque=%p, ...\n", opaque);
	mg_log(info->conn, "info", "          cr_exec_id=[%s], cr_currency=[%s], cr_commission=%f, cr_realized_pnl=%f, cr_yield=%f, cr_yield_redemption_date=%d (%08X) (YYYYMMDD format)\n",
		report->cr_exec_id, report->cr_currency, report->cr_commission, report->cr_realized_pnl, report->cr_yield, 
		report->cr_yield_redemption_date, report->cr_yield_redemption_date);
}


