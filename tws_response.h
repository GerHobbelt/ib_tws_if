
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

#ifndef TWS_RESPONSE_MSG_HEADER_INCLUDED
#define TWS_RESPONSE_MSG_HEADER_INCLUDED

#include "tws_message_base.h"
#include "tws_data_structures.h"






class tws_response_message: public tws_reqresp_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

public:
	tws_response_message(tier2_message_processor *from, tier2_message_processor *to = NULL) :
		tws_reqresp_message(from, to)
	{
	}
protected:
	virtual ~tws_response_message()
	{
	}

public:
	virtual int process_response_message(tier2_message *received_response);
};


class tws_response_w_ticker_message: public tws_response_message
{
protected:
	ib_int_t m_ticker_id;

public:
	tws_response_w_ticker_message(tier2_message_processor *from, tier2_message_processor *to, int ticker_id) :
		tws_response_message(from, to),
		m_ticker_id(ticker_id)
	{
	}
protected:
	virtual ~tws_response_w_ticker_message()
	{
	}

public:
	ib_int_t get_ticker_id(void) const
	{
		return m_ticker_id;
	}
};




/* fired by: TICK_PRICE */
class ib_msg_resp_tick_price: public tws_response_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	tws_tick_type_t m_field;
	ib_double_t m_price;
	ib_bool_t m_can_auto_execute;

public:
	ib_msg_resp_tick_price(tier2_message_processor *from, tier2_message_processor *to, int ticker_id, tws_tick_type_t field, double price, int can_auto_execute) 
		: tws_response_w_ticker_message(from, to, ticker_id)
		, m_field(field), m_price(price), m_can_auto_execute(!!can_auto_execute)
	{
	}
protected:
	virtual ~ib_msg_resp_tick_price()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};

/* fired by: TICK_PRICE (for modern versions, then immediately preceeded by an invocation of event_tick_price()), TICK_SIZE */
class ib_msg_resp_tick_size: public tws_response_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	tws_tick_type_t m_field;
	ib_int_t m_size;

public:
	ib_msg_resp_tick_size(tier2_message_processor *from, tier2_message_processor *to, int ticker_id, tws_tick_type_t field, int size) :
	  tws_response_w_ticker_message(from, to, ticker_id)
		  , m_field(field), m_size(size)
	  {
	  }
protected:
	virtual ~ib_msg_resp_tick_size()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: TICK_OPTION_COMPUTATION */
class ib_msg_resp_tick_option_computation: public tws_response_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	tws_tick_type_t m_type;
	ib_double_t m_implied_vol;
	ib_double_t m_delta;
	ib_double_t m_opt_price;
	ib_double_t m_pv_dividend;
	ib_double_t m_gamma;
	ib_double_t m_vega;
	ib_double_t m_theta;
	ib_double_t m_und_price;

public:
	ib_msg_resp_tick_option_computation(tier2_message_processor *from, tier2_message_processor *to, int ticker_id, tws_tick_type_t type, double implied_vol, double delta, double opt_price, double pv_dividend, double gamma, double vega, double theta, double und_price) :
	  tws_response_w_ticker_message(from, to, ticker_id)
		  , m_type(type), m_implied_vol(implied_vol), m_delta(delta)
		  , m_opt_price(opt_price), m_pv_dividend(pv_dividend)
		  , m_gamma(gamma), m_vega(vega), m_theta(theta), m_und_price(und_price)
	  {
	  }
protected:
	virtual ~ib_msg_resp_tick_option_computation()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: TICK_GENERIC */
class ib_msg_resp_tick_generic: public tws_response_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	tws_tick_type_t m_type;
	ib_double_t m_value;

public:
	ib_msg_resp_tick_generic(tier2_message_processor *from, tier2_message_processor *to, int ticker_id, tws_tick_type_t type, double value) :
	  tws_response_w_ticker_message(from, to, ticker_id)
		  , m_type(type), m_value(value)
	  {
	  }
protected:
	virtual ~ib_msg_resp_tick_generic()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: TICK_STRING */
class ib_msg_resp_tick_string: public tws_response_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	tws_tick_type_t m_type;
	ib_string_t m_value;

public:
	ib_msg_resp_tick_string(tier2_message_processor *from, tier2_message_processor *to, int ticker_id, tws_tick_type_t type, const char value[]) :
	  tws_response_w_ticker_message(from, to, ticker_id)
		  , m_type(type), m_value(value)
	  {
	  }
protected:
	virtual ~ib_msg_resp_tick_string()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: TICK_EFP */
class ib_msg_resp_tick_efp: public tws_response_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	tws_tick_type_t m_tick_type;
	ib_double_t m_basis_points;
	ib_string_t m_formatted_basis_points;
	ib_double_t m_implied_futures_price;
	ib_int_t m_hold_days;
	ib_string_t m_future_expiry;
	ib_double_t m_dividend_impact;
	ib_double_t m_dividends_to_expiry;

public:
	ib_msg_resp_tick_efp(tier2_message_processor *from, tier2_message_processor *to, int ticker_id, tws_tick_type_t tick_type, double basis_points, const char formatted_basis_points[], double implied_futures_price, int hold_days, const char future_expiry[], double dividend_impact, double dividends_to_expiry) :
	  tws_response_w_ticker_message(from, to, ticker_id)
		  , m_tick_type(tick_type), m_basis_points(basis_points)
		  , m_formatted_basis_points(formatted_basis_points), m_implied_futures_price(implied_futures_price), m_hold_days(hold_days)
		  , m_future_expiry(future_expiry), m_dividend_impact(dividend_impact), m_dividends_to_expiry(dividends_to_expiry)
	  {
	  }
protected:
	virtual ~ib_msg_resp_tick_efp()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: ORDER_STATUS */
class ib_msg_resp_order_status: public tws_response_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_int_t m_order_id;
	ib_string_t m_status;
	ib_int_t m_filled;
	ib_int_t m_remaining;
	ib_double_t m_avg_fill_price;
	ib_int_t m_perm_id;
	ib_int_t m_parent_id;
	ib_double_t m_last_fill_price;
	ib_int_t m_client_id;
	ib_string_t m_why_held;

public:
	ib_msg_resp_order_status(tier2_message_processor *from, tier2_message_processor *to, int order_id, const char status[], int filled, int remaining, double avg_fill_price, int perm_id, int parent_id, double last_fill_price, int client_id, const char why_held[]) :
	  tws_response_message(from, to)
		  , m_order_id(order_id), m_status(status), m_filled(filled), m_remaining(remaining), m_avg_fill_price(avg_fill_price)
		  , m_perm_id(perm_id), m_parent_id(parent_id), m_last_fill_price(last_fill_price), m_client_id(client_id), m_why_held(why_held)
	  {
	  }
protected:
	virtual ~ib_msg_resp_order_status()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: OPEN_ORDER */
class ib_msg_resp_open_order: public tws_response_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_int_t m_order_id;
	ib_contract m_contract;
	ib_order m_order;
	ib_order_status m_ost;

public:
	ib_msg_resp_open_order(tier2_message_processor *from, tier2_message_processor *to, int order_id, const tws::tr_contract *contract, const tws::tr_order *order, const tws::tr_order_status *ost) :
	  tws_response_message(from, to)
		  , m_order_id(order_id), m_contract(contract), m_order(order), m_ost(ost)
	  {
	  }
protected:
	virtual ~ib_msg_resp_open_order()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: OPEN_ORDER_END */
class ib_msg_resp_open_order_end: public tws_response_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

public:
	ib_msg_resp_open_order_end(tier2_message_processor *from, tier2_message_processor *to) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_open_order_end()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: ACCT_VALUE */
class ib_msg_resp_update_account_value: public tws_response_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_string_t m_key;
	ib_string_t m_val;
	ib_string_t m_currency;
	ib_string_t m_account_name;

public:
	ib_msg_resp_update_account_value(tier2_message_processor *from, tier2_message_processor *to, const char key[], const char val[], const char currency[], const char account_name[]) :
	  tws_response_message(from, to)
		  , m_key(key), m_val(val), m_currency(currency), m_account_name(account_name)
	  {
	  }
protected:
	virtual ~ib_msg_resp_update_account_value()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: PORTFOLIO_VALUE */
class ib_msg_resp_update_portfolio: public tws_response_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_contract m_contract;
	ib_int_t m_position;
	ib_double_t m_mkt_price;
	ib_double_t m_mkt_value;
	ib_double_t m_average_cost;
	ib_double_t m_unrealized_pnl;
	ib_double_t m_realized_pnl;
	ib_string_t m_account_name;

public:
	ib_msg_resp_update_portfolio(tier2_message_processor *from, tier2_message_processor *to, const tws::tr_contract *contract, int position, double mkt_price, double mkt_value, double average_cost, double unrealized_pnl, double realized_pnl, const char account_name[]) :
	  tws_response_message(from, to)
		  , m_contract(contract), m_position(position), m_mkt_price(mkt_price), m_mkt_value(mkt_value), m_average_cost(average_cost), m_unrealized_pnl(unrealized_pnl), m_realized_pnl(realized_pnl), m_account_name(account_name)
	  {
	  }
protected:
	virtual ~ib_msg_resp_update_portfolio()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: ACCT_UPDATE_TIME */
class ib_msg_resp_update_account_time: public tws_response_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_date_t m_time_stamp;

public:
	ib_msg_resp_update_account_time(tier2_message_processor *from, tier2_message_processor *to, const char time_stamp[]) :
	  tws_response_message(from, to)
		  , m_time_stamp(time_stamp)
	  {
	  }
protected:
	virtual ~ib_msg_resp_update_account_time()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: NEXT_VALID_ID */
class ib_msg_resp_next_valid_id: public tws_response_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_int_t m_order_id;

public:
	ib_msg_resp_next_valid_id(tier2_message_processor *from, tier2_message_processor *to, int order_id) :
	  tws_response_message(from, to)
		  , m_order_id(order_id)
	  {
	  }
protected:
	virtual ~ib_msg_resp_next_valid_id()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);

	ib_int_t get_order_id(void) const
	{
		return m_order_id;
	}
};


/* fired by: CONTRACT_DATA */
class ib_msg_resp_contract_details: public tws_response_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_contract_details m_contract_details;

public:
	ib_msg_resp_contract_details(tier2_message_processor *from, tier2_message_processor *to, int req_id, const tws::tr_contract_details *contract_details) :
	  tws_response_w_ticker_message(from, to, req_id)
		  , m_contract_details(contract_details)
	  {
	  }
protected:
	virtual ~ib_msg_resp_contract_details()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: CONTRACT_DATA_END */
class ib_msg_resp_contract_details_end: public tws_response_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

public:
	ib_msg_resp_contract_details_end(tier2_message_processor *from, tier2_message_processor *to, int req_id) :
	  tws_response_w_ticker_message(from, to, req_id)
	  {
	  }
protected:
	virtual ~ib_msg_resp_contract_details_end()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: BOND_CONTRACT_DATA */
class ib_msg_resp_bond_contract_details: public tws_response_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_contract_details m_contract_details;

public:
	ib_msg_resp_bond_contract_details(tier2_message_processor *from, tier2_message_processor *to, int req_id, const tws::tr_contract_details *contract_details) :
	  tws_response_w_ticker_message(from, to, req_id)
		  , m_contract_details(contract_details)
	  {
	  }
protected:
	virtual ~ib_msg_resp_bond_contract_details()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: EXECUTION_DATA */
class ib_msg_resp_exec_details: public tws_response_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_int_t m_order_id;
	ib_contract m_contract;
	ib_execution m_execution;

public:
	ib_msg_resp_exec_details(tier2_message_processor *from, tier2_message_processor *to, int order_id, const tws::tr_contract *contract, const tws::tr_execution *execution) :
	  tws_response_message(from, to)
		  , m_order_id(order_id), m_contract(contract), m_execution(execution)
	  {
	  }
protected:
	virtual ~ib_msg_resp_exec_details()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: EXECUTION_DATA_END */
class ib_msg_resp_exec_details_end: public tws_response_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

public:
	ib_msg_resp_exec_details_end(tier2_message_processor *from, tier2_message_processor *to, int req_id) :
	  tws_response_w_ticker_message(from, to, req_id)
	  {
	  }
protected:
	virtual ~ib_msg_resp_exec_details_end()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: ERR_MSG */
class ib_msg_resp_error: public tws_response_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_int_t m_error_code;
	ib_string_t m_error_string;

public:
	ib_msg_resp_error(tier2_message_processor *from, tier2_message_processor *to, int id, int error_code, const char error_string[])
		: tws_response_w_ticker_message(from, to, id)
		, m_error_code(error_code) 
		, m_error_string(error_string)
	{
	}
protected:
	virtual ~ib_msg_resp_error()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);

	ib_int_t get_error_code(void) const
	{
		return m_error_code;
	}
	const char *get_error_string(void) const
	{
		return m_error_string.c_str();
	}
};


/* fired by: MARKET_DEPTH */
class ib_msg_resp_update_mkt_depth: public tws_response_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_int_t m_position;
	ib_int_t m_operation;
	ib_int_t m_side;
	ib_double_t m_price;
	ib_int_t m_size;

public:
	ib_msg_resp_update_mkt_depth(tier2_message_processor *from, tier2_message_processor *to, int ticker_id, int position, int operation, int side, double price, int size) :
	  tws_response_w_ticker_message(from, to, ticker_id)
		  , m_position(position), m_operation(operation), m_side(side), m_price(price), m_size(size)
	  {
	  }
protected:
	virtual ~ib_msg_resp_update_mkt_depth()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: MARKET_DEPTH_L2 */
class ib_msg_resp_update_mkt_depth_l2: public tws_response_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_int_t m_position;
	ib_string_t m_market_maker;
	ib_int_t m_operation;
	ib_int_t m_side;
	ib_double_t m_price;
	ib_int_t m_size;

public:
	ib_msg_resp_update_mkt_depth_l2(tier2_message_processor *from, tier2_message_processor *to, int ticker_id, int position, const char *market_maker, int operation, int side, double price, int size) :
	  tws_response_w_ticker_message(from, to, ticker_id)
		  , m_position(position), m_market_maker(market_maker), m_operation(operation), m_side(side), m_price(price), m_size(size)
	  {
	  }
protected:
	virtual ~ib_msg_resp_update_mkt_depth_l2()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: NEWS_BULLETINS */
class ib_msg_resp_update_news_bulletin: public tws_response_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_int_t m_msgid;
	ib_int_t m_msg_type;
	ib_string_t m_news_msg;
	ib_string_t m_origin_exch;

public:
	ib_msg_resp_update_news_bulletin(tier2_message_processor *from, tier2_message_processor *to, int msgid, int msg_type, const char news_msg[], const char origin_exch[]) :
	  tws_response_message(from, to)
		  , m_msgid(msgid), m_msg_type(msg_type), m_news_msg(news_msg), m_origin_exch(origin_exch)
	  {
	  }
protected:
	virtual ~ib_msg_resp_update_news_bulletin()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: MANAGED_ACCTS */
class ib_msg_resp_managed_accounts: public tws_response_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_string_t m_accounts_list;

public:
	ib_msg_resp_managed_accounts(tier2_message_processor *from, tier2_message_processor *to, const char accounts_list[]) :
	  tws_response_message(from, to)
		  , m_accounts_list(accounts_list)
	  {
	  }
protected:
	virtual ~ib_msg_resp_managed_accounts()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);

	const char *get_accounts_list(void) const
	{
		return m_accounts_list.c_str();
	}
};


/* fired by: RECEIVE_FA */
class ib_msg_resp_receive_fa: public tws_response_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	tws_fa_msg_type_t m_fa_data_type;
	ib_string_t m_cxml;

public:
	ib_msg_resp_receive_fa(tier2_message_processor *from, tier2_message_processor *to, tws_fa_msg_type_t fa_data_type, const char *cxml) :
		tws_response_message(from, to),
		m_fa_data_type(fa_data_type), m_cxml(cxml)
	{
	}
protected:
	virtual ~ib_msg_resp_receive_fa()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: HISTORICAL_DATA (possibly multiple times per incoming message) */
class ib_msg_resp_historical_data: public tws_response_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_string_t m_date;
	ib_double_t m_open;
	ib_double_t m_high;
	ib_double_t m_low;
	ib_double_t m_close;
	ib_long_t m_volume;
	ib_int_t m_bar_count;
	ib_double_t m_wap;
	ib_int_t m_has_gaps;

public:
	ib_msg_resp_historical_data(tier2_message_processor *from, tier2_message_processor *to, int req_id, const char date[], double open, double high, double low, double close, long int volume, int bar_count, double wap, int has_gaps) :
	  tws_response_w_ticker_message(from, to, req_id)
		  , m_date(date), m_open(open), m_high(high), m_low(low), m_close(close), m_volume(volume), m_bar_count(bar_count), m_wap(wap), m_has_gaps(has_gaps)
	  {
	  }
protected:
	virtual ~ib_msg_resp_historical_data()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: HISTORICAL_DATA  (once, after one or more invocations of event_historical_data()) */
class ib_msg_resp_historical_data_end: public tws_response_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_string_t m_completion_from;
	ib_string_t m_completion_to;

public:
	ib_msg_resp_historical_data_end(tier2_message_processor *from, tier2_message_processor *to, int req_id, const char completion_from[], const char completion_to[]) :
	  tws_response_w_ticker_message(from, to, req_id)
		  , m_completion_from(completion_from), m_completion_to(completion_to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_historical_data_end()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: SCANNER_PARAMETERS */
class ib_msg_resp_scanner_parameters: public tws_response_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_string_t m_xml;

public:
	ib_msg_resp_scanner_parameters(tier2_message_processor *from, tier2_message_processor *to, const char xml[]) :
		tws_response_message(from, to)
			, m_xml(xml)
	{
	}
protected:
	virtual ~ib_msg_resp_scanner_parameters()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: SCANNER_DATA (possibly multiple times per incoming message) */
class ib_msg_resp_scanner_data: public tws_response_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_int_t m_rank;
	ib_contract_details m_cd;
	ib_string_t m_distance;
	ib_string_t m_benchmark;
	ib_string_t m_projection;
	ib_string_t m_legs_str;

public:
	ib_msg_resp_scanner_data(tier2_message_processor *from, tier2_message_processor *to, int ticker_id, int rank, tws::tr_contract_details *cd, const char distance[], const char benchmark[], const char projection[], const char legs_str[]) :
	  tws_response_w_ticker_message(from, to, ticker_id)
		  , m_rank(rank), m_cd(cd), m_distance(distance), m_benchmark(benchmark), m_projection(projection), m_legs_str(legs_str)
	  {
	  }
protected:
	virtual ~ib_msg_resp_scanner_data()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);

	const ib_contract_details *get_contract_details(void) const
	{
		return &m_cd;
	}
};


/* fired by: SCANNER_DATA (once, after one or more invocations of event_scanner_data()) */
class ib_msg_resp_scanner_data_end: public tws_response_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_int_t m_num_elements;

public:
	ib_msg_resp_scanner_data_end(tier2_message_processor *from, tier2_message_processor *to, int ticker_id, int num_elements) :
	  tws_response_w_ticker_message(from, to, ticker_id)
		  , m_num_elements(num_elements)
	  {
	  }
protected:
	virtual ~ib_msg_resp_scanner_data_end()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: SCANNER_DATA (once, before any invocations of event_scanner_data()) */
class ib_msg_resp_scanner_data_start: public tws_response_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_int_t m_num_elements;

public:
	ib_msg_resp_scanner_data_start(tier2_message_processor *from, tier2_message_processor *to, int ticker_id, int num_elements) :
	  tws_response_w_ticker_message(from, to, ticker_id)
		  , m_num_elements(num_elements)
	  {
	  }
protected:
	virtual ~ib_msg_resp_scanner_data_start()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: CURRENT_TIME -- in response to REQ_CURRENT_TIME */
class ib_msg_resp_current_time: public tws_response_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_date_t m_time;

public:
	ib_msg_resp_current_time(tier2_message_processor *from, tier2_message_processor *to, long time) :
	  tws_response_message(from, to)
		  , m_time(time)
	  {
	  }
protected:
	virtual ~ib_msg_resp_current_time()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: REAL_TIME_BARS */
class ib_msg_resp_realtime_bar: public tws_response_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_date_t m_time;
	ib_double_t m_open;
	ib_double_t m_high;
	ib_double_t m_low;
	ib_double_t m_close;
	ib_long_t m_volume;
	ib_double_t m_wap;
	ib_int_t m_count;

public:
	ib_msg_resp_realtime_bar(tier2_message_processor *from, tier2_message_processor *to, int req_id, long time, double open, double high, double low, double close, long int volume, double wap, int count) :
	  tws_response_w_ticker_message(from, to, req_id)
		  , m_time(time), m_open(open), m_high(high), m_low(low), m_close(close), m_volume(volume), m_wap(wap), m_count(count)
	  {
	  }
protected:
	virtual ~ib_msg_resp_realtime_bar()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: FUNDAMENTAL_DATA */
class ib_msg_resp_fundamental_data: public tws_response_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_string_t m_data;

public:
	ib_msg_resp_fundamental_data(tier2_message_processor *from, tier2_message_processor *to, int req_id, const char data[]) :
	  tws_response_w_ticker_message(from, to, req_id)
		  , m_data(data)
	  {
	  }
protected:
	virtual ~ib_msg_resp_fundamental_data()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: DELTA_NEUTRAL_VALIDATION */
class ib_msg_resp_delta_neutral_validation: public tws_response_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_under_comp m_und;

public:
	ib_msg_resp_delta_neutral_validation(tier2_message_processor *from, tier2_message_processor *to, int req_id, const tws::under_comp *und) :
	  tws_response_w_ticker_message(from, to, req_id)
		  , m_und(und)
	  {
	  }
protected:
	virtual ~ib_msg_resp_delta_neutral_validation()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: ACCT_DOWNLOAD_END */
class ib_msg_resp_acct_download_end: public tws_response_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_string_t m_acct_name;

public:
	ib_msg_resp_acct_download_end(tier2_message_processor *from, tier2_message_processor *to, const char acct_name[]) :
	  tws_response_message(from, to)
		  , m_acct_name(acct_name)
	  {
	  }
protected:
	virtual ~ib_msg_resp_acct_download_end()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: TICK_SNAPSHOT_END */
class ib_msg_resp_tick_snapshot_end: public tws_response_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

public:
	ib_msg_resp_tick_snapshot_end(tier2_message_processor *from, tier2_message_processor *to, int req_id) :
	  tws_response_w_ticker_message(from, to, req_id)
	  {
	  }
protected:
	virtual ~ib_msg_resp_tick_snapshot_end()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: MARKET_DATA_TYPE */
class ib_msg_resp_market_data_type: public tws_response_w_ticker_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	tws_market_data_type_t m_data_type;

public:
	ib_msg_resp_market_data_type(tier2_message_processor *from, tier2_message_processor *to, int req_id, tws_market_data_type_t data_type) :
	  tws_response_w_ticker_message(from, to, req_id)
		  , m_data_type(data_type)
	  {
	  }
protected:
	virtual ~ib_msg_resp_market_data_type()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};


/* fired by: COMMISSION_REPORT */
class ib_msg_resp_commission_report: public tws_response_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

protected:
	ib_commission_report m_report;

public:
	ib_msg_resp_commission_report(tier2_message_processor *from, tier2_message_processor *to, tws::tr_commission_report *report) :
	  tws_response_message(from, to)
		  , m_report(report)
	  {
	  }
protected:
	virtual ~ib_msg_resp_commission_report()
	{
	}

public:
	virtual int process_response_message(tier2_message *response);
};














#endif // TWS_RESPONSE_MSG_HEADER_INCLUDED
