
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

#ifndef TWS_COMM_THREAD_HEADER_INCLUDED
#define TWS_COMM_THREAD_HEADER_INCLUDED

#include "tws_backend.h"

#include "tws_database_io.h"








/*
===============================================================================

backend ('middle tier' bla bla bla) request / response queue structures and functions:

===============================================================================
*/

enum tier2_command_code
{
    TIER2_NO_COMMAND = 0,
	/* transmit connect message and wait for response */
	TIER2_REQUEST_TWS_SCANNER_PARAMETERS,
	TIER2_REQUEST_TWS_SCANNER_SUBSCRIPTION,
	TIER2_CANCEL_TWS_SCANNER_SUBSCRIPTION,
	TIER2_REQUEST_TWS_MKT_DATA,
	TIER2_REQUEST_TWS_MARKET_DATA_TYPE,
	TIER2_REQUEST_TWS_HISTORICAL_DATA,
	TIER2_CANCEL_TWS_HISTORICAL_DATA,
	TIER2_CANCEL_TWS_MKT_DATA,
	TIER2_TWS_EXERCISE_OPTIONS,
	TIER2_TWS_PLACE_ORDER,
	TIER2_CANCEL_TWS_ORDER,
	TIER2_REQUEST_TWS_OPEN_ORDERS,
	TIER2_REQUEST_TWS_ACCOUNT_UPDATES,
	TIER2_REQUEST_TWS_EXECUTIONS,
	TIER2_REQUEST_TWS_IDS,
	TIER2_REQUEST_TWS_CONTRACT_DETAILS,
	TIER2_REQUEST_TWS_MKT_DEPTH,
	TIER2_CANCEL_TWS_MKT_DEPTH,
	TIER2_REQUEST_TWS_NEWS_BULLETINS,
	TIER2_CANCEL_TWS_NEWS_BULLETINS,
	TIER2_SET_TWS_SERVER_LOG_LEVEL,
	TIER2_REQUEST_TWS_AUTO_OPEN_ORDERS,
	TIER2_REQUEST_TWS_ALL_OPEN_ORDERS,
	TIER2_REQUEST_TWS_MANAGED_ACCTS,
	TIER2_TWS_REQUEST_FA,
	TIER2_TWS_REPLACE_FA,
	TIER2_REQUEST_TWS_CURRENT_TIME,
	TIER2_REQUEST_TWS_FUNDAMENTAL_DATA,
	TIER2_CANCEL_TWS_FUNDAMENTAL_DATA,
	TIER2_TWS_CALCULATE_IMPLIED_VOLATILITY,
	TIER2_CANCEL_TWS_CALCULATE_IMPLIED_VOLATILITY,
	TIER2_TWS_CALCULATE_OPTION_PRICE,
	TIER2_CANCEL_TWS_CALCULATE_OPTION_PRICE,
	TIER2_REQUEST_TWS_GLOBAL_CANCEL,
	TIER2_TWS_REQUEST_REALTIME_BARS,
	TIER2_CANCEL_TWS_REALTIME_BARS,
	/* auxiliary routines */
	TIER2_REQUEST_TWS_SERVER_VERSION,
	TIER2_REQUEST_TWS_CONNECTION_TIME,
};

enum tier2_request_state
{
    TIER2_ABORTED = -1,
    TIER2_MUST_EXEC_COMMAND = 0,
    TIER2_RESPONSE_PENDING,
    TIER2_RESPONSE_READY,
    TIER2_RESPONSE_PROCESSED
};

// forward reference:
namespace tws
{
	typedef struct tws_instance tws_instance_t;
}
class tier2_queue_item;
class my_tws_io_info;


/*
callback which is invoked when the command state changes.
*/
typedef int tier2_command_state_change_handler(tier2_queue_item *cmd);

/*
callback which is invoked when the queued command is destroyed.
*/
typedef int tier2_command_cleanup_handler(tier2_queue_item *cmd);




class scanner_subscription_request
{
protected:
    int ticker_id;

    char  *instrument;
    char  *location_code;
    char  *scan_code;

    double above_price;
    int above_volume;

public:
	scanner_subscription_request() :
		ticker_id(0), 
		instrument(NULL), location_code(NULL), scan_code(NULL),
		above_price(DBL_MAX), above_volume(INT_MAX)
	{
	}

	~scanner_subscription_request()
	{
		free(instrument);
		free(location_code);
		free(scan_code);
	}

public:
    int set_ticker_id(int id)
	{
		ticker_id = id;
		return id;
	}
    int get_ticker_id(void) const
	{
		return ticker_id;
	}

    const char *set_instrument(const char *val)
	{
		free(instrument);
		instrument = strdup(val);
		return instrument;
	}
    const char *get_instrument(void) const
	{
		return instrument;
	}
    const char *set_location_code(const char *val)
	{
		free(location_code);
		location_code = strdup(val);
		return location_code;
	}
    const char *get_location_code(void) const 
	{
		return location_code;
	}
    const char *set_scan_code(const char *val)
	{
		free(scan_code);
		scan_code = strdup(val);
		return scan_code;
	}
    const char *get_scan_code(void) const
	{
		return scan_code;
	}

    double set_above_price(double val)
	{
		above_price = val;
		return val;
	}
    double get_above_price(void) const
	{
		return above_price;
	}
    int set_above_volume(int val)
	{
		above_volume = val;
		return val;
	}
    int get_above_volume(void) const
	{
		return above_volume;
	}

public:
	int send_request(my_tws_io_info *info);
};













class tier2_request
{
public:
    tier2_command_code command_code;       // what's the front-end requesting we do?

	union
    {
		// for various cancellations:
		int ticker_id;
		int order_id;
		int reqid;

        scanner_subscription_request *scanner_subscription;

		struct
		{
			int reqid;
			ib_contract *contract;
			const char *report_type;
		} fundamental_data;

		struct
		{
			int ticker_id;
			ib_contract *contract;
			const char *generic_tick_list;
			int snapshot;
		} market_data;

		struct
		{
			int ticker_id;
			ib_contract *contract;
			const char *end_date_time;
			const char *duration_str;
			const char *bar_size_setting;
			const char *what_to_show;
			int use_rth;
			int format_date;
		} historical_data;

		struct
		{
			int ticker_id;
			ib_contract *contract;
			int exercise_action;
			int exercise_quantity;
			const char *account;
			int exc_override;
		} exec_options;

		struct
		{
			int id;
			ib_contract *contract;
			ib_order *order;
		} order;

		struct
		{
			int subscribe;
			const char *acct_code;
		} account_update;

		struct
		{
			int reqid;
			ib_exec_filter *filter;
		} req_executions;

		// for tws_req_ids:
		int numids;

		struct
		{
			int reqid;
			ib_contract *contract;
		} contract_details;

		struct
		{
			int ticker_id;
			ib_contract *contract;
			int num_rows;
		} market_depth;

		// for tws_req_news_bulletins:
		int allmsgs;

		// for tws_set_server_log_level:
		int log_level;

		// for tws_req_auto_open_orders:
		int auto_bind;

		// for request_fa:
		int fa_data_type;

		struct
		{
			int fa_data_type;
			const char *xml;
		} replace_fa;

		struct
		{
			int reqid;
			ib_contract *contract;
			double option_price;
			double under_price;
		} implied_vola;

		struct
		{
			int reqid;
			ib_contract *contract;
			double volatility;
			double under_price;
		} option_price;

		struct
		{
			int ticker_id;
			ib_contract *contract;
			int bar_size;
			const char *what_to_show;
			int use_Rth;
		} rt_bars;

		tws::market_data_type_t market_data_type;

    } command_params;

	// The moment this request should become 'active', i.e. should be executed
	time_t activation_time;
	// and the number of times this command should be executed at the given interval (seconds)
	int exec_run_count;
	unsigned int exec_time_interval;

	// The last time this request has been sent
	time_t last_transmit_time;
	// The last time a response for this request has been received
	time_t last_response_time;

    enum tier2_request_state request_state; // 0: pending, 1: success/done; < 0: error; > 1: warning/notification

    void *propagate_ptr;

	tier2_command_state_change_handler *state_change_cb; 
	tier2_command_cleanup_handler *cleanup_cb; 

public:
	tier2_request(tier2_command_code cmd, 
			time_t activate = 0, 
			int run_count = 1,
			int interval = 3600,
			void *propagate = NULL,
			tier2_command_state_change_handler *state_cb = NULL,
			tier2_command_cleanup_handler *end_cb = NULL,
			int prio = 0) :
		command_code(cmd),
		//command_params(0),
		activation_time(activate),
		exec_run_count(run_count),
		exec_time_interval(interval),
		last_transmit_time(0),
		last_response_time(0),
		request_state(TIER2_MUST_EXEC_COMMAND),
		propagate_ptr(propagate),
		state_change_cb(state_cb),
		cleanup_cb(end_cb),
		priority(prio)
	{
	}

	~tier2_request()
	{
	}
};







int tier2_send_request(struct tws_conn_cfg *tws_cfg, const tier2_queue_item *cmd)
{
	tws_thread_exch *exch = tws_cfg->exch;
	my_tws_io_info *info = tws_cfg->tws_thread_info;

	assert(info);
	assert(exch);
	
	pthread_mutex_lock(&exch->tws_queue_mutex);

	{
		int i;
		tier2_queue_item *item;

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

	/* auxiliary routines */

	case TIER2_REQUEST_TWS_SERVER_VERSION:
		tws_server_version(info->tws_handle);
		break;

	case TIER2_REQUEST_TWS_CONNECTION_TIME:
		tws_connection_time(info->tws_handle);
		break;
	}

	return 0;
}






class tier2_queue_item
{
public:
    tier2_command_code command_code;       // what's the front-end requesting we do?

	int priority;							// higher is more important

	union
    {
		// for various cancellations:
		int ticker_id;
		int order_id;
		int reqid;

        scanner_subscription_request *scanner_subscription;

		struct
		{
			int reqid;
			ib_contract *contract;
			const char *report_type;
		} fundamental_data;

		struct
		{
			int ticker_id;
			ib_contract *contract;
			const char *generic_tick_list;
			int snapshot;
		} market_data;

		struct
		{
			int ticker_id;
			ib_contract *contract;
			const char *end_date_time;
			const char *duration_str;
			const char *bar_size_setting;
			const char *what_to_show;
			int use_rth;
			int format_date;
		} historical_data;

		struct
		{
			int ticker_id;
			ib_contract *contract;
			int exercise_action;
			int exercise_quantity;
			const char *account;
			int exc_override;
		} exec_options;

		struct
		{
			int id;
			ib_contract *contract;
			tr_order_t *order;
		} order;

		struct
		{
			int subscribe;
			const char *acct_code;
		} account_update;

		struct
		{
			int reqid;
			tr_exec_filter_t *filter;
		} req_executions;

		// for tws_req_ids:
		int numids;

		struct
		{
			int reqid;
			ib_contract *contract;
		} contract_details;

		struct
		{
			int ticker_id;
			ib_contract *contract;
			int num_rows;
		} market_depth;

		// for tws_req_news_bulletins:
		int allmsgs;

		// for tws_set_server_log_level:
		int log_level;

		// for tws_req_auto_open_orders:
		int auto_bind;

		// for request_fa:
		int fa_data_type;

		struct
		{
			int fa_data_type;
			const char *xml;
		} replace_fa;

		struct
		{
			int reqid;
			ib_contract *contract;
			double option_price;
			double under_price;
		} implied_vola;

		struct
		{
			int reqid;
			ib_contract *contract;
			double volatility;
			double under_price;
		} option_price;

		struct
		{
			int ticker_id;
			ib_contract *contract;
			int bar_size;
			const char *what_to_show;
			int use_Rth;
		} rt_bars;

		market_data_type_t market_data_type;

    } command_params;

	// The moment this request should become 'active', i.e. should be executed
	time_t activation_time;
	// and the number of times this command should be executed at the given interval (seconds)
	int exec_run_count;
	unsigned int exec_time_interval;

	// The last time this request has been sent
	time_t last_transmit_time;
	// The last time a response for this request has been received
	time_t last_response_time;

    enum tier2_request_state request_state; // 0: pending, 1: success/done; < 0: error; > 1: warning/notification

    void *propagate_ptr;

	tier2_command_state_change_handler *state_change_cb; 
	tier2_command_cleanup_handler *cleanup_cb; 

public:
	tier2_queue_item(tier2_command_code cmd, 
			time_t activate = 0, 
			int run_count = 1,
			int interval = 3600,
			void *propagate = NULL,
			tier2_command_state_change_handler *state_cb = NULL,
			tier2_command_cleanup_handler *end_cb = NULL,
			int prio = 0) :
		command_code(cmd),
		//command_params(0),
		activation_time(activate),
		exec_run_count(run_count),
		exec_time_interval(interval),
		last_transmit_time(0),
		last_response_time(0),
		request_state(TIER2_MUST_EXEC_COMMAND),
		propagate_ptr(propagate),
		state_change_cb(state_cb),
		cleanup_cb(end_cb),
		priority(prio)
	{
	}

	~tier2_queue_item()
	{
	}
};





/* internal communication stuff between mongoose threads and the tws back-end thread goes here: */
class tws_thread_exch
{
public:
    pthread_mutex_t tws_queue_mutex;    // mutex used to protect the queue itself
    pthread_mutex_t tws_tx_mutex;		// mutex related to the TX condition in here
    pthread_cond_t tws_tx_signal;       // signalled when a TX request is pending (should be processed by the tws 'back-end' thread.
    pthread_mutex_t tws_rx_mutex;		// mutex related to the RX condition in here
    pthread_cond_t tws_rx_signal;       // signalled when a RX response is pending (should be processed by the related TX-invoking 'front-end' mongoose thread.

    tier2_queue_item **work_queue;

    size_t work_queue_size;           // allocated size
    size_t work_queue_fill;           // number of requests pending in the queue
    size_t work_queue_poppos;         // the position of the queue 'head' for popping
    size_t work_queue_diane;          // the position of the queue 'head' for pushing

public:
	tws_thread_exch(size_t initial_size = 16)
	{
		pthread_mutex_init(&tws_queue_mutex, NULL);
		pthread_mutex_init(&tws_rx_mutex, NULL);
		pthread_cond_init(&tws_rx_signal, NULL);
		pthread_mutex_init(&tws_tx_mutex, NULL);
		pthread_cond_init(&tws_tx_signal, NULL);

		// set up the queue:
		if (initial_size < 8)
			initial_size = 8;
		work_queue_size = initial_size;
		work_queue_fill = 0;
		work_queue_poppos = 0;
		work_queue_diane = 0;

		work_queue = (tier2_queue_item **)calloc(work_queue_size, sizeof(work_queue[0]));
	}

	~tws_thread_exch() 
	{
		if (work_queue_size)
		{
			size_t i;

			// TODO: signal all pending requests as ABORTED:
			for (i = 0; i < work_queue_size; i++)
			{
				tier2_queue_item *elem = work_queue[i];

				if (elem)
				{
					elem->request_state = TIER2_ABORTED;
					elem->exec_run_count = -1;

					// TODO: wait for the front-ends to recognize this change of affairs.

				}
			}

			pthread_cond_destroy(&tws_rx_signal);
			pthread_cond_destroy(&tws_tx_signal);
			pthread_mutex_destroy(&tws_rx_mutex);
			pthread_mutex_destroy(&tws_tx_mutex);
			pthread_mutex_destroy(&tws_queue_mutex);

			free(work_queue);
			work_queue = NULL;
			work_queue_size = 0;
		}
	}

protected:
	int grow(void)
	{
		size_t new_size = work_queue_size * 2;

		work_queue = (tier2_queue_item **)realloc(work_queue, new_size * sizeof(work_queue[0]));
		if (!work_queue)
			return -1;

		memset(work_queue + work_queue_size, 0, (new_size - work_queue_size) * sizeof(work_queue[0]));
		work_queue_size = new_size;
		return 0;
	}

public:
	int push(tier2_queue_item *cmd)
	{
		int rv = pthread_mutex_lock(&tws_queue_mutex);

		if (!rv)
		{
			if (work_queue_fill == work_queue_size)
			{
				rv = grow();
			}
		}

		if (!rv) 
		{
			size_t i;

			for (i = (work_queue_diane + 1) % work_queue_size; 
					i != work_queue_diane; 
					i = (i + 1) % work_queue_size)
			{
				if (!work_queue[i])
					break;
			}

			work_queue_diane = i; // (i + 1) % exch->work_queue_size;

			// make sure the exec count is at least 1:
			work_queue[i] = cmd;
			if (cmd->exec_run_count <= 0)
			{
				cmd->exec_run_count = 1;
			}
			work_queue_fill++;
			cmd->request_state = TIER2_MUST_EXEC_COMMAND;

			rv = pthread_mutex_unlock(&tws_queue_mutex);
		}

		return rv;
	}

	int abort(tier2_queue_item *cmd)
	{
		int rv = pthread_mutex_lock(&tws_queue_mutex);

		if (!rv)
		{
			size_t i;

			rv = -1;
			for (i = (work_queue_poppos + 1) % work_queue_size; 
					i != work_queue_poppos; 
					i = (i + 1) % work_queue_size)
			{
				if (work_queue[i] == cmd)
				{
					cmd->exec_run_count = -1;
					cmd->request_state = TIER2_ABORTED;

					rv = 0;
					break;
				}
			}

			int rv2 = pthread_mutex_unlock(&tws_queue_mutex);
			if (!rv) rv = rv2;
		}
		return rv;
	}

	/*
	Check which queued request has become active (round robin!) and 
	copy the queue entry to the referenced location. 
	
	Return NULL when there is no pending entry.
	*/
	tier2_queue_item *fetch(void)
	{
		time_t timestamp = time(NULL);
		int rv = pthread_mutex_lock(&tws_queue_mutex);
		tier2_queue_item *result = NULL;

		if (!rv)
		{
			size_t i;

			for (i = (work_queue_poppos + 1) % work_queue_size; 
				 i != work_queue_poppos; 
				 i = (i + 1) % work_queue_size)
			{
				tier2_queue_item *item = work_queue[i];
				if (item
					&& item->exec_run_count > 0
					&& (item->activation_time == 0
						|| item->activation_time >= timestamp))
				{
					// round robin polling of the queue:
					work_queue_poppos = i;

					// update the periodical when it's a repeat performance request:
					if (--item->exec_run_count > 0)
					{
						item->activation_time += item->exec_time_interval;

						// and cope with the situation where the original request had an activation timestamp of zero or pickup was much delayed
						if (item->activation_time < timestamp)
						{
							item->activation_time = timestamp + item->exec_time_interval;
						}
					}

					result = item;
					break;
				}
			}

			(void)pthread_mutex_unlock(&tws_queue_mutex);
		}
		return result;
	}

	int update(tier2_queue_item *&cmd)
	{
		int rv = pthread_mutex_lock(&tws_queue_mutex);

		if (!rv)
		{
			size_t i;
			bool end_it = false;

			rv = -1;
			for (i = (work_queue_poppos + 1) % work_queue_size; 
					i != work_queue_poppos; 
					i = (i + 1) % work_queue_size)
			{
				if (work_queue[i] == cmd)
				{
					if (cmd->request_state == TIER2_ABORTED
						|| (cmd->request_state == TIER2_RESPONSE_PROCESSED
							&& cmd->exec_run_count <= 0))
					{
						work_queue[i] = NULL;
						work_queue_fill--;
						end_it = true;
					}
					rv = 0;
					break;
				}
			}
			(void)pthread_mutex_unlock(&tws_queue_mutex);

			if (!rv && cmd->state_change_cb)
			{
				rv = cmd->state_change_cb(cmd);
			}
			if (!rv && end_it)
			{
				delete cmd;
				cmd = NULL;
			}
		}
		return rv;
	}

	int cleanup(tier2_queue_item *&cmd)
	{
		int rv = pthread_mutex_lock(&tws_queue_mutex);

		if (!rv)
		{
			size_t i;

			rv = -1;
			for (i = (work_queue_poppos + 1) % work_queue_size; 
					i != work_queue_poppos; 
					i = (i + 1) % work_queue_size)
			{
				if (work_queue[i] == cmd)
				{
					work_queue[i] = NULL;
					work_queue_fill--;

					rv = 0;
					break;
				}
			}
			(void)pthread_mutex_lock(&tws_queue_mutex);

			if (!rv && cmd->cleanup_cb)
			{
				rv = cmd->cleanup_cb(cmd);
			}
			if (!rv)
			{
				delete cmd;
				cmd = NULL;
			}
		}
		return rv;
	}

	// utility calls:
	size_t get_queue_depth(void)
	{
		int rv = pthread_mutex_lock(&tws_queue_mutex);
		size_t result = UINT_MAX;

		if (!rv)
		{
			result = work_queue_fill;
			rv = pthread_mutex_unlock(&tws_queue_mutex);
		}
		return result;
	}
};





/*
struct passed around as user parameter for all TWS API callbacks.
*/
class my_tws_io_info
{
public:
    struct mg_connection *conn;
    struct mg_context *ctx;
    struct tws_conn_cfg *tws_cfg;
    tws::tws_instance_t *tws_handle;

    /* tracking some TWS values here as well: */
    int next_order_id;

    /* -- and the working men -- */

    /* scanner subscription request active set and queue: */
    size_t active_scanner_subscription_count;
    scanner_subscription_request *active_scanner_subscriptions[10 /* limit imposed by TWS/IB */];

	/* -- the databases which keep track of things for us -- */
	struct my_databases_info dbi;
};




void tws_worker_thread(struct mg_context *ctx);


#endif // TWS_COMM_THREAD_HEADER_INCLUDED
