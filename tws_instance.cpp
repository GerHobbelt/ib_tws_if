
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

#include "system-includes.h"

#include "tws_instance.h"

#include "app_manager.h"
#include "tier2_message_processor.h"
#include "tws_request.h"
#include "tws_response.h"

#include <mongoose/mongoose_ex.h>




ib_tws_manager::ib_tws_manager(app_manager *mgr) :
	app_mgr(mgr),
	tws_conn(NULL), tws_ctx(NULL), tws_handle(NULL), 
	fake_ib_tws_connection(true),
	next_order_id(0),
	req_scanner_parameters_active_set(),
	req_scanner_subscription_active_set(10 /* limit imposed by TWS/IB */),
	req_mkt_data_active_set(),
	req_historical_data_active_set(),
	exercise_options_active_set(),
	place_order_active_set(),
	req_open_orders_active_set(),
	req_account_updates_active_set(),
	req_executions_active_set(),
	req_ids_active_set(),
	req_contract_details_active_set(),
	req_mkt_depth_active_set(),
	req_news_bulletins_active_set(),
	set_server_log_level_active_set(),
	req_auto_open_orders_active_set(),
	req_all_open_orders_active_set(),
	req_managed_accts_active_set(),
	request_fa_active_set(),
	replace_fa_active_set(),
	req_current_time_active_set(),
	req_fundamental_data_active_set(),
	calculate_implied_volatility_active_set(),
	calculate_option_price_active_set(),
	req_market_data_type_active_set(),
	request_realtime_bars_active_set()
{
	fake_conn[0] = NULL;
	fake_conn[1] = NULL;
}

ib_tws_manager::~ib_tws_manager()
{
}




tier2_message_processor *ib_tws_manager::get_receiver(void)
{
	app_manager *mgr = get_app_manager();
	mg_context *ctx = get_context();
	mg_connection *conn = get_connection();

	tier2_message_processor *rv = mgr->get_receiver(ctx, app_manager::IB_TWS_API_CONNECTION_THREAD);
	assert(rv);
	return rv;
}










/* sends message REQ_SCANNER_PARAMETERS to IB/TWS */
int ib_tws_manager::tx_request_scanner_parameters(ib_msg_req_scanner_parameters *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message REQ_SCANNER_SUBSCRIPTION to IB/TWS */
int ib_tws_manager::tx_request_scanner_subscription(ib_msg_req_scanner_subscription *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message CANCEL_SCANNER_SUBSCRIPTION to IB/TWS */
int ib_tws_manager::tx_cancel_scanner_subscription(ib_msg_cancel_scanner_subscription *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message REQ_MKT_DATA to IB/TWS */
int ib_tws_manager::tx_request_mkt_data(ib_msg_req_mkt_data *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message REQ_HISTORICAL_DATA to IB/TWS */
int ib_tws_manager::tx_request_historical_data(ib_msg_req_historical_data *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message CANCEL_HISTORICAL_DATA to IB/TWS */
int ib_tws_manager::tx_cancel_historical_data(ib_msg_cancel_historical_data *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message CANCEL_MKT_DATA to IB/TWS */
int ib_tws_manager::tx_cancel_mkt_data(ib_msg_cancel_mkt_data *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message EXERCISE_OPTIONS to IB/TWS */
int ib_tws_manager::tx_exercise_options(ib_msg_exercise_options *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message PLACE_ORDER to IB/TWS */
int ib_tws_manager::tx_place_order(ib_msg_place_order *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message CANCEL_ORDER to IB/TWS */
int ib_tws_manager::tx_cancel_order(ib_msg_cancel_order *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message REQ_OPEN_ORDERS to IB/TWS */
int ib_tws_manager::tx_request_open_orders(ib_msg_req_open_orders *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message REQ_ACCOUNT_DATA to IB/TWS */
int ib_tws_manager::tx_request_account_updates(ib_msg_req_account_updates *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message REQ_EXECUTIONS to IB/TWS */
int ib_tws_manager::tx_request_executions(ib_msg_req_executions *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message REQ_IDS to IB/TWS */
int ib_tws_manager::tx_request_ids(ib_msg_req_ids *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message REQ_CONTRACT_DATA to IB/TWS */
int ib_tws_manager::tx_request_contract_details(ib_msg_req_contract_details *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message REQ_MKT_DEPTH to IB/TWS */
int ib_tws_manager::tx_request_mkt_depth(ib_msg_req_mkt_depth *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message CANCEL_MKT_DEPTH to IB/TWS */
int ib_tws_manager::tx_cancel_mkt_depth(ib_msg_cancel_mkt_depth *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message REQ_NEWS_BULLETINS to IB/TWS */
int ib_tws_manager::tx_request_news_bulletins(ib_msg_req_news_bulletins *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message CANCEL_NEWS_BULLETINS to IB/TWS */
int ib_tws_manager::tx_cancel_news_bulletins(ib_msg_cancel_news_bulletins *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message SET_SERVER_LOGLEVEL to IB/TWS */
int ib_tws_manager::tx_set_server_log_level(ib_msg_set_server_log_level *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message REQ_AUTO_OPEN_ORDERS to IB/TWS */
int ib_tws_manager::tx_request_auto_open_orders(ib_msg_req_auto_open_orders *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message REQ_ALL_OPEN_ORDERS to IB/TWS */
int ib_tws_manager::tx_request_all_open_orders(ib_msg_req_all_open_orders *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message REQ_MANAGED_ACCTS to IB/TWS */
int ib_tws_manager::tx_request_managed_accts(ib_msg_req_managed_accts *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message REQ_FA to IB/TWS */
int ib_tws_manager::tx_request_fa(ib_msg_request_fa *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message REPLACE_FA to IB/TWS */
int ib_tws_manager::tx_replace_fa(ib_msg_replace_fa *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message REQ_CURRENT_TIME to IB/TWS */
int ib_tws_manager::tx_request_current_time(ib_msg_req_current_time *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message REQ_FUNDAMENTAL_DATA to IB/TWS */
int ib_tws_manager::tx_request_fundamental_data(ib_msg_req_fundamental_data *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message CANCEL_FUNDAMENTAL_DATA to IB/TWS */
int ib_tws_manager::tx_cancel_fundamental_data(ib_msg_cancel_fundamental_data *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message REQ_CALC_IMPLIED_VOLAT to IB/TWS */
int ib_tws_manager::tx_calculate_implied_volatility(ib_msg_calculate_implied_volatility *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message CANCEL_CALC_IMPLIED_VOLAT to IB/TWS */
int ib_tws_manager::tx_cancel_calculate_implied_volatility(ib_msg_cancel_calculate_implied_volatility *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message REQ_CALC_OPTION_PRICE to IB/TWS */
int ib_tws_manager::tx_calculate_option_price(ib_msg_calculate_option_price *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message CANCEL_CALC_OPTION_PRICE to IB/TWS */
int ib_tws_manager::tx_cancel_calculate_option_price(ib_msg_cancel_calculate_option_price *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message REQ_GLOBAL_CANCEL to IB/TWS */
int ib_tws_manager::tx_request_global_cancel(ib_msg_req_global_cancel *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message REQ_MARKET_DATA_TYPE to IB/TWS */
int ib_tws_manager::tx_request_market_data_type(ib_msg_req_market_data_type *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message REQ_REAL_TIME_BARS to IB/TWS */
int ib_tws_manager::tx_request_realtime_bars(ib_msg_request_realtime_bars *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}
/* sends message CANCEL_REAL_TIME_BARS to IB/TWS */
int ib_tws_manager::tx_cancel_realtime_bars(ib_msg_cancel_realtime_bars *req_msg)
{
	int rv = req_msg->tx(get_tws_instance());

	return rv;
}















/* fired by: TICK_PRICE */
int ib_tws_manager::process_response_message(ib_msg_resp_tick_price *resp_msg)
{
	return 0;
}
/* fired by: TICK_PRICE (for modern versions, then immediately preceeded by an invocation of event_tick_price()), TICK_SIZE */
int ib_tws_manager::process_response_message(ib_msg_resp_tick_size *resp_msg)
{
	return 0;
}
/* fired by: TICK_OPTION_COMPUTATION */
int ib_tws_manager::process_response_message(ib_msg_resp_tick_option_computation *resp_msg)
{
	return 0;
}
/* fired by: TICK_GENERIC */
int ib_tws_manager::process_response_message(ib_msg_resp_tick_generic *resp_msg)
{
	return 0;
}
/* fired by: TICK_STRING */
int ib_tws_manager::process_response_message(ib_msg_resp_tick_string *resp_msg)
{
	return 0;
}
/* fired by: TICK_EFP */
int ib_tws_manager::process_response_message(ib_msg_resp_tick_efp *resp_msg)
{
	return 0;
}
/* fired by: ORDER_STATUS */
int ib_tws_manager::process_response_message(ib_msg_resp_order_status *resp_msg)
{
	return 0;
}
/* fired by: OPEN_ORDER */
int ib_tws_manager::process_response_message(ib_msg_resp_open_order *resp_msg)
{
	return 0;
}
/* fired by: OPEN_ORDER_END */
int ib_tws_manager::process_response_message(ib_msg_resp_open_order_end *resp_msg)
{
	return 0;
}
/* fired by: ACCT_VALUE */
int ib_tws_manager::process_response_message(ib_msg_resp_update_account_value *resp_msg)
{
	return 0;
}
/* fired by: PORTFOLIO_VALUE */
int ib_tws_manager::process_response_message(ib_msg_resp_update_portfolio *resp_msg)
{
	return 0;
}
/* fired by: ACCT_UPDATE_TIME */
int ib_tws_manager::process_response_message(ib_msg_resp_update_account_time *resp_msg)
{
	return 0;
}
/* fired by: NEXT_VALID_ID */
int ib_tws_manager::process_response_message(ib_msg_resp_next_valid_id *resp_msg)
{
	return 0;
}
/* fired by: CONTRACT_DATA */
int ib_tws_manager::process_response_message(ib_msg_resp_contract_details *resp_msg)
{
	return 0;
}
/* fired by: CONTRACT_DATA_END */
int ib_tws_manager::process_response_message(ib_msg_resp_contract_details_end *resp_msg)
{
	return 0;
}
/* fired by: BOND_CONTRACT_DATA */
int ib_tws_manager::process_response_message(ib_msg_resp_bond_contract_details *resp_msg)
{
	return 0;
}
/* fired by: EXECUTION_DATA */
int ib_tws_manager::process_response_message(ib_msg_resp_exec_details *resp_msg)
{
	return 0;
}
/* fired by: EXECUTION_DATA_END */
int ib_tws_manager::process_response_message(ib_msg_resp_exec_details_end *resp_msg)
{
	return 0;
}
/* fired by: ERR_MSG */
int ib_tws_manager::process_response_message(ib_msg_resp_error *resp_msg)
{
	return 0;
}
/* fired by: MARKET_DEPTH */
int ib_tws_manager::process_response_message(ib_msg_resp_update_mkt_depth *resp_msg)
{
	return 0;
}
/* fired by: MARKET_DEPTH_L2 */
int ib_tws_manager::process_response_message(ib_msg_resp_update_mkt_depth_l2 *resp_msg)
{
	return 0;
}
/* fired by: NEWS_BULLETINS */
int ib_tws_manager::process_response_message(ib_msg_resp_update_news_bulletin *resp_msg)
{
	return 0;
}
/* fired by: MANAGED_ACCTS */
int ib_tws_manager::process_response_message(ib_msg_resp_managed_accounts *resp_msg)
{
	return 0;
}
/* fired by: RECEIVE_FA */
int ib_tws_manager::process_response_message(ib_msg_resp_receive_fa *resp_msg)
{
	return 0;
}
/* fired by: HISTORICAL_DATA (possibly multiple times per incoming message) */
int ib_tws_manager::process_response_message(ib_msg_resp_historical_data *resp_msg)
{
	return 0;
}
/* fired by: HISTORICAL_DATA  (once, after one or more invocations of event_historical_data()) */
int ib_tws_manager::process_response_message(ib_msg_resp_historical_data_end *resp_msg)
{
	return 0;
}
/* fired by: SCANNER_PARAMETERS */
int ib_tws_manager::process_response_message(ib_msg_resp_scanner_parameters *resp_msg)
{
	return 0;
}
/* fired by: SCANNER_DATA (possibly multiple times per incoming message) */
int ib_tws_manager::process_response_message(ib_msg_resp_scanner_data *resp_msg)
{
	return 0;
}
/* fired by: SCANNER_DATA (once, after one or more invocations of event_scanner_data()) */
int ib_tws_manager::process_response_message(ib_msg_resp_scanner_data_end *resp_msg)
{
	return 0;
}
/* fired by: SCANNER_DATA (once, before any invocations of event_scanner_data()) */
int ib_tws_manager::process_response_message(ib_msg_resp_scanner_data_start *resp_msg)
{
	return 0;
}
/* fired by: CURRENT_TIME */
int ib_tws_manager::process_response_message(ib_msg_resp_current_time *resp_msg)
{
	return 0;
}
/* fired by: REAL_TIME_BARS */
int ib_tws_manager::process_response_message(ib_msg_resp_realtime_bar *resp_msg)
{
	return 0;
}
/* fired by: FUNDAMENTAL_DATA */
int ib_tws_manager::process_response_message(ib_msg_resp_fundamental_data *resp_msg)
{
	return 0;
}
/* fired by: DELTA_NEUTRAL_VALIDATION */
int ib_tws_manager::process_response_message(ib_msg_resp_delta_neutral_validation *resp_msg)
{
	return 0;
}
/* fired by: ACCT_DOWNLOAD_END */
int ib_tws_manager::process_response_message(ib_msg_resp_acct_download_end *resp_msg)
{
	return 0;
}
/* fired by: TICK_SNAPSHOT_END */
int ib_tws_manager::process_response_message(ib_msg_resp_tick_snapshot_end *resp_msg)
{
	return 0;
}
/* fired by: MARKET_DATA_TYPE */
int ib_tws_manager::process_response_message(ib_msg_resp_market_data_type *resp_msg)
{
	return 0;
}
/* fired by: COMMISSION_REPORT */
int ib_tws_manager::process_response_message(ib_msg_resp_commission_report *resp_msg)
{
	return 0;
}

