
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

#ifndef MESSAGE_QUEUE_HEADER_INCLUDED
#define MESSAGE_QUEUE_HEADER_INCLUDED

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


#endif // MESSAGE_QUEUE_HEADER_INCLUDED
