
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
#include "tws_response.h"

#include <mongoose/mongoose_ex.h>





int tws_request_message::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}





/* sends message REQ_SCANNER_PARAMETERS to IB/TWS */
int ib_msg_req_scanner_parameters::f_exec_command(void)
{
	app_manager *mgr = owner->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();

	return ibm->tx_request_scanner_parameters(this);
}

/* sends message REQ_SCANNER_SUBSCRIPTION to IB/TWS */
int ib_msg_req_scanner_subscription::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message CANCEL_SCANNER_SUBSCRIPTION to IB/TWS */
int ib_msg_cancel_scanner_subscription::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_MKT_DATA to IB/TWS */
int ib_msg_req_mkt_data::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_HISTORICAL_DATA to IB/TWS */
int ib_msg_req_historical_data::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message CANCEL_HISTORICAL_DATA to IB/TWS */
int ib_msg_cancel_historical_data::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message CANCEL_MKT_DATA to IB/TWS */
int ib_msg_cancel_mkt_data::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message EXERCISE_OPTIONS to IB/TWS */
int ib_msg_exercise_options::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message PLACE_ORDER to IB/TWS */
int ib_msg_place_order::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message CANCEL_ORDER to IB/TWS */
int ib_msg_cancel_order::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_OPEN_ORDERS to IB/TWS */
int ib_msg_req_open_orders::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_ACCOUNT_DATA to IB/TWS */
int ib_msg_req_account_updates::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_EXECUTIONS to IB/TWS */
int ib_msg_req_executions::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_IDS to IB/TWS */
int ib_msg_req_ids::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_CONTRACT_DATA to IB/TWS */
int ib_msg_req_contract_details::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_MKT_DEPTH to IB/TWS */
int ib_msg_req_mkt_depth::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message CANCEL_MKT_DEPTH to IB/TWS */
int ib_msg_cancel_mkt_depth::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_NEWS_BULLETINS to IB/TWS */
int ib_msg_req_news_bulletins::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message CANCEL_NEWS_BULLETINS to IB/TWS */
int ib_msg_cancel_news_bulletins::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message SET_SERVER_LOGLEVEL to IB/TWS */
int ib_msg_set_server_log_level::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_AUTO_OPEN_ORDERS to IB/TWS */
int ib_msg_req_auto_open_orders::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_ALL_OPEN_ORDERS to IB/TWS */
int ib_msg_req_all_open_orders::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_MANAGED_ACCTS to IB/TWS */
int ib_msg_req_managed_accts::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_FA to IB/TWS */
int ib_msg_request_fa::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REPLACE_FA to IB/TWS */
int ib_msg_replace_fa::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_CURRENT_TIME to IB/TWS */
int ib_msg_req_current_time::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_FUNDAMENTAL_DATA to IB/TWS */
int ib_msg_req_fundamental_data::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message CANCEL_FUNDAMENTAL_DATA to IB/TWS */
int ib_msg_cancel_fundamental_data::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_CALC_IMPLIED_VOLAT to IB/TWS */
int ib_msg_calculate_implied_volatility::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message CANCEL_CALC_IMPLIED_VOLAT to IB/TWS */
int ib_msg_cancel_calculate_implied_volatility::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_CALC_OPTION_PRICE to IB/TWS */
int ib_msg_calculate_option_price::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message CANCEL_CALC_OPTION_PRICE to IB/TWS */
int ib_msg_cancel_calculate_option_price::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_GLOBAL_CANCEL to IB/TWS */
int ib_msg_req_global_cancel::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_MARKET_DATA_TYPE to IB/TWS */
int ib_msg_req_market_data_type::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_REAL_TIME_BARS to IB/TWS */
int ib_msg_request_realtime_bars::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message CANCEL_REAL_TIME_BARS to IB/TWS */
int ib_msg_cancel_realtime_bars::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}














/* 
auxiliary message: fetches server version and connection time datums 

int    tws_server_version();

const char *tws_connection_time();
*/
int ib_msg_fetch_tws_info::f_exec_command(void)
{
	tier2_message_processor *rcvr = get_receiver();
	app_manager *mgr = rcvr->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	tws::tws_instance_t *tws = ibm->get_tws_instance();

	server_version = tws::tws_server_version(tws);
	connect_timestamp = tws::tws_connection_time(tws);

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
	int rv = tws::tws_req_scanner_subscription(tws, ticker_id, &tws::tr_scanner_subscription_t(subscription));

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
	int rv = tws::tws_req_mkt_data(tws, ticker_id, &tws::tr_contract_t(contract), ib_string_t(generic_tick_list), snapshot);

	return rv;
}
/* sends message REQ_HISTORICAL_DATA to IB/TWS */
int ib_msg_req_historical_data::tx(tws::tws_instance_t *tws)
{
	// http://stackoverflow.com/questions/584824/guaranteed-lifetime-of-temporary-in-c
	int rv = tws::tws_req_historical_data(tws, ticker_id, &tws::tr_contract_t(contract), ib_string_t(end_date_time), duration, bar_size_setting, what_to_show, use_rth, format_date);

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
	int rv = tws::tws_exercise_options(tws, ticker_id, &tws::tr_contract_t(contract), exercise_action, exercise_quantity, account, exc_override);

	return rv;
}
/* sends message PLACE_ORDER to IB/TWS */
int ib_msg_place_order::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_place_order(tws, order_id, &tws::tr_contract_t(contract), &tws::tr_order_t(order));

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
	int rv = tws::tws_req_executions(tws, reqid, &tws::tr_exec_filter_t(filter));

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
	int rv = tws::tws_req_contract_details(tws, reqid, &tws::tr_contract_t(contract));

	return rv;
}
/* sends message REQ_MKT_DEPTH to IB/TWS */
int ib_msg_req_mkt_depth::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_req_mkt_depth(tws, ticker_id, &tws::tr_contract_t(contract), num_rows);

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
	int rv = tws::tws_req_fundamental_data(tws, reqid, &tws::tr_contract_t(contract), report_type);

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
	int rv = tws::tws_calculate_implied_volatility(tws, reqid, &tws::tr_contract_t(contract), option_price, under_price);

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
	int rv = tws::tws_calculate_option_price(tws, reqid, &tws::tr_contract_t(contract), volatility, under_price);

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
	int rv = tws::tws_request_realtime_bars(tws, ticker_id, &tws::tr_contract_t(contract), bar_size, what_to_show, use_rth);

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



bool tws_request_message::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	/* Assume NO by default */
	return false;
}






/* sends message REQ_SCANNER_PARAMETERS to IB/TWS */
bool ib_msg_req_scanner_parameters::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return resp_msg->type_matches(resp_msg);
}
/* sends message REQ_SCANNER_SUBSCRIPTION to IB/TWS */
bool ib_msg_req_scanner_subscription::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message CANCEL_SCANNER_SUBSCRIPTION to IB/TWS */
bool ib_msg_cancel_scanner_subscription::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;	
}
/* sends message REQ_MKT_DATA to IB/TWS */
bool ib_msg_req_mkt_data::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message REQ_HISTORICAL_DATA to IB/TWS */
bool ib_msg_req_historical_data::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message CANCEL_HISTORICAL_DATA to IB/TWS */
bool ib_msg_cancel_historical_data::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message CANCEL_MKT_DATA to IB/TWS */
bool ib_msg_cancel_mkt_data::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message EXERCISE_OPTIONS to IB/TWS */
bool ib_msg_exercise_options::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message PLACE_ORDER to IB/TWS */
bool ib_msg_place_order::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message CANCEL_ORDER to IB/TWS */
bool ib_msg_cancel_order::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message REQ_OPEN_ORDERS to IB/TWS */
bool ib_msg_req_open_orders::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message REQ_ACCOUNT_DATA to IB/TWS */
bool ib_msg_req_account_updates::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message REQ_EXECUTIONS to IB/TWS */
bool ib_msg_req_executions::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message REQ_IDS to IB/TWS */
bool ib_msg_req_ids::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message REQ_CONTRACT_DATA to IB/TWS */
bool ib_msg_req_contract_details::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message REQ_MKT_DEPTH to IB/TWS */
bool ib_msg_req_mkt_depth::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message CANCEL_MKT_DEPTH to IB/TWS */
bool ib_msg_cancel_mkt_depth::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message REQ_NEWS_BULLETINS to IB/TWS */
bool ib_msg_req_news_bulletins::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message CANCEL_NEWS_BULLETINS to IB/TWS */
bool ib_msg_cancel_news_bulletins::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message SET_SERVER_LOGLEVEL to IB/TWS */
bool ib_msg_set_server_log_level::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message REQ_AUTO_OPEN_ORDERS to IB/TWS */
bool ib_msg_req_auto_open_orders::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message REQ_ALL_OPEN_ORDERS to IB/TWS */
bool ib_msg_req_all_open_orders::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message REQ_MANAGED_ACCTS to IB/TWS */
bool ib_msg_req_managed_accts::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message REQ_FA to IB/TWS */
bool ib_msg_request_fa::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message REPLACE_FA to IB/TWS */
bool ib_msg_replace_fa::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message REQ_CURRENT_TIME to IB/TWS */
bool ib_msg_req_current_time::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message REQ_FUNDAMENTAL_DATA to IB/TWS */
bool ib_msg_req_fundamental_data::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message CANCEL_FUNDAMENTAL_DATA to IB/TWS */
bool ib_msg_cancel_fundamental_data::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message REQ_CALC_IMPLIED_VOLAT to IB/TWS */
bool ib_msg_calculate_implied_volatility::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message CANCEL_CALC_IMPLIED_VOLAT to IB/TWS */
bool ib_msg_cancel_calculate_implied_volatility::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message REQ_CALC_OPTION_PRICE to IB/TWS */
bool ib_msg_calculate_option_price::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message CANCEL_CALC_OPTION_PRICE to IB/TWS */
bool ib_msg_cancel_calculate_option_price::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message REQ_GLOBAL_CANCEL to IB/TWS */
bool ib_msg_req_global_cancel::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message REQ_MARKET_DATA_TYPE to IB/TWS */
bool ib_msg_req_market_data_type::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message REQ_REAL_TIME_BARS to IB/TWS */
bool ib_msg_request_realtime_bars::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}
/* sends message CANCEL_REAL_TIME_BARS to IB/TWS */
bool ib_msg_cancel_realtime_bars::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	return false;
}










UNIQUE_TYPE_ID_CLASSIMPL(tier2_message, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(cancel_message, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(schedule_message, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(tws_reqresp_message, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(tws_request_message, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(tws_response_message, tier2_msg_typeid_mgr);

UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_update_mkt_depth, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_market_data_type, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_open_order, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_order_status, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_scanner_data, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_historical_data, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_update_mkt_depth_l2, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_delta_neutral_validation, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_bond_contract_details, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_scanner_data_end, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_scanner_data_start, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_tick_size, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_tick_snapshot_end, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_update_account_value, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_receive_fa, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_open_order_end, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_contract_details, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_exec_details, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_managed_accounts, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_tick_efp, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_update_account_time, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_tick_option_computation, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_tick_price, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_error, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_acct_download_end, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_commission_report, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_fundamental_data, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_realtime_bar, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_historical_data_end, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_next_valid_id, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_contract_details_end, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_update_news_bulletin, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_exec_details_end, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_tick_string, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_scanner_parameters, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_current_time, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_tick_generic, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_update_portfolio, tier2_msg_typeid_mgr);

UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_scanner_parameters, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_scanner_subscription, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_cancel_scanner_subscription, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_mkt_data, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_historical_data, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_cancel_historical_data, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_cancel_mkt_data, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_exercise_options, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_place_order, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_cancel_order, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_open_orders, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_account_updates, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_executions, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_ids, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_contract_details, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_mkt_depth, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_cancel_mkt_depth, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_news_bulletins, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_cancel_news_bulletins, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_set_server_log_level, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_auto_open_orders, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_all_open_orders, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_managed_accts, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_request_fa, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_replace_fa, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_current_time, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_fundamental_data, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_cancel_fundamental_data, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_calculate_implied_volatility, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_cancel_calculate_implied_volatility, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_calculate_option_price, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_cancel_calculate_option_price, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_global_cancel, tier2_msg_typeid_mgr);
UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_market_data_type, tier2_msg_typeid_mgr);






int ib_msg_req_scanner_parameters::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_scanner_parameters");

	// sneaky: let the response message handle itself:
	return resp_msg->process_response(this);
}

int ib_msg_req_scanner_subscription::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_scanner_subscription");

	return 0;
}

int ib_msg_cancel_scanner_subscription::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_cancel_scanner_subscription");

	return 0;
}

int ib_msg_req_mkt_data::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_mkt_data");

	return 0;
}

int ib_msg_req_historical_data::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_historical_data");

	return 0;
}

int ib_msg_cancel_historical_data::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_cancel_historical_data");

	return 0;
}

int ib_msg_cancel_mkt_data::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_cancel_mkt_data");

	return 0;
}

int ib_msg_exercise_options::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_exercise_options");

	return 0;
}

int ib_msg_place_order::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_place_order");

	return 0;
}

int ib_msg_cancel_order::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_cancel_order");

	return 0;
}

int ib_msg_req_open_orders::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_open_orders");

	return 0;
}

int ib_msg_req_account_updates::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_account_updates");

	return 0;
}

int ib_msg_req_executions::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_executions");

	return 0;
}

int ib_msg_req_ids::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_ids");

	return 0;
}

int ib_msg_req_contract_details::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_contract_details");

	return 0;
}

int ib_msg_req_mkt_depth::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_mkt_depth");

	return 0;
}

int ib_msg_cancel_mkt_depth::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_cancel_mkt_depth");

	return 0;
}

int ib_msg_req_news_bulletins::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_news_bulletins");

	return 0;
}

int ib_msg_cancel_news_bulletins::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_cancel_news_bulletins");

	return 0;
}

int ib_msg_set_server_log_level::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_set_server_log_level");

	return 0;
}

int ib_msg_req_auto_open_orders::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_auto_open_orders");

	return 0;
}

int ib_msg_req_all_open_orders::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_all_open_orders");

	return 0;
}

int ib_msg_req_managed_accts::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_managed_accts");

	return 0;
}

int ib_msg_request_fa::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_request_fa");

	return 0;
}

int ib_msg_replace_fa::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_replace_fa");

	return 0;
}

int ib_msg_req_current_time::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_current_time");

	return 0;
}

int ib_msg_req_fundamental_data::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_fundamental_data");

	return 0;
}

int ib_msg_cancel_fundamental_data::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_cancel_fundamental_data");

	return 0;
}

int ib_msg_calculate_implied_volatility::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_calculate_implied_volatility");

	return 0;
}

int ib_msg_cancel_calculate_implied_volatility::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_cancel_calculate_implied_volatility");

	return 0;
}

int ib_msg_calculate_option_price::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_calculate_option_price");

	return 0;
}

int ib_msg_cancel_calculate_option_price::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_cancel_calculate_option_price");

	return 0;
}

int ib_msg_req_global_cancel::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_global_cancel");

	return 0;
}

int ib_msg_req_market_data_type::process_response(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_market_data_type");

	return 0;
}




