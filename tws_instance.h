
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

#include "tws_request.h"
#include "tier2_message_queue.h"


// forward reference:
class app_manager;
struct tws_instance;
namespace tws
{
	typedef struct tws_instance tws_instance_t;
}



struct tws_conn_cfg
{
    /* configuration parameters: how to connect to TWS */
    const char *ip_address;
    int port;
    int our_id_code;

    long backend_poll_period; // unit: milliseconds
	long backend_reconnect_delay; // unit: milliseconds

public:
	tws_conn_cfg() :
		ip_address(NULL), port(0), our_id_code(0),
		backend_poll_period(0), backend_reconnect_delay(0)
	{
	}
	~tws_conn_cfg()
	{
	}
};



template <typename /* tws_request_message* */ T> class tws_req_active_msg_set: public std::vector<T>
{
protected:
	int max_active_limit;

	typedef std::vector<T>::iterator iter_t;

public:
	tws_req_active_msg_set(int max_allowed = INT_MAX) :
	  max_active_limit(max_allowed)
	{
	}
	virtual ~tws_req_active_msg_set()
	{
		for (iter_t i = this->begin(); i != this->end(); i++)
		{
			T elem = *i;
			elem->destroy();
		}
	}

public:
	T find(int unique_id) const
	{
		for (iter_t i = this->begin(); i != this->end(); i++)
		{
			T elem = *i;
			if (elem->matches(unique_id))
				return T;
		}
		return 0;
	}

	// return false when the item could not be pushed due to limitations
	bool push(T msg)
	{
		if (this->size() < max_active_limit)
		{
			this->push_back(msg);
			return true;
		}
		return false;
	}

	bool are_limits_reached(void) const
	{
		return this->size() >= max_active_limit;
	}
};





class ib_tws_manager
{
protected:
	struct tws_conn_cfg tws_cfg;
    struct mg_connection *conn;
    tws::tws_instance_t *tws_handle;
	app_manager *app_mgr;

    /* tracking some TWS values here as well: */
    int next_order_id;

	/* the connection pairs used to communicate between front-end threads and tier2/TWS threads: */
	//hash_map

    /* -- and the working men -- */

	/* sends message REQ_SCANNER_PARAMETERS to IB/TWS */
	tws_req_active_msg_set<ib_req_scanner_parameters *> req_scanner_parameters_active_set;

	/* sends message REQ_SCANNER_SUBSCRIPTION to IB/TWS */
	tws_req_active_msg_set<ib_req_scanner_subscription *> req_scanner_subscription_active_set;

	/* sends message REQ_MKT_DATA to IB/TWS */
	tws_req_active_msg_set<ib_req_mkt_data *> req_mkt_data_active_set;
	
	/* sends message REQ_HISTORICAL_DATA to IB/TWS */
	tws_req_active_msg_set<ib_req_historical_data *> req_historical_data_active_set;
	
	/* sends message EXERCISE_OPTIONS to IB/TWS */
	tws_req_active_msg_set<ib_exercise_options *> exercise_options_active_set;
	
	/* sends message PLACE_ORDER to IB/TWS */
	tws_req_active_msg_set<ib_place_order *> place_order_active_set;
	
	/* sends message REQ_OPEN_ORDERS to IB/TWS */
	tws_req_active_msg_set<ib_req_open_orders *> req_open_orders_active_set;
	
	/* sends message REQ_ACCOUNT_DATA to IB/TWS */
	tws_req_active_msg_set<ib_req_account_updates *> req_account_updates_active_set;
	
	/* sends message REQ_EXECUTIONS to IB/TWS */
	tws_req_active_msg_set<ib_req_executions *> req_executions_active_set;
	
	/* sends message REQ_IDS to IB/TWS */
	tws_req_active_msg_set<ib_req_ids *> req_ids_active_set;
	
	/* sends message REQ_CONTRACT_DATA to IB/TWS */
	tws_req_active_msg_set<ib_req_contract_details *> req_contract_details_active_set;
	
	/* sends message REQ_MKT_DEPTH to IB/TWS */
	tws_req_active_msg_set<ib_req_mkt_depth *> req_mkt_depth_active_set;
	
	/* sends message REQ_NEWS_BULLETINS to IB/TWS */
	tws_req_active_msg_set<ib_req_news_bulletins *> req_news_bulletins_active_set;
	
	/* sends message SET_SERVER_LOGLEVEL to IB/TWS */
	tws_req_active_msg_set<ib_set_server_log_level *> set_server_log_level_active_set;
	
	/* sends message REQ_AUTO_OPEN_ORDERS to IB/TWS */
	tws_req_active_msg_set<ib_req_auto_open_orders *> req_auto_open_orders_active_set;
	
	/* sends message REQ_ALL_OPEN_ORDERS to IB/TWS */
	tws_req_active_msg_set<ib_req_all_open_orders *> req_all_open_orders_active_set;
	
	/* sends message REQ_MANAGED_ACCTS to IB/TWS */
	tws_req_active_msg_set<ib_req_managed_accts *> req_managed_accts_active_set;
	
	/* sends message REQ_FA to IB/TWS */
	tws_req_active_msg_set<ib_request_fa *> request_fa_active_set;
	
	/* sends message REPLACE_FA to IB/TWS */
	tws_req_active_msg_set<ib_replace_fa *> replace_fa_active_set;
	
	/* sends message REQ_CURRENT_TIME to IB/TWS */
	tws_req_active_msg_set<ib_req_current_time *> req_current_time_active_set;
	
	/* sends message REQ_FUNDAMENTAL_DATA to IB/TWS */
	tws_req_active_msg_set<ib_req_fundamental_data *> req_fundamental_data_active_set;
	
	/* sends message REQ_CALC_IMPLIED_VOLAT to IB/TWS */
	tws_req_active_msg_set<ib_calculate_implied_volatility *> calculate_implied_volatility_active_set;
	
	/* sends message REQ_CALC_OPTION_PRICE to IB/TWS */
	tws_req_active_msg_set<ib_calculate_option_price *> calculate_option_price_active_set;
	
	/* sends message REQ_MARKET_DATA_TYPE to IB/TWS */
	tws_req_active_msg_set<ib_req_market_data_type *> req_market_data_type_active_set;
	
	/* sends message REQ_REAL_TIME_BARS to IB/TWS */
	tws_req_active_msg_set<ib_request_realtime_bars *> request_realtime_bars_active_set;

public:
	ib_tws_manager(app_manager *mgr) :
		app_mgr(mgr),
		conn(NULL), tws_handle(NULL), 
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
	}
	virtual ~ib_tws_manager()
	{
	}

public:
	virtual int init();

	void set_next_order_id(int id);
	int get_next_order_id(void);

	int get_next_ticker_id(void);

	int init_tws_api(void);
	int exit_tws_api(void);

	int is_tws_connected(void);
	int process_tws_event(void);

	const char *strerror(int errcode);
};





#endif // TWS_INSTANCE_CPP_HEADER_INCLUDED
