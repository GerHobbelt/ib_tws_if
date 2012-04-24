
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

#ifndef TWS_DATA_STRUCTURES_HEADER_INCLUDED
#define TWS_DATA_STRUCTURES_HEADER_INCLUDED


#if !defined(TWS_DEFINITIONS)
#define TWS_DEFINITIONS              1 /* enums only */
#endif
#include <tws_c_api/twsapi.h>



typedef tws::tr_origin_t tws_origin_t;
typedef tws::tr_oca_type_t tws_oca_type_t;
typedef tws::tr_auction_strategy_t tws_auction_strategy_t;
typedef tws::market_data_type_t tws_market_data_type_t;
typedef tws::tr_tick_type_t tws_tick_type_t;
typedef tws::tr_comboleg_type_t tws_comboleg_type_t;





#define DECLARE_TWS_FORWARD_REFERENCE(t)						\
    namespace tws												\
    { 															\
        extern "C"												\
        {														\
            t;													\
        }														\
    }



enum tws_order_type_t
{
    ORDER_MARKET_TO_LIMIT										,
    ORDER_MARKET_WITH_PROTECTION								,
    ORDER_REQUEST_FOR_QUOTE									,
    ORDER_STOP													,
    ORDER_STOP_LIMIT											,
    ORDER_TRAILING_LIMIT_IF_TOUCHED							,
    ORDER_TRAILING_MARKET_IF_TOUCHED							,
    ORDER_TRAILING_STOP										,
    ORDER_TRAILING_STOP_LIMIT									,
    ORDER_MARKET												,
    ORDER_MARKET_IF_TOUCHED									,
    ORDER_MARKET_ON_CLOSE										,
    ORDER_MARKET_ON_OPEN										,
    ORDER_PEGGED_TO_MARKET										,
    ORDER_RELATIVE												,
    ORDER_BOX_TOP												,
    ORDER_LIMIT_ON_CLOSE										,
    ORDER_LIMIT_ON_OPEN										,
    ORDER_LIMIT_IF_TOUCHED										,
    ORDER_PEGGED_TO_MIDPOINT									,
    ORDER_VWAP_GUARANTEED										,
    ORDER_GOOD_AFTER_TIME_DATE									,
    ORDER_GOOD_TILL_DATE_TIME									,
    ORDER_GOOD_TILL_CANCELED									,
    ORDER_IMMEDIATE_OR_CANCEL									,
    ORDER_ONE_CANCELS_ALL										,
    ORDER_VOLATILITY											,
    ORDER_LIMIT												,
    ORDER_ACTIVETIM											,
    ORDER_ADJUST												,
    ORDER_ALERT												,
    ORDER_ALLOC												,
    ORDER_AVGCOST												,
    ORDER_BASKET												,
    ORDER_COND													,
    ORDER_CONDORDER											,
    ORDER_CONSCOST												,
    ORDER_DAY													,
    ORDER_DEACT												,
    ORDER_DEACTDIS												,
    ORDER_DEACTEOD												,
    ORDER_GTT													,
    ORDER_HID													,
    ORDER_LTH													,
    ORDER_NONALGO												,
    ORDER_SCALE												,
    ORDER_SCALERST												,
    ORDER_WHATIF												,
};




/*
scanner scan codes:
*/
enum tws_scanner_type_t
{
    SCANNER_BOND_CUSIP_AZ                           ,
    SCANNER_BOND_CUSIP_ZA                           ,
    SCANNER_FAR_MATURITY_DATE                       ,
    SCANNER_HALTED                                  ,
    SCANNER_HIGHEST_SLB_BID                         ,
    SCANNER_HIGH_BOND_ASK_CURRENT_YIELD_ALL         ,
    SCANNER_HIGH_BOND_ASK_YIELD_ALL                 ,
    SCANNER_HIGH_BOND_DEBT_2_BOOK_RATIO             ,
    SCANNER_HIGH_BOND_DEBT_2_EQUITY_RATIO           ,
    SCANNER_HIGH_BOND_DEBT_2_TAN_BOOK_RATIO         ,
    SCANNER_HIGH_BOND_EQUITY_2_BOOK_RATIO           ,
    SCANNER_HIGH_BOND_EQUITY_2_TAN_BOOK_RATIO       ,
    SCANNER_HIGH_BOND_SPREAD_ALL                    ,
    SCANNER_HIGH_COUPON_RATE                        ,
    SCANNER_HIGH_DIVIDEND_YIELD                     ,
    SCANNER_HIGH_DIVIDEND_YIELD_IB                  ,
    SCANNER_HIGH_GROWTH_RATE                        ,
    SCANNER_HIGH_MOODY_RATING_ALL                   ,
    SCANNER_HIGH_OPEN_GAP                           ,
    SCANNER_HIGH_OPT_IMP_VOLAT                      ,
    SCANNER_HIGH_OPT_IMP_VOLAT_OVER_HIST            ,
    SCANNER_HIGH_OPT_OPEN_INTEREST_PUT_CALL_RATIO   ,
    SCANNER_HIGH_OPT_OPEN_INT_PUT_CALL_RATIO        ,
    SCANNER_HIGH_OPT_VOLUME_PUT_CALL_RATIO          ,
    SCANNER_HIGH_PE_RATIO                           ,
    SCANNER_HIGH_PRICE_2_BOOK_RATIO                 ,
    SCANNER_HIGH_PRICE_2_TAN_BOOK_RATIO             ,
    SCANNER_HIGH_QUICK_RATIO                        ,
    SCANNER_HIGH_RETURN_ON_EQUITY                   ,
    SCANNER_HIGH_SYNTH_BID_REV_NAT_YIELD            ,
    SCANNER_HIGH_VS_13W_HL                          ,
    SCANNER_HIGH_VS_26W_HL                          ,
    SCANNER_HIGH_VS_52W_HL                          ,
    SCANNER_HOT_BY_OPT_VOLUME                       ,
    SCANNER_HOT_BY_PRICE                            ,
    SCANNER_HOT_BY_PRICE_RANGE                      ,
    SCANNER_HOT_BY_VOLUME                           ,
    SCANNER_HOT_VOLUME                              ,
    SCANNER_LIMIT_UP_DOWN                           ,
    SCANNER_LOWEST_SLB_ASK                          ,
    SCANNER_LOW_BOND_BID_CURRENT_YIELD_ALL          ,
    SCANNER_LOW_BOND_BID_YIELD_ALL                  ,
    SCANNER_LOW_BOND_DEBT_2_BOOK_RATIO              ,
    SCANNER_LOW_BOND_DEBT_2_EQUITY_RATIO            ,
    SCANNER_LOW_BOND_DEBT_2_TAN_BOOK_RATIO          ,
    SCANNER_LOW_BOND_EQUITY_2_BOOK_RATIO            ,
    SCANNER_LOW_BOND_EQUITY_2_TAN_BOOK_RATIO        ,
    SCANNER_LOW_BOND_SPREAD_ALL                     ,
    SCANNER_LOW_COUPON_RATE                         ,
    SCANNER_LOW_GROWTH_RATE                         ,
    SCANNER_LOW_MOODY_RATING_ALL                    ,
    SCANNER_LOW_OPEN_GAP                            ,
    SCANNER_LOW_OPT_IMP_VOLAT                       ,
    SCANNER_LOW_OPT_IMP_VOLAT_OVER_HIST             ,
    SCANNER_LOW_OPT_OPEN_INTEREST_PUT_CALL_RATIO    ,
    SCANNER_LOW_OPT_OPEN_INT_PUT_CALL_RATIO         ,
    SCANNER_LOW_OPT_VOLUME_PUT_CALL_RATIO           ,
    SCANNER_LOW_PE_RATIO                            ,
    SCANNER_LOW_PRICE_2_BOOK_RATIO                  ,
    SCANNER_LOW_PRICE_2_TAN_BOOK_RATIO              ,
    SCANNER_LOW_QUICK_RATIO                         ,
    SCANNER_LOW_RETURN_ON_EQUITY                    ,
    SCANNER_LOW_SYNTH_ASK_REV_NAT_YIELD             ,
    SCANNER_LOW_VS_13W_HL                           ,
    SCANNER_LOW_VS_26W_HL                           ,
    SCANNER_LOW_VS_52W_HL                           ,
    SCANNER_LOW_WAR_REL_IMP_VOLAT                   ,
    SCANNER_MOST_ACTIVE                             ,
    SCANNER_MOST_ACTIVE_AVG_USD                     ,
    SCANNER_MOST_ACTIVE_USD                         ,
    SCANNER_NEAR_MATURITY_DATE                      ,
    SCANNER_NOT_OPEN                                ,
    SCANNER_OPT_OPEN_INTEREST_MOST_ACTIVE           ,
    SCANNER_OPT_VOLUME_MOST_ACTIVE                  ,
    SCANNER_PMONITOR_AVAIL_CONTRACTS                ,
    SCANNER_PMONITOR_CTT                            ,
    SCANNER_PMONITOR_RFQ                            ,
    SCANNER_TOP_OPEN_PERC_GAIN                      ,
    SCANNER_TOP_OPEN_PERC_LOSE                      ,
    SCANNER_TOP_OPT_IMP_VOLAT_GAIN                  ,
    SCANNER_TOP_OPT_IMP_VOLAT_LOSE                  ,
    SCANNER_TOP_PERC_GAIN                           ,
    SCANNER_TOP_PERC_LOSE                           ,
    SCANNER_TOP_PRICE_RANGE                         ,
    SCANNER_TOP_TRADE_COUNT                         ,
    SCANNER_TOP_TRADE_RATE                          ,
    SCANNER_TOP_VOLUME_RATE                         ,
    SCANNER_WSH_NEXT_ANALYST_MEETING                ,
    SCANNER_WSH_NEXT_EARNINGS                       ,
    SCANNER_WSH_NEXT_EVENT                          ,
    SCANNER_WSH_NEXT_MAJOR_EVENT                    ,
    SCANNER_WSH_PREV_ANALYST_MEETING                ,
    SCANNER_WSH_PREV_EARNINGS                       ,
    SCANNER_WSH_PREV_EVENT                          ,
};


enum tws_action_code_t
{
    ACTION_UNKNOWN = 0,
    ACTION_BUY = 1,
    ACTION_SELL = 2,
    ACTION_SSHORT = 3,
    ACTION_SSHORTX = 4,
};


enum tws_short_sale_slot_type_t
{
    SSS_UNKNOWN = 0,
    SSS_CLEARING_BROKER = 1,
    SSS_THIRD_PARTY = 2,
};



class ib_string_t : public std::string 
{
public:
	ib_string_t(const char *s) :
		std::string(s)
	{
	}
	ib_string_t() :
		std::string()
	{
	}

public:
	operator const char *()
	{
		return c_str();
	}
};
typedef int ib_int_t;
typedef bool ib_bool_t;
typedef double ib_double_t;
class ib_date_t
{
protected:
	// the number of 100-nanosecond intervals since January 1, 1601 (eqv. to Win32 FILETIME)
	time_t t;

public:
	ib_date_t() :
		t(0)
	{
	}
	ib_date_t(time_t _t) :
		t(_t)
	{
	}
	ib_date_t(const char *_t);

	virtual ~ib_date_t()
	{
	}

public:
	operator ib_string_t();
	ib_date_t &operator =(const char *timestamp);
};





template <typename T> class optional_value
{
protected:
    T stored_value;
    bool is_valid;

public:
    optional_value() : is_valid(false)
    {
    }
    optional_value(T &v) : stored_value(v), is_valid(true)
    {
    }
    ~optional_value()
    {
    }

public:
    void clear(void)
    {
        is_valid = false;
    }
    T set(T v)
    {
        is_valid = true;
        stored_value = v;
        return v;
    }
    bool valid(void) const
    {
        return is_valid;
    }
    T value(T default_value = T(0))
    {
        if (is_valid)
            return stored_value;
        else
            return default_value;
    }
	operator T()
	{
		return value();
	}

    optional_value<T> &operator =(T v)
    {
        is_valid = true;
        stored_value = v;
        return *this;
    }
    optional_value<T> &operator =(const optional_value<T> &v)
    {
        is_valid = v.is_valid;
        stored_value = v.stored_value;
        return *this;
    }
};



typedef optional_value<double>			o_double_t;
typedef optional_value<int>				o_int_t;
typedef optional_value<bool>			o_bool_t;
class o_string_t : public optional_value<std::string>
{
public:
    o_string_t(const char *v)
    {
        is_valid = true;
        stored_value = v;
    }
    o_string_t(const unsigned char *v)
    {
        is_valid = true;
        stored_value = (const char *)v;
    }
    o_string_t()
    {
        is_valid = false;
    }
    o_string_t(std::string &v)
    {
        is_valid = true;
        stored_value = v;
    }
    ~o_string_t()
    {
    }

public:
    const char *set(const char *v)
    {
        is_valid = true;
        stored_value = v;
        return v;
    }
    std::string value(const char *default_value = "") const
    {
        if (is_valid)
            return stored_value;
        else
            return default_value;
    }

    o_string_t &operator =(const char *v)
    {
        is_valid = true;
        stored_value = v;
        return *this;
    }
};
typedef optional_value<tws_origin_t>				o_origin_t;
typedef optional_value<tws_oca_type_t>				o_oca_type_t;
typedef optional_value<tws_auction_strategy_t>		o_auction_strategy_t;
typedef optional_value<tws_short_sale_slot_type_t>	o_short_sale_slot_type_t;







DECLARE_TWS_FORWARD_REFERENCE(struct under_comp);

class ib_under_comp
{
public:
    ib_double_t u_price;
    ib_double_t u_delta;
    ib_int_t    u_conid;

public:
    ib_under_comp();
    ib_under_comp(const tws::under_comp &uc);
    virtual ~ib_under_comp();
};

DECLARE_TWS_FORWARD_REFERENCE(struct tr_comboleg);

class ib_comboleg
{
public:
    tws_action_code_t			co_action;					/* BUY/SELL/SSHORT/SSHORTX */
    ib_string_t					co_exchange;
    ib_string_t					co_designated_location;     /* set to "" if unused, as usual */
    ib_int_t					co_conid;
    ib_int_t					co_ratio;
    tws_comboleg_type_t			co_open_close;
    tws_short_sale_slot_type_t	co_short_sale_slot;			/* 1 = clearing broker, 2 = third party */
    ib_int_t					co_exempt_code;             /* set to -1 if you do not use it */

public:
    ib_comboleg();
    ib_comboleg(const tws::tr_comboleg &leg);
    virtual ~ib_comboleg();
};

typedef optional_value<ib_under_comp>	o_ib_under_comp_t;



DECLARE_TWS_FORWARD_REFERENCE(struct tr_contract);


class ib_contract
{
public:
    o_ib_under_comp_t c_undercomp;                      /* delta neutral */
    o_double_t         c_strike;                        /* strike price for options */
    o_string_t         c_symbol;                        /* underlying symbol */
    o_string_t         c_sectype;                       /* security type ("BAG" -> transmits combo legs, "" -> does not transmit combo legs, "BOND" -> strike/expiry/right/multiplier/local_symbol must be set) */
    o_string_t         c_exchange;
    o_string_t         c_primary_exch;                  /* for SMART orders, specify an actual exchange where the contract trades, e.g. ISLAND.  Pick a non-aggregate (ie not the SMART exchange) exchange that the contract trades on.  DO NOT SET TO SMART. */
    o_string_t         c_expiry;                        /* expiration for futures and options */
    o_string_t         c_currency;                      /* currency, e.g. USD */
    o_string_t         c_right;                         /* put or call (P or C) */
    o_string_t         c_local_symbol;                  /* local symbol for futures or options, e.g. ESZN for ES DEC09 contract  */
    o_string_t         c_multiplier;
    o_string_t         c_combolegs_descrip;             /* received in open order version 14 and up for all combos */
    o_string_t         c_secid_type;                    /* CUSIP;SEDOL;ISIN;RIC */
    o_string_t         c_secid;

    typedef std::vector<ib_comboleg> ib_comboleg_collection_t;

    ib_comboleg_collection_t c_comboleg;                /* COMBOS */
    o_int_t            c_conid;                         /* contract id, returned from TWS */
    o_bool_t           c_include_expired;               /* for contract requests, specifies that even expired contracts should be returned.  Can not be set to true for orders. */

public:
    ib_contract();
    ib_contract(const tws::tr_contract &c);
    virtual ~ib_contract();

public:
	operator tws::tr_contract();
};


DECLARE_TWS_FORWARD_REFERENCE(struct tr_tag_value);

class ib_tag_value
{
public:
    ib_string_t  t_tag;
    ib_string_t  t_val;

public:
    ib_tag_value();
    ib_tag_value(const tws::tr_tag_value &t);
    virtual ~ib_tag_value();
};

typedef std::vector<ib_tag_value> ib_tag_value_collection_t;


DECLARE_TWS_FORWARD_REFERENCE(struct tr_contract_details);

class ib_contract_details
{
public:
    ib_double_t                     d_mintick;
    ib_double_t                     d_coupon;                /* for bonds */
    ib_double_t                     d_ev_multiplier;
    ib_contract						d_summary;
    ib_string_t                     d_market_name;
    ib_string_t                     d_trading_class;
    ib_string_t                     d_order_types;
    ib_string_t                     d_valid_exchanges;
    ib_string_t                     d_cusip;                 /* for bonds */
    ib_string_t                     d_maturity;              /* for bonds */
    ib_string_t                     d_issue_date;            /* for bonds */
    ib_string_t                     d_ratings;               /* for bonds */
    ib_string_t                     d_bond_type;             /* for bonds */
    ib_string_t                     d_coupon_type;           /* for bonds */
    ib_string_t                     d_desc_append;           /* for bonds */
    ib_string_t                     d_next_option_date;      /* for bonds */
    ib_string_t                     d_next_option_type;      /* for bonds */
    ib_string_t                     d_notes;                 /* for bonds */
    ib_string_t                     d_long_name;
    ib_string_t                     d_contract_month;
    ib_string_t                     d_industry;
    ib_string_t                     d_category;
    ib_string_t                     d_subcategory;
    ib_string_t                     d_timezone_id;
    ib_string_t                     d_trading_hours;
    ib_string_t                     d_liquid_hours;
    ib_string_t                     d_ev_rule;

    ib_tag_value_collection_t  d_sec_id_list;

    ib_int_t                        d_price_magnifier;
    ib_int_t                        d_under_conid;
    ib_bool_t 					    d_convertible;         /* for bonds */
    ib_bool_t                       d_callable;            /* for bonds */
    ib_bool_t                       d_putable;             /* for bonds */
    ib_bool_t                       d_next_option_partial; /* for bonds */

public:
    ib_contract_details();
    ib_contract_details(const tws::tr_contract_details &cd);
    virtual ~ib_contract_details();
};


DECLARE_TWS_FORWARD_REFERENCE(struct tr_order_combo_leg);

class ib_order_combo_leg
{
public:
    ib_double_t cl_price;									/* price per leg */

public:
    ib_order_combo_leg();
    ib_order_combo_leg(const tws::tr_order_combo_leg &cl);
    virtual ~ib_order_combo_leg();
};


DECLARE_TWS_FORWARD_REFERENCE(struct tr_order);

class ib_order
{
public:
    o_double_t   o_discretionary_amt;                       /* SMART routing only: amount you are willing to pay above your specified limit price */
    o_double_t   o_lmt_price;                               /* Basic Order Field: limit price  */
    o_double_t   o_aux_price;                               /* Basic Order Field: stop price, trailing amount, or offset amount  */
    o_double_t   o_percent_offset;                          /* Advanced order field: the offset amount for Relative (REL) orders, specified as a percent; specify either this OR the offset amount in m_auxPrice */
    o_double_t   o_nbbo_price_cap;                          /* SMART routing only: see 'm_firmQuoteOnly' comment */
    o_double_t   o_starting_price;                          /* For BOX option-pegged-to-stock orders only */
    o_double_t   o_stock_ref_price;                         /* For BOX option-pegged-to-stock orders only */
    o_double_t   o_delta;                                   /* For BOX option-pegged-to-stock orders only */
    o_double_t   o_stock_range_lower;                       /* For Pegged-to-stock or Volatility orders only: if stock price moves below this price, order will be canceled */
    o_double_t   o_stock_range_upper;                       /* For Pegged-to-stock or Volatility orders only: if stock price moves above this price, order will be canceled */
    o_double_t   o_volatility;                              /* For Volatility orders only: volatility (percent) */
    o_double_t   o_delta_neutral_aux_price;
    o_double_t   o_trail_stop_price;                        /* Advanced order field: initial stop price for trailing stop (TRAIL) orders */
    o_double_t   o_trailing_percent;
    o_double_t   o_basis_points;
    o_double_t   o_scale_price_increment;					/* For SCALE orders only */
    o_double_t   o_scale_price_adjust_value;				/* For SCALE orders only */
    o_double_t   o_scale_profit_offset;						/* For SCALE orders only */
    o_string_t   o_algo_strategy;
    o_string_t   o_good_after_time;                         /* Advanced order field: format: YYYYMMDD HH:MM:SS {time zone}  e.g. 20060505 08:00:00 EST */
    o_string_t   o_good_till_date;                          /* Advanced order field: format: YYYYMMDD HH:MM:SS {time zone}  e.g. 20060505 08:00:00 EST */
    o_string_t   o_fagroup;                                 /* For Financial advisors (FA) only: Advisor group, e.g. "All" */
    o_string_t   o_famethod;                                /* For Financial advisors (FA) only: Advisor method: PctChange, AvailableEquity, NetLiq, or EqualQuantity */
    o_string_t   o_fapercentage;                            /* For Financial advisors (FA) only: Advisor percentage, used when the method is set to PctChange */
    o_string_t   o_faprofile;                               /* For Financial advisors (FA) only: Advisor profile */
    o_string_t   o_action;                                  /* Basic Order Field: specify BUY or SELL; non-cleared customers can specify SSHORT */
    o_string_t   o_order_type;                              /* Basic Order Field: order type, e.g. LMT, MKT, STOP, TRAIL, REL */
    o_string_t   o_tif;                                     /* Advanced order field: Time in force, e.g. DAY, GTC */
    o_string_t   o_oca_group;                               /* Advanced order field: OCA group name (OCA = "one cancels all") */
    o_string_t   o_account;                                 /* Clearing info: IB account; can be left blank for users with only a single account   */
    o_string_t   o_open_close;                              /* For non-cleared (i.e. institutional) customers only: open/close flag: O=Open, C=Close */
    o_string_t   o_orderref;                                /* Advanced order field: order reference, enter any free-form text */
    o_string_t   o_designated_location;                     /* For non-cleared (i.e. institutional) customers only: specifies where the shares are held; set only when m_shortSaleSlot=2 */
    o_string_t   o_rule80a;                                 /* Advanced order field: Individual = 'I', Agency = 'A', AgentOtherMember = 'W', IndividualPTIA = 'J', AgencyPTIA = 'U', AgentOtherMemberPTIA = 'M', IndividualPT = 'K', AgencyPT = 'Y', AgentOtherMemberPT = 'N' */
    o_string_t   o_settling_firm;
    o_string_t   o_delta_neutral_order_type;
    o_string_t   o_clearing_account;                        /* Clearing info: True beneficiary of the order */
    o_string_t   o_clearing_intent;                         /* Clearing info: "" (Default), "IB", "Away", "PTA" (PostTrade) */
    o_string_t   o_hedge_type;								/* Hedge Orders: 'D' - delta, 'B' - beta, 'F' - FX, 'P' - pair */
    o_string_t   o_hedge_param;								/* Hedge Orders: 'beta=X' value for beta hedge, 'ratio=Y' for pair hedge */
    o_string_t   o_delta_neutral_settling_firm;				/* For Volatility orders only: */
    o_string_t   o_delta_neutral_clearing_account;			/* For Volatility orders only: */
    o_string_t   o_delta_neutral_clearing_intent;			/* For Volatility orders only: */

    ib_tag_value_collection_t o_algo_params;                /* 'm_algoParams': array of length o_algo_params_count, API user responsible for alloc/free */
    ib_tag_value_collection_t o_smart_combo_routing_params;	/* Smart combo routing params: 'm_smartComboRoutingParams': array of length o_smart_combo_routing_params, API user responsible for alloc/free */

    typedef std::vector<ib_order_combo_leg> ib_order_combo_leg_collection_t;

    ib_order_combo_leg_collection_t o_combo_legs;

    o_int_t      o_orderid;                                 /* Basic Order Field: order id generated by API client */
    o_int_t      o_total_quantity;                          /* Basic Order Field: order size */
    o_origin_t   o_origin;									/* For non-cleared (i.e. institutional) customers only: origin: 0=Customer, 1=Firm */
    o_int_t      o_clientid;                                /* Basic Order Field: client id of the API client that submitted the order */
    o_int_t      o_permid;                                  /* Basic Order Field: TWS order ID (not specified by API) */
    o_int_t      o_parentid;                                /* Advanced order field: order id of parent order, to associate attached stop, trailing stop, or bracket orders with parent order (Auto STP, TRAIL)  */
    o_int_t      o_display_size;                            /* Advanced order field: the portion of the order that will be visible to the world */
    o_int_t      o_trigger_method;                          /* Advanced order field: for stop orders:  0=Default, 1=Double_Bid_Ask, 2=Last, 3=Double_Last, 4=Bid_Ask, 7=Last_or_Bid_Ask, 8=Mid-point */
    o_int_t      o_min_qty;                                 /* Advanced order field: no partial fills less than the size specified here */
    o_int_t      o_volatility_type;                         /* For Volatility orders only: volatility type: 1=daily, 2=annual */
    o_int_t      o_reference_price_type;                    /* For Volatility orders only: what to use as the current stock price: 1=bid/ask average, 2 = bid or ask */
    o_int_t      o_basis_points_type;
    o_int_t      o_scale_subs_level_size;					/* For SCALE orders only */
    o_int_t      o_scale_init_level_size;					/* For SCALE orders only */
    o_int_t      o_scale_price_adjust_interval;				/* For SCALE orders only */
    o_int_t      o_scale_init_position;						/* For SCALE orders only */
    o_int_t      o_scale_init_fill_qty;						/* For SCALE orders only */
    o_int_t      o_exempt_code;                             /* set to -1 if you do not use it */
    o_int_t      o_delta_neutral_con_id;					/* For Volatility orders only: */
    o_oca_type_t o_oca_type;								/* Advanced order field: OCA group type  1 = CANCEL_WITH_BLOCK, 2 = REDUCE_WITH_BLOCK, 3 = REDUCE_NON_BLOCK */
    o_auction_strategy_t o_auction_strategy;				/* For BOX option-pegged-to-stock and Volatility orders only: 1=AUCTION_MATCH, 2=AUCTION_IMPROVEMENT, 3=AUCTION_TRANSPARENT */
    o_short_sale_slot_type_t o_short_sale_slot;				/* For non-cleared (i.e. institutional) customers only: specify only if m_action is "SSHORT": 1 if you hold the shares, 2 if they will be delivered from elsewhere */
    o_bool_t     o_override_percentage_constraints;			/* Advanced order field: set true to override normal percentage constraint checks */
    o_bool_t     o_firm_quote_only;							/* SMART routing only: if true, specifies that order should be routed to exchanges showing a "firm" quote, but not if the exchange is off the NBBO by more than the 'm_nbboPriceCap' amount */
    o_bool_t     o_etrade_only;
    o_bool_t     o_all_or_none;								/* Advanced order field: if set to true, there can be no partial fills for the order */
    o_bool_t     o_outside_rth;								/* Advanced order field: if true, order could fill or trigger anytime; if false, order will fill or trigger only during regular trading hours */
    o_bool_t     o_hidden;									/* Advanced order field: if true, order will be hidden, and will not be reflected in the market data or deep book */
    o_bool_t     o_transmit;								/* Advanced order field: if false, order will be created in TWS but not transmitted */
    o_bool_t     o_block_order;								/* Advanced order field: block order, for ISE option orders only */
    o_bool_t     o_sweep_to_fill;							/* Advanced order field: for SMART orders, specifies that orders should be split and sent to multiple exchanges at the same time */
    o_bool_t     o_continuous_update;						/* For Volatility orders only: if true, price will be continuously recalculated after order submission */
    o_bool_t     o_whatif;									/* if true, the order will not be submitted, but margin info will be returned */
    o_bool_t     o_not_held;
    o_bool_t     o_opt_out_smart_routing;					/* SMART routing only: */
    o_bool_t     o_scale_auto_reset;						/* For SCALE orders only */
    o_bool_t     o_scale_random_percent;					/* For SCALE orders only */

public:
    ib_order();
    ib_order(const tws::tr_order &o);
    virtual ~ib_order();

public:
    operator tws::tr_order();
};



DECLARE_TWS_FORWARD_REFERENCE(struct tr_order_status);

/*
OrderState
*/
class ib_order_status
{
public:
    ib_double_t ost_commission;
    ib_double_t ost_min_commission;
    ib_double_t ost_max_commission;
    ib_string_t ost_status;
    ib_string_t ost_init_margin;
    ib_string_t ost_maint_margin;
    ib_string_t ost_equity_with_loan;
    ib_string_t ost_commission_currency;
    ib_string_t ost_warning_text;

public:
    ib_order_status();
    ib_order_status(const tws::tr_order_status &o);
    virtual ~ib_order_status();
};


DECLARE_TWS_FORWARD_REFERENCE(struct tr_execution);

class ib_execution
{
public:
    ib_double_t e_price;
    ib_double_t e_avg_price;
    ib_double_t e_ev_multiplier;
    ib_string_t e_execid;
    ib_string_t e_time;
    ib_string_t e_acct_number;
    ib_string_t e_exchange;
    ib_string_t e_side;
    ib_string_t e_orderref;
    ib_string_t e_ev_rule;
    ib_int_t    e_shares;
    ib_int_t    e_permid;
    ib_int_t    e_clientid;
    ib_int_t    e_liquidation;
    ib_int_t    e_orderid;
    ib_int_t    e_cum_qty;

public:
    ib_execution();
    ib_execution(const tws::tr_execution &e);
    virtual ~ib_execution();
};


DECLARE_TWS_FORWARD_REFERENCE(struct tr_exec_filter);

class ib_exec_filter
{
public:
    o_string_t f_acctcode;
    o_string_t f_time;
    o_string_t f_symbol;
    o_string_t f_sectype;
    o_string_t f_exchange;
    o_string_t f_side;
    o_int_t    f_clientid;

public:
    ib_exec_filter();
    ib_exec_filter(const tws::tr_exec_filter &f);
    virtual ~ib_exec_filter();

public:
    operator tws::tr_exec_filter();
};



DECLARE_TWS_FORWARD_REFERENCE(struct tr_scanner_subscription);


class ib_scanner_subscription
{
public:
    o_double_t scan_above_price;
    o_double_t scan_below_price;
    o_double_t scan_coupon_rate_above;
    o_double_t scan_coupon_rate_below;
    o_double_t scan_market_cap_above;
    o_double_t scan_market_cap_below;
    o_string_t scan_exclude_convertible;
    o_string_t scan_instrument;
    o_string_t scan_location_code;
    o_string_t scan_maturity_date_above;
    o_string_t scan_maturity_date_below;
    o_string_t scan_moody_rating_above;
    o_string_t scan_moody_rating_below;
    o_string_t scan_code;
    o_string_t scan_sp_rating_above;
    o_string_t scan_sp_rating_below;
    o_string_t scan_scanner_setting_pairs;
    o_string_t scan_stock_type_filter;
    o_int_t    scan_above_volume;
    o_int_t    scan_number_of_rows;
    o_int_t    scan_average_option_volume_above;

public:
    ib_scanner_subscription();
    ib_scanner_subscription(const tws::tr_scanner_subscription &s);
    virtual ~ib_scanner_subscription();

public:
    operator tws::tr_scanner_subscription();
};



DECLARE_TWS_FORWARD_REFERENCE(struct tr_commission_report);

class ib_commission_report
{
public:
    ib_string_t cr_exec_id;
    ib_string_t cr_currency;
    ib_double_t	cr_commission;
    ib_double_t cr_realized_pnl;
    ib_double_t cr_yield;
    ib_int_t    cr_yield_redemption_date;							/* YYYYMMDD format */

public:
    ib_commission_report();
    ib_commission_report(const tws::tr_commission_report &s);
    virtual ~ib_commission_report();
};





#endif // TWS_DATA_STRUCTURES_HEADER_INCLUDED
