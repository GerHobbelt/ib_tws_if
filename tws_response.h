
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
public:
	tws_response_message(tier2_message_processor *from, tier2_message_processor *to = NULL) :
		tws_reqresp_message(from, to)
	{
	}
protected:
	virtual ~tws_response_message()
	{
	}

};




/* fired by: TICK_PRICE */
class ib_msg_resp_tick_price: public tws_response_message
{
public:
	ib_msg_resp_tick_price(tier2_message_processor *from, tier2_message_processor *to, int ticker_id, tws_tick_type_t field, double price, int can_auto_execute) :
		tws_response_message(from, to)
	{
	}
protected:
	virtual ~ib_msg_resp_tick_price()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: TICK_PRICE (for modern versions, then immediately preceeded by an invocation of event_tick_price()), TICK_SIZE */
class ib_msg_resp_tick_size: public tws_response_message
{
public:
	ib_msg_resp_tick_size(tier2_message_processor *from, tier2_message_processor *to, int ticker_id, tws_tick_type_t field, int size) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_tick_size()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: TICK_OPTION_COMPUTATION */
class ib_msg_resp_tick_option_computation: public tws_response_message
{
public:
	ib_msg_resp_tick_option_computation(tier2_message_processor *from, tier2_message_processor *to, int ticker_id, tws_tick_type_t type, double implied_vol, double delta, double opt_price, double pv_dividend, double gamma, double vega, double theta, double und_price) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_tick_option_computation()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: TICK_GENERIC */
class ib_msg_resp_tick_generic: public tws_response_message
{
public:
	ib_msg_resp_tick_generic(tier2_message_processor *from, tier2_message_processor *to, int ticker_id, tws_tick_type_t type, double value) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_tick_generic()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: TICK_STRING */
class ib_msg_resp_tick_string: public tws_response_message
{
public:
	ib_msg_resp_tick_string(tier2_message_processor *from, tier2_message_processor *to, int ticker_id, tws_tick_type_t type, const char value[]) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_tick_string()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: TICK_EFP */
class ib_msg_resp_tick_efp: public tws_response_message
{
public:
	ib_msg_resp_tick_efp(tier2_message_processor *from, tier2_message_processor *to, int ticker_id, tws_tick_type_t tick_type, double basis_points, const char formatted_basis_points[], double implied_futures_price, int hold_days, const char future_expiry[], double dividend_impact, double dividends_to_expiry) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_tick_efp()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: ORDER_STATUS */
class ib_msg_resp_order_status: public tws_response_message
{
public:
	ib_msg_resp_order_status(tier2_message_processor *from, tier2_message_processor *to, int order_id, const char status[], int filled, int remaining, double avg_fill_price, int perm_id, int parent_id, double last_fill_price, int client_id, const char why_held[]) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_order_status()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: OPEN_ORDER */
class ib_msg_resp_open_order: public tws_response_message
{
public:
	ib_msg_resp_open_order(tier2_message_processor *from, tier2_message_processor *to, int order_id, const ib_contract *contract, const ib_order *order, const ib_order_status *ost) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_open_order()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: OPEN_ORDER_END */
class ib_msg_resp_open_order_end: public tws_response_message
{
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
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: ACCT_VALUE */
class ib_msg_resp_update_account_value: public tws_response_message
{
public:
	ib_msg_resp_update_account_value(tier2_message_processor *from, tier2_message_processor *to, const char key[], const char val[], const char currency[], const char account_name[]) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_update_account_value()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: PORTFOLIO_VALUE */
class ib_msg_resp_update_portfolio: public tws_response_message
{
public:
	ib_msg_resp_update_portfolio(tier2_message_processor *from, tier2_message_processor *to, const ib_contract *contract, int position, double mkt_price, double mkt_value, double average_cost, double unrealized_pnl, double realized_pnl, const char account_name[]) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_update_portfolio()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: ACCT_UPDATE_TIME */
class ib_msg_resp_update_account_time: public tws_response_message
{
public:
	ib_msg_resp_update_account_time(tier2_message_processor *from, tier2_message_processor *to, const char time_stamp[]) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_update_account_time()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: NEXT_VALID_ID */
class ib_msg_resp_next_valid_id: public tws_response_message
{
public:
	ib_msg_resp_next_valid_id(tier2_message_processor *from, tier2_message_processor *to, int order_id) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_next_valid_id()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: CONTRACT_DATA */
class ib_msg_resp_contract_details: public tws_response_message
{
public:
	ib_msg_resp_contract_details(tier2_message_processor *from, tier2_message_processor *to, int req_id, const ib_contract_details *contract_details) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_contract_details()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: CONTRACT_DATA_END */
class ib_msg_resp_contract_details_end: public tws_response_message
{
public:
	ib_msg_resp_contract_details_end(tier2_message_processor *from, tier2_message_processor *to, int reqid) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_contract_details_end()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: BOND_CONTRACT_DATA */
class ib_msg_resp_bond_contract_details: public tws_response_message
{
public:
	ib_msg_resp_bond_contract_details(tier2_message_processor *from, tier2_message_processor *to, int req_id, const ib_contract_details *contract_details) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_bond_contract_details()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: EXECUTION_DATA */
class ib_msg_resp_exec_details: public tws_response_message
{
public:
	ib_msg_resp_exec_details(tier2_message_processor *from, tier2_message_processor *to, int order_id, const ib_contract *contract, const ib_execution *execution) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_exec_details()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: EXECUTION_DATA_END */
class ib_msg_resp_exec_details_end: public tws_response_message
{
public:
	ib_msg_resp_exec_details_end(tier2_message_processor *from, tier2_message_processor *to, int reqid) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_exec_details_end()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: ERR_MSG */
class ib_msg_resp_error: public tws_response_message
{
public:
	ib_msg_resp_error(tier2_message_processor *from, tier2_message_processor *to, int id, int error_code, const char error_string[]) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_error()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: MARKET_DEPTH */
class ib_msg_resp_update_mkt_depth: public tws_response_message
{
public:
	ib_msg_resp_update_mkt_depth(tier2_message_processor *from, tier2_message_processor *to, int ticker_id, int position, int operation, int side, double price, int size) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_update_mkt_depth()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: MARKET_DEPTH_L2 */
class ib_msg_resp_update_mkt_depth_l2: public tws_response_message
{
public:
	ib_msg_resp_update_mkt_depth_l2(tier2_message_processor *from, tier2_message_processor *to, int ticker_id, int position, char *market_maker, int operation, int side, double price, int size) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_update_mkt_depth_l2()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: NEWS_BULLETINS */
class ib_msg_resp_update_news_bulletin: public tws_response_message
{
public:
	ib_msg_resp_update_news_bulletin(tier2_message_processor *from, tier2_message_processor *to, int msgid, int msg_type, const char news_msg[], const char origin_exch[]) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_update_news_bulletin()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: MANAGED_ACCTS */
class ib_msg_resp_managed_accounts: public tws_response_message
{
public:
	ib_msg_resp_managed_accounts(tier2_message_processor *from, tier2_message_processor *to, const char accounts_list[]) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_managed_accounts()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: RECEIVE_FA */
class ib_msg_resp_receive_fa: public tws_response_message
{
public:
	ib_msg_resp_receive_fa(tier2_message_processor *from, tier2_message_processor *to, int fa_data_type, const char cxml[]) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_receive_fa()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: HISTORICAL_DATA (possibly multiple times per incoming message) */
class ib_msg_resp_historical_data: public tws_response_message
{
public:
	ib_msg_resp_historical_data(tier2_message_processor *from, tier2_message_processor *to, int reqid, const char date[], double open, double high, double low, double close, long int volume, int bar_count, double wap, int has_gaps) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_historical_data()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: HISTORICAL_DATA  (once, after one or more invocations of event_historical_data()) */
class ib_msg_resp_historical_data_end: public tws_response_message
{
public:
	ib_msg_resp_historical_data_end(tier2_message_processor *from, tier2_message_processor *to, int reqid, const char completion_from[], const char completion_to[]) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_historical_data_end()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: SCANNER_PARAMETERS */
class ib_msg_resp_scanner_parameters: public tws_response_message
{
public:
	ib_msg_resp_scanner_parameters(tier2_message_processor *from, tier2_message_processor *to, const char xml[]) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_scanner_parameters()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: SCANNER_DATA (possibly multiple times per incoming message) */
class ib_msg_resp_scanner_data: public tws_response_message
{
public:
	ib_msg_resp_scanner_data(tier2_message_processor *from, tier2_message_processor *to, int ticker_id, int rank, ib_contract_details *cd, const char distance[], const char benchmark[], const char projection[], const char legs_str[]) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_scanner_data()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: SCANNER_DATA (once, after one or more invocations of event_scanner_data()) */
class ib_msg_resp_scanner_data_end: public tws_response_message
{
public:
	ib_msg_resp_scanner_data_end(tier2_message_processor *from, tier2_message_processor *to, int ticker_id, int num_elements) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_scanner_data_end()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: SCANNER_DATA (once, before any invocations of event_scanner_data()) */
class ib_msg_resp_scanner_data_start: public tws_response_message
{
public:
	ib_msg_resp_scanner_data_start(tier2_message_processor *from, tier2_message_processor *to, int ticker_id, int num_elements) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_scanner_data_start()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: CURRENT_TIME -- in response to REQ_CURRENT_TIME */
class ib_msg_resp_current_time: public tws_response_message
{
public:
	ib_msg_resp_current_time(tier2_message_processor *from, tier2_message_processor *to, long time) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_current_time()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: REAL_TIME_BARS */
class ib_msg_resp_realtime_bar: public tws_response_message
{
public:
	ib_msg_resp_realtime_bar(tier2_message_processor *from, tier2_message_processor *to, int reqid, long time, double open, double high, double low, double close, long int volume, double wap, int count) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_realtime_bar()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: FUNDAMENTAL_DATA */
class ib_msg_resp_fundamental_data: public tws_response_message
{
public:
	ib_msg_resp_fundamental_data(tier2_message_processor *from, tier2_message_processor *to, int reqid, const char data[]) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_fundamental_data()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: DELTA_NEUTRAL_VALIDATION */
class ib_msg_resp_delta_neutral_validation: public tws_response_message
{
public:
	ib_msg_resp_delta_neutral_validation(tier2_message_processor *from, tier2_message_processor *to, int reqid, ib_under_comp *und) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_delta_neutral_validation()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: ACCT_DOWNLOAD_END */
class ib_msg_resp_acct_download_end: public tws_response_message
{
public:
	ib_msg_resp_acct_download_end(tier2_message_processor *from, tier2_message_processor *to, char acct_name[]) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_acct_download_end()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: TICK_SNAPSHOT_END */
class ib_msg_resp_tick_snapshot_end: public tws_response_message
{
public:
	ib_msg_resp_tick_snapshot_end(tier2_message_processor *from, tier2_message_processor *to, int reqid) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_tick_snapshot_end()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: MARKET_DATA_TYPE */
class ib_msg_resp_market_data_type: public tws_response_message
{
public:
	ib_msg_resp_market_data_type(tier2_message_processor *from, tier2_message_processor *to, int reqid, tws_market_data_type_t data_type) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_market_data_type()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};

/* fired by: COMMISSION_REPORT */
class ib_msg_resp_commission_report: public tws_response_message
{
public:
	ib_msg_resp_commission_report(tier2_message_processor *from, tier2_message_processor *to, ib_commission_report *report) :
	  tws_response_message(from, to)
	  {
	  }
protected:
	virtual ~ib_msg_resp_commission_report()
	{
	}

public:
	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};















































#endif // TWS_RESPONSE_MSG_HEADER_INCLUDED
