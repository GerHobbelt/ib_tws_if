
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

#ifndef TWS_REQUEST_CPP_HEADER_INCLUDED
#define TWS_REQUEST_CPP_HEADER_INCLUDED

#include "tws_message_base.h"



// forward references
class ib_scanner_subscription;
class ib_contract;
class ib_order;
class ib_exec_filter;




class ib_ticker_list : std::vector<int>
{
public:
	ib_ticker_list()
	{
	}
	~ib_ticker_list()
	{
	}

public:
	std::string to_tws(void) const;
	int from_tws(const char *s);
};





class tws_request_message: public tws_reqresp_message
{
public:
	tws_request_message(ib_instance *tws, int ticker_id) :
		tws_reqresp_message(tws, ticker_id)
	{
	}
protected:
	virtual ~tws_request_message()
	{
	}
};





/* sends message REQ_SCANNER_PARAMETERS to IB/TWS */
class ib_req_scanner_parameters: public tws_request_message
{
public:
	ib_req_scanner_parameters(ib_instance *tws) :
	  tws_request_message(tws, -1)
	  {
	  }
protected:
	virtual ~ib_req_scanner_parameters()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message REQ_SCANNER_SUBSCRIPTION to IB/TWS */
class ib_req_scanner_subscription: public tws_request_message
{
public:
	ib_req_scanner_subscription(ib_instance *tws, int ticker_id, ib_scanner_subscription *subscription) :
	  tws_request_message(tws, ticker_id)
	  {
	  }
protected:
	virtual ~ib_req_scanner_subscription()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message CANCEL_SCANNER_SUBSCRIPTION to IB/TWS */
class ib_cancel_scanner_subscription: public tws_request_message
{
public:
	ib_cancel_scanner_subscription(ib_instance *tws, int ticker_id) :
	  tws_request_message(tws, ticker_id)
	  {
	  }
protected:
	virtual ~ib_cancel_scanner_subscription()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message REQ_MKT_DATA to IB/TWS */
class ib_req_mkt_data: public tws_request_message
{
public:
	ib_req_mkt_data(ib_instance *tws, int ticker_id, ib_contract *contract, const ib_ticker_list *generic_tick_list, int snapshot) :
	  tws_request_message(tws, ticker_id)
	  {
	  }
protected:
	virtual ~ib_req_mkt_data()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message REQ_HISTORICAL_DATA to IB/TWS */
class ib_req_historical_data: public tws_request_message
{
public:
	ib_req_historical_data(ib_instance *tws, int ticker_id, ib_contract *contract, time_t end_date_time, double duration, const char bar_size_setting[], const char what_to_show[], int use_rth, int format_date) :
	  tws_request_message(tws, ticker_id)
	  {
	  }
protected:
	virtual ~ib_req_historical_data()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message CANCEL_HISTORICAL_DATA to IB/TWS */
class ib_cancel_historical_data: public tws_request_message
{
public:
	ib_cancel_historical_data(ib_instance *tws, int ticker_id) :
	  tws_request_message(tws, ticker_id)
	  {
	  }
protected:
	virtual ~ib_cancel_historical_data()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message CANCEL_MKT_DATA to IB/TWS */
class ib_cancel_mkt_data: public tws_request_message
{
public:
	ib_cancel_mkt_data(ib_instance *tws, int ticker_id) :
	  tws_request_message(tws, ticker_id)
	  {
	  }
protected:
	virtual ~ib_cancel_mkt_data()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message EXERCISE_OPTIONS to IB/TWS */
class ib_exercise_options: public tws_request_message
{
public:
	ib_exercise_options(ib_instance *tws, int ticker_id, ib_contract *contract, int exercise_action, int exercise_quantity, const char account[], int exc_override) :
	  tws_request_message(tws, ticker_id)
	  {
	  }
protected:
	virtual ~ib_exercise_options()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message PLACE_ORDER to IB/TWS */
class ib_place_order: public tws_request_message
{
public:
	ib_place_order(ib_instance *tws, int order_id, ib_contract *contract, ib_order *order) :
	  tws_request_message(tws, -1)
	  {
	  }
protected:
	virtual ~ib_place_order()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message CANCEL_ORDER to IB/TWS */
class ib_cancel_order: public tws_request_message
{
public:
	ib_cancel_order(ib_instance *tws, int order_id) :
	  tws_request_message(tws, -1)
	  {
	  }
protected:
	virtual ~ib_cancel_order()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message REQ_OPEN_ORDERS to IB/TWS */
class ib_req_open_orders: public tws_request_message
{
public:
	ib_req_open_orders(ib_instance *tws) :
	  tws_request_message(tws, -1)
	  {
	  }
protected:
	virtual ~ib_req_open_orders()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message REQ_ACCOUNT_DATA to IB/TWS */
class ib_req_account_updates: public tws_request_message
{
public:
	ib_req_account_updates(ib_instance *tws, int subscribe, const char acct_code[]) :
	  tws_request_message(tws, -1)
	  {
	  }
protected:
	virtual ~ib_req_account_updates()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message REQ_EXECUTIONS to IB/TWS */
class ib_req_executions: public tws_request_message
{
public:
	ib_req_executions(ib_instance *tws, int reqid, ib_exec_filter *filter) :
	  tws_request_message(tws, -1)
	  {
	  }
protected:
	virtual ~ib_req_executions()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message REQ_IDS to IB/TWS */
class ib_req_ids: public tws_request_message
{
public:
	ib_req_ids(ib_instance *tws, int num_ids) :
	  tws_request_message(tws, -1)
	  {
	  }
protected:
	virtual ~ib_req_ids()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message REQ_CONTRACT_DATA to IB/TWS */
class ib_req_contract_details: public tws_request_message
{
public:
	ib_req_contract_details(ib_instance *tws, int reqid, ib_contract *contract) :
	  tws_request_message(tws, -1)
	  {
	  }
protected:
	virtual ~ib_req_contract_details()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message REQ_MKT_DEPTH to IB/TWS */
class ib_req_mkt_depth: public tws_request_message
{
public:
	ib_req_mkt_depth(ib_instance *tws, int ticker_id, ib_contract *contract, int num_rows) :
	  tws_request_message(tws, ticker_id)
	  {
	  }
protected:
	virtual ~ib_req_mkt_depth()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message CANCEL_MKT_DEPTH to IB/TWS */
class ib_cancel_mkt_depth: public tws_request_message
{
public:
	ib_cancel_mkt_depth(ib_instance *tws, int ticker_id) :
	  tws_request_message(tws, ticker_id)
	  {
	  }
protected:
	virtual ~ib_cancel_mkt_depth()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message REQ_NEWS_BULLETINS to IB/TWS */
class ib_req_news_bulletins: public tws_request_message
{
public:
	ib_req_news_bulletins(ib_instance *tws, int all_msgs) :
	  tws_request_message(tws, -1)
	  {
	  }
protected:
	virtual ~ib_req_news_bulletins()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message CANCEL_NEWS_BULLETINS to IB/TWS */
class ib_cancel_news_bulletins: public tws_request_message
{
public:
	ib_cancel_news_bulletins(ib_instance *tws) :
	  tws_request_message(tws, -1)
	  {
	  }
protected:
	virtual ~ib_cancel_news_bulletins()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message SET_SERVER_LOGLEVEL to IB/TWS */
class ib_set_server_log_level: public tws_request_message
{
public:
	ib_set_server_log_level(ib_instance *tws, int level) :
	  tws_request_message(tws, -1)
	  {
	  }
protected:
	virtual ~ib_set_server_log_level()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message REQ_AUTO_OPEN_ORDERS to IB/TWS */
class ib_req_auto_open_orders: public tws_request_message
{
public:
	ib_req_auto_open_orders(ib_instance *tws, int auto_bind) :
	  tws_request_message(tws, -1)
	  {
	  }
protected:
	virtual ~ib_req_auto_open_orders()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message REQ_ALL_OPEN_ORDERS to IB/TWS */
class ib_req_all_open_orders: public tws_request_message
{
public:
	ib_req_all_open_orders(ib_instance *tws) :
	  tws_request_message(tws, -1)
	  {
	  }
protected:
	virtual ~ib_req_all_open_orders()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message REQ_MANAGED_ACCTS to IB/TWS */
class ib_req_managed_accts: public tws_request_message
{
public:
	ib_req_managed_accts(ib_instance *tws) :
	  tws_request_message(tws, -1)
	  {
	  }
protected:
	virtual ~ib_req_managed_accts()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message REQ_FA to IB/TWS */
class ib_request_fa: public tws_request_message
{
public:
	ib_request_fa(ib_instance *tws, int fa_data_type) :
	  tws_request_message(tws, -1)
	  {
	  }
protected:
	virtual ~ib_request_fa()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message REPLACE_FA to IB/TWS */
class ib_replace_fa: public tws_request_message
{
public:
	ib_replace_fa(ib_instance *tws, int fa_data_type, const char cxml[]) :
	  tws_request_message(tws, -1)
	  {
	  }
protected:
	virtual ~ib_replace_fa()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message REQ_CURRENT_TIME to IB/TWS */
class ib_req_current_time: public tws_request_message
{
public:
	ib_req_current_time(ib_instance *tws) :
	  tws_request_message(tws, -1)
	  {
	  }
protected:
	virtual ~ib_req_current_time()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message REQ_FUNDAMENTAL_DATA to IB/TWS */
class ib_req_fundamental_data: public tws_request_message
{
public:
	ib_req_fundamental_data(ib_instance *tws, int reqid, ib_contract *contract, const char report_type[]) :
	  tws_request_message(tws, -1)
	  {
	  }
protected:
	virtual ~ib_req_fundamental_data()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message CANCEL_FUNDAMENTAL_DATA to IB/TWS */
class ib_cancel_fundamental_data: public tws_request_message
{
public:
	ib_cancel_fundamental_data(ib_instance *tws, int reqid) :
	  tws_request_message(tws, -1)
	  {
	  }
protected:
	virtual ~ib_cancel_fundamental_data()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message REQ_CALC_IMPLIED_VOLAT to IB/TWS */
class ib_calculate_implied_volatility: public tws_request_message
{
public:
	ib_calculate_implied_volatility(ib_instance *tws, int reqid, ib_contract *contract, double option_price, double under_price) :
	  tws_request_message(tws, -1)
	  {
	  }
protected:
	virtual ~ib_calculate_implied_volatility()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message CANCEL_CALC_IMPLIED_VOLAT to IB/TWS */
class ib_cancel_calculate_implied_volatility: public tws_request_message
{
public:
	ib_cancel_calculate_implied_volatility(ib_instance *tws, int reqid) :
	  tws_request_message(tws, -1)
	  {
	  }
protected:
	virtual ~ib_cancel_calculate_implied_volatility()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message REQ_CALC_OPTION_PRICE to IB/TWS */
class ib_calculate_option_price: public tws_request_message
{
public:
	ib_calculate_option_price(ib_instance *tws, int reqid, ib_contract *contract, double volatility, double under_price) :
	  tws_request_message(tws, -1)
	  {
	  }
protected:
	virtual ~ib_calculate_option_price()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message CANCEL_CALC_OPTION_PRICE to IB/TWS */
class ib_cancel_calculate_option_price: public tws_request_message
{
public:
	ib_cancel_calculate_option_price(ib_instance *tws, int reqid) :
	  tws_request_message(tws, -1)
	  {
	  }
protected:
	virtual ~ib_cancel_calculate_option_price()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message REQ_GLOBAL_CANCEL to IB/TWS */
class ib_req_global_cancel: public tws_request_message
{
public:
	ib_req_global_cancel(ib_instance *tws) :
	  tws_request_message(tws, -1)
	  {
	  }
protected:
	virtual ~ib_req_global_cancel()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message REQ_MARKET_DATA_TYPE to IB/TWS */
class ib_req_market_data_type: public tws_request_message
{
public:
	ib_req_market_data_type(ib_instance *tws, tws_market_data_type_t market_data_type) :
	  tws_request_message(tws, -1)
	  {
	  }
protected:
	virtual ~ib_req_market_data_type()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message REQ_REAL_TIME_BARS to IB/TWS */
class ib_request_realtime_bars: public tws_request_message
{
public:
	ib_request_realtime_bars(ib_instance *tws, int ticker_id, ib_contract *c, int bar_size, const char what_to_show[], int use_rth) :
	  tws_request_message(tws, ticker_id)
	  {
	  }
protected:
	virtual ~ib_request_realtime_bars()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};

/* sends message CANCEL_REAL_TIME_BARS to IB/TWS */
class ib_cancel_realtime_bars: public tws_request_message
{
public:
	ib_cancel_realtime_bars(ib_instance *tws, int ticker_id) :
	  tws_request_message(tws, ticker_id)
	  {
	  }
protected:
	virtual ~ib_cancel_realtime_bars()
	{
	}

public:
	virtual int transmit(my_tws_io_info *info);
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};














/* 
auxiliary message: fetches server version and connection time datums 

int    tws_server_version(ib_instance *tws);

const char *tws_connection_time(ib_instance *tws);
*/
class ib_fetch_tws_info: public tws_reqresp_message
{
public:
	ib_fetch_tws_info(ib_instance *tws) :
	  tws_reqresp_message(tws, -1)
	  {
	  }
protected:
	virtual ~ib_fetch_tws_info()
	{
	}

public:
	virtual int store(my_tws_io_info *info);
	virtual int load(my_tws_io_info *info);

	virtual bool equal(const tier2_message &alt) const;
};








#endif // TWS_REQUEST_CPP_HEADER_INCLUDED
