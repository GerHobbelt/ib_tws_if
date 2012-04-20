
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

#include <tws_c_api/twsapi.h>
#include <tws_c_api/twsapi-debug.h>

#include "tws_request.h"

#include "tier2_message_processor.h"
#include "interthread_comm.h"
#include "app_manager.h"
#include "tws_instance.h"

#include <mongoose/mongoose_ex.h>





int tws_request_message::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}





/* sends message REQ_SCANNER_PARAMETERS to IB/TWS */
int ib_msg_req_scanner_parameters::send_to_final_destination(void)
{
	app_manager *mgr = owner->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();

	return ibm->tx_request_scanner_parameters(this);
}

/* sends message REQ_SCANNER_SUBSCRIPTION to IB/TWS */
int ib_msg_req_scanner_subscription::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message CANCEL_SCANNER_SUBSCRIPTION to IB/TWS */
int ib_msg_cancel_scanner_subscription::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_MKT_DATA to IB/TWS */
int ib_msg_req_mkt_data::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_HISTORICAL_DATA to IB/TWS */
int ib_msg_req_historical_data::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message CANCEL_HISTORICAL_DATA to IB/TWS */
int ib_msg_cancel_historical_data::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message CANCEL_MKT_DATA to IB/TWS */
int ib_msg_cancel_mkt_data::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message EXERCISE_OPTIONS to IB/TWS */
int ib_msg_exercise_options::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message PLACE_ORDER to IB/TWS */
int ib_msg_place_order::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message CANCEL_ORDER to IB/TWS */
int ib_msg_cancel_order::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_OPEN_ORDERS to IB/TWS */
int ib_msg_req_open_orders::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_ACCOUNT_DATA to IB/TWS */
int ib_msg_req_account_updates::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_EXECUTIONS to IB/TWS */
int ib_msg_req_executions::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_IDS to IB/TWS */
int ib_msg_req_ids::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_CONTRACT_DATA to IB/TWS */
int ib_msg_req_contract_details::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_MKT_DEPTH to IB/TWS */
int ib_msg_req_mkt_depth::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message CANCEL_MKT_DEPTH to IB/TWS */
int ib_msg_cancel_mkt_depth::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_NEWS_BULLETINS to IB/TWS */
int ib_msg_req_news_bulletins::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message CANCEL_NEWS_BULLETINS to IB/TWS */
int ib_msg_cancel_news_bulletins::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message SET_SERVER_LOGLEVEL to IB/TWS */
int ib_msg_set_server_log_level::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_AUTO_OPEN_ORDERS to IB/TWS */
int ib_msg_req_auto_open_orders::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_ALL_OPEN_ORDERS to IB/TWS */
int ib_msg_req_all_open_orders::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_MANAGED_ACCTS to IB/TWS */
int ib_msg_req_managed_accts::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_FA to IB/TWS */
int ib_msg_request_fa::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REPLACE_FA to IB/TWS */
int ib_msg_replace_fa::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_CURRENT_TIME to IB/TWS */
int ib_msg_req_current_time::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_FUNDAMENTAL_DATA to IB/TWS */
int ib_msg_req_fundamental_data::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message CANCEL_FUNDAMENTAL_DATA to IB/TWS */
int ib_msg_cancel_fundamental_data::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_CALC_IMPLIED_VOLAT to IB/TWS */
int ib_msg_calculate_implied_volatility::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message CANCEL_CALC_IMPLIED_VOLAT to IB/TWS */
int ib_msg_cancel_calculate_implied_volatility::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_CALC_OPTION_PRICE to IB/TWS */
int ib_msg_calculate_option_price::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message CANCEL_CALC_OPTION_PRICE to IB/TWS */
int ib_msg_cancel_calculate_option_price::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_GLOBAL_CANCEL to IB/TWS */
int ib_msg_req_global_cancel::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_MARKET_DATA_TYPE to IB/TWS */
int ib_msg_req_market_data_type::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_REAL_TIME_BARS to IB/TWS */
int ib_msg_request_realtime_bars::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message CANCEL_REAL_TIME_BARS to IB/TWS */
int ib_msg_cancel_realtime_bars::send_to_final_destination(void)
{
	assert(!"Should never get here");
	return 0;
}














/* 
auxiliary message: fetches server version and connection time datums 

int    tws_server_version();

const char *tws_connection_time();
*/
int ib_msg_fetch_tws_info::send_to_final_destination(void)
{
	tier2_message_processor *rcvr = get_receiver();
	app_manager *mgr = rcvr->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	tws::tws_instance_t *tws = ibm->get_tws_instance();

	int server_v = tws::tws_server_version(tws);
	const char *server_t = tws::tws_connection_time(tws);

	// and return an immediate response...
	state(RESPONSE_COMPLETE);

	return 0;
}






































/* sends message REQ_SCANNER_PARAMETERS to IB/TWS */
int ib_msg_req_scanner_parameters::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_req_scanner_parameters(tws);

	return rv;
}
/* sends message REQ_SCANNER_SUBSCRIPTION to IB/TWS */
int ib_msg_req_scanner_subscription::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_req_scanner_subscription(tws, ticker_id, &tws::tr_scanner_subscription(subscription));

	return rv;
}
/* sends message CANCEL_SCANNER_SUBSCRIPTION to IB/TWS */
int ib_msg_cancel_scanner_subscription::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_cancel_scanner_subscription(tws, ticker_id);

	return rv;
}
/* sends message REQ_MKT_DATA to IB/TWS */
int ib_msg_req_mkt_data::tx(tws::tws_instance_t *tws)
{
	// http://stackoverflow.com/questions/584824/guaranteed-lifetime-of-temporary-in-c
	int rv = tws::tws_req_mkt_data(tws, ticker_id, &tws::tr_contract(contract), ib_string_t(generic_tick_list), snapshot);

	return rv;
}
/* sends message REQ_HISTORICAL_DATA to IB/TWS */
int ib_msg_req_historical_data::tx(tws::tws_instance_t *tws)
{
	// http://stackoverflow.com/questions/584824/guaranteed-lifetime-of-temporary-in-c
	int rv = tws::tws_req_historical_data(tws, ticker_id, &tws::tr_contract(contract), ib_string_t(end_date_time), duration, bar_size_setting, what_to_show, use_rth, format_date);

	return rv;
}
/* sends message CANCEL_HISTORICAL_DATA to IB/TWS */
int ib_msg_cancel_historical_data::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_cancel_historical_data(tws, ticker_id);

	return rv;
}
/* sends message CANCEL_MKT_DATA to IB/TWS */
int ib_msg_cancel_mkt_data::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_cancel_mkt_data(tws, ticker_id);

	return rv;
}
/* sends message EXERCISE_OPTIONS to IB/TWS */
int ib_msg_exercise_options::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_exercise_options(tws, ticker_id, &tws::tr_contract(contract), exercise_action, exercise_quantity, account, exc_override);

	return rv;
}
/* sends message PLACE_ORDER to IB/TWS */
int ib_msg_place_order::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_place_order(tws, order_id, &tws::tr_contract(contract), &tws::tr_order(order));

	return rv;
}
/* sends message CANCEL_ORDER to IB/TWS */
int ib_msg_cancel_order::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_cancel_order(tws, order_id);

	return rv;
}
/* sends message REQ_OPEN_ORDERS to IB/TWS */
int ib_msg_req_open_orders::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_req_open_orders(tws);

	return rv;
}
/* sends message REQ_ACCOUNT_DATA to IB/TWS */
int ib_msg_req_account_updates::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_req_account_updates(tws, subscribe, acct_code);

	return rv;
}
/* sends message REQ_EXECUTIONS to IB/TWS */
int ib_msg_req_executions::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_req_executions(tws, reqid, &tws::tr_exec_filter(filter));

	return rv;
}
/* sends message REQ_IDS to IB/TWS */
int ib_msg_req_ids::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_req_ids(tws, num_ids);

	return rv;
}
/* sends message REQ_CONTRACT_DATA to IB/TWS */
int ib_msg_req_contract_details::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_req_contract_details(tws, reqid, &tws::tr_contract(contract));

	return rv;
}
/* sends message REQ_MKT_DEPTH to IB/TWS */
int ib_msg_req_mkt_depth::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_req_mkt_depth(tws, ticker_id, &tws::tr_contract(contract), num_rows);

	return rv;
}
/* sends message CANCEL_MKT_DEPTH to IB/TWS */
int ib_msg_cancel_mkt_depth::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_cancel_mkt_depth(tws, ticker_id);

	return rv;
}
/* sends message REQ_NEWS_BULLETINS to IB/TWS */
int ib_msg_req_news_bulletins::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_req_news_bulletins(tws, all_msgs);

	return rv;
}
/* sends message CANCEL_NEWS_BULLETINS to IB/TWS */
int ib_msg_cancel_news_bulletins::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_cancel_news_bulletins(tws);

	return rv;
}
/* sends message SET_SERVER_LOGLEVEL to IB/TWS */
int ib_msg_set_server_log_level::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_set_server_log_level(tws, level);

	return rv;
}
/* sends message REQ_AUTO_OPEN_ORDERS to IB/TWS */
int ib_msg_req_auto_open_orders::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_req_auto_open_orders(tws, auto_bind);

	return rv;
}
/* sends message REQ_ALL_OPEN_ORDERS to IB/TWS */
int ib_msg_req_all_open_orders::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_req_all_open_orders(tws);

	return rv;
}
/* sends message REQ_MANAGED_ACCTS to IB/TWS */
int ib_msg_req_managed_accts::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_req_managed_accts(tws);

	return rv;
}
/* sends message REQ_FA to IB/TWS */
int ib_msg_request_fa::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_request_fa(tws, fa_data_type);

	return rv;
}
/* sends message REPLACE_FA to IB/TWS */
int ib_msg_replace_fa::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_replace_fa(tws, fa_data_type, cxml);

	return rv;
}
/* sends message REQ_CURRENT_TIME to IB/TWS */
int ib_msg_req_current_time::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_req_current_time(tws);

	return rv;
}
/* sends message REQ_FUNDAMENTAL_DATA to IB/TWS */
int ib_msg_req_fundamental_data::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_req_fundamental_data(tws, reqid, &tws::tr_contract(contract), report_type);

	return rv;
}
/* sends message CANCEL_FUNDAMENTAL_DATA to IB/TWS */
int ib_msg_cancel_fundamental_data::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_cancel_fundamental_data(tws, reqid);

	return rv;
}
/* sends message REQ_CALC_IMPLIED_VOLAT to IB/TWS */
int ib_msg_calculate_implied_volatility::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_calculate_implied_volatility(tws, reqid, &tws::tr_contract(contract), option_price, under_price);

	return rv;
}
/* sends message CANCEL_CALC_IMPLIED_VOLAT to IB/TWS */
int ib_msg_cancel_calculate_implied_volatility::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_cancel_calculate_implied_volatility(tws, reqid);

	return rv;
}
/* sends message REQ_CALC_OPTION_PRICE to IB/TWS */
int ib_msg_calculate_option_price::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_calculate_option_price(tws, reqid, &tws::tr_contract(contract), volatility, under_price);

	return rv;
}
/* sends message CANCEL_CALC_OPTION_PRICE to IB/TWS */
int ib_msg_cancel_calculate_option_price::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_cancel_calculate_option_price(tws, reqid);

	return rv;
}
/* sends message REQ_GLOBAL_CANCEL to IB/TWS */
int ib_msg_req_global_cancel::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_req_global_cancel(tws);

	return rv;
}
/* sends message REQ_MARKET_DATA_TYPE to IB/TWS */
int ib_msg_req_market_data_type::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_req_market_data_type(tws, market_data_type);

	return rv;
}
/* sends message REQ_REAL_TIME_BARS to IB/TWS */
int ib_msg_request_realtime_bars::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_request_realtime_bars(tws, ticker_id, &tws::tr_contract(contract), bar_size, what_to_show, use_rth);

	return rv;
}
/* sends message CANCEL_REAL_TIME_BARS to IB/TWS */
int ib_msg_cancel_realtime_bars::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_cancel_realtime_bars(tws, ticker_id);

	return rv;
}









ib_ticker_list::operator class ib_string_t(void)
{
	return "";
}

