
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
#include "tws_data_structures.h"



// forward reference:
namespace tws
{
	typedef struct tws_instance tws_instance_t;
}




class ib_ticker_list
{
public:
	/*
	  mdoff blocks regular market data so that only specified generic ticks are received. 
	  In response to a '233,mdoff' request, pure RTVolume data will be returned without 
	  any additional records.
	*/
	ib_bool_t t_mdoff;
	ib_int_list_t t_list;

public:
	ib_ticker_list()
	{
	}
	~ib_ticker_list()
	{
	}

public:
	operator ib_string_t();
	int from_tws(const char *s);
};





class tws_request_message: public tws_reqresp_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

public:
	// The message (and optional response) has been completely processed
	virtual int f_task_completed(void);
	// The message has been marked as having failed (an error occurred)
	virtual int f_task_failed(void);
	// The message is being aborted
	virtual int f_task_aborted(void);

public:
	tws_request_message(tier2_message_processor *from, tier2_message_processor *to = NULL) :
		tws_reqresp_message(from, to)
	{
	}
protected:
	virtual ~tws_request_message()
	{
	}

public:
	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};






class tws_request_w_ticker_message: public tws_request_message
{
protected:
	ib_int_t m_ticker_id;

public:
	tws_request_w_ticker_message(tier2_message_processor *from, tier2_message_processor *to, int ticker_id) :
		tws_request_message(from, to),
		m_ticker_id(ticker_id)
	{
	}
protected:
	virtual ~tws_request_w_ticker_message()
	{
	}

public:
	virtual ib_int_t get_ticker_id(void) const
	{
		return m_ticker_id;
	}
};






/* sends message REQ_SCANNER_PARAMETERS to IB/TWS */
class ib_msg_req_scanner_parameters: public tws_request_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

public:
	ib_msg_req_scanner_parameters(tier2_message_processor *from, tier2_message_processor *to) :
		tws_request_message(from, to)
	{
	}
protected:
	virtual ~ib_msg_req_scanner_parameters()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};

/* sends message REQ_SCANNER_SUBSCRIPTION to IB/TWS */
class ib_msg_req_scanner_subscription: public tws_request_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_scanner_subscription m_subscription;

public:
	ib_msg_req_scanner_subscription(tier2_message_processor *from, tier2_message_processor *to, int ticker_id, ib_scanner_subscription &_subscription) :
		tws_request_w_ticker_message(from, to, ticker_id),
		m_subscription(_subscription)
	{
	}
protected:
	virtual ~ib_msg_req_scanner_subscription()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

	friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};

/* sends message CANCEL_SCANNER_SUBSCRIPTION to IB/TWS */
class ib_msg_cancel_scanner_subscription: public tws_request_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

public:
	ib_msg_cancel_scanner_subscription(tier2_message_processor *from, tier2_message_processor *to, int _ticker_id) :
		tws_request_w_ticker_message(from, to, _ticker_id)
	{
	}
protected:
	virtual ~ib_msg_cancel_scanner_subscription()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message REQ_MKT_DATA to IB/TWS */
class ib_msg_req_mkt_data: public tws_request_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_contract m_contract;
	ib_ticker_list m_generic_tick_list;
	ib_int_t m_snapshot;

public:
	ib_msg_req_mkt_data(tier2_message_processor *from, tier2_message_processor *to, int _ticker_id, ib_contract &_contract, const ib_ticker_list &_generic_tick_list, int _snapshot) :
		tws_request_w_ticker_message(from, to, _ticker_id),
		m_contract(_contract), m_generic_tick_list(_generic_tick_list), m_snapshot(_snapshot)
	{
	}
protected:
	virtual ~ib_msg_req_mkt_data()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message REQ_HISTORICAL_DATA to IB/TWS */
class ib_msg_req_historical_data: public tws_request_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_contract m_contract;
	ib_date_t m_end_date_time;
	ib_string_t m_duration;
	ib_string_t m_bar_size_setting;
	ib_string_t m_what_to_show;
	ib_int_t m_use_rth;
	ib_int_t m_format_date;

public:
	ib_msg_req_historical_data(tier2_message_processor *from, tier2_message_processor *to, int _ticker_id, ib_contract &_contract, time_t _end_date_time, const char *_duration, const char *_bar_size_setting, const char _what_to_show[], int _use_rth, int _format_date) :
	    tws_request_w_ticker_message(from, to, _ticker_id),
		m_contract(_contract), m_end_date_time(_end_date_time), m_duration(_duration), m_bar_size_setting(_bar_size_setting), m_what_to_show(_what_to_show), m_use_rth(_use_rth), m_format_date(_format_date)
	{
	}
protected:
	virtual ~ib_msg_req_historical_data()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message CANCEL_HISTORICAL_DATA to IB/TWS */
class ib_msg_cancel_historical_data: public tws_request_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

public:
	ib_msg_cancel_historical_data(tier2_message_processor *from, tier2_message_processor *to, int _ticker_id) :
	    tws_request_w_ticker_message(from, to, _ticker_id)
	{
	}
protected:
	virtual ~ib_msg_cancel_historical_data()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message CANCEL_MKT_DATA to IB/TWS */
class ib_msg_cancel_mkt_data: public tws_request_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

public:
	ib_msg_cancel_mkt_data(tier2_message_processor *from, tier2_message_processor *to, int _ticker_id) :
	    tws_request_w_ticker_message(from, to, _ticker_id)
	{
	}
protected:
	virtual ~ib_msg_cancel_mkt_data()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message EXERCISE_OPTIONS to IB/TWS */
class ib_msg_exercise_options: public tws_request_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_contract m_contract;
	ib_int_t m_exercise_action;
	ib_int_t m_exercise_quantity;
	ib_string_t m_account;
	ib_int_t m_exc_override;

public:
	ib_msg_exercise_options(tier2_message_processor *from, tier2_message_processor *to, int _ticker_id, ib_contract &_contract, int _exercise_action, int _exercise_quantity, const char *_account, int _exc_override) :
	    tws_request_w_ticker_message(from, to, _ticker_id),
		m_contract(_contract), m_exercise_action(_exercise_action), m_exercise_quantity(_exercise_quantity), m_account(_account), m_exc_override(_exc_override)
	{
	}
protected:
	virtual ~ib_msg_exercise_options()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message PLACE_ORDER to IB/TWS */
class ib_msg_place_order: public tws_request_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_int_t m_order_id;
	ib_contract m_contract;
	ib_order m_order;

public:
	ib_msg_place_order(tier2_message_processor *from, tier2_message_processor *to, int _order_id, ib_contract &_contract, ib_order &_order) :
	    tws_request_message(from, to),
		m_order_id(_order_id), m_contract(_contract), m_order(_order)
	{
	}
protected:
	virtual ~ib_msg_place_order()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message CANCEL_ORDER to IB/TWS */
class ib_msg_cancel_order: public tws_request_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_int_t m_order_id;

public:
	ib_msg_cancel_order(tier2_message_processor *from, tier2_message_processor *to, int _order_id) :
	    tws_request_message(from, to),
		m_order_id(_order_id)
	{
	}
protected:
	virtual ~ib_msg_cancel_order()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message REQ_OPEN_ORDERS to IB/TWS */
class ib_msg_req_open_orders: public tws_request_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

public:
	ib_msg_req_open_orders(tier2_message_processor *from, tier2_message_processor *to) :
	    tws_request_message(from, to)
	{
	}
protected:
	virtual ~ib_msg_req_open_orders()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message REQ_ACCOUNT_DATA to IB/TWS */
class ib_msg_req_account_updates: public tws_request_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_int_t m_subscribe;
	ib_string_t m_acct_code;

public:
	ib_msg_req_account_updates(tier2_message_processor *from, tier2_message_processor *to, int _subscribe, const char *_acct_code) :
	    tws_request_message(from, to),
		m_subscribe(_subscribe), m_acct_code(_acct_code)
	{
	}
protected:
	virtual ~ib_msg_req_account_updates()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message REQ_EXECUTIONS to IB/TWS */
class ib_msg_req_executions: public tws_request_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_exec_filter m_filter;

public:
	ib_msg_req_executions(tier2_message_processor *from, tier2_message_processor *to, int _req_id, ib_exec_filter &_filter) :
	    tws_request_w_ticker_message(from, to, _req_id),
		m_filter(_filter)
	{
	}
protected:
	virtual ~ib_msg_req_executions()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message REQ_IDS to IB/TWS */
class ib_msg_req_ids: public tws_request_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_int_t m_num_ids;

public:
	ib_msg_req_ids(tier2_message_processor *from, tier2_message_processor *to, int _num_ids) :
	    tws_request_message(from, to),
		m_num_ids(_num_ids)
	{
	}
protected:
	virtual ~ib_msg_req_ids()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message REQ_CONTRACT_DATA to IB/TWS */
class ib_msg_req_contract_details: public tws_request_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_contract m_contract;

public:
	ib_msg_req_contract_details(tier2_message_processor *from, tier2_message_processor *to, int _req_id, ib_contract &_contract) :
	    tws_request_w_ticker_message(from, to, _req_id),
		m_contract(_contract)
	{
	}
protected:
	virtual ~ib_msg_req_contract_details()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message REQ_MKT_DEPTH to IB/TWS */
class ib_msg_req_mkt_depth: public tws_request_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_contract m_contract;
	ib_int_t m_num_rows;

public:
	ib_msg_req_mkt_depth(tier2_message_processor *from, tier2_message_processor *to, int _ticker_id, ib_contract &_contract, int _num_rows) :
	    tws_request_w_ticker_message(from, to, _ticker_id),
		m_contract(_contract), m_num_rows(_num_rows)
	{
	}
protected:
	virtual ~ib_msg_req_mkt_depth()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message CANCEL_MKT_DEPTH to IB/TWS */
class ib_msg_cancel_mkt_depth: public tws_request_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

public:
	ib_msg_cancel_mkt_depth(tier2_message_processor *from, tier2_message_processor *to, int _ticker_id) :
	    tws_request_w_ticker_message(from, to, _ticker_id)
	{
	}
protected:
	virtual ~ib_msg_cancel_mkt_depth()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message REQ_NEWS_BULLETINS to IB/TWS */
class ib_msg_req_news_bulletins: public tws_request_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_int_t m_all_msgs;

public:
	ib_msg_req_news_bulletins(tier2_message_processor *from, tier2_message_processor *to, int _all_msgs) :
	    tws_request_message(from, to),
		m_all_msgs(_all_msgs)
	{
	}
protected:
	virtual ~ib_msg_req_news_bulletins()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message CANCEL_NEWS_BULLETINS to IB/TWS */
class ib_msg_cancel_news_bulletins: public tws_request_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

public:
	ib_msg_cancel_news_bulletins(tier2_message_processor *from, tier2_message_processor *to) :
	    tws_request_message(from, to)
	{
	}
protected:
	virtual ~ib_msg_cancel_news_bulletins()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message SET_SERVER_LOGLEVEL to IB/TWS */
class ib_msg_set_server_log_level: public tws_request_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_int_t m_level;

public:
	ib_msg_set_server_log_level(tier2_message_processor *from, tier2_message_processor *to, int _level) :
	    tws_request_message(from, to),
		m_level(_level)
	{
	}
protected:
	virtual ~ib_msg_set_server_log_level()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message REQ_AUTO_OPEN_ORDERS to IB/TWS */
class ib_msg_req_auto_open_orders: public tws_request_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_int_t m_auto_bind;

public:
	ib_msg_req_auto_open_orders(tier2_message_processor *from, tier2_message_processor *to, int _auto_bind) :
	    tws_request_message(from, to),
		m_auto_bind(_auto_bind)
	{
	}
protected:
	virtual ~ib_msg_req_auto_open_orders()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message REQ_ALL_OPEN_ORDERS to IB/TWS */
class ib_msg_req_all_open_orders: public tws_request_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

public:
	ib_msg_req_all_open_orders(tier2_message_processor *from, tier2_message_processor *to) :
	    tws_request_message(from, to)
	{
	}
protected:
	virtual ~ib_msg_req_all_open_orders()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message REQ_MANAGED_ACCTS to IB/TWS */
class ib_msg_req_managed_accts: public tws_request_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

public:
	ib_msg_req_managed_accts(tier2_message_processor *from, tier2_message_processor *to) :
	    tws_request_message(from, to)
	{
	}
protected:
	virtual ~ib_msg_req_managed_accts()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message REQ_FA to IB/TWS */
class ib_msg_request_fa: public tws_request_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	tws_fa_msg_type_t m_fa_data_type;

public:
	ib_msg_request_fa(tier2_message_processor *from, tier2_message_processor *to, tws_fa_msg_type_t _fa_data_type) :
	    tws_request_message(from, to),
		m_fa_data_type(_fa_data_type)
	{
	}
protected:
	virtual ~ib_msg_request_fa()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message REPLACE_FA to IB/TWS */
class ib_msg_replace_fa: public tws_request_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	tws_fa_msg_type_t m_fa_data_type;
	ib_string_t m_cxml;

public:
	ib_msg_replace_fa(tier2_message_processor *from, tier2_message_processor *to, tws_fa_msg_type_t _fa_data_type, const char *_cxml) :
	    tws_request_message(from, to),
		m_fa_data_type(_fa_data_type), m_cxml(_cxml)
	{
	}
protected:
	virtual ~ib_msg_replace_fa()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message REQ_CURRENT_TIME to IB/TWS */
class ib_msg_req_current_time: public tws_request_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

public:
	ib_msg_req_current_time(tier2_message_processor *from, tier2_message_processor *to) :
	    tws_request_message(from, to)
	{
	}
protected:
	virtual ~ib_msg_req_current_time()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;

protected:
	struct response_values
	{
		o_date_t m_time;
	} m_response;

public:
	const o_date_t &get_response_timestamp(void) const
	{
		return m_response.m_time;
	}
};


/* sends message REQ_FUNDAMENTAL_DATA to IB/TWS */
class ib_msg_req_fundamental_data: public tws_request_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_contract m_contract;
	ib_string_t m_report_type;

public:
	ib_msg_req_fundamental_data(tier2_message_processor *from, tier2_message_processor *to, int _req_id, ib_contract &_contract, const char *_report_type) :
	    tws_request_w_ticker_message(from, to, _req_id),
		m_contract(_contract), m_report_type(_report_type)
	{
	}
protected:
	virtual ~ib_msg_req_fundamental_data()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message CANCEL_FUNDAMENTAL_DATA to IB/TWS */
class ib_msg_cancel_fundamental_data: public tws_request_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

public:
	ib_msg_cancel_fundamental_data(tier2_message_processor *from, tier2_message_processor *to, int _req_id) :
		tws_request_w_ticker_message(from, to, _req_id)
	{
	}
protected:
	virtual ~ib_msg_cancel_fundamental_data()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message REQ_CALC_IMPLIED_VOLAT to IB/TWS */
class ib_msg_calculate_implied_volatility: public tws_request_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_contract m_contract;
	ib_double_t m_option_price;
	ib_double_t m_under_price;

public:
	ib_msg_calculate_implied_volatility(tier2_message_processor *from, tier2_message_processor *to, int _req_id, ib_contract &_contract, double _option_price, double _under_price) :
	    tws_request_w_ticker_message(from, to, _req_id),
		m_contract(_contract), m_option_price(_option_price), m_under_price(_under_price)
	{
	}
protected:
	virtual ~ib_msg_calculate_implied_volatility()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message CANCEL_CALC_IMPLIED_VOLAT to IB/TWS */
class ib_msg_cancel_calculate_implied_volatility: public tws_request_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

public:
	ib_msg_cancel_calculate_implied_volatility(tier2_message_processor *from, tier2_message_processor *to, int _req_id) :
	    tws_request_w_ticker_message(from, to, _req_id)
	{
	}
protected:
	virtual ~ib_msg_cancel_calculate_implied_volatility()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message REQ_CALC_OPTION_PRICE to IB/TWS */
class ib_msg_calculate_option_price: public tws_request_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_contract m_contract;
	ib_double_t m_volatility;
	ib_double_t m_under_price;

public:
	ib_msg_calculate_option_price(tier2_message_processor *from, tier2_message_processor *to, int _req_id, ib_contract &_contract, double _volatility, double _under_price) :
	    tws_request_w_ticker_message(from, to, _req_id),
		m_contract(_contract), m_volatility(_volatility), m_under_price(_under_price)
	{
	}
protected:
	virtual ~ib_msg_calculate_option_price()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message CANCEL_CALC_OPTION_PRICE to IB/TWS */
class ib_msg_cancel_calculate_option_price: public tws_request_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

public:
	ib_msg_cancel_calculate_option_price(tier2_message_processor *from, tier2_message_processor *to, int _req_id) :
	    tws_request_w_ticker_message(from, to, _req_id)
	{
	}
protected:
	virtual ~ib_msg_cancel_calculate_option_price()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message REQ_GLOBAL_CANCEL to IB/TWS */
class ib_msg_req_global_cancel: public tws_request_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

public:
	ib_msg_req_global_cancel(tier2_message_processor *from, tier2_message_processor *to) :
	    tws_request_message(from, to)
	{
	}
protected:
	virtual ~ib_msg_req_global_cancel()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message REQ_MARKET_DATA_TYPE to IB/TWS */
class ib_msg_req_market_data_type: public tws_request_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	tws_market_data_type_t m_market_data_type;

public:
	ib_msg_req_market_data_type(tier2_message_processor *from, tier2_message_processor *to, tws_market_data_type_t _market_data_type) :
	    tws_request_message(from, to),
		m_market_data_type(_market_data_type)
	{
	}
protected:
	virtual ~ib_msg_req_market_data_type()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message REQ_REAL_TIME_BARS to IB/TWS */
class ib_msg_request_realtime_bars: public tws_request_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_contract m_contract;
	ib_int_t m_bar_size;
	ib_string_t m_what_to_show;
	ib_int_t m_use_rth;

public:
	ib_msg_request_realtime_bars(tier2_message_processor *from, tier2_message_processor *to, int _ticker_id, ib_contract &_contract, int _bar_size, const char *_what_to_show, int _use_rth) :
	    tws_request_w_ticker_message(from, to, _ticker_id),
		m_contract(_contract), m_bar_size(_bar_size), m_what_to_show(_what_to_show), m_use_rth(_use_rth)
	{
	}
protected:
	virtual ~ib_msg_request_realtime_bars()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};


/* sends message CANCEL_REAL_TIME_BARS to IB/TWS */
class ib_msg_cancel_realtime_bars: public tws_request_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

public:
	ib_msg_cancel_realtime_bars(tier2_message_processor *from, tier2_message_processor *to, int _ticker_id) :
	    tws_request_w_ticker_message(from, to, _ticker_id)
	{
	}

protected:
	virtual ~ib_msg_cancel_realtime_bars()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

friend class ib_tws_manager;
	virtual int tx(tws::tws_instance_t *tws);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};















/* 
auxiliary message: fetches server version and connection time datums 

int    tws_server_version();

const char *tws_connection_time();
*/
class ib_msg_fetch_tws_info: public tws_request_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_int_t m_server_version;
	ib_date_t m_connect_timestamp;

public:
	ib_msg_fetch_tws_info(tier2_message_processor *from, tier2_message_processor *to = NULL) :
		tws_request_message(from, to),
		m_server_version(0), m_connect_timestamp()
	{
	}
protected:
	virtual ~ib_msg_fetch_tws_info()
	{
	}

protected:
	virtual int f_exec_command(void);
	virtual int f_commence_transmit(void);

public:
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response_message(tier2_message *response);

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const;
};









#endif // TWS_REQUEST_CPP_HEADER_INCLUDED
