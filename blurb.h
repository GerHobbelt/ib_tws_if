
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

#ifndef TIER2_REQUEST_HEADER_INCLUDED
#define TIER2_REQUEST_HEADER_INCLUDED

#include "tws_backend.h"

#include <tws_c_api/twsapi.h>
#include <tws_c_api/twsapi-debug.h>

#include "tws_database_io.h"

// forward reference:
class my_tws_io_info;



class tier2_message
{
public:
	virtual int transmit(my_tws_io_info *info) = 0;

	virtual int store(my_tws_io_info *info) = 0;
	virtual int load(my_tws_io_info *info) = 0;
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
class tier2_queue_item;


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
		// for various cancelations:
		int ticker_id;
		int order_id;
		int reqid;

        scanner_subscription_request *scanner_subscription;

		struct
		{
			int reqid;
			tr_contract_t *contract;
			const char *report_type;
		} fundamental_data;

		struct
		{
			int ticker_id;
			tr_contract_t *contract;
			const char *generic_tick_list;
			int snapshot;
		} market_data;

		struct
		{
			int ticker_id;
			tr_contract_t *contract;
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
			tr_contract_t *contract;
			int exercise_action;
			int exercise_quantity;
			const char *account;
			int exc_override;
		} exec_options;

		struct
		{
			int id;
			tr_contract_t *contract;
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
			tr_contract_t *contract;
		} contract_details;

		struct
		{
			int ticker_id;
			tr_contract_t *contract;
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
			tr_contract_t *contract;
			double option_price;
			double under_price;
		} implied_vola;

		struct
		{
			int reqid;
			tr_contract_t *contract;
			double volatility;
			double under_price;
		} option_price;

		struct
		{
			int ticker_id;
			tr_contract_t *contract;
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






class tier2_queue_item
{
public:
    tier2_command_code command_code;       // what's the front-end requesting we do?

	int priority;							// higher is more important

	union
    {
		// for various cancelations:
		int ticker_id;
		int order_id;
		int reqid;

        scanner_subscription_request *scanner_subscription;

		struct
		{
			int reqid;
			tr_contract_t *contract;
			const char *report_type;
		} fundamental_data;

		struct
		{
			int ticker_id;
			tr_contract_t *contract;
			const char *generic_tick_list;
			int snapshot;
		} market_data;

		struct
		{
			int ticker_id;
			tr_contract_t *contract;
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
			tr_contract_t *contract;
			int exercise_action;
			int exercise_quantity;
			const char *account;
			int exc_override;
		} exec_options;

		struct
		{
			int id;
			tr_contract_t *contract;
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
			tr_contract_t *contract;
		} contract_details;

		struct
		{
			int ticker_id;
			tr_contract_t *contract;
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
			tr_contract_t *contract;
			double option_price;
			double under_price;
		} implied_vola;

		struct
		{
			int reqid;
			tr_contract_t *contract;
			double volatility;
			double under_price;
		} option_price;

		struct
		{
			int ticker_id;
			tr_contract_t *contract;
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
    tws_instance_t *tws_handle;

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


#endif // TIER2_REQUEST_HEADER_INCLUDED

























































































#define TWS_CONNECT_RETRY_DELAY     10 // unit: seconds




static void process_one_queued_tier2_request(my_tws_io_info *info);


/*
replace TWSAPI debug printf call.
*/
void tws_debug_printf(void *opaque, const char *fmt, ...)
{
	my_tws_io_info *info = (my_tws_io_info *)opaque;
	va_list ap;

	va_start(ap, fmt);
	//mg_vlog((info ? info->conn : NULL), "debug", fmt, ap);
	va_end(ap);
}












static int tws_transmit_func(void *arg, const void *buf, unsigned int buflen)
{
    my_tws_io_info *info = (my_tws_io_info *)arg;

    if (info->conn)
    {
        return mg_write(info->conn, buf, buflen);
    }
    return -1;
}

static int tws_receive_func(void *arg, void *buf, unsigned int max_bufsize)
{
    my_tws_io_info *info = (my_tws_io_info *)arg;
    tws_thread_exch *exch = info->tws_cfg->exch;

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
    }
    return -1;
}
8
/* 'flush()' marks the end of the outgoing message: it should be transmitted ASAP */
static int tws_flush_func(void *arg)
{
    //my_tws_io_info *info = (my_tws_io_info *)arg;

    return 0;
}

/* open callback is invoked when tws_connect is invoked and no connection has been established yet (tws_connected() == false); return 0 on success; a twsclient_error_codes error code on failure. */
static int tws_open_func(void *arg)
{
    my_tws_io_info *info = (my_tws_io_info *)arg;
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
    my_tws_io_info *info = (my_tws_io_info *)arg;

    if (info->conn)
    {
        mg_close_connection(info->conn);
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


















int scanner_subscription_request::send_request(my_tws_io_info *info)
{
	tr_scanner_subscription_t s;
	tws_init_scanner_subscription(info->tws_handle, &s);
	tws_copy(s.scan_code, scan_code);
	tws_copy(s.scan_instrument, instrument);
	tws_copy(s.scan_location_code, location_code);
	tws_copy(s.scan_above_price, above_price);
	tws_copy(s.scan_above_volume, above_volume);

    int rv = tws_req_scanner_subscription(info->tws_handle, ticker_id, &s);
	tws_destroy_scanner_subscription(info->tws_handle, &s);
	return rv;
}


















/*
Check whether there are any queued requests which are 'activated' and if there
are, process one.
*/
static void process_one_queued_tier2_request(my_tws_io_info *info)
{
	struct tws_conn_cfg *tws_cfg = info->tws_cfg;
	tier2_queue_item cmd;
	
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
		tws_thread_exch *exch = tws_cfg->exch;
        my_tws_io_info info = {0};
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
                    // process another message
                    if (0 != tws_event_process(info.tws_handle))
                    {
                        break;
                    }

					/*
					The 'tws_receive_func' takes care of processing front-end messages 
					while waiting for incoming TWS traffic...
					*/
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
            mg_sleep(TWS_CONNECT_RETRY_DELAY);
        }
    }

    mg_signal_mgr_this_thread_is_done(ctx);

    DEBUG_TRACE(("exiting"));
}





















































































#include "tier2_message.h"






/*
===============================================================================

serialized forms of IB/TWS I/F structures: used for storage in the cache/database

===============================================================================
*/

// serialized string data is indexed in the data[] member...
typedef ham_u16_t  str_index_t;

#include <ham/packstart.h>

typedef HAM_PACK_0 struct HAM_PACK_1 trser_comboleg
{
    str_index_t co_action;                                      /* BUY/SELL/SSHORT/SSHORTX */
    str_index_t co_exchange;
    str_index_t co_designated_location;
    ham_s16_t co_open_close;					/* type: tr_comboleg_type_t */
    ham_s32_t co_conid;
    ham_s32_t   co_ratio;
    ham_s32_t   co_short_sale_slot;                           /* 1 = clearing broker, 2 = third party */
    ham_s32_t   co_exempt_code;                               /* set to -1 if you do not use it */
} HAM_PACK_2 trser_comboleg_t;

typedef HAM_PACK_0 struct HAM_PACK_1 trser_contract_summary
{
	struct ser_under_comp
	{
		double u_price;
		double u_delta;
		ham_s32_t  u_conid;
	} c_undercomp;									/* delta neutral */
	ham_u16_t	has_c_undercomp;     /* boolean: !0 when c_undercomp data is valid */

	double         c_strike;                            /* strike price for options */
	str_index_t         c_symbol;                            /* underlying symbol */
	str_index_t         c_sectype;                           /* security type ("BAG" -> transmits combo legs, "" -> does not transmit combo legs) */
	str_index_t         c_exchange;
	str_index_t         c_primary_exch;                      /* for SMART orders, specify an actual exchange where the contract trades, e.g. ISLAND.  Pick a non-aggregate (ie not the SMART exchange) exchange that the contract trades on.  DO NOT SET TO SMART. */
	str_index_t         c_expiry;                            /* expiration for futures and options */
	str_index_t         c_currency;                          /* currency, e.g. USD */
	str_index_t         c_right;                             /* put or call (P or C) */
	str_index_t         c_local_symbol;                      /* local symbol for futures or options, e.g. ESZN for ES DEC09 contract  */
	str_index_t         c_multiplier;
	str_index_t         c_combolegs_descrip;                 /* received in open order version 14 and up for all combos */
	str_index_t         c_secid_type;                        /* CUSIP;SEDOL;ISIN;RIC */
	str_index_t         c_secid;
	trser_comboleg_t c_comboleg[16];                          /* COMBOS */
	ham_s32_t            c_num_combolegs;
	ham_s32_t            c_conid;                             /* contract id, returned from TWS */
	ham_u16_t c_include_expired;                /* for contract requests, specifies that even expired contracts should be returned.  Can not be set to true for orders. */
} HAM_PACK_2 trser_contract_summary_t;

typedef HAM_PACK_0 struct HAM_PACK_1 trser_contract
{
	ham_u32_t struct_size;							/* allocated size of this structure */

	struct trser_contract_summary c_contract;

	/* strings are stored starting from here */
	char data[4];
} HAM_PACK_2 trser_contract_t;

typedef HAM_PACK_0 struct HAM_PACK_1 trser_contract_details
{
	ham_u32_t struct_size;							/* allocated size of this structure */

    double                     d_mintick;
    double                     d_coupon;                /* for bonds */
    trser_contract_summary_t  d_summary;
    str_index_t                     d_market_name;
    str_index_t                     d_trading_class;
    str_index_t                     d_order_types;
    str_index_t                     d_valid_exchanges;
    str_index_t                     d_cusip;                 /* for bonds */
    str_index_t                     d_maturity;              /* for bonds */
    str_index_t                     d_issue_date;            /* for bonds */
    str_index_t                     d_ratings;               /* for bonds */
    str_index_t                     d_bond_type;             /* for bonds */
    str_index_t                     d_coupon_type;           /* for bonds */
    str_index_t                     d_desc_append;           /* for bonds */
    str_index_t                     d_next_option_date;      /* for bonds */
    str_index_t                     d_next_option_type;      /* for bonds */
    str_index_t                     d_notes;                 /* for bonds */
    str_index_t                     d_long_name;
    str_index_t                     d_contract_month;
    str_index_t                     d_industry;
    str_index_t                     d_category;
    str_index_t                     d_subcategory;
    str_index_t                     d_timezone_id;
    str_index_t                     d_trading_hours;
    str_index_t                     d_liquid_hours;
    ham_s32_t                        d_price_magnifier;
    ham_s32_t                        d_under_conid;
    ham_u16_t d_convertible;        /* for bonds */
    ham_u16_t d_callable;           /* for bonds */
    ham_u16_t d_putable;            /* for bonds */
    ham_u16_t d_next_option_partial; /* for bonds */

	/* strings are stored starting from here */
	char data[4];
} HAM_PACK_2 trser_contract_details_t;

typedef HAM_PACK_0 struct HAM_PACK_1 trser_tag_value {
    str_index_t t_tag;
    str_index_t t_val;
} HAM_PACK_2 trser_tag_value_t;

typedef HAM_PACK_0 struct HAM_PACK_1 trser_order_comboleg
{
	double cl_price; /* price per leg */
} HAM_PACK_2 trser_order_comboleg_t;

typedef HAM_PACK_0 struct HAM_PACK_1 trser_order
{
	ham_u32_t struct_size;							/* allocated size of this structure */

    double   o_discretionary_amt;                       /* SMART routing only: amount you are willing to pay above your specified limit price */
    double   o_lmt_price;                               /* Basic Order Field: limit price  */
    double   o_aux_price;                               /* Basic Order Field: stop price, trailing amount, or offset amount  */
    double   o_percent_offset;                          /* Advanced order field: the offset amount for Relative (REL) orders, specified as a percent; specify either this OR the offset amount in m_auxPrice */
    double   o_nbbo_price_cap;                          /* SMART routing only: see 'm_firmQuoteOnly' comment */
    double   o_starting_price;                          /* For BOX option-pegged-to-stock orders only */
    double   o_stock_ref_price;                         /* For BOX option-pegged-to-stock orders only */
    double   o_delta;                                   /* For BOX option-pegged-to-stock orders only */
    double   o_stock_range_lower;                       /* For Pegged-to-stock or Volatility orders only: if stock price moves below this price, order will be canceled */
    double   o_stock_range_upper;                       /* For Pegged-to-stock or Volatility orders only: if stock price moves above this price, order will be canceled */
    double   o_volatility;                              /* For Volatility orders only: volatility (percent) */
    double   o_delta_neutral_aux_price;
    double   o_trail_stop_price;                        /* Advanced order field: initial stop price for trailing stop (TRAIL) orders */
    double   o_trailing_percent;
    double   o_basis_points;
    double   o_scale_price_increment;					/* For SCALE orders only */
    double   o_scale_price_adjust_value;				/* For SCALE orders only */
    double   o_scale_profit_offset;						/* For SCALE orders only */
    str_index_t   o_algo_strategy;
    str_index_t   o_good_after_time;                         /* Advanced order field: format: YYYYMMDD HH:MM:SS {time zone}  e.g. 20060505 08:00:00 EST */
    str_index_t   o_good_till_date;                          /* Advanced order field: format: YYYYMMDD HH:MM:SS {time zone}  e.g. 20060505 08:00:00 EST */
    str_index_t   o_fagroup;                                 /* For Financial advisors (FA) only: Advisor group, e.g. "All" */
    str_index_t   o_famethod;                                /* For Financial advisors (FA) only: Advisor method: PctChange, AvailableEquity, NetLiq, or EqualQuantity */
    str_index_t   o_fapercentage;                            /* For Financial advisors (FA) only: Advisor percentage, used when the method is set to PctChange */
    str_index_t   o_faprofile;                               /* For Financial advisors (FA) only: Advisor profile */
    str_index_t   o_action;                                  /* Basic Order Field: specify BUY or SELL; non-cleared customers can specify SSHORT */
    str_index_t   o_order_type;                              /* Basic Order Field: order type, e.g. LMT, MKT, STOP, TRAIL, REL */
    str_index_t   o_tif;                                     /* Advanced order field: Time in force, e.g. DAY, GTC */
    str_index_t   o_oca_group;                               /* Advanced order field: OCA group name (OCA = "one cancels all") */
    str_index_t   o_account;                                 /* Clearing info: IB account; can be left blank for users with only a single account   */
    str_index_t   o_open_close;                              /* For non-cleared (i.e. institutional) customers only: open/close flag: O=Open, C=Close */
    str_index_t   o_orderref;                                /* Advanced order field: order reference, enter any free-form text */
    str_index_t   o_designated_location;                     /* For non-cleared (i.e. institutional) customers only: specifies where the shares are held; set only when m_shortSaleSlot=2 */
    str_index_t   o_rule80a;                                 /* Advanced order field: Individual = 'I', Agency = 'A', AgentOtherMember = 'W', IndividualPTIA = 'J', AgencyPTIA = 'U', AgentOtherMemberPTIA = 'M', IndividualPT = 'K', AgencyPT = 'Y', AgentOtherMemberPT = 'N' */
    str_index_t   o_settling_firm;
    str_index_t   o_delta_neutral_order_type;
    str_index_t   o_clearing_account;                        /* Clearing info: True beneficiary of the order */
    str_index_t   o_clearing_intent;                         /* Clearing info: "" (Default), "IB", "Away", "PTA" (PostTrade) */
	str_index_t   o_hedge_type;								/* Hedge Orders: 'D' - delta, 'B' - beta, 'F' - FX, 'P' - pair */
	str_index_t   o_hedge_param;								/* Hedge Orders: 'beta=X' value for beta hedge, 'ratio=Y' for pair hedge */
	str_index_t   o_delta_neutral_settling_firm;				/* For Volatility orders only: */
	str_index_t   o_delta_neutral_clearing_account;			/* For Volatility orders only: */
	str_index_t   o_delta_neutral_clearing_intent;			/* For Volatility orders only: */
    trser_tag_value_t o_algo_params[64];                 /* 'm_algoParams': array of length o_algo_params_count */
	trser_tag_value_t o_smart_combo_routing_params[64];		/* Smart combo routing params: 'm_smartComboRoutingParams': array of length o_smart_combo_routing_params, API user responsible for alloc/free */

	trser_order_comboleg_t o_combo_legs[64];

    ham_s32_t      o_algo_params_count;                       /* how many tag values are in o_algo_params, 0 if unused */
    ham_s32_t      o_smart_combo_routing_params_count;        /* how many tag values are in o_smart_combo_routing_params, 0 if unused */
    ham_s32_t      o_combo_legs_count;                  /* how many tag values are in o_combo_legs, 0 if unused */

    ham_s32_t      o_orderid;                                 /* Basic Order Field: order id generated by API client */
    ham_s32_t      o_total_quantity;                          /* Basic Order Field: order size */
    ham_s16_t o_origin;                               /* tr_origin_t : For non-cleared (i.e. institutional) customers only: origin: 0=Customer, 1=Firm */
    ham_s32_t      o_clientid;                                /* Basic Order Field: client id of the API client that submitted the order */
    ham_s32_t      o_permid;                                  /* Basic Order Field: TWS order ID (not specified by API) */
    ham_s32_t      o_parentid;                                /* Advanced order field: order id of parent order, to associate attached stop, trailing stop, or bracket orders with parent order (Auto STP, TRAIL)  */
    ham_s32_t      o_display_size;                            /* Advanced order field: the portion of the order that will be visible to the world */
    ham_s32_t      o_trigger_method;                          /* Advanced order field: for stop orders:  0=Default, 1=Double_Bid_Ask, 2=Last, 3=Double_Last, 4=Bid_Ask, 7=Last_or_Bid_Ask, 8=Mid-point */
    ham_s32_t      o_min_qty;                                 /* Advanced order field: no partial fills less than the size specified here */
    ham_s32_t      o_volatility_type;                         /* For Volatility orders only: volatility type: 1=daily, 2=annual */
    ham_s32_t      o_reference_price_type;                    /* For Volatility orders only: what to use as the current stock price: 1=bid/ask average, 2 = bid or ask */
    ham_s32_t      o_basis_points_type;
    ham_s32_t      o_scale_subs_level_size;
    ham_s32_t      o_scale_init_level_size;
    ham_s32_t      o_scale_price_adjust_interval;				/* For SCALE orders only */
    ham_s32_t      o_scale_init_position;						/* For SCALE orders only */
    ham_s32_t      o_scale_init_fill_qty;						/* For SCALE orders only */
    ham_s32_t      o_exempt_code;                             /* set to -1 if you do not use it */
    ham_s32_t      o_delta_neutral_con_id;					/* For Volatility orders only: */
    ham_s16_t o_oca_type;                           /* tr_oca_type_t : Advanced order field: OCA group type  1 = CANCEL_WITH_BLOCK, 2 = REDUCE_WITH_BLOCK, 3 = REDUCE_NON_BLOCK */
    ham_s16_t o_auction_strategy;           /* tr_auction_strategy_t : For BOX option-pegged-to-stock and Volatility orders only: 1=AUCTION_MATCH, 2=AUCTION_IMPROVEMENT, 3=AUCTION_TRANSPARENT */
    ham_u16_t o_short_sale_slot;                      /* For non-cleared (i.e. institutional) customers only: specify only if m_action is "SSHORT": 1 if you hold the shares, 2 if they will be delivered from elsewhere */
    ham_u16_t o_override_percentage_constraints;      /* Advanced order field: set true to override normal percentage constraint checks */
    ham_u16_t o_firm_quote_only;                      /* SMART routing only: if true, specifies that order should be routed to exchanges showing a "firm" quote, but not if the exchange is off the NBBO by more than the 'm_nbboPriceCap' amount */
    ham_u16_t o_etrade_only;
    ham_u16_t o_all_or_none;                          /* Advanced order field: if set to true, there can be no partial fills for the order */
    ham_u16_t o_outside_rth;                          /* Advanced order field: if true, order could fill or trigger anytime; if false, order will fill or trigger only during regular trading hours */
    ham_u16_t o_hidden;                               /* Advanced order field: if true, order will be hidden, and will not be reflected in the market data or deep book */
    ham_u16_t o_transmit;                             /* Advanced order field: if false, order will be created in TWS but not transmitted */
    ham_u16_t o_block_order;                          /* Advanced order field: block order, for ISE option orders only */
    ham_u16_t o_sweep_to_fill;                        /* Advanced order field: for SMART orders, specifies that orders should be split and sent to multiple exchanges at the same time */
    ham_u16_t o_continuous_update;                    /* For Volatility orders only: if true, price will be continuously recalculated after order submission */
    ham_u16_t o_whatif;                               /* if true, the order will not be submitted, but margin info will be returned */
    ham_u16_t o_not_held;
    ham_u16_t o_opt_out_smart_routing;				/* SMART routing only: */
    ham_u16_t o_scale_auto_reset;						/* For SCALE orders only */
    ham_u16_t o_scale_random_percent;					/* For SCALE orders only */

	/* strings are stored starting from here */
	char data[4];
} HAM_PACK_2 trser_order_t;

typedef HAM_PACK_0 struct HAM_PACK_1 trser_order_status
{
	ham_u32_t struct_size;							/* allocated size of this structure */

    double ost_commission;
    double ost_min_commission;
    double ost_max_commission;
    str_index_t ost_status;
    str_index_t ost_init_margin;
    str_index_t ost_maint_margin;
    str_index_t ost_equity_with_loan;
    str_index_t ost_commission_currency;
    str_index_t ost_warning_text;

	/* strings are stored starting from here */
	char data[4];
} HAM_PACK_2 trser_order_status_t;

typedef HAM_PACK_0 struct HAM_PACK_1 trser_execution
{
	ham_u32_t struct_size;							/* allocated size of this structure */

    double e_price;
    double e_avg_price;
    str_index_t e_execid;
    str_index_t e_time;
    str_index_t e_acct_number;
    str_index_t e_exchange;
    str_index_t e_side;
    str_index_t e_orderref;
    ham_s32_t    e_shares;
    ham_s32_t    e_permid;
    ham_s32_t    e_clientid;
    ham_s32_t    e_liquidation;
    ham_s32_t    e_orderid;
    ham_s32_t    e_cum_qty;

	/* strings are stored starting from here */
	char data[4];
} HAM_PACK_2 trser_execution_t;

typedef HAM_PACK_0 struct HAM_PACK_1 trser_exec_filter
{
	ham_u32_t struct_size;							/* allocated size of this structure */

    str_index_t f_acctcode;
    str_index_t f_time;
    str_index_t f_symbol;
    str_index_t f_sectype;
    str_index_t f_exchange;
    str_index_t f_side;
    ham_s32_t   f_clientid;

	/* strings are stored starting from here */
	char data[4];
} HAM_PACK_2 trser_exec_filter_t;

typedef HAM_PACK_0 struct HAM_PACK_1 trser_scanner_subscription
{
	ham_u32_t struct_size;							/* allocated size of this structure */

    double scan_above_price;
    double scan_below_price;
    double scan_coupon_rate_above;
    double scan_coupon_rate_below;
    double scan_market_cap_above;
    double scan_market_cap_below;
    str_index_t scan_exclude_convertible;
    str_index_t scan_instrument;
    str_index_t scan_location_code;
    str_index_t scan_maturity_date_above;
    str_index_t scan_maturity_date_below;
    str_index_t scan_moody_rating_above;
    str_index_t scan_moody_rating_below;
    str_index_t scan_code;
    str_index_t scan_sp_rating_above;
    str_index_t scan_sp_rating_below;
    str_index_t scan_scanner_setting_pairs;
    str_index_t scan_stock_type_filter;
    ham_s32_t    scan_above_volume;
    ham_s32_t    scan_number_of_rows;
    ham_s32_t    scan_average_option_volume_above;

	/* strings are stored starting from here */
	char data[4];
} HAM_PACK_2 trser_scanner_subscription_t;

typedef HAM_PACK_0 struct HAM_PACK_1 trser_commission_report
{
	ham_u32_t struct_size;							/* allocated size of this structure */

    str_index_t cr_exec_id;
    str_index_t cr_currency;
    double cr_commission;
    double cr_realized_pnl;
    double cr_yield;
    ham_s32_t   cr_yield_redemption_date; /* YYYYMMDD format */

	/* strings are stored starting from here */
	char data[4];
} HAM_PACK_2 trser_commission_report_t;

#include <ham/packstop.h>



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
		// for various cancelations:
		int ticker_id;
		int order_id;
		int reqid;

        scanner_subscription_request *scanner_subscription;

		struct
		{
			int reqid;
			tr_contract_t *contract;
			const char *report_type;
		} fundamental_data;

		struct
		{
			int ticker_id;
			tr_contract_t *contract;
			const char *generic_tick_list;
			int snapshot;
		} market_data;

		struct
		{
			int ticker_id;
			tr_contract_t *contract;
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
			tr_contract_t *contract;
			int exercise_action;
			int exercise_quantity;
			const char *account;
			int exc_override;
		} exec_options;

		struct
		{
			int id;
			tr_contract_t *contract;
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
			tr_contract_t *contract;
		} contract_details;

		struct
		{
			int ticker_id;
			tr_contract_t *contract;
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
			tr_contract_t *contract;
			double option_price;
			double under_price;
		} implied_vola;

		struct
		{
			int reqid;
			tr_contract_t *contract;
			double volatility;
			double under_price;
		} option_price;

		struct
		{
			int ticker_id;
			tr_contract_t *contract;
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






class tier2_queue_item
{
public:
    tier2_command_code command_code;       // what's the front-end requesting we do?

	int priority;							// higher is more important

	union
    {
		// for various cancelations:
		int ticker_id;
		int order_id;
		int reqid;

        scanner_subscription_request *scanner_subscription;

		struct
		{
			int reqid;
			tr_contract_t *contract;
			const char *report_type;
		} fundamental_data;

		struct
		{
			int ticker_id;
			tr_contract_t *contract;
			const char *generic_tick_list;
			int snapshot;
		} market_data;

		struct
		{
			int ticker_id;
			tr_contract_t *contract;
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
			tr_contract_t *contract;
			int exercise_action;
			int exercise_quantity;
			const char *account;
			int exc_override;
		} exec_options;

		struct
		{
			int id;
			tr_contract_t *contract;
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
			tr_contract_t *contract;
		} contract_details;

		struct
		{
			int ticker_id;
			tr_contract_t *contract;
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
			tr_contract_t *contract;
			double option_price;
			double under_price;
		} implied_vola;

		struct
		{
			int reqid;
			tr_contract_t *contract;
			double volatility;
			double under_price;
		} option_price;

		struct
		{
			int ticker_id;
			tr_contract_t *contract;
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
    tws_instance_t *tws_handle;

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














static __inline void tws_copy(char *dst, const char *src)
{
	tws_strcpy(dst, src);
}
static __inline void tws_copy(double &dst, double src)
{
	if (src != DBL_MAX)
		dst = src;
}
static __inline void tws_copy(int &dst, int src)
{
	if (src != INT_MAX)
		dst = src;
}




int scanner_subscription_request::send_request(my_tws_io_info *info)
{
	tr_scanner_subscription_t s;
	tws_init_scanner_subscription(info->tws_handle, &s);
	tws_copy(s.scan_code, scan_code);
	tws_copy(s.scan_instrument, instrument);
	tws_copy(s.scan_location_code, location_code);
	tws_copy(s.scan_above_price, above_price);
	tws_copy(s.scan_above_volume, above_volume);

	int rv = tws_req_scanner_subscription(info->tws_handle, ticker_id, &s);
	tws_destroy_scanner_subscription(info->tws_handle, &s);
	return rv;
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
when there's a free slot in TWS, send the scanner subscription request to TWS, otherwise push it on a stack.
*/
void push_tws_req_scanner_subscription(my_tws_io_info *info, scanner_subscription_request *reqdata)
{
	mg_log(info->conn, "info", "REQUEST scanner subscription: code [%s], instrument [%s], location [%s]", 
		reqdata->get_scan_code(), reqdata->get_instrument(), reqdata->get_location_code());





	// push in FIFO: push at the bottom, pop off the top.
	if (info->queued_scanner_subscription_count > 0)
	{
		memmove(&info->scanner_subscription_queue[1], &info->scanner_subscription_queue[0], info->queued_scanner_subscription_count * sizeof(info->scanner_subscription_queue[0]));
	}
	info->scanner_subscription_queue[0] = reqdata;
	info->queued_scanner_subscription_count++;
}


/*
when there's a free slot in TWS, pop a scanner subscription request off the stack and send it to TWS

Return the number of queued requests still waiting in the queue after this call.
*/
size_t pop_tws_req_scanner_subscription(my_tws_io_info *info)
{
	if (info->active_scanner_subscription_count < ARRAY_SIZE(info->active_scanner_subscriptions)
		&& info->queued_scanner_subscription_count > 0)
	{
		// there's room for one more active subscription: pop it and transmit it!
		scanner_subscription_request *reqdata = info->scanner_subscription_queue[--info->queued_scanner_subscription_count];
		info->active_scanner_subscriptions[info->active_scanner_subscription_count++] = reqdata;

		reqdata->send_request(info);
	}
	return info->queued_scanner_subscription_count;
}


/*
send a CANCEL REQUEST to TWS for the given scanner subscription
*/
void cancel_tws_scanner_subscription(my_tws_io_info *info, int ticker_id)
{
	size_t i;

	for (i = 0; i < info->active_scanner_subscription_count; i++)
	{
		scanner_subscription_request *item = info->active_scanner_subscriptions[i];

		if (item->get_ticker_id() == ticker_id)
		{
			tws_cancel_scanner_subscription(info->tws_handle, ticker_id);

			delete item;

			if (i != info->active_scanner_subscription_count)
			{
				memmove(&info->active_scanner_subscriptions[i], &info->active_scanner_subscriptions[i + 1], (info->active_scanner_subscription_count - i - 1) * sizeof(info->active_scanner_subscriptions[0]));
			}
			info->active_scanner_subscription_count--;
			return;
		}
	}
}



/*
return reference to scanner subscription request when the given ticker_id represents an active subscription.
*/
scanner_subscription_request *get_active_tws_scanner_subscription(my_tws_io_info *info, int ticker_id)
{
	size_t i;

	for (i = 0; i < info->active_scanner_subscription_count; i++)
	{
		scanner_subscription_request *item = info->active_scanner_subscriptions[i];

		if (item->get_ticker_id() == ticker_id)
		{
			return item;
		}
	}

	return NULL;
}

/*
return !0 when the given ticker_id represents an active subscription.
*/
int is_active_tws_scanner_subscription(my_tws_io_info *info, int ticker_id)
{
	return !!get_active_tws_scanner_subscription(info, ticker_id);
}











/*
optionally request the full contract details so that we'll receive the extended info for this stock.

'optional' because we only do this when we don't have the desired info yet in our own caches.
*/
void request_contract_details_from_tws(my_tws_io_info *info, tr_contract_details_t *cd)
{
	if (!ib_get_ticker_info(cd))
	{
		tr_contract_t contract;
		int reqid = tws_mkNextOrderId(info);

		contract = cd->d_summary;

		tws_req_contract_details(info->tws_handle, reqid, &contract);
	}
}



