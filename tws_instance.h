
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

#ifndef TWS_INSTANCE_CPP_HEADER_INCLUDED
#define TWS_INSTANCE_CPP_HEADER_INCLUDED

/*
The 'ib_tws_manager' represents that single (back-end) thread which can send/receive to/from this TWS instance/connection
at any time.
*/

#include "tws_config_struct.h"
#include "tws_request.h"
#include "tws_response.h"
#include "tier2_message_queue.h"
#include "unique_type_id.h"
#include "tws_backend_io.h"


// forward reference:
class app_manager;
namespace tws
{
	typedef struct tws_instance tws_instance_t;
}
class ib_backend_io_logger;



/* sends message REQ_SCANNER_PARAMETERS to IB/TWS */
class ib_msg_req_scanner_parameters;
/* sends message REQ_SCANNER_SUBSCRIPTION to IB/TWS */
class ib_msg_req_scanner_subscription;
/* sends message CANCEL_SCANNER_SUBSCRIPTION to IB/TWS */
class ib_msg_cancel_scanner_subscription;
/* sends message REQ_MKT_DATA to IB/TWS */
class ib_msg_req_mkt_data;
/* sends message REQ_HISTORICAL_DATA to IB/TWS */
class ib_msg_req_historical_data;
/* sends message CANCEL_HISTORICAL_DATA to IB/TWS */
class ib_msg_cancel_historical_data;
/* sends message CANCEL_MKT_DATA to IB/TWS */
class ib_msg_cancel_mkt_data;
/* sends message EXERCISE_OPTIONS to IB/TWS */
class ib_msg_exercise_options;
/* sends message PLACE_ORDER to IB/TWS */
class ib_msg_place_order;
/* sends message CANCEL_ORDER to IB/TWS */
class ib_msg_cancel_order;
/* sends message REQ_OPEN_ORDERS to IB/TWS */
class ib_msg_req_open_orders;
/* sends message REQ_ACCOUNT_DATA to IB/TWS */
class ib_msg_req_account_updates;
/* sends message REQ_EXECUTIONS to IB/TWS */
class ib_msg_req_executions;
/* sends message REQ_IDS to IB/TWS */
class ib_msg_req_ids;
/* sends message REQ_CONTRACT_DATA to IB/TWS */
class ib_msg_req_contract_details;
/* sends message REQ_MKT_DEPTH to IB/TWS */
class ib_msg_req_mkt_depth;
/* sends message CANCEL_MKT_DEPTH to IB/TWS */
class ib_msg_cancel_mkt_depth;
/* sends message REQ_NEWS_BULLETINS to IB/TWS */
class ib_msg_req_news_bulletins;
/* sends message CANCEL_NEWS_BULLETINS to IB/TWS */
class ib_msg_cancel_news_bulletins;
/* sends message SET_SERVER_LOGLEVEL to IB/TWS */
class ib_msg_set_server_log_level;
/* sends message REQ_AUTO_OPEN_ORDERS to IB/TWS */
class ib_msg_req_auto_open_orders;
/* sends message REQ_ALL_OPEN_ORDERS to IB/TWS */
class ib_msg_req_all_open_orders;
/* sends message REQ_MANAGED_ACCTS to IB/TWS */
class ib_msg_req_managed_accts;
/* sends message REQ_FA to IB/TWS */
class ib_msg_request_fa;
/* sends message REPLACE_FA to IB/TWS */
class ib_msg_replace_fa;
/* sends message REQ_CURRENT_TIME to IB/TWS */
class ib_msg_req_current_time;
/* sends message REQ_FUNDAMENTAL_DATA to IB/TWS */
class ib_msg_req_fundamental_data;
/* sends message CANCEL_FUNDAMENTAL_DATA to IB/TWS */
class ib_msg_cancel_fundamental_data;
/* sends message REQ_CALC_IMPLIED_VOLAT to IB/TWS */
class ib_msg_calculate_implied_volatility;
/* sends message CANCEL_CALC_IMPLIED_VOLAT to IB/TWS */
class ib_msg_cancel_calculate_implied_volatility;
/* sends message REQ_CALC_OPTION_PRICE to IB/TWS */
class ib_msg_calculate_option_price;
/* sends message CANCEL_CALC_OPTION_PRICE to IB/TWS */
class ib_msg_cancel_calculate_option_price;
/* sends message REQ_GLOBAL_CANCEL to IB/TWS */
class ib_msg_req_global_cancel;
/* sends message REQ_MARKET_DATA_TYPE to IB/TWS */
class ib_msg_req_market_data_type;
/* sends message REQ_REAL_TIME_BARS to IB/TWS */
class ib_msg_request_realtime_bars;
/* sends message CANCEL_REAL_TIME_BARS to IB/TWS */
class ib_msg_cancel_realtime_bars;


/* fired by: TICK_PRICE */
class ib_msg_resp_tick_price;
/* fired by: TICK_PRICE (for modern versions, then immediately preceeded by an invocation of event_tick_price()), TICK_SIZE */
class ib_msg_resp_tick_size;
/* fired by: TICK_OPTION_COMPUTATION */
class ib_msg_resp_tick_option_computation;
/* fired by: TICK_GENERIC */
class ib_msg_resp_tick_generic;
/* fired by: TICK_STRING */
class ib_msg_resp_tick_string;
/* fired by: TICK_EFP */
class ib_msg_resp_tick_efp;
/* fired by: ORDER_STATUS */
class ib_msg_resp_order_status;
/* fired by: OPEN_ORDER */
class ib_msg_resp_open_order;
/* fired by: OPEN_ORDER_END */
class ib_msg_resp_open_order_end;
/* fired by: ACCT_VALUE */
class ib_msg_resp_update_account_value;
/* fired by: PORTFOLIO_VALUE */
class ib_msg_resp_update_portfolio;
/* fired by: ACCT_UPDATE_TIME */
class ib_msg_resp_update_account_time;
/* fired by: NEXT_VALID_ID */
class ib_msg_resp_next_valid_id;
/* fired by: CONTRACT_DATA */
class ib_msg_resp_contract_details;
/* fired by: CONTRACT_DATA_END */
class ib_msg_resp_contract_details_end;
/* fired by: BOND_CONTRACT_DATA */
class ib_msg_resp_bond_contract_details;
/* fired by: EXECUTION_DATA */
class ib_msg_resp_exec_details;
/* fired by: EXECUTION_DATA_END */
class ib_msg_resp_exec_details_end;
/* fired by: ERR_MSG */
class ib_msg_resp_error;
/* fired by: MARKET_DEPTH */
class ib_msg_resp_update_mkt_depth;
/* fired by: MARKET_DEPTH_L2 */
class ib_msg_resp_update_mkt_depth_l2;
/* fired by: NEWS_BULLETINS */
class ib_msg_resp_update_news_bulletin;
/* fired by: MANAGED_ACCTS */
class ib_msg_resp_managed_accounts;
/* fired by: RECEIVE_FA */
class ib_msg_resp_receive_fa;
/* fired by: HISTORICAL_DATA (possibly multiple times per incoming message) */
class ib_msg_resp_historical_data;
/* fired by: HISTORICAL_DATA  (once, after one or more invocations of event_historical_data()) */
class ib_msg_resp_historical_data_end;
/* fired by: SCANNER_PARAMETERS */
class ib_msg_resp_scanner_parameters;
/* fired by: SCANNER_DATA (possibly multiple times per incoming message) */
class ib_msg_resp_scanner_data;
/* fired by: SCANNER_DATA (once, after one or more invocations of event_scanner_data()) */
class ib_msg_resp_scanner_data_end;
/* fired by: SCANNER_DATA (once, before any invocations of event_scanner_data()) */
class ib_msg_resp_scanner_data_start;
/* fired by: CURRENT_TIME -- in response to REQ_CURRENT_TIME */
class ib_msg_resp_current_time;
/* fired by: REAL_TIME_BARS */
class ib_msg_resp_realtime_bar;
/* fired by: FUNDAMENTAL_DATA */
class ib_msg_resp_fundamental_data;
/* fired by: DELTA_NEUTRAL_VALIDATION */
class ib_msg_resp_delta_neutral_validation;
/* fired by: ACCT_DOWNLOAD_END */
class ib_msg_resp_acct_download_end;
/* fired by: TICK_SNAPSHOT_END */
class ib_msg_resp_tick_snapshot_end;
/* fired by: MARKET_DATA_TYPE */
class ib_msg_resp_market_data_type;
/* fired by: COMMISSION_REPORT */
class ib_msg_resp_commission_report;







class ib_tws_scanner_subscription_limitation: public tier2_message_state_change_handler
{
protected:
	std::vector<tier2_message *> m_active_scanner_subscriptions;
	int m_max_scanner_subscriptions;

public:
	ib_tws_scanner_subscription_limitation()
		: m_active_scanner_subscriptions()
		, m_max_scanner_subscriptions(10 /* limit imposed by TWS/IB */)
	{
	}
	virtual ~ib_tws_scanner_subscription_limitation()
	{
	}

public:
	virtual tier2_message::state_change process(tier2_message &msg, tier2_message::request_state_t new_state);
};






class ib_tws_manager : public ib_backend_io_channel
{
	// overrides:
public: 
	/* open callback is invoked when tws_connect is invoked and no connection has been established yet (tws_connected() == false); return 0 on success; a twsclient_error_codes error code on failure. */
	virtual int io_open(void);
	/* close callback is invoked on error or when tws_disconnect is invoked */
	virtual int io_close(void);

	virtual int destroy(void);

protected:
	ib_backend_io_logger *m_io_logger;

    /* tracking some TWS values here as well: */
    unique_type_id_manager next_order_id;		// dual use: orders and tickers, so that all ids we send to/use with IB/TWS are unique

	/* the connection pairs used to communicate between front-end threads and tier2/TWS threads: */
	//hash_map

    /* -- and the working men -- */
	typedef std::vector<tier2_message *> store_t;
	store_t m_msg_queue;

	ib_tws_scanner_subscription_limitation m_scanner_subscription_limit;

protected:
	ib_tws_manager(app_manager *mgr);
public:
	virtual ~ib_tws_manager();
	static ib_tws_manager *get_instance(app_manager *mgr, bool instantiate_singleton = false);

public:
	int set_next_order_id(int id)
	{
		// only allow the ID to be bumped UPWARDS
		return next_order_id.update_unique_id(id);
	}
	int get_next_order_id(void)
	{
		return next_order_id.obtain_unique_id();
	}

	int get_next_ticker_id(void)
	{
		return next_order_id.obtain_unique_id();
	}

	int init_tws_api(void);
	int exit_tws_api(void);

	int is_tws_connected(void);
	int process_tws_event(void);

	const char *strerror(int errcode);

public:
	/* sends message REQ_SCANNER_PARAMETERS to IB/TWS */
	int tx_request_scanner_parameters(ib_msg_req_scanner_parameters *req_msg);
	/* sends message REQ_SCANNER_SUBSCRIPTION to IB/TWS */
	int tx_request_scanner_subscription(ib_msg_req_scanner_subscription *req_msg);
	/* sends message CANCEL_SCANNER_SUBSCRIPTION to IB/TWS */
	int tx_cancel_scanner_subscription(ib_msg_cancel_scanner_subscription *req_msg);
	/* sends message REQ_MKT_DATA to IB/TWS */
	int tx_request_mkt_data(ib_msg_req_mkt_data *req_msg);
	/* sends message REQ_HISTORICAL_DATA to IB/TWS */
	int tx_request_historical_data(ib_msg_req_historical_data *req_msg);
	/* sends message CANCEL_HISTORICAL_DATA to IB/TWS */
	int tx_cancel_historical_data(ib_msg_cancel_historical_data *req_msg);
	/* sends message CANCEL_MKT_DATA to IB/TWS */
	int tx_cancel_mkt_data(ib_msg_cancel_mkt_data *req_msg);
	/* sends message EXERCISE_OPTIONS to IB/TWS */
	int tx_exercise_options(ib_msg_exercise_options *req_msg);
	/* sends message PLACE_ORDER to IB/TWS */
	int tx_place_order(ib_msg_place_order *req_msg);
	/* sends message CANCEL_ORDER to IB/TWS */
	int tx_cancel_order(ib_msg_cancel_order *req_msg);
	/* sends message REQ_OPEN_ORDERS to IB/TWS */
	int tx_request_open_orders(ib_msg_req_open_orders *req_msg);
	/* sends message REQ_ACCOUNT_DATA to IB/TWS */
	int tx_request_account_updates(ib_msg_req_account_updates *req_msg);
	/* sends message REQ_EXECUTIONS to IB/TWS */
	int tx_request_executions(ib_msg_req_executions *req_msg);
	/* sends message REQ_IDS to IB/TWS */
	int tx_request_ids(ib_msg_req_ids *req_msg);
	/* sends message REQ_CONTRACT_DATA to IB/TWS */
	int tx_request_contract_details(ib_msg_req_contract_details *req_msg);
	/* sends message REQ_MKT_DEPTH to IB/TWS */
	int tx_request_mkt_depth(ib_msg_req_mkt_depth *req_msg);
	/* sends message CANCEL_MKT_DEPTH to IB/TWS */
	int tx_cancel_mkt_depth(ib_msg_cancel_mkt_depth *req_msg);
	/* sends message REQ_NEWS_BULLETINS to IB/TWS */
	int tx_request_news_bulletins(ib_msg_req_news_bulletins *req_msg);
	/* sends message CANCEL_NEWS_BULLETINS to IB/TWS */
	int tx_cancel_news_bulletins(ib_msg_cancel_news_bulletins *req_msg);
	/* sends message SET_SERVER_LOGLEVEL to IB/TWS */
	int tx_set_server_log_level(ib_msg_set_server_log_level *req_msg);
	/* sends message REQ_AUTO_OPEN_ORDERS to IB/TWS */
	int tx_request_auto_open_orders(ib_msg_req_auto_open_orders *req_msg);
	/* sends message REQ_ALL_OPEN_ORDERS to IB/TWS */
	int tx_request_all_open_orders(ib_msg_req_all_open_orders *req_msg);
	/* sends message REQ_MANAGED_ACCTS to IB/TWS */
	int tx_request_managed_accts(ib_msg_req_managed_accts *req_msg);
	/* sends message REQ_FA to IB/TWS */
	int tx_request_fa(ib_msg_request_fa *req_msg);
	/* sends message REPLACE_FA to IB/TWS */
	int tx_replace_fa(ib_msg_replace_fa *req_msg);
	/* sends message REQ_CURRENT_TIME to IB/TWS */
	int tx_request_current_time(ib_msg_req_current_time *req_msg);
	/* sends message REQ_FUNDAMENTAL_DATA to IB/TWS */
	int tx_request_fundamental_data(ib_msg_req_fundamental_data *req_msg);
	/* sends message CANCEL_FUNDAMENTAL_DATA to IB/TWS */
	int tx_cancel_fundamental_data(ib_msg_cancel_fundamental_data *req_msg);
	/* sends message REQ_CALC_IMPLIED_VOLAT to IB/TWS */
	int tx_calculate_implied_volatility(ib_msg_calculate_implied_volatility *req_msg);
	/* sends message CANCEL_CALC_IMPLIED_VOLAT to IB/TWS */
	int tx_cancel_calculate_implied_volatility(ib_msg_cancel_calculate_implied_volatility *req_msg);
	/* sends message REQ_CALC_OPTION_PRICE to IB/TWS */
	int tx_calculate_option_price(ib_msg_calculate_option_price *req_msg);
	/* sends message CANCEL_CALC_OPTION_PRICE to IB/TWS */
	int tx_cancel_calculate_option_price(ib_msg_cancel_calculate_option_price *req_msg);
	/* sends message REQ_GLOBAL_CANCEL to IB/TWS */
	int tx_request_global_cancel(ib_msg_req_global_cancel *req_msg);
	/* sends message REQ_MARKET_DATA_TYPE to IB/TWS */
	int tx_request_market_data_type(ib_msg_req_market_data_type *req_msg);
	/* sends message REQ_REAL_TIME_BARS to IB/TWS */
	int tx_request_realtime_bars(ib_msg_request_realtime_bars *req_msg);
	/* sends message CANCEL_REAL_TIME_BARS to IB/TWS */
	int tx_cancel_realtime_bars(ib_msg_cancel_realtime_bars *req_msg);

public:
	/* fired by: TICK_PRICE */
	int process_response_message(ib_msg_resp_tick_price *resp_msg);
	/* fired by: TICK_PRICE (for modern versions, then immediately preceeded by an invocation of event_tick_price()), TICK_SIZE */
	int process_response_message(ib_msg_resp_tick_size *resp_msg);
	/* fired by: TICK_OPTION_COMPUTATION */
	int process_response_message(ib_msg_resp_tick_option_computation *resp_msg);
	/* fired by: TICK_GENERIC */
	int process_response_message(ib_msg_resp_tick_generic *resp_msg);
	/* fired by: TICK_STRING */
	int process_response_message(ib_msg_resp_tick_string *resp_msg);
	/* fired by: TICK_EFP */
	int process_response_message(ib_msg_resp_tick_efp *resp_msg);
	/* fired by: ORDER_STATUS */
	int process_response_message(ib_msg_resp_order_status *resp_msg);
	/* fired by: OPEN_ORDER */
	int process_response_message(ib_msg_resp_open_order *resp_msg);
	/* fired by: OPEN_ORDER_END */
	int process_response_message(ib_msg_resp_open_order_end *resp_msg);
	/* fired by: ACCT_VALUE */
	int process_response_message(ib_msg_resp_update_account_value *resp_msg);
	/* fired by: PORTFOLIO_VALUE */
	int process_response_message(ib_msg_resp_update_portfolio *resp_msg);
	/* fired by: ACCT_UPDATE_TIME */
	int process_response_message(ib_msg_resp_update_account_time *resp_msg);
	/* fired by: NEXT_VALID_ID */
	int process_response_message(ib_msg_resp_next_valid_id *resp_msg);
	/* fired by: CONTRACT_DATA */
	int process_response_message(ib_msg_resp_contract_details *resp_msg);
	/* fired by: CONTRACT_DATA_END */
	int process_response_message(ib_msg_resp_contract_details_end *resp_msg);
	/* fired by: BOND_CONTRACT_DATA */
	int process_response_message(ib_msg_resp_bond_contract_details *resp_msg);
	/* fired by: EXECUTION_DATA */
	int process_response_message(ib_msg_resp_exec_details *resp_msg);
	/* fired by: EXECUTION_DATA_END */
	int process_response_message(ib_msg_resp_exec_details_end *resp_msg);
	/* fired by: ERR_MSG */
	int process_response_message(ib_msg_resp_error *resp_msg);
	/* fired by: MARKET_DEPTH */
	int process_response_message(ib_msg_resp_update_mkt_depth *resp_msg);
	/* fired by: MARKET_DEPTH_L2 */
	int process_response_message(ib_msg_resp_update_mkt_depth_l2 *resp_msg);
	/* fired by: NEWS_BULLETINS */
	int process_response_message(ib_msg_resp_update_news_bulletin *resp_msg);
	/* fired by: MANAGED_ACCTS */
	int process_response_message(ib_msg_resp_managed_accounts *resp_msg);
	/* fired by: RECEIVE_FA */
	int process_response_message(ib_msg_resp_receive_fa *resp_msg);
	/* fired by: HISTORICAL_DATA (possibly multiple times per incoming message) */
	int process_response_message(ib_msg_resp_historical_data *resp_msg);
	/* fired by: HISTORICAL_DATA  (once, after one or more invocations of event_historical_data()) */
	int process_response_message(ib_msg_resp_historical_data_end *resp_msg);
	/* fired by: SCANNER_PARAMETERS */
	int process_response_message(ib_msg_resp_scanner_parameters *resp_msg);
	/* fired by: SCANNER_DATA (possibly multiple times per incoming message) */
	int process_response_message(ib_msg_resp_scanner_data *resp_msg);
	/* fired by: SCANNER_DATA (once, after one or more invocations of event_scanner_data()) */
	int process_response_message(ib_msg_resp_scanner_data_end *resp_msg);
	/* fired by: SCANNER_DATA (once, before any invocations of event_scanner_data()) */
	int process_response_message(ib_msg_resp_scanner_data_start *resp_msg);
	/* fired by: CURRENT_TIME -- in response to REQ_CURRENT_TIME */
	int process_response_message(ib_msg_resp_current_time *resp_msg);
	/* fired by: REAL_TIME_BARS */
	int process_response_message(ib_msg_resp_realtime_bar *resp_msg);
	/* fired by: FUNDAMENTAL_DATA */
	int process_response_message(ib_msg_resp_fundamental_data *resp_msg);
	/* fired by: DELTA_NEUTRAL_VALIDATION */
	int process_response_message(ib_msg_resp_delta_neutral_validation *resp_msg);
	/* fired by: ACCT_DOWNLOAD_END */
	int process_response_message(ib_msg_resp_acct_download_end *resp_msg);
	/* fired by: TICK_SNAPSHOT_END */
	int process_response_message(ib_msg_resp_tick_snapshot_end *resp_msg);
	/* fired by: MARKET_DATA_TYPE */
	int process_response_message(ib_msg_resp_market_data_type *resp_msg);
	/* fired by: COMMISSION_REPORT */
	int process_response_message(ib_msg_resp_commission_report *resp_msg);

};





#endif // TWS_INSTANCE_CPP_HEADER_INCLUDED
