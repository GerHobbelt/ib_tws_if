
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
	app_manager *mgr = owner->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();

	return ibm->tx_request_scanner_subscription(this);
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














/* sends message REQ_SCANNER_PARAMETERS to IB/TWS */
int ib_msg_req_scanner_parameters::f_commence_transmit(void)
{
	app_manager *mgr = owner->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();

	return tx(ibm->get_tws_instance());
}

/* sends message REQ_SCANNER_SUBSCRIPTION to IB/TWS */
int ib_msg_req_scanner_subscription::f_commence_transmit(void)
{
	app_manager *mgr = owner->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();

	return tx(ibm->get_tws_instance());
}

/* sends message CANCEL_SCANNER_SUBSCRIPTION to IB/TWS */
int ib_msg_cancel_scanner_subscription::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_MKT_DATA to IB/TWS */
int ib_msg_req_mkt_data::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_HISTORICAL_DATA to IB/TWS */
int ib_msg_req_historical_data::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message CANCEL_HISTORICAL_DATA to IB/TWS */
int ib_msg_cancel_historical_data::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message CANCEL_MKT_DATA to IB/TWS */
int ib_msg_cancel_mkt_data::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message EXERCISE_OPTIONS to IB/TWS */
int ib_msg_exercise_options::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message PLACE_ORDER to IB/TWS */
int ib_msg_place_order::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message CANCEL_ORDER to IB/TWS */
int ib_msg_cancel_order::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_OPEN_ORDERS to IB/TWS */
int ib_msg_req_open_orders::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_ACCOUNT_DATA to IB/TWS */
int ib_msg_req_account_updates::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_EXECUTIONS to IB/TWS */
int ib_msg_req_executions::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_IDS to IB/TWS */
int ib_msg_req_ids::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_CONTRACT_DATA to IB/TWS */
int ib_msg_req_contract_details::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_MKT_DEPTH to IB/TWS */
int ib_msg_req_mkt_depth::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message CANCEL_MKT_DEPTH to IB/TWS */
int ib_msg_cancel_mkt_depth::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_NEWS_BULLETINS to IB/TWS */
int ib_msg_req_news_bulletins::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message CANCEL_NEWS_BULLETINS to IB/TWS */
int ib_msg_cancel_news_bulletins::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message SET_SERVER_LOGLEVEL to IB/TWS */
int ib_msg_set_server_log_level::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_AUTO_OPEN_ORDERS to IB/TWS */
int ib_msg_req_auto_open_orders::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_ALL_OPEN_ORDERS to IB/TWS */
int ib_msg_req_all_open_orders::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_MANAGED_ACCTS to IB/TWS */
int ib_msg_req_managed_accts::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_FA to IB/TWS */
int ib_msg_request_fa::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REPLACE_FA to IB/TWS */
int ib_msg_replace_fa::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_CURRENT_TIME to IB/TWS */
int ib_msg_req_current_time::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_FUNDAMENTAL_DATA to IB/TWS */
int ib_msg_req_fundamental_data::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message CANCEL_FUNDAMENTAL_DATA to IB/TWS */
int ib_msg_cancel_fundamental_data::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_CALC_IMPLIED_VOLAT to IB/TWS */
int ib_msg_calculate_implied_volatility::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message CANCEL_CALC_IMPLIED_VOLAT to IB/TWS */
int ib_msg_cancel_calculate_implied_volatility::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_CALC_OPTION_PRICE to IB/TWS */
int ib_msg_calculate_option_price::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message CANCEL_CALC_OPTION_PRICE to IB/TWS */
int ib_msg_cancel_calculate_option_price::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_GLOBAL_CANCEL to IB/TWS */
int ib_msg_req_global_cancel::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_MARKET_DATA_TYPE to IB/TWS */
int ib_msg_req_market_data_type::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message REQ_REAL_TIME_BARS to IB/TWS */
int ib_msg_request_realtime_bars::f_commence_transmit(void)
{
	assert(!"Should never get here");
	return 0;
}

/* sends message CANCEL_REAL_TIME_BARS to IB/TWS */
int ib_msg_cancel_realtime_bars::f_commence_transmit(void)
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

	m_server_version = tws::tws_server_version(tws);
	m_connect_timestamp = tws::tws_connection_time(tws);

	// and return an immediate response...
	state(RESPONSE_COMPLETE);

	return 0;
}







































/* sends message REQ_SCANNER_PARAMETERS to IB/TWS */
int ib_msg_req_scanner_parameters::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_req_scanner_parameters(tws);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message REQ_SCANNER_SUBSCRIPTION to IB/TWS */
int ib_msg_req_scanner_subscription::tx(tws::tws_instance_t *tws)
{
	int rv;

	m_subscription.prep_for_tws(tws);
	rv = tws::tws_req_scanner_subscription(tws, m_ticker_id, m_subscription.to_tws());
	m_subscription.cleanup_after_tws(tws);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message CANCEL_SCANNER_SUBSCRIPTION to IB/TWS */
int ib_msg_cancel_scanner_subscription::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_cancel_scanner_subscription(tws, m_ticker_id);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message REQ_MKT_DATA to IB/TWS */
int ib_msg_req_mkt_data::tx(tws::tws_instance_t *tws)
{
	m_contract.prep_for_tws(tws);
	// http://stackoverflow.com/questions/584824/guaranteed-lifetime-of-temporary-in-c
	int rv = tws::tws_req_mkt_data(tws, m_ticker_id, m_contract.to_tws(), ib_string_t(m_generic_tick_list), m_snapshot);
	m_contract.cleanup_after_tws(tws);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message REQ_HISTORICAL_DATA to IB/TWS */
int ib_msg_req_historical_data::tx(tws::tws_instance_t *tws)
{
	m_contract.prep_for_tws(tws);
	// http://stackoverflow.com/questions/584824/guaranteed-lifetime-of-temporary-in-c
	int rv = tws::tws_req_historical_data(tws, m_ticker_id, m_contract.to_tws(), ib_string_t(m_end_date_time), m_duration, m_bar_size_setting, m_what_to_show, m_use_rth, m_format_date);
	m_contract.cleanup_after_tws(tws);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message CANCEL_HISTORICAL_DATA to IB/TWS */
int ib_msg_cancel_historical_data::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_cancel_historical_data(tws, m_ticker_id);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message CANCEL_MKT_DATA to IB/TWS */
int ib_msg_cancel_mkt_data::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_cancel_mkt_data(tws, m_ticker_id);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message EXERCISE_OPTIONS to IB/TWS */
int ib_msg_exercise_options::tx(tws::tws_instance_t *tws)
{
	m_contract.prep_for_tws(tws);
	int rv = tws::tws_exercise_options(tws, m_ticker_id, m_contract.to_tws(), m_exercise_action, m_exercise_quantity, m_account, m_exc_override);
	m_contract.cleanup_after_tws(tws);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message PLACE_ORDER to IB/TWS */
int ib_msg_place_order::tx(tws::tws_instance_t *tws)
{
	m_contract.prep_for_tws(tws);
	m_order.prep_for_tws(tws);
	int rv = tws::tws_place_order(tws, m_order_id, m_contract.to_tws(), m_order.to_tws());
	m_order.cleanup_after_tws(tws);
	m_contract.cleanup_after_tws(tws);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message CANCEL_ORDER to IB/TWS */
int ib_msg_cancel_order::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_cancel_order(tws, m_order_id);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message REQ_OPEN_ORDERS to IB/TWS */
int ib_msg_req_open_orders::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_req_open_orders(tws);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message REQ_ACCOUNT_DATA to IB/TWS */
int ib_msg_req_account_updates::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_req_account_updates(tws, m_subscribe, m_acct_code);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message REQ_EXECUTIONS to IB/TWS */
int ib_msg_req_executions::tx(tws::tws_instance_t *tws)
{
	m_filter.prep_for_tws(tws);
	int rv = tws::tws_req_executions(tws, m_reqid, m_filter.to_tws());
	m_filter.cleanup_after_tws(tws);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message REQ_IDS to IB/TWS */
int ib_msg_req_ids::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_req_ids(tws, m_num_ids);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message REQ_CONTRACT_DATA to IB/TWS */
int ib_msg_req_contract_details::tx(tws::tws_instance_t *tws)
{
	m_contract.prep_for_tws(tws);
	int rv = tws::tws_req_contract_details(tws, m_reqid, m_contract.to_tws());
	m_contract.cleanup_after_tws(tws);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message REQ_MKT_DEPTH to IB/TWS */
int ib_msg_req_mkt_depth::tx(tws::tws_instance_t *tws)
{
	m_contract.prep_for_tws(tws);
	int rv = tws::tws_req_mkt_depth(tws, m_ticker_id, m_contract.to_tws(), m_num_rows);
	m_contract.cleanup_after_tws(tws);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message CANCEL_MKT_DEPTH to IB/TWS */
int ib_msg_cancel_mkt_depth::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_cancel_mkt_depth(tws, m_ticker_id);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message REQ_NEWS_BULLETINS to IB/TWS */
int ib_msg_req_news_bulletins::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_req_news_bulletins(tws, m_all_msgs);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message CANCEL_NEWS_BULLETINS to IB/TWS */
int ib_msg_cancel_news_bulletins::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_cancel_news_bulletins(tws);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message SET_SERVER_LOGLEVEL to IB/TWS */
int ib_msg_set_server_log_level::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_set_server_log_level(tws, m_level);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message REQ_AUTO_OPEN_ORDERS to IB/TWS */
int ib_msg_req_auto_open_orders::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_req_auto_open_orders(tws, m_auto_bind);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message REQ_ALL_OPEN_ORDERS to IB/TWS */
int ib_msg_req_all_open_orders::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_req_all_open_orders(tws);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message REQ_MANAGED_ACCTS to IB/TWS */
int ib_msg_req_managed_accts::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_req_managed_accts(tws);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message REQ_FA to IB/TWS */
int ib_msg_request_fa::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_request_fa(tws, m_fa_data_type);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message REPLACE_FA to IB/TWS */
int ib_msg_replace_fa::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_replace_fa(tws, m_fa_data_type, m_cxml);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message REQ_CURRENT_TIME to IB/TWS */
int ib_msg_req_current_time::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_req_current_time(tws);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message REQ_FUNDAMENTAL_DATA to IB/TWS */
int ib_msg_req_fundamental_data::tx(tws::tws_instance_t *tws)
{
	m_contract.prep_for_tws(tws);
	int rv = tws::tws_req_fundamental_data(tws, m_reqid, m_contract.to_tws(), m_report_type);
	m_contract.cleanup_after_tws(tws);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message CANCEL_FUNDAMENTAL_DATA to IB/TWS */
int ib_msg_cancel_fundamental_data::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_cancel_fundamental_data(tws, m_reqid);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message REQ_CALC_IMPLIED_VOLAT to IB/TWS */
int ib_msg_calculate_implied_volatility::tx(tws::tws_instance_t *tws)
{
	m_contract.prep_for_tws(tws);
	int rv = tws::tws_calculate_implied_volatility(tws, m_reqid, m_contract.to_tws(), m_option_price, m_under_price);
	m_contract.cleanup_after_tws(tws);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message CANCEL_CALC_IMPLIED_VOLAT to IB/TWS */
int ib_msg_cancel_calculate_implied_volatility::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_cancel_calculate_implied_volatility(tws, m_reqid);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message REQ_CALC_OPTION_PRICE to IB/TWS */
int ib_msg_calculate_option_price::tx(tws::tws_instance_t *tws)
{
	m_contract.prep_for_tws(tws);
	int rv = tws::tws_calculate_option_price(tws, m_reqid, m_contract.to_tws(), m_volatility, m_under_price);
	m_contract.cleanup_after_tws(tws);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message CANCEL_CALC_OPTION_PRICE to IB/TWS */
int ib_msg_cancel_calculate_option_price::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_cancel_calculate_option_price(tws, m_reqid);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message REQ_GLOBAL_CANCEL to IB/TWS */
int ib_msg_req_global_cancel::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_req_global_cancel(tws);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message REQ_MARKET_DATA_TYPE to IB/TWS */
int ib_msg_req_market_data_type::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_req_market_data_type(tws, m_market_data_type);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message REQ_REAL_TIME_BARS to IB/TWS */
int ib_msg_request_realtime_bars::tx(tws::tws_instance_t *tws)
{
	m_contract.prep_for_tws(tws);
	int rv = tws::tws_request_realtime_bars(tws, m_ticker_id, m_contract.to_tws(), m_bar_size, m_what_to_show, m_use_rth);
	m_contract.cleanup_after_tws(tws);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}
/* sends message CANCEL_REAL_TIME_BARS to IB/TWS */
int ib_msg_cancel_realtime_bars::tx(tws::tws_instance_t *tws)
{
	int rv = tws::tws_cancel_realtime_bars(tws, m_ticker_id);

	state(tier2_message::READY_TO_RECEIVE_RESPONSE);

	return rv;
}









ib_ticker_list::operator class ib_string_t(void)
{
	ib_string_t s;
	int i;

	for (i = 0; i < t_list.size(); i++)
	{
		char buf[5*(sizeof t_list[i])/2 + 2 + 1];

		sprintf(buf, ",%d", t_list[i]);
		if (s.length() == 0)
			s = buf + 1;
		else
			s.append(buf);
	}
	if (t_mdoff)
		s.append(",mdoff" + (s.length() == 0));
	return s;
}






bool tws_request_message::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	/* Assume NO by default */
	return false;
}






/* sends message REQ_SCANNER_PARAMETERS to IB/TWS */
bool ib_msg_req_scanner_parameters::response_is_meant_for_us(class tier2_message *resp_msg) const
{
	if (!ib_msg_resp_scanner_parameters::type_matches_class(resp_msg))
		return false;

	return state() == tier2_message::READY_TO_RECEIVE_RESPONSE;
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














int ib_msg_req_scanner_parameters::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_scanner_parameters");

	// sneaky: let the response message handle itself:
	return resp_msg->process_response_message(this);
}

int ib_msg_req_scanner_subscription::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_scanner_subscription");

	return 0;
}

int ib_msg_cancel_scanner_subscription::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_cancel_scanner_subscription");

	return 0;
}

int ib_msg_req_mkt_data::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_mkt_data");

	return 0;
}

int ib_msg_req_historical_data::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_historical_data");

	return 0;
}

int ib_msg_cancel_historical_data::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_cancel_historical_data");

	return 0;
}

int ib_msg_cancel_mkt_data::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_cancel_mkt_data");

	return 0;
}

int ib_msg_exercise_options::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_exercise_options");

	return 0;
}

int ib_msg_place_order::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_place_order");

	return 0;
}

int ib_msg_cancel_order::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_cancel_order");

	return 0;
}

int ib_msg_req_open_orders::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_open_orders");

	return 0;
}

int ib_msg_req_account_updates::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_account_updates");

	return 0;
}

int ib_msg_req_executions::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_executions");

	return 0;
}

int ib_msg_req_ids::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_ids");

	return 0;
}

int ib_msg_req_contract_details::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_contract_details");

	return 0;
}

int ib_msg_req_mkt_depth::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_mkt_depth");

	return 0;
}

int ib_msg_cancel_mkt_depth::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_cancel_mkt_depth");

	return 0;
}

int ib_msg_req_news_bulletins::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_news_bulletins");

	return 0;
}

int ib_msg_cancel_news_bulletins::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_cancel_news_bulletins");

	return 0;
}

int ib_msg_set_server_log_level::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_set_server_log_level");

	return 0;
}

int ib_msg_req_auto_open_orders::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_auto_open_orders");

	return 0;
}

int ib_msg_req_all_open_orders::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_all_open_orders");

	return 0;
}

int ib_msg_req_managed_accts::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_managed_accts");

	return 0;
}

int ib_msg_request_fa::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_request_fa");

	return 0;
}

int ib_msg_replace_fa::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_replace_fa");

	return 0;
}

int ib_msg_req_current_time::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_current_time");

	return 0;
}

int ib_msg_req_fundamental_data::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_fundamental_data");

	return 0;
}

int ib_msg_cancel_fundamental_data::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_cancel_fundamental_data");

	return 0;
}

int ib_msg_calculate_implied_volatility::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_calculate_implied_volatility");

	return 0;
}

int ib_msg_cancel_calculate_implied_volatility::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_cancel_calculate_implied_volatility");

	return 0;
}

int ib_msg_calculate_option_price::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_calculate_option_price");

	return 0;
}

int ib_msg_cancel_calculate_option_price::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_cancel_calculate_option_price");

	return 0;
}

int ib_msg_req_global_cancel::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_global_cancel");

	return 0;
}

int ib_msg_req_market_data_type::process_response_message(class tier2_message *resp_msg)
{
	app_manager *mgr = resp_msg->get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_req_market_data_type");

	return 0;
}




