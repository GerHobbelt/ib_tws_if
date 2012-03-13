
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

#include <tws_c_api/twsapi.h>

#include "tws_database_io.h"




#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


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

// forward reference:
typedef struct tier2_queue_item tier2_queue_item_t;

/*
callback which is invoked when the command has been received and has been parsed,
i.e. has been popped from the queue and the backend doesn't need the *command* data
any longer; it WILL continue to access to the response data/hooks though!

Use this one to clean up allocation of dynamic resources done in the front-end.
*/
typedef int tier2_command_done_handler(tier2_queue_item_t *cmd);
/*
And this one can be invoked by the front-end to have the back-end clean up its own resources
related to the response, as the front-end will invoke this one when it's completed processing
and accessing the response data contained in the tier2_queue_item.
*/
typedef int tier2_response_done_handler(tier2_queue_item_t **cmd);

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
    TIER2_NO_RESPONSE_YET = 1,
    TIER2_RESPONSE_READY = 2,
};


struct tier2_queue_item
{
    enum tier2_command_code command_code;       // what's the front-end requesting we do?
    void *propagate_ptr;
    union
    {
        struct
		{
			int ticker_id;
			tr_scanner_subscription_t *s;
		} scanner_subscription;

		// for various cancelations:
		int ticker_id;
		int order_id;
		int reqid;

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
	int exec_repeat_count;
	unsigned int exec_time_interval;

    tier2_command_done_handler *cleanup_after_command; // invoked when front-end MAY clean the command-side related resources it allocated for us
    tier2_response_done_handler *cleanup_after_response; // invoked when the front-end has completed processing the reponse and the backend can release any allocated resources which are related to this request.

    enum tier2_request_state request_state; // 0: pending, 1: success/done; < 0: error; > 1: warning/notification
    union
    {
        void *propagate_ptr;
        // ...
    } response_params;
};

// front-end invoked queue calls:
int tier2_send_request(struct tws_conn_cfg *tws_cfg, const tier2_queue_item_t *cmd);
int tier2_abort_request(struct tws_conn_cfg *tws_cfg, const tier2_queue_item_t *cmd);

// back-end invoked queue calls:
int tier2_pop_request(struct tws_conn_cfg *tws_cfg, tier2_queue_item_t *dst);
int tier2_send_response(struct tws_conn_cfg *tws_cfg, const tier2_queue_item_t *cmd, const tier2_queue_item_t *response);

// utility calls:
unsigned int tier2_queue_depth(struct tws_conn_cfg *tws_cfg);






/* internal communication stuff between mongoose threads and the tws back-end thread goes here: */
struct tws_thread_exch
{
    pthread_mutex_t tws_queue_mutex;    // mutex used to protect the queue itself
    pthread_mutex_t tws_ib_send_mutex;  // mutex used to protect message transmission to TWS/IB
    pthread_mutex_t tws_tx_mutex;		// mutex related to the TX condition in here
    pthread_cond_t tws_tx_signal;       // signalled when a TX request is pending (should be processed by the tws 'back-end' thread.
    pthread_mutex_t tws_rx_mutex;		// mutex related to the RX condition in here
    pthread_cond_t tws_rx_signal;       // signalled when a RX response is pending (should be processed by the related TX-invoking 'front-end' mongoose thread.

    int command;
    int response;
    time_t current_time;

    tier2_queue_item_t *work_queue;
    size_t work_queue_size;           // allocated size
    size_t work_queue_fill;           // number of requests pending in the queue
    size_t work_queue_poppos;         // the position of the queue 'head' for popping
    size_t work_queue_diane;          // the position of the queue 'head' for pushing
};





void init_tws_thread_exch(struct tws_thread_exch **ptr);
void destroy_tws_thread_exch(struct tws_thread_exch **ptr);





struct scanner_subscription_request_t;

/*
struct passed around as user parameter for all TWS API callbacks.
*/
struct my_tws_io_info
{
    struct mg_connection *conn;
    struct mg_context *ctx;
    struct tws_conn_cfg *tws_cfg;
    tws_instance_t *tws_handle;

    /* tracking some TWS values here as well: */
    int next_order_id;

    /* -- and the working men -- */

    /* scanner subscription request active set and queue: */
    size_t active_scanner_subscription_count;
    struct scanner_subscription_request_t *active_scanner_subscriptions[10];

    size_t queued_scanner_subscription_count;
    size_t queued_scanner_subscription_allocsize;
    struct scanner_subscription_request_t **scanner_subscription_queue;

    /* keeping track of the current TWS response message's number of items in the TWS response message: */
    size_t row_count;

	/* -- the databases which keep track of things for us -- */
	struct my_databases_info dbi;
};



/* --- scanner subscription request queue --- */


typedef struct scanner_subscription_request_t
{
    int ticker_id;
    tr_scanner_subscription_t reqdata;
} scanner_subscription_request_t;







void tws_worker_thread(struct mg_context *ctx);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // TWS_COMM_THREAD_HEADER_INCLUDED
