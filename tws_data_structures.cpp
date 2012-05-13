
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

/*
 * Mongoose-based server app which interfaces with a TWS station and displays data in HTML form,
 * suitable for loading by, for example, 64-bit Excel 2010, using web queries->
 */

#include <tws_c_api/twsapi.h>

#include "system-includes.h"

#include "tws_data_structures.h"
#include "mongoose_headers.h"



using namespace tws;


const char *to_tws(tws_order_type_t typ)
{
	switch (typ)
{
	case ORDER_MARKET_TO_LIMIT										:
		return ORDER_TYPE_MARKET_TO_LIMIT										;

	case ORDER_MARKET_WITH_PROTECTION								:
		return ORDER_TYPE_MARKET_WITH_PROTECTION								;

	case ORDER_REQUEST_FOR_QUOTE									:
		return ORDER_TYPE_REQUEST_FOR_QUOTE									;

	case ORDER_STOP													:
		return ORDER_TYPE_STOP													;

	case ORDER_STOP_LIMIT											:
		return ORDER_TYPE_STOP_LIMIT											;

	case ORDER_TRAILING_LIMIT_IF_TOUCHED							:
		return ORDER_TYPE_TRAILING_LIMIT_IF_TOUCHED							;

	case ORDER_TRAILING_MARKET_IF_TOUCHED							:
		return ORDER_TYPE_TRAILING_MARKET_IF_TOUCHED							;

	case ORDER_TRAILING_STOP										:
		return ORDER_TYPE_TRAILING_STOP										;

	case ORDER_TRAILING_STOP_LIMIT									:
		return ORDER_TYPE_TRAILING_STOP_LIMIT									;

	case ORDER_MARKET												:
		return ORDER_TYPE_MARKET												;

	case ORDER_MARKET_IF_TOUCHED									:
		return ORDER_TYPE_MARKET_IF_TOUCHED									;

	case ORDER_MARKET_ON_CLOSE										:
		return ORDER_TYPE_MARKET_ON_CLOSE										;

	case ORDER_MARKET_ON_OPEN										:
		return ORDER_TYPE_MARKET_ON_OPEN										;

	case ORDER_PEGGED_TO_MARKET										:
		return ORDER_TYPE_PEGGED_TO_MARKET										;

	case ORDER_RELATIVE												:
		return ORDER_TYPE_RELATIVE												;

	case ORDER_BOX_TOP												:
		return ORDER_TYPE_BOX_TOP												;

	case ORDER_LIMIT_ON_CLOSE										:
		return ORDER_TYPE_LIMIT_ON_CLOSE										;

	case ORDER_LIMIT_ON_OPEN										:
		return ORDER_TYPE_LIMIT_ON_OPEN										;

	case ORDER_LIMIT_IF_TOUCHED										:
		return ORDER_TYPE_LIMIT_IF_TOUCHED										;

	case ORDER_PEGGED_TO_MIDPOINT									:
		return ORDER_TYPE_PEGGED_TO_MIDPOINT									;

	case ORDER_VWAP_GUARANTEED										:
		return ORDER_TYPE_VWAP_GUARANTEED										;

	case ORDER_GOOD_AFTER_TIME_DATE									:
		return ORDER_TYPE_GOOD_AFTER_TIME_DATE									;

	case ORDER_GOOD_TILL_DATE_TIME									:
		return ORDER_TYPE_GOOD_TILL_DATE_TIME									;

	case ORDER_GOOD_TILL_CANCELED									:
		return ORDER_TYPE_GOOD_TILL_CANCELED									;

	case ORDER_IMMEDIATE_OR_CANCEL									:
		return ORDER_TYPE_IMMEDIATE_OR_CANCEL									;

	case ORDER_ONE_CANCELS_ALL										:
		return ORDER_TYPE_ONE_CANCELS_ALL										;

	case ORDER_VOLATILITY											:
		return ORDER_TYPE_VOLATILITY											;

	case ORDER_LIMIT												:
		return ORDER_TYPE_LIMIT												;

	case ORDER_ACTIVETIM											:
		return ORDER_TYPE_ACTIVETIM											;

	case ORDER_ADJUST												:
		return ORDER_TYPE_ADJUST												;

	case ORDER_ALERT												:
		return ORDER_TYPE_ALERT												;

	case ORDER_ALLOC												:
		return ORDER_TYPE_ALLOC												;

	case ORDER_AVGCOST												:
		return ORDER_TYPE_AVGCOST												;

	case ORDER_BASKET												:
		return ORDER_TYPE_BASKET												;

	case ORDER_COND													:
		return ORDER_TYPE_COND													;

	case ORDER_CONDORDER											:
		return ORDER_TYPE_CONDORDER											;

	case ORDER_CONSCOST												:
		return ORDER_TYPE_CONSCOST												;

	case ORDER_DAY													:
		return ORDER_TYPE_DAY													;

	case ORDER_DEACT												:
		return ORDER_TYPE_DEACT												;

	case ORDER_DEACTDIS												:
		return ORDER_TYPE_DEACTDIS												;

	case ORDER_DEACTEOD												:
		return ORDER_TYPE_DEACTEOD												;

	case ORDER_GTT													:
		return ORDER_TYPE_GTT													;

	case ORDER_HID													:
		return ORDER_TYPE_HID													;

	case ORDER_LTH													:
		return ORDER_TYPE_LTH													;

	case ORDER_NONALGO												:
		return ORDER_TYPE_NONALGO												;

	case ORDER_SCALE												:
		return ORDER_TYPE_SCALE												;

	case ORDER_SCALERST												:
		return ORDER_TYPE_SCALERST												;

	case ORDER_WHATIF												:
		return ORDER_TYPE_WHATIF												;

	default:
		return NULL;
	}
}



const char *to_tws(tws_scanner_type_t typ)
{
	switch (typ)
{
	case SCANNER_BOND_CUSIP_AZ                           :
		return TWS_SCANNER_BOND_CUSIP_AZ                           ;

	case SCANNER_BOND_CUSIP_ZA                           :
		return TWS_SCANNER_BOND_CUSIP_ZA                           ;

	case SCANNER_FAR_MATURITY_DATE                       :
		return TWS_SCANNER_FAR_MATURITY_DATE                       ;

	case SCANNER_HALTED                                  :
		return TWS_SCANNER_HALTED                                  ;

	case SCANNER_HIGHEST_SLB_BID                         :
		return TWS_SCANNER_HIGHEST_SLB_BID                         ;

	case SCANNER_HIGH_BOND_ASK_CURRENT_YIELD_ALL         :
		return TWS_SCANNER_HIGH_BOND_ASK_CURRENT_YIELD_ALL         ;

	case SCANNER_HIGH_BOND_ASK_YIELD_ALL                 :
		return TWS_SCANNER_HIGH_BOND_ASK_YIELD_ALL                 ;

	case SCANNER_HIGH_BOND_DEBT_2_BOOK_RATIO             :
		return TWS_SCANNER_HIGH_BOND_DEBT_2_BOOK_RATIO             ;

	case SCANNER_HIGH_BOND_DEBT_2_EQUITY_RATIO           :
		return TWS_SCANNER_HIGH_BOND_DEBT_2_EQUITY_RATIO           ;

	case SCANNER_HIGH_BOND_DEBT_2_TAN_BOOK_RATIO         :
		return TWS_SCANNER_HIGH_BOND_DEBT_2_TAN_BOOK_RATIO         ;

	case SCANNER_HIGH_BOND_EQUITY_2_BOOK_RATIO           :
		return TWS_SCANNER_HIGH_BOND_EQUITY_2_BOOK_RATIO           ;

	case SCANNER_HIGH_BOND_EQUITY_2_TAN_BOOK_RATIO       :
		return TWS_SCANNER_HIGH_BOND_EQUITY_2_TAN_BOOK_RATIO       ;

	case SCANNER_HIGH_BOND_SPREAD_ALL                    :
		return TWS_SCANNER_HIGH_BOND_SPREAD_ALL                    ;

	case SCANNER_HIGH_COUPON_RATE                        :
		return TWS_SCANNER_HIGH_COUPON_RATE                        ;

	case SCANNER_HIGH_DIVIDEND_YIELD                     :
		return TWS_SCANNER_HIGH_DIVIDEND_YIELD                     ;

	case SCANNER_HIGH_DIVIDEND_YIELD_IB                  :
		return TWS_SCANNER_HIGH_DIVIDEND_YIELD_IB                  ;

	case SCANNER_HIGH_GROWTH_RATE                        :
		return TWS_SCANNER_HIGH_GROWTH_RATE                        ;

	case SCANNER_HIGH_MOODY_RATING_ALL                   :
		return TWS_SCANNER_HIGH_MOODY_RATING_ALL                   ;

	case SCANNER_HIGH_OPEN_GAP                           :
		return TWS_SCANNER_HIGH_OPEN_GAP                           ;

	case SCANNER_HIGH_OPT_IMP_VOLAT                      :
		return TWS_SCANNER_HIGH_OPT_IMP_VOLAT                      ;

	case SCANNER_HIGH_OPT_IMP_VOLAT_OVER_HIST            :
		return TWS_SCANNER_HIGH_OPT_IMP_VOLAT_OVER_HIST            ;

	case SCANNER_HIGH_OPT_OPEN_INTEREST_PUT_CALL_RATIO   :
		return TWS_SCANNER_HIGH_OPT_OPEN_INTEREST_PUT_CALL_RATIO   ;

	case SCANNER_HIGH_OPT_OPEN_INT_PUT_CALL_RATIO        :
		return TWS_SCANNER_HIGH_OPT_OPEN_INT_PUT_CALL_RATIO        ;

	case SCANNER_HIGH_OPT_VOLUME_PUT_CALL_RATIO          :
		return TWS_SCANNER_HIGH_OPT_VOLUME_PUT_CALL_RATIO          ;

	case SCANNER_HIGH_PE_RATIO                           :
		return TWS_SCANNER_HIGH_PE_RATIO                           ;

	case SCANNER_HIGH_PRICE_2_BOOK_RATIO                 :
		return TWS_SCANNER_HIGH_PRICE_2_BOOK_RATIO                 ;

	case SCANNER_HIGH_PRICE_2_TAN_BOOK_RATIO             :
		return TWS_SCANNER_HIGH_PRICE_2_TAN_BOOK_RATIO             ;

	case SCANNER_HIGH_QUICK_RATIO                        :
		return TWS_SCANNER_HIGH_QUICK_RATIO                        ;

	case SCANNER_HIGH_RETURN_ON_EQUITY                   :
		return TWS_SCANNER_HIGH_RETURN_ON_EQUITY                   ;

	case SCANNER_HIGH_SYNTH_BID_REV_NAT_YIELD            :
		return TWS_SCANNER_HIGH_SYNTH_BID_REV_NAT_YIELD            ;

	case SCANNER_HIGH_VS_13W_HL                          :
		return TWS_SCANNER_HIGH_VS_13W_HL                          ;

	case SCANNER_HIGH_VS_26W_HL                          :
		return TWS_SCANNER_HIGH_VS_26W_HL                          ;

	case SCANNER_HIGH_VS_52W_HL                          :
		return TWS_SCANNER_HIGH_VS_52W_HL                          ;

	case SCANNER_HOT_BY_OPT_VOLUME                       :
		return TWS_SCANNER_HOT_BY_OPT_VOLUME                       ;

	case SCANNER_HOT_BY_PRICE                            :
		return TWS_SCANNER_HOT_BY_PRICE                            ;

	case SCANNER_HOT_BY_PRICE_RANGE                      :
		return TWS_SCANNER_HOT_BY_PRICE_RANGE                      ;

	case SCANNER_HOT_BY_VOLUME                           :
		return TWS_SCANNER_HOT_BY_VOLUME                           ;

	case SCANNER_HOT_VOLUME                              :
		return TWS_SCANNER_HOT_VOLUME                              ;

	case SCANNER_LIMIT_UP_DOWN                           :
		return TWS_SCANNER_LIMIT_UP_DOWN                           ;

	case SCANNER_LOWEST_SLB_ASK                          :
		return TWS_SCANNER_LOWEST_SLB_ASK                          ;

	case SCANNER_LOW_BOND_BID_CURRENT_YIELD_ALL          :
		return TWS_SCANNER_LOW_BOND_BID_CURRENT_YIELD_ALL          ;

	case SCANNER_LOW_BOND_BID_YIELD_ALL                  :
		return TWS_SCANNER_LOW_BOND_BID_YIELD_ALL                  ;

	case SCANNER_LOW_BOND_DEBT_2_BOOK_RATIO              :
		return TWS_SCANNER_LOW_BOND_DEBT_2_BOOK_RATIO              ;

	case SCANNER_LOW_BOND_DEBT_2_EQUITY_RATIO            :
		return TWS_SCANNER_LOW_BOND_DEBT_2_EQUITY_RATIO            ;

	case SCANNER_LOW_BOND_DEBT_2_TAN_BOOK_RATIO          :
		return TWS_SCANNER_LOW_BOND_DEBT_2_TAN_BOOK_RATIO          ;

	case SCANNER_LOW_BOND_EQUITY_2_BOOK_RATIO            :
		return TWS_SCANNER_LOW_BOND_EQUITY_2_BOOK_RATIO            ;

	case SCANNER_LOW_BOND_EQUITY_2_TAN_BOOK_RATIO        :
		return TWS_SCANNER_LOW_BOND_EQUITY_2_TAN_BOOK_RATIO        ;

	case SCANNER_LOW_BOND_SPREAD_ALL                     :
		return TWS_SCANNER_LOW_BOND_SPREAD_ALL                     ;

	case SCANNER_LOW_COUPON_RATE                         :
		return TWS_SCANNER_LOW_COUPON_RATE                         ;

	case SCANNER_LOW_GROWTH_RATE                         :
		return TWS_SCANNER_LOW_GROWTH_RATE                         ;

	case SCANNER_LOW_MOODY_RATING_ALL                    :
		return TWS_SCANNER_LOW_MOODY_RATING_ALL                    ;

	case SCANNER_LOW_OPEN_GAP                            :
		return TWS_SCANNER_LOW_OPEN_GAP                            ;

	case SCANNER_LOW_OPT_IMP_VOLAT                       :
		return TWS_SCANNER_LOW_OPT_IMP_VOLAT                       ;

	case SCANNER_LOW_OPT_IMP_VOLAT_OVER_HIST             :
		return TWS_SCANNER_LOW_OPT_IMP_VOLAT_OVER_HIST             ;

	case SCANNER_LOW_OPT_OPEN_INTEREST_PUT_CALL_RATIO    :
		return TWS_SCANNER_LOW_OPT_OPEN_INTEREST_PUT_CALL_RATIO    ;

	case SCANNER_LOW_OPT_OPEN_INT_PUT_CALL_RATIO         :
		return TWS_SCANNER_LOW_OPT_OPEN_INT_PUT_CALL_RATIO         ;

	case SCANNER_LOW_OPT_VOLUME_PUT_CALL_RATIO           :
		return TWS_SCANNER_LOW_OPT_VOLUME_PUT_CALL_RATIO           ;

	case SCANNER_LOW_PE_RATIO                            :
		return TWS_SCANNER_LOW_PE_RATIO                            ;

	case SCANNER_LOW_PRICE_2_BOOK_RATIO                  :
		return TWS_SCANNER_LOW_PRICE_2_BOOK_RATIO                  ;

	case SCANNER_LOW_PRICE_2_TAN_BOOK_RATIO              :
		return TWS_SCANNER_LOW_PRICE_2_TAN_BOOK_RATIO              ;

	case SCANNER_LOW_QUICK_RATIO                         :
		return TWS_SCANNER_LOW_QUICK_RATIO                         ;

	case SCANNER_LOW_RETURN_ON_EQUITY                    :
		return TWS_SCANNER_LOW_RETURN_ON_EQUITY                    ;

	case SCANNER_LOW_SYNTH_ASK_REV_NAT_YIELD             :
		return TWS_SCANNER_LOW_SYNTH_ASK_REV_NAT_YIELD             ;

	case SCANNER_LOW_VS_13W_HL                           :
		return TWS_SCANNER_LOW_VS_13W_HL                           ;

	case SCANNER_LOW_VS_26W_HL                           :
		return TWS_SCANNER_LOW_VS_26W_HL                           ;

	case SCANNER_LOW_VS_52W_HL                           :
		return TWS_SCANNER_LOW_VS_52W_HL                           ;

	case SCANNER_LOW_WAR_REL_IMP_VOLAT                   :
		return TWS_SCANNER_LOW_WAR_REL_IMP_VOLAT                   ;

	case SCANNER_MOST_ACTIVE                             :
		return TWS_SCANNER_MOST_ACTIVE                             ;

	case SCANNER_MOST_ACTIVE_AVG_USD                     :
		return TWS_SCANNER_MOST_ACTIVE_AVG_USD                     ;

	case SCANNER_MOST_ACTIVE_USD                         :
		return TWS_SCANNER_MOST_ACTIVE_USD                         ;

	case SCANNER_NEAR_MATURITY_DATE                      :
		return TWS_SCANNER_NEAR_MATURITY_DATE                      ;

	case SCANNER_NOT_OPEN                                :
		return TWS_SCANNER_NOT_OPEN                                ;

	case SCANNER_OPT_OPEN_INTEREST_MOST_ACTIVE           :
		return TWS_SCANNER_OPT_OPEN_INTEREST_MOST_ACTIVE           ;

	case SCANNER_OPT_VOLUME_MOST_ACTIVE                  :
		return TWS_SCANNER_OPT_VOLUME_MOST_ACTIVE                  ;

	case SCANNER_PMONITOR_AVAIL_CONTRACTS                :
		return TWS_SCANNER_PMONITOR_AVAIL_CONTRACTS                ;

	case SCANNER_PMONITOR_CTT                            :
		return TWS_SCANNER_PMONITOR_CTT                            ;

	case SCANNER_PMONITOR_RFQ                            :
		return TWS_SCANNER_PMONITOR_RFQ                            ;

	case SCANNER_TOP_OPEN_PERC_GAIN                      :
		return TWS_SCANNER_TOP_OPEN_PERC_GAIN                      ;

	case SCANNER_TOP_OPEN_PERC_LOSE                      :
		return TWS_SCANNER_TOP_OPEN_PERC_LOSE                      ;

	case SCANNER_TOP_OPT_IMP_VOLAT_GAIN                  :
		return TWS_SCANNER_TOP_OPT_IMP_VOLAT_GAIN                  ;

	case SCANNER_TOP_OPT_IMP_VOLAT_LOSE                  :
		return TWS_SCANNER_TOP_OPT_IMP_VOLAT_LOSE                  ;

	case SCANNER_TOP_PERC_GAIN                           :
		return TWS_SCANNER_TOP_PERC_GAIN                           ;

	case SCANNER_TOP_PERC_LOSE                           :
		return TWS_SCANNER_TOP_PERC_LOSE                           ;

	case SCANNER_TOP_PRICE_RANGE                         :
		return TWS_SCANNER_TOP_PRICE_RANGE                         ;

	case SCANNER_TOP_TRADE_COUNT                         :
		return TWS_SCANNER_TOP_TRADE_COUNT                         ;

	case SCANNER_TOP_TRADE_RATE                          :
		return TWS_SCANNER_TOP_TRADE_RATE                          ;

	case SCANNER_TOP_VOLUME_RATE                         :
		return TWS_SCANNER_TOP_VOLUME_RATE                         ;

	case SCANNER_WSH_NEXT_ANALYST_MEETING                :
		return TWS_SCANNER_WSH_NEXT_ANALYST_MEETING                ;

	case SCANNER_WSH_NEXT_EARNINGS                       :
		return TWS_SCANNER_WSH_NEXT_EARNINGS                       ;

	case SCANNER_WSH_NEXT_EVENT                          :
		return TWS_SCANNER_WSH_NEXT_EVENT                          ;

	case SCANNER_WSH_NEXT_MAJOR_EVENT                    :
		return TWS_SCANNER_WSH_NEXT_MAJOR_EVENT                    ;

	case SCANNER_WSH_PREV_ANALYST_MEETING                :
		return TWS_SCANNER_WSH_PREV_ANALYST_MEETING                ;

	case SCANNER_WSH_PREV_EARNINGS                       :
		return TWS_SCANNER_WSH_PREV_EARNINGS                       ;

	case SCANNER_WSH_PREV_EVENT                          :
		return TWS_SCANNER_WSH_PREV_EVENT                          ;

	default:
		return NULL;
	}
}




const char *to_tws(tws_action_code_t typ)
{
	switch (typ)
{
	case ACTION_BUY:
		return "BUY";

	case ACTION_SELL:
		return "SELL";

	case ACTION_SSHORT:
		return "SSHORT";

	case ACTION_SSHORTX:
		return "SSHORTX";

	default:
		return NULL;
	}
}























































tws_order_type_t from_tws(const char *typ, tws_order_type_t default_value = (tws_order_type_t)-1)
{
	if (!mg_strcasecmp(typ, ORDER_TYPE_MARKET_TO_LIMIT										))
		return ORDER_MARKET_TO_LIMIT										;

	if (!mg_strcasecmp(typ, ORDER_TYPE_MARKET_WITH_PROTECTION								))
		return ORDER_MARKET_WITH_PROTECTION								;

	if (!mg_strcasecmp(typ, ORDER_TYPE_REQUEST_FOR_QUOTE									))
		return ORDER_REQUEST_FOR_QUOTE									;

	if (!mg_strcasecmp(typ, ORDER_TYPE_STOP													))
		return ORDER_STOP													;

	if (!mg_strcasecmp(typ, ORDER_TYPE_STOP_LIMIT											))
		return ORDER_STOP_LIMIT											;

	if (!mg_strcasecmp(typ, ORDER_TYPE_TRAILING_LIMIT_IF_TOUCHED							))
		return ORDER_TRAILING_LIMIT_IF_TOUCHED							;

	if (!mg_strcasecmp(typ, ORDER_TYPE_TRAILING_MARKET_IF_TOUCHED							))
		return ORDER_TRAILING_MARKET_IF_TOUCHED							;

	if (!mg_strcasecmp(typ, ORDER_TYPE_TRAILING_STOP										))
		return ORDER_TRAILING_STOP										;

	if (!mg_strcasecmp(typ, ORDER_TYPE_TRAILING_STOP_LIMIT									))
		return ORDER_TRAILING_STOP_LIMIT									;

	if (!mg_strcasecmp(typ, ORDER_TYPE_MARKET												))
		return ORDER_MARKET												;

	if (!mg_strcasecmp(typ, ORDER_TYPE_MARKET_IF_TOUCHED									))
		return ORDER_MARKET_IF_TOUCHED									;

	if (!mg_strcasecmp(typ, ORDER_TYPE_MARKET_ON_CLOSE										))
		return ORDER_MARKET_ON_CLOSE										;

	if (!mg_strcasecmp(typ, ORDER_TYPE_MARKET_ON_OPEN										))
		return ORDER_MARKET_ON_OPEN										;

	if (!mg_strcasecmp(typ, ORDER_TYPE_PEGGED_TO_MARKET										))
		return ORDER_PEGGED_TO_MARKET										;

	if (!mg_strcasecmp(typ, ORDER_TYPE_RELATIVE												))
		return ORDER_RELATIVE												;

	if (!mg_strcasecmp(typ, ORDER_TYPE_BOX_TOP												))
		return ORDER_BOX_TOP												;

	if (!mg_strcasecmp(typ, ORDER_TYPE_LIMIT_ON_CLOSE										))
		return ORDER_LIMIT_ON_CLOSE										;

	if (!mg_strcasecmp(typ, ORDER_TYPE_LIMIT_ON_OPEN										))
		return ORDER_LIMIT_ON_OPEN										;

	if (!mg_strcasecmp(typ, ORDER_TYPE_LIMIT_IF_TOUCHED										))
		return ORDER_LIMIT_IF_TOUCHED										;

	if (!mg_strcasecmp(typ, ORDER_TYPE_PEGGED_TO_MIDPOINT									))
		return ORDER_PEGGED_TO_MIDPOINT									;

	if (!mg_strcasecmp(typ, ORDER_TYPE_VWAP_GUARANTEED										))
		return ORDER_VWAP_GUARANTEED										;

	if (!mg_strcasecmp(typ, ORDER_TYPE_GOOD_AFTER_TIME_DATE									))
		return ORDER_GOOD_AFTER_TIME_DATE									;

	if (!mg_strcasecmp(typ, ORDER_TYPE_GOOD_TILL_DATE_TIME									))
		return ORDER_GOOD_TILL_DATE_TIME									;

	if (!mg_strcasecmp(typ, ORDER_TYPE_GOOD_TILL_CANCELED									))
		return ORDER_GOOD_TILL_CANCELED									;

	if (!mg_strcasecmp(typ, ORDER_TYPE_IMMEDIATE_OR_CANCEL									))
		return ORDER_IMMEDIATE_OR_CANCEL									;

	if (!mg_strcasecmp(typ, ORDER_TYPE_ONE_CANCELS_ALL										))
		return ORDER_ONE_CANCELS_ALL										;

	if (!mg_strcasecmp(typ, ORDER_TYPE_VOLATILITY											))
		return ORDER_VOLATILITY											;

	if (!mg_strcasecmp(typ, ORDER_TYPE_LIMIT												))
		return ORDER_LIMIT												;

	if (!mg_strcasecmp(typ, ORDER_TYPE_ACTIVETIM											))
		return ORDER_ACTIVETIM											;

	if (!mg_strcasecmp(typ, ORDER_TYPE_ADJUST												))
		return ORDER_ADJUST												;

	if (!mg_strcasecmp(typ, ORDER_TYPE_ALERT												))
		return ORDER_ALERT												;

	if (!mg_strcasecmp(typ, ORDER_TYPE_ALLOC												))
		return ORDER_ALLOC												;

	if (!mg_strcasecmp(typ, ORDER_TYPE_AVGCOST												))
		return ORDER_AVGCOST												;

	if (!mg_strcasecmp(typ, ORDER_TYPE_BASKET												))
		return ORDER_BASKET												;

	if (!mg_strcasecmp(typ, ORDER_TYPE_COND													))
		return ORDER_COND													;

	if (!mg_strcasecmp(typ, ORDER_TYPE_CONDORDER											))
		return ORDER_CONDORDER											;

	if (!mg_strcasecmp(typ, ORDER_TYPE_CONSCOST												))
		return ORDER_CONSCOST												;

	if (!mg_strcasecmp(typ, ORDER_TYPE_DAY													))
		return ORDER_DAY													;

	if (!mg_strcasecmp(typ, ORDER_TYPE_DEACT												))
		return ORDER_DEACT												;

	if (!mg_strcasecmp(typ, ORDER_TYPE_DEACTDIS												))
		return ORDER_DEACTDIS												;

	if (!mg_strcasecmp(typ, ORDER_TYPE_DEACTEOD												))
		return ORDER_DEACTEOD												;

	if (!mg_strcasecmp(typ, ORDER_TYPE_GTT													))
		return ORDER_GTT													;

	if (!mg_strcasecmp(typ, ORDER_TYPE_HID													))
		return ORDER_HID													;

	if (!mg_strcasecmp(typ, ORDER_TYPE_LTH													))
		return ORDER_LTH													;

	if (!mg_strcasecmp(typ, ORDER_TYPE_NONALGO												))
		return ORDER_NONALGO												;

	if (!mg_strcasecmp(typ, ORDER_TYPE_SCALE												))
		return ORDER_SCALE												;

	if (!mg_strcasecmp(typ, ORDER_TYPE_SCALERST												))
		return ORDER_SCALERST												;

	if (!mg_strcasecmp(typ, ORDER_TYPE_WHATIF												))
		return ORDER_WHATIF												;

	return default_value;
}



tws_scanner_type_t from_tws(const char *typ, tws_scanner_type_t default_value = (tws_scanner_type_t)-1)
{
	if (!mg_strcasecmp(typ, TWS_SCANNER_BOND_CUSIP_AZ                           ))
		return SCANNER_BOND_CUSIP_AZ                           ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_BOND_CUSIP_ZA                           ))
		return SCANNER_BOND_CUSIP_ZA                           ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_FAR_MATURITY_DATE                       ))
		return SCANNER_FAR_MATURITY_DATE                       ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HALTED                                  ))
		return SCANNER_HALTED                                  ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HIGHEST_SLB_BID                         ))
		return SCANNER_HIGHEST_SLB_BID                         ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HIGH_BOND_ASK_CURRENT_YIELD_ALL         ))
		return SCANNER_HIGH_BOND_ASK_CURRENT_YIELD_ALL         ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HIGH_BOND_ASK_YIELD_ALL                 ))
		return SCANNER_HIGH_BOND_ASK_YIELD_ALL                 ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HIGH_BOND_DEBT_2_BOOK_RATIO             ))
		return SCANNER_HIGH_BOND_DEBT_2_BOOK_RATIO             ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HIGH_BOND_DEBT_2_EQUITY_RATIO           ))
		return SCANNER_HIGH_BOND_DEBT_2_EQUITY_RATIO           ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HIGH_BOND_DEBT_2_TAN_BOOK_RATIO         ))
		return SCANNER_HIGH_BOND_DEBT_2_TAN_BOOK_RATIO         ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HIGH_BOND_EQUITY_2_BOOK_RATIO           ))
		return SCANNER_HIGH_BOND_EQUITY_2_BOOK_RATIO           ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HIGH_BOND_EQUITY_2_TAN_BOOK_RATIO       ))
		return SCANNER_HIGH_BOND_EQUITY_2_TAN_BOOK_RATIO       ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HIGH_BOND_SPREAD_ALL                    ))
		return SCANNER_HIGH_BOND_SPREAD_ALL                    ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HIGH_COUPON_RATE                        ))
		return SCANNER_HIGH_COUPON_RATE                        ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HIGH_DIVIDEND_YIELD                     ))
		return SCANNER_HIGH_DIVIDEND_YIELD                     ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HIGH_DIVIDEND_YIELD_IB                  ))
		return SCANNER_HIGH_DIVIDEND_YIELD_IB                  ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HIGH_GROWTH_RATE                        ))
		return SCANNER_HIGH_GROWTH_RATE                        ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HIGH_MOODY_RATING_ALL                   ))
		return SCANNER_HIGH_MOODY_RATING_ALL                   ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HIGH_OPEN_GAP                           ))
		return SCANNER_HIGH_OPEN_GAP                           ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HIGH_OPT_IMP_VOLAT                      ))
		return SCANNER_HIGH_OPT_IMP_VOLAT                      ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HIGH_OPT_IMP_VOLAT_OVER_HIST            ))
		return SCANNER_HIGH_OPT_IMP_VOLAT_OVER_HIST            ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HIGH_OPT_OPEN_INTEREST_PUT_CALL_RATIO   ))
		return SCANNER_HIGH_OPT_OPEN_INTEREST_PUT_CALL_RATIO   ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HIGH_OPT_OPEN_INT_PUT_CALL_RATIO        ))
		return SCANNER_HIGH_OPT_OPEN_INT_PUT_CALL_RATIO        ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HIGH_OPT_VOLUME_PUT_CALL_RATIO          ))
		return SCANNER_HIGH_OPT_VOLUME_PUT_CALL_RATIO          ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HIGH_PE_RATIO                           ))
		return SCANNER_HIGH_PE_RATIO                           ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HIGH_PRICE_2_BOOK_RATIO                 ))
		return SCANNER_HIGH_PRICE_2_BOOK_RATIO                 ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HIGH_PRICE_2_TAN_BOOK_RATIO             ))
		return SCANNER_HIGH_PRICE_2_TAN_BOOK_RATIO             ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HIGH_QUICK_RATIO                        ))
		return SCANNER_HIGH_QUICK_RATIO                        ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HIGH_RETURN_ON_EQUITY                   ))
		return SCANNER_HIGH_RETURN_ON_EQUITY                   ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HIGH_SYNTH_BID_REV_NAT_YIELD            ))
		return SCANNER_HIGH_SYNTH_BID_REV_NAT_YIELD            ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HIGH_VS_13W_HL                          ))
		return SCANNER_HIGH_VS_13W_HL                          ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HIGH_VS_26W_HL                          ))
		return SCANNER_HIGH_VS_26W_HL                          ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HIGH_VS_52W_HL                          ))
		return SCANNER_HIGH_VS_52W_HL                          ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HOT_BY_OPT_VOLUME                       ))
		return SCANNER_HOT_BY_OPT_VOLUME                       ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HOT_BY_PRICE                            ))
		return SCANNER_HOT_BY_PRICE                            ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HOT_BY_PRICE_RANGE                      ))
		return SCANNER_HOT_BY_PRICE_RANGE                      ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HOT_BY_VOLUME                           ))
		return SCANNER_HOT_BY_VOLUME                           ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_HOT_VOLUME                              ))
		return SCANNER_HOT_VOLUME                              ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_LIMIT_UP_DOWN                           ))
		return SCANNER_LIMIT_UP_DOWN                           ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_LOWEST_SLB_ASK                          ))
		return SCANNER_LOWEST_SLB_ASK                          ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_LOW_BOND_BID_CURRENT_YIELD_ALL          ))
		return SCANNER_LOW_BOND_BID_CURRENT_YIELD_ALL          ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_LOW_BOND_BID_YIELD_ALL                  ))
		return SCANNER_LOW_BOND_BID_YIELD_ALL                  ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_LOW_BOND_DEBT_2_BOOK_RATIO              ))
		return SCANNER_LOW_BOND_DEBT_2_BOOK_RATIO              ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_LOW_BOND_DEBT_2_EQUITY_RATIO            ))
		return SCANNER_LOW_BOND_DEBT_2_EQUITY_RATIO            ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_LOW_BOND_DEBT_2_TAN_BOOK_RATIO          ))
		return SCANNER_LOW_BOND_DEBT_2_TAN_BOOK_RATIO          ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_LOW_BOND_EQUITY_2_BOOK_RATIO            ))
		return SCANNER_LOW_BOND_EQUITY_2_BOOK_RATIO            ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_LOW_BOND_EQUITY_2_TAN_BOOK_RATIO        ))
		return SCANNER_LOW_BOND_EQUITY_2_TAN_BOOK_RATIO        ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_LOW_BOND_SPREAD_ALL                     ))
		return SCANNER_LOW_BOND_SPREAD_ALL                     ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_LOW_COUPON_RATE                         ))
		return SCANNER_LOW_COUPON_RATE                         ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_LOW_GROWTH_RATE                         ))
		return SCANNER_LOW_GROWTH_RATE                         ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_LOW_MOODY_RATING_ALL                    ))
		return SCANNER_LOW_MOODY_RATING_ALL                    ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_LOW_OPEN_GAP                            ))
		return SCANNER_LOW_OPEN_GAP                            ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_LOW_OPT_IMP_VOLAT                       ))
		return SCANNER_LOW_OPT_IMP_VOLAT                       ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_LOW_OPT_IMP_VOLAT_OVER_HIST             ))
		return SCANNER_LOW_OPT_IMP_VOLAT_OVER_HIST             ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_LOW_OPT_OPEN_INTEREST_PUT_CALL_RATIO    ))
		return SCANNER_LOW_OPT_OPEN_INTEREST_PUT_CALL_RATIO    ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_LOW_OPT_OPEN_INT_PUT_CALL_RATIO         ))
		return SCANNER_LOW_OPT_OPEN_INT_PUT_CALL_RATIO         ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_LOW_OPT_VOLUME_PUT_CALL_RATIO           ))
		return SCANNER_LOW_OPT_VOLUME_PUT_CALL_RATIO           ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_LOW_PE_RATIO                            ))
		return SCANNER_LOW_PE_RATIO                            ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_LOW_PRICE_2_BOOK_RATIO                  ))
		return SCANNER_LOW_PRICE_2_BOOK_RATIO                  ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_LOW_PRICE_2_TAN_BOOK_RATIO              ))
		return SCANNER_LOW_PRICE_2_TAN_BOOK_RATIO              ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_LOW_QUICK_RATIO                         ))
		return SCANNER_LOW_QUICK_RATIO                         ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_LOW_RETURN_ON_EQUITY                    ))
		return SCANNER_LOW_RETURN_ON_EQUITY                    ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_LOW_SYNTH_ASK_REV_NAT_YIELD             ))
		return SCANNER_LOW_SYNTH_ASK_REV_NAT_YIELD             ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_LOW_VS_13W_HL                           ))
		return SCANNER_LOW_VS_13W_HL                           ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_LOW_VS_26W_HL                           ))
		return SCANNER_LOW_VS_26W_HL                           ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_LOW_VS_52W_HL                           ))
		return SCANNER_LOW_VS_52W_HL                           ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_LOW_WAR_REL_IMP_VOLAT                   ))
		return SCANNER_LOW_WAR_REL_IMP_VOLAT                   ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_MOST_ACTIVE                             ))
		return SCANNER_MOST_ACTIVE                             ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_MOST_ACTIVE_AVG_USD                     ))
		return SCANNER_MOST_ACTIVE_AVG_USD                     ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_MOST_ACTIVE_USD                         ))
		return SCANNER_MOST_ACTIVE_USD                         ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_NEAR_MATURITY_DATE                      ))
		return SCANNER_NEAR_MATURITY_DATE                      ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_NOT_OPEN                                ))
		return SCANNER_NOT_OPEN                                ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_OPT_OPEN_INTEREST_MOST_ACTIVE           ))
		return SCANNER_OPT_OPEN_INTEREST_MOST_ACTIVE           ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_OPT_VOLUME_MOST_ACTIVE                  ))
		return SCANNER_OPT_VOLUME_MOST_ACTIVE                  ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_PMONITOR_AVAIL_CONTRACTS                ))
		return SCANNER_PMONITOR_AVAIL_CONTRACTS                ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_PMONITOR_CTT                            ))
		return SCANNER_PMONITOR_CTT                            ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_PMONITOR_RFQ                            ))
		return SCANNER_PMONITOR_RFQ                            ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_TOP_OPEN_PERC_GAIN                      ))
		return SCANNER_TOP_OPEN_PERC_GAIN                      ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_TOP_OPEN_PERC_LOSE                      ))
		return SCANNER_TOP_OPEN_PERC_LOSE                      ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_TOP_OPT_IMP_VOLAT_GAIN                  ))
		return SCANNER_TOP_OPT_IMP_VOLAT_GAIN                  ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_TOP_OPT_IMP_VOLAT_LOSE                  ))
		return SCANNER_TOP_OPT_IMP_VOLAT_LOSE                  ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_TOP_PERC_GAIN                           ))
		return SCANNER_TOP_PERC_GAIN                           ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_TOP_PERC_LOSE                           ))
		return SCANNER_TOP_PERC_LOSE                           ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_TOP_PRICE_RANGE                         ))
		return SCANNER_TOP_PRICE_RANGE                         ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_TOP_TRADE_COUNT                         ))
		return SCANNER_TOP_TRADE_COUNT                         ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_TOP_TRADE_RATE                          ))
		return SCANNER_TOP_TRADE_RATE                          ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_TOP_VOLUME_RATE                         ))
		return SCANNER_TOP_VOLUME_RATE                         ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_WSH_NEXT_ANALYST_MEETING                ))
		return SCANNER_WSH_NEXT_ANALYST_MEETING                ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_WSH_NEXT_EARNINGS                       ))
		return SCANNER_WSH_NEXT_EARNINGS                       ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_WSH_NEXT_EVENT                          ))
		return SCANNER_WSH_NEXT_EVENT                          ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_WSH_NEXT_MAJOR_EVENT                    ))
		return SCANNER_WSH_NEXT_MAJOR_EVENT                    ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_WSH_PREV_ANALYST_MEETING                ))
		return SCANNER_WSH_PREV_ANALYST_MEETING                ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_WSH_PREV_EARNINGS                       ))
		return SCANNER_WSH_PREV_EARNINGS                       ;

	if (!mg_strcasecmp(typ, TWS_SCANNER_WSH_PREV_EVENT                          ))
		return SCANNER_WSH_PREV_EVENT                          ;

	return default_value;
}




tws_action_code_t from_tws(const char *typ, tws_action_code_t default_value = ACTION_UNKNOWN)
{
	if (!mg_strcasecmp(typ, "BUY"))
		return ACTION_BUY;

	if (!mg_strcasecmp(typ, "SELL"))
		return ACTION_SELL;

	if (!mg_strcasecmp(typ, "SSHORT"))
		return ACTION_SSHORT;

	if (!mg_strcasecmp(typ, "SSHORTX"))
		return ACTION_SSHORTX;

	return default_value;
}







tws_short_sale_slot_type_t from_tws(unsigned int v, tws_short_sale_slot_type_t default_value)
{
	switch (v)
	{
	case 1:
		return SSS_CLEARING_BROKER;
		
	case 2:
		return SSS_THIRD_PARTY;

	default:
		return SSS_UNKNOWN;
	}
}









































ib_under_comp::ib_under_comp() :
	u_price(0),
	u_delta(0),
	u_conid(0)
{
}

ib_under_comp::ib_under_comp(const tws::under_comp *s) :
	u_price(0),
	u_delta(0),
	u_conid(0)
{
	if (s)
	{
		u_price = s->u_price;
		u_delta = s->u_delta;
		u_conid = s->u_conid;
	}
}

ib_under_comp::~ib_under_comp()
{
}

bool ib_under_comp::equals(const ib_under_comp *s) const
{
	if (!s)
		return false;

	return (u_price == s->u_price
		&& u_delta == s->u_delta
		&& u_conid == s->u_conid);
}

   

ib_comboleg::ib_comboleg() :
    co_action(ACTION_UNKNOWN),
    co_conid(0),
    co_ratio(0),
    co_open_close(COMBOLEG_UNKNOWN),
    co_short_sale_slot(SSS_UNKNOWN),
    co_exempt_code(0)
{
}
ib_comboleg::ib_comboleg(const tws::tr_comboleg *s)
{
    co_action = from_tws(s->co_action, ACTION_UNKNOWN);
    co_exchange = s->co_exchange;
    co_designated_location = s->co_designated_location;
    co_conid = s->co_conid;
    co_ratio = s->co_ratio;
    co_open_close = s->co_open_close;
    co_short_sale_slot = from_tws(s->co_short_sale_slot, SSS_UNKNOWN);
    co_exempt_code = s->co_exempt_code;
}
ib_comboleg::~ib_comboleg()
{
}

bool ib_comboleg::equals(const ib_comboleg *s) const
{
	if (!s)
		return false;

	return (co_action == s->co_action
		&& co_exchange == s->co_exchange
		&& co_designated_location == s->co_designated_location
		&& co_conid == s->co_conid
		&& co_ratio == s->co_ratio
		&& co_open_close == s->co_open_close
		&& co_short_sale_slot == s->co_short_sale_slot
		&& co_exempt_code == s->co_exempt_code);
}




ib_contract::ib_contract() : m_tws_data(NULL)
{
}
ib_contract::ib_contract(const tws::tr_contract *s)
	: c_undercomp(s->c_undercomp)
{
	m_tws_data = NULL;

	if (s->c_strike)
		c_strike = s->c_strike;

	if (s->c_symbol && s->c_symbol[0])
		c_symbol = s->c_symbol;
    if (s->c_sectype && s->c_sectype[0])
		c_sectype = s->c_sectype;
    if (s->c_exchange && s->c_exchange[0])
		c_exchange = s->c_exchange;
    if (s->c_primary_exch && s->c_primary_exch[0])
		c_primary_exch = s->c_primary_exch;
    if (s->c_expiry && s->c_expiry[0])
		c_expiry = s->c_expiry;
    if (s->c_currency && s->c_currency[0])
		c_currency = s->c_currency;
    if (s->c_right && s->c_right[0])
		c_right = s->c_right;
    if (s->c_local_symbol && s->c_local_symbol[0])
		c_local_symbol = s->c_local_symbol;
    if (s->c_multiplier && s->c_multiplier[0])
		c_multiplier = s->c_multiplier;
    if (s->c_combolegs_descrip && s->c_combolegs_descrip[0])
		c_combolegs_descrip = s->c_combolegs_descrip;
    if (s->c_secid_type && s->c_secid_type[0])
		c_secid_type = s->c_secid_type;
    if (s->c_secid && s->c_secid[0])
		c_secid = s->c_secid;

	if (s->c_comboleg && s->c_num_combolegs > 0)
	{
		for (int i = 0; i < s->c_num_combolegs; i++)
		{
			ib_comboleg cl(&s->c_comboleg[i]);

			c_comboleg.push_back(cl);
		}
	}

	if (s->c_conid)
		c_conid = s->c_conid;

	if (s->c_include_expired)
		c_include_expired = !!s->c_include_expired;
}
ib_contract::~ib_contract()
{
}

tws::tr_contract *ib_contract::to_tws(void) const
{
	return m_tws_data;
}

void ib_contract::prep_for_tws(tws::tws_instance_t *tws)
{
	if (!m_tws_data)
	{
		m_tws_data = new struct tws::tr_contract();
		if (!m_tws_data)
			return;
	}
	else
	{
		struct tws::tr_contract &dst = *m_tws_data;

		if (dst.c_undercomp)
		{
			delete dst.c_undercomp;
		}
		dst.c_undercomp = NULL;

		if (dst.c_comboleg)
		{
			for (int i = 0; i < dst.c_num_combolegs; i++)
			{
				tws::tr_comboleg_t &cl = dst.c_comboleg[i];

				tws::tws_destroy_tr_comboleg(tws, &cl);
			}
			delete[] dst.c_comboleg;
		}
		dst.c_comboleg = NULL;
		dst.c_num_combolegs = 0;
	}

	struct tws::tr_contract &dst = *m_tws_data;

	tws::tws_init_contract(tws, m_tws_data);

	if (c_undercomp.valid())
	{
		ib_under_comp v = c_undercomp;
		dst.c_undercomp = new tws::under_comp_t();
		if (!dst.c_undercomp)
		{
			cleanup_after_tws(tws);
			return;
		}
		dst.c_undercomp->u_conid = v.u_conid;                       
		dst.c_undercomp->u_delta = v.u_delta;                       
		dst.c_undercomp->u_price = v.u_price;                       
	}

	if (c_strike.valid())
		dst.c_strike = c_strike;                         

	if (c_symbol.valid())
		tws_strcpy(dst.c_symbol, c_symbol);                          
	if (c_sectype.valid())
		tws_strcpy(dst.c_sectype, c_sectype);                         
	if (c_exchange.valid())
		tws_strcpy(dst.c_exchange, c_exchange);
	if (c_primary_exch.valid())
		tws_strcpy(dst.c_primary_exch, c_primary_exch);                    
	if (c_expiry.valid())
		tws_strcpy(dst.c_expiry, c_expiry);                          
	if (c_currency.valid())
		tws_strcpy(dst.c_currency, c_currency);                        
	if (c_right.valid())
		tws_strcpy(dst.c_right, c_right);                           
	if (c_local_symbol.valid())
		tws_strcpy(dst.c_local_symbol, c_local_symbol);                    
	if (c_multiplier.valid())
		tws_strcpy(dst.c_multiplier, c_multiplier);
	if (c_combolegs_descrip.valid())
		tws_strcpy(dst.c_combolegs_descrip, c_combolegs_descrip);               
	if (c_secid_type.valid())
		tws_strcpy(dst.c_secid_type, c_secid_type);                      
	if (c_secid.valid())
		tws_strcpy(dst.c_secid, c_secid);

	dst.c_comboleg = NULL;                      
	dst.c_num_combolegs = (int)c_comboleg.size();
	if (dst.c_num_combolegs)
	{
		dst.c_comboleg = new tws::tr_comboleg_t[dst.c_num_combolegs];
		if (!dst.c_comboleg)
		{
			cleanup_after_tws(tws);
			return;
		}
		for (int i = 0; i < dst.c_num_combolegs; i++)
		{
			tws::tr_comboleg_t &cl = dst.c_comboleg[i];
			ib_comboleg &srcleg = c_comboleg[i];

			tws::tws_init_tr_comboleg(tws, &cl);
			if (srcleg.co_action.valid())
				tws_strcpy(cl.co_action, ::to_tws(srcleg.co_action));                                    
			if (srcleg.co_exchange)
				tws_strcpy(cl.co_exchange, srcleg.co_exchange);
			if (srcleg.co_designated_location)
				tws_strcpy(cl.co_designated_location, srcleg.co_designated_location);                       

			if (srcleg.co_conid)
				cl.co_conid = srcleg.co_conid;
			if (srcleg.co_ratio)
				cl.co_ratio = srcleg.co_ratio;
			if (srcleg.co_open_close)
				cl.co_open_close = srcleg.co_open_close;
			if (srcleg.co_short_sale_slot)
				cl.co_short_sale_slot = srcleg.co_short_sale_slot;                          
			if (srcleg.co_exempt_code)
				cl.co_exempt_code = srcleg.co_exempt_code;                              
		}
	}

	if (c_conid.valid())
		dst.c_conid = c_conid;                          

	if (c_include_expired.valid())
		dst.c_include_expired = c_include_expired;                
}

void ib_contract::cleanup_after_tws(tws::tws_instance_t *tws)
{
	if (!m_tws_data)
		return;

	struct tws::tr_contract &dst = *m_tws_data;

	if (dst.c_undercomp)
	{
		delete dst.c_undercomp;
	}

	if (dst.c_comboleg)
	{
		for (int i = 0; i < dst.c_num_combolegs; i++)
		{
			tws::tr_comboleg_t &cl = dst.c_comboleg[i];

			tws::tws_destroy_tr_comboleg(tws, &cl);
		}
		delete[] dst.c_comboleg;
	}

	tws::tws_destroy_contract(tws, m_tws_data);

	delete m_tws_data;
	m_tws_data = NULL;
}

bool ib_contract::equals(const ib_contract *s) const
{
	if (!s)
		return false;

	if (c_conid.value(0) && c_conid == s->c_conid)
		return true;

	return (
		//   c_strike == s->c_strike
		   c_symbol == s->c_symbol
		&& c_sectype == s->c_sectype
		&& c_exchange == s->c_exchange
		&& c_primary_exch == s->c_primary_exch
		&& c_expiry == s->c_expiry
		&& c_currency == s->c_currency
		&& c_right == s->c_right
		&& c_local_symbol == s->c_local_symbol
		&& c_multiplier == s->c_multiplier
		&& c_combolegs_descrip == s->c_combolegs_descrip
		&& c_secid_type == s->c_secid_type
		&& c_secid == s->c_secid
		//&& c_comboleg == s->c_comboleg
		&& c_conid == s->c_conid
		//&& c_include_expired == s->c_include_expired
		);
}



ib_contract_details::ib_contract_details() :
    d_mintick(0),
    d_coupon(0),
	d_ev_multiplier(0),
    d_price_magnifier(0),
    d_under_conid(0),
    d_convertible(false),
    d_callable(false),
    d_putable(false),
    d_next_option_partial(false)
{
}
ib_contract_details::ib_contract_details(const tws::tr_contract_details *s)
	: d_summary(&s->d_summary)
{
    d_mintick = s->d_mintick;
    d_coupon = s->d_coupon;
	d_ev_multiplier = s->d_ev_multiplier;
    d_market_name = s->d_market_name;
    d_trading_class = s->d_trading_class;
    d_order_types = s->d_order_types;
    d_valid_exchanges = s->d_valid_exchanges;
    d_cusip = s->d_cusip;
    d_maturity = s->d_maturity;
    d_issue_date = s->d_issue_date;
    d_ratings = s->d_ratings;
    d_bond_type = s->d_bond_type;
    d_coupon_type = s->d_coupon_type;
    d_desc_append = s->d_desc_append;
    d_next_option_date = s->d_next_option_date;
    d_next_option_type = s->d_next_option_type;
    d_notes = s->d_notes;
    d_long_name = s->d_long_name;
    d_contract_month = s->d_contract_month;
    d_industry = s->d_industry;
    d_category = s->d_category;
    d_subcategory = s->d_subcategory;
    d_timezone_id = s->d_timezone_id;
    d_trading_hours = s->d_trading_hours;
    d_liquid_hours = s->d_liquid_hours;
    d_ev_rule = s->d_ev_rule;
    d_price_magnifier = s->d_price_magnifier;
    d_under_conid = s->d_under_conid;
    d_convertible = s->d_convertible;
    d_callable = s->d_callable;
    d_putable = s->d_putable;
    d_next_option_partial = s->d_next_option_partial;

	if (s->d_sec_id_list && s->d_sec_id_list_count > 0)
	{
		for (int i = 0; i < s->d_sec_id_list_count; i++)
		{
			ib_tag_value tv(&s->d_sec_id_list[i]);

			d_sec_id_list.push_back(tv);
		}
	}
}
ib_contract_details::~ib_contract_details()
{
}

bool ib_contract_details::equals(const ib_contract_details *s) const
{
	if (!s)
		return false;

	if (!d_summary.equals(&s->d_summary))
		return false;

	return (
		//   d_mintick == s->d_mintick
		//&& d_coupon == s->d_coupon
		//&& d_ev_multiplier == s->d_ev_multiplier
    	   d_market_name == s->d_market_name
    	&& d_trading_class == s->d_trading_class
    	&& d_order_types == s->d_order_types
    	&& d_valid_exchanges == s->d_valid_exchanges
    	&& d_cusip == s->d_cusip
    	&& d_maturity == s->d_maturity
    	&& d_issue_date == s->d_issue_date
    	&& d_ratings == s->d_ratings
    	&& d_bond_type == s->d_bond_type
    	&& d_coupon_type == s->d_coupon_type
    	&& d_desc_append == s->d_desc_append
    	//&& d_next_option_date == s->d_next_option_date
    	//&& d_next_option_type == s->d_next_option_type
    	&& d_notes == s->d_notes
    	&& d_long_name == s->d_long_name
    	&& d_contract_month == s->d_contract_month
    	&& d_industry == s->d_industry
    	&& d_category == s->d_category
    	&& d_subcategory == s->d_subcategory
    	&& d_timezone_id == s->d_timezone_id
    	&& d_trading_hours == s->d_trading_hours
    	&& d_liquid_hours == s->d_liquid_hours
    	&& d_ev_rule == s->d_ev_rule
    	&& d_price_magnifier == s->d_price_magnifier
    	&& d_under_conid == s->d_under_conid
    	&& d_convertible == s->d_convertible
    	&& d_callable == s->d_callable
    	&& d_putable == s->d_putable
    	//&& d_next_option_partial == s->d_next_option_partial
		//&& d_sec_id_list == s->d_sec_id_list
		);
}







ib_tag_value::ib_tag_value()
{
}
ib_tag_value::ib_tag_value(const tws::tr_tag_value *s)
{
	t_tag = s->t_tag;
	t_val = s->t_val;
}
ib_tag_value::~ib_tag_value()
{
}






ib_order_combo_leg::ib_order_combo_leg() :
	cl_price(0)
{
}
ib_order_combo_leg::ib_order_combo_leg(const tws::tr_order_combo_leg *s)
{
	cl_price = s->cl_price;
}
ib_order_combo_leg::~ib_order_combo_leg()
{
}






ib_order::ib_order() : m_tws_data(NULL)
{
}
ib_order::ib_order(const tws::tr_order *s)
{
	m_tws_data = NULL;

    if (s->o_discretionary_amt)
		o_discretionary_amt = s->o_discretionary_amt;
    if (s->o_lmt_price)
		o_lmt_price = s->o_lmt_price;
    if (s->o_aux_price)
		o_aux_price = s->o_aux_price;
    if (s->o_percent_offset)
		o_percent_offset = s->o_percent_offset;
    if (s->o_nbbo_price_cap)
		o_nbbo_price_cap = s->o_nbbo_price_cap;
    if (s->o_starting_price)
		o_starting_price = s->o_starting_price;
    if (s->o_stock_ref_price)
		o_stock_ref_price = s->o_stock_ref_price;
    if (s->o_delta)
		o_delta = s->o_delta;
    if (s->o_stock_range_lower)
		o_stock_range_lower = s->o_stock_range_lower;
    if (s->o_stock_range_upper)
		o_stock_range_upper = s->o_stock_range_upper;
    if (s->o_volatility)
		o_volatility = s->o_volatility;
    if (s->o_delta_neutral_aux_price)
		o_delta_neutral_aux_price = s->o_delta_neutral_aux_price;
    if (s->o_trail_stop_price)
		o_trail_stop_price = s->o_trail_stop_price;
    if (s->o_trailing_percent)
		o_trailing_percent = s->o_trailing_percent;
    if (s->o_basis_points)
		o_basis_points = s->o_basis_points;
    if (s->o_scale_price_increment)
		o_scale_price_increment = s->o_scale_price_increment;
    if (s->o_scale_price_adjust_value)
		o_scale_price_adjust_value = s->o_scale_price_adjust_value;
    if (s->o_scale_profit_offset)
		o_scale_profit_offset = s->o_scale_profit_offset;

	if (s->o_algo_strategy && s->o_algo_strategy[0])
		o_algo_strategy = s->o_algo_strategy;
    if (s->o_good_after_time && s->o_good_after_time[0])
		o_good_after_time = s->o_good_after_time;
    if (s->o_good_till_date && s->o_good_till_date[0])
		o_good_till_date = s->o_good_till_date;
    if (s->o_fagroup && s->o_fagroup[0])
		o_fagroup = s->o_fagroup;
    if (s->o_famethod && s->o_famethod[0])
		o_famethod = s->o_famethod;
    if (s->o_fapercentage && s->o_fapercentage[0])
		o_fapercentage = s->o_fapercentage;
    if (s->o_faprofile && s->o_faprofile[0])
		o_faprofile = s->o_faprofile;
    if (s->o_action && s->o_action[0])
		o_action = s->o_action;
    if (s->o_order_type && s->o_order_type[0])
		o_order_type = s->o_order_type;
    if (s->o_tif && s->o_tif[0])
		o_tif = s->o_tif;
    if (s->o_oca_group && s->o_oca_group[0])
		o_oca_group = s->o_oca_group;
    if (s->o_account && s->o_account[0])
		o_account = s->o_account;
    if (s->o_open_close && s->o_open_close[0])
		o_open_close = s->o_open_close;
    if (s->o_orderref && s->o_orderref[0])
		o_orderref = s->o_orderref;
    if (s->o_designated_location && s->o_designated_location[0])
		o_designated_location = s->o_designated_location;
    if (s->o_rule80a && s->o_rule80a[0])
		o_rule80a = s->o_rule80a;
    if (s->o_settling_firm && s->o_settling_firm[0])
		o_settling_firm = s->o_settling_firm;
    if (s->o_delta_neutral_order_type && s->o_delta_neutral_order_type[0])
		o_delta_neutral_order_type = s->o_delta_neutral_order_type;
    if (s->o_clearing_account && s->o_clearing_account[0])
		o_clearing_account = s->o_clearing_account;
    if (s->o_clearing_intent && s->o_clearing_intent[0])
		o_clearing_intent = s->o_clearing_intent;
	if (s->o_hedge_type && s->o_hedge_type[0])
		o_hedge_type = s->o_hedge_type;
	if (s->o_hedge_param && s->o_hedge_param[0])
		o_hedge_param = s->o_hedge_param;
    if (s->o_delta_neutral_settling_firm && s->o_delta_neutral_settling_firm[0])
		o_delta_neutral_settling_firm = s->o_delta_neutral_settling_firm;
    if (s->o_delta_neutral_clearing_account && s->o_delta_neutral_clearing_account[0])
		o_delta_neutral_clearing_account = s->o_delta_neutral_clearing_account;
    if (s->o_delta_neutral_clearing_intent && s->o_delta_neutral_clearing_intent[0])
		o_delta_neutral_clearing_intent = s->o_delta_neutral_clearing_intent;

	if (s->o_algo_params && s->o_algo_params_count > 0)
	{
		for (int i = 0; i < s->o_algo_params_count; i++)
		{
			ib_tag_value tv(&s->o_algo_params[i]);

			o_algo_params.push_back(tv);
		}
	}
	if (s->o_smart_combo_routing_params && s->o_smart_combo_routing_params_count > 0)
	{
		for (int i = 0; i < s->o_smart_combo_routing_params_count; i++)
		{
			ib_tag_value tv(&s->o_smart_combo_routing_params[i]);

			o_smart_combo_routing_params.push_back(tv);
		}
	}

	if (s->o_combo_legs && s->o_combo_legs_count > 0)
	{
		for (int i = 0; i < s->o_combo_legs_count; i++)
		{
			ib_order_combo_leg cl(&s->o_combo_legs[i]);

			o_combo_legs.push_back(cl);
		}
	}

    if (s->o_orderid)
		o_orderid = s->o_orderid;
    if (s->o_total_quantity)
		o_total_quantity = s->o_total_quantity;
    
	if (s->o_origin)
		o_origin = s->o_origin;
    
	if (s->o_clientid)
		o_clientid = s->o_clientid;
    if (s->o_permid)
		o_permid = s->o_permid;
    if (s->o_parentid)
		o_parentid = s->o_parentid;
    if (s->o_display_size)
		o_display_size = s->o_display_size;
    if (s->o_trigger_method)
		o_trigger_method = s->o_trigger_method;
    if (s->o_min_qty)
		o_min_qty = s->o_min_qty;
    if (s->o_volatility_type)
		o_volatility_type = s->o_volatility_type;
    if (s->o_reference_price_type)
		o_reference_price_type = s->o_reference_price_type;
    if (s->o_basis_points_type)
		o_basis_points_type = s->o_basis_points_type;
    if (s->o_scale_subs_level_size)
		o_scale_subs_level_size = s->o_scale_subs_level_size;
    if (s->o_scale_init_level_size)
		o_scale_init_level_size = s->o_scale_init_level_size;
    if (s->o_scale_price_adjust_interval)
		o_scale_price_adjust_interval = s->o_scale_price_adjust_interval;
    if (s->o_scale_init_position)
		o_scale_init_position = s->o_scale_init_position;
    if (s->o_scale_init_fill_qty)
		o_scale_init_fill_qty = s->o_scale_init_fill_qty;
    if (s->o_exempt_code)
		o_exempt_code = s->o_exempt_code;
    if (s->o_delta_neutral_con_id)
		o_delta_neutral_con_id = s->o_delta_neutral_con_id;

    if (s->o_oca_type)
		o_oca_type = s->o_oca_type;
    
	if (s->o_auction_strategy)
		o_auction_strategy = s->o_auction_strategy;
    
	if (s->o_short_sale_slot)
		o_short_sale_slot = from_tws(s->o_short_sale_slot, SSS_UNKNOWN);
    
	o_override_percentage_constraints = s->o_override_percentage_constraints;
    o_firm_quote_only = s->o_firm_quote_only;
    o_etrade_only = s->o_etrade_only;
    o_all_or_none = s->o_all_or_none;
    o_outside_rth = s->o_outside_rth;
    o_hidden = s->o_hidden;
    o_transmit = s->o_transmit;
    o_block_order = s->o_block_order;
    o_sweep_to_fill = s->o_sweep_to_fill;
    o_continuous_update = s->o_continuous_update;
    o_whatif = s->o_whatif;
    o_not_held = s->o_not_held;
	o_opt_out_smart_routing = s->o_opt_out_smart_routing;
    o_scale_auto_reset = s->o_scale_auto_reset;
    o_scale_random_percent = s->o_scale_random_percent;
}
ib_order::~ib_order()
{
}

tws::tr_order *ib_order::to_tws(void) const
{
	return m_tws_data;
}

void ib_order::prep_for_tws(tws::tws_instance_t *tws)
{
	if (!m_tws_data)
	{
		m_tws_data = new struct tws::tr_order();
		if (!m_tws_data)
			return;
	}
	else
	{
		struct tws::tr_order &dst = *m_tws_data;

		if (dst.o_algo_params)
		{
			for (int i = 0; i < dst.o_algo_params_count; i++)
			{
				tws::tr_tag_value_t &d = dst.o_algo_params[i];

				tws::tws_destroy_tag_value(tws, &d);
			}
			delete[] dst.o_algo_params;
		}
		dst.o_algo_params = NULL;
		dst.o_algo_params_count = 0;

		if (dst.o_smart_combo_routing_params)
		{
			for (int i = 0; i < dst.o_smart_combo_routing_params_count; i++)
			{
				tws::tr_tag_value_t &d = dst.o_smart_combo_routing_params[i];

				tws::tws_destroy_tag_value(tws, &d);
			}
			delete[] dst.o_smart_combo_routing_params;
		}
		dst.o_smart_combo_routing_params = NULL;
		dst.o_smart_combo_routing_params_count = 0;

		if (dst.o_combo_legs)
		{
			for (int i = 0; i < dst.o_combo_legs_count; i++)
			{
				tws::tr_order_combo_leg_t &d = dst.o_combo_legs[i];

				tws::tws_destroy_order_combo_leg(tws, &d);
			}
			delete[] dst.o_combo_legs;
		}
		dst.o_combo_legs = NULL;
		dst.o_combo_legs_count = 0;
	}

	struct tws::tr_order &dst = *m_tws_data;

	tws::tws_init_order(tws, m_tws_data);

	if (o_discretionary_amt.valid())
		dst.o_discretionary_amt = o_discretionary_amt;                       
	if (o_lmt_price.valid())
		dst.o_lmt_price = o_lmt_price;                               
	if (o_aux_price.valid())
		dst.o_aux_price = o_aux_price;                               
	if (o_percent_offset.valid())
		dst.o_percent_offset = o_percent_offset;                          
	if (o_nbbo_price_cap.valid())
		dst.o_nbbo_price_cap = o_nbbo_price_cap;                          
	if (o_starting_price.valid())
		dst.o_starting_price = o_starting_price;                          
	if (o_stock_ref_price.valid())
		dst.o_stock_ref_price = o_stock_ref_price;                         
	if (o_delta.valid())
		dst.o_delta = o_delta;                                   
	if (o_stock_range_lower.valid())
		dst.o_stock_range_lower = o_stock_range_lower;                       
	if (o_stock_range_upper.valid())
		dst.o_stock_range_upper = o_stock_range_upper;                       
	if (o_volatility.valid())
		dst.o_volatility = o_volatility;                              
	if (o_delta_neutral_aux_price.valid())
		dst.o_delta_neutral_aux_price = o_delta_neutral_aux_price;
	if (o_trail_stop_price.valid())
		dst.o_trail_stop_price = o_trail_stop_price;                        
	if (o_trailing_percent.valid())
		dst.o_trailing_percent = o_trailing_percent;
	if (o_basis_points.valid())
		dst.o_basis_points = o_basis_points;
	if (o_scale_price_increment.valid())
		dst.o_scale_price_increment = o_scale_price_increment;					
	if (o_scale_price_adjust_value.valid())
		dst.o_scale_price_adjust_value = o_scale_price_adjust_value;				
	if (o_scale_profit_offset.valid())
		dst.o_scale_profit_offset = o_scale_profit_offset;						

	if (o_algo_strategy.valid())
		tws_strcpy(dst.o_algo_strategy, o_algo_strategy);
	if (o_good_after_time.valid())
		tws_strcpy(dst.o_good_after_time, o_good_after_time);                         
	if (o_good_till_date.valid())
		tws_strcpy(dst.o_good_till_date, o_good_till_date);                          
	if (o_fagroup.valid())
		tws_strcpy(dst.o_fagroup, o_fagroup);                                 
	if (o_famethod.valid())
		tws_strcpy(dst.o_famethod, o_famethod);                                
	if (o_fapercentage.valid())
		tws_strcpy(dst.o_fapercentage, o_fapercentage);                            
	if (o_faprofile.valid())
		tws_strcpy(dst.o_faprofile, o_faprofile);                               
	if (o_action.valid())
		tws_strcpy(dst.o_action, o_action);                                  
	if (o_order_type.valid())
		tws_strcpy(dst.o_order_type, o_order_type);                              
	if (o_tif.valid())
		tws_strcpy(dst.o_tif, o_tif);                                     
	if (o_oca_group.valid())
		tws_strcpy(dst.o_oca_group, o_oca_group);                               
	if (o_account.valid())
		tws_strcpy(dst.o_account, o_account);                                 
	if (o_open_close.valid())
		tws_strcpy(dst.o_open_close, o_open_close);                              
	if (o_orderref.valid())
		tws_strcpy(dst.o_orderref, o_orderref);                                
	if (o_designated_location.valid())
		tws_strcpy(dst.o_designated_location, o_designated_location);                     
	if (o_rule80a.valid())
		tws_strcpy(dst.o_rule80a, o_rule80a);                                 
	if (o_settling_firm.valid())
		tws_strcpy(dst.o_settling_firm, o_settling_firm);
	if (o_delta_neutral_order_type.valid())
		tws_strcpy(dst.o_delta_neutral_order_type, o_delta_neutral_order_type);
	if (o_clearing_account.valid())
		tws_strcpy(dst.o_clearing_account, o_clearing_account);                        
	if (o_clearing_intent.valid())
		tws_strcpy(dst.o_clearing_intent, o_clearing_intent);                         
	if (o_hedge_type.valid())
		tws_strcpy(dst.o_hedge_type, o_hedge_type);								
	if (o_hedge_param.valid())
		tws_strcpy(dst.o_hedge_param, o_hedge_param);								
	if (o_delta_neutral_settling_firm.valid())
		tws_strcpy(dst.o_delta_neutral_settling_firm, o_delta_neutral_settling_firm);				
	if (o_delta_neutral_clearing_account.valid())
		tws_strcpy(dst.o_delta_neutral_clearing_account, o_delta_neutral_clearing_account);			
	if (o_delta_neutral_clearing_intent.valid())
		tws_strcpy(dst.o_delta_neutral_clearing_intent, o_delta_neutral_clearing_intent);			

	dst.o_algo_params_count = o_algo_params.size();
	if (dst.o_algo_params_count)
	{
		dst.o_algo_params = new tws::tr_tag_value_t[dst.o_algo_params_count];
		if (!dst.o_algo_params)
		{
			cleanup_after_tws(tws);
			return;
		}
		for (int i = 0; i < dst.o_algo_params_count; i++)
		{
			tws::tr_tag_value_t &d = dst.o_algo_params[i];
			ib_tag_value *s = &o_algo_params[i];

			tws::tws_init_tag_value(tws, &d);

			tws_strcpy(d.t_tag, s->t_tag);
			tws_strcpy(d.t_val, s->t_val);
		}
	}
	dst.o_smart_combo_routing_params_count = o_smart_combo_routing_params.size();
	if (dst.o_smart_combo_routing_params_count)
	{
		dst.o_smart_combo_routing_params = new tws::tr_tag_value_t[dst.o_smart_combo_routing_params_count];
		if (!dst.o_smart_combo_routing_params)
		{
			cleanup_after_tws(tws);
			return;
		}
		for (int i = 0; i < dst.o_smart_combo_routing_params_count; i++)
		{
			tws::tr_tag_value_t &d = dst.o_smart_combo_routing_params[i];
			ib_tag_value *s = &o_smart_combo_routing_params[i];

			tws::tws_init_tag_value(tws, &d);

			tws_strcpy(d.t_tag, s->t_tag);
			tws_strcpy(d.t_val, s->t_val);
		}
	}
	dst.o_combo_legs_count = o_combo_legs.size();
	if (dst.o_combo_legs_count)
	{
		dst.o_combo_legs = new tws::tr_order_combo_leg_t[dst.o_combo_legs_count];
		if (!dst.o_combo_legs)
		{
			cleanup_after_tws(tws);
			return;
		}
		for (int i = 0; i < dst.o_combo_legs_count; i++)
		{
			tws::tr_order_combo_leg_t &d = dst.o_combo_legs[i];
			ib_order_combo_leg *s = &o_combo_legs[i];

			tws::tws_init_order_combo_leg(tws, &d);

			d.cl_price = s->cl_price;
		}
	}

	if (o_orderid.valid())
		dst.o_orderid = o_orderid;                                 
	if (o_total_quantity.valid())
		dst.o_total_quantity = o_total_quantity;                          
	if (o_origin.valid())
		dst.o_origin = o_origin;                               
	if (o_clientid.valid())
		dst.o_clientid = o_clientid;                                
	if (o_permid.valid())
		dst.o_permid = o_permid;                                  
	if (o_parentid.valid())
		dst.o_parentid = o_parentid;                                
	if (o_display_size.valid())
		dst.o_display_size = o_display_size;                            
	if (o_trigger_method.valid())
		dst.o_trigger_method = o_trigger_method;                          
	if (o_min_qty.valid())
		dst.o_min_qty = o_min_qty;                                 
	if (o_volatility_type.valid())
		dst.o_volatility_type = o_volatility_type;                         
	if (o_reference_price_type.valid())
		dst.o_reference_price_type = o_reference_price_type;                    
	if (o_basis_points_type.valid())
		dst.o_basis_points_type = o_basis_points_type;
	if (o_scale_subs_level_size.valid())
		dst.o_scale_subs_level_size = o_scale_subs_level_size;					
	if (o_scale_init_level_size.valid())
		dst.o_scale_init_level_size = o_scale_init_level_size;					
	if (o_scale_price_adjust_interval.valid())
		dst.o_scale_price_adjust_interval = o_scale_price_adjust_interval;				
	if (o_scale_init_position.valid())
		dst.o_scale_init_position = o_scale_init_position;						
	if (o_scale_init_fill_qty.valid())
		dst.o_scale_init_fill_qty = o_scale_init_fill_qty;						
	if (o_exempt_code.valid())
		dst.o_exempt_code = o_exempt_code;                             
	if (o_delta_neutral_con_id.valid())
		dst.o_delta_neutral_con_id = o_delta_neutral_con_id;					

	if (o_oca_type.valid())
		dst.o_oca_type = o_oca_type;                           
	if (o_auction_strategy.valid())
		dst.o_auction_strategy = o_auction_strategy;           

	if (o_short_sale_slot.valid())
		dst.o_short_sale_slot = o_short_sale_slot;                      
	if (o_override_percentage_constraints.valid())
		dst.o_override_percentage_constraints = o_override_percentage_constraints;      
	if (o_firm_quote_only.valid())
		dst.o_firm_quote_only = o_firm_quote_only;                      
	if (o_etrade_only.valid())
		dst.o_etrade_only = o_etrade_only;
	if (o_all_or_none.valid())
		dst.o_all_or_none = o_all_or_none;                          
	if (o_outside_rth.valid())
		dst.o_outside_rth = o_outside_rth;                          
	if (o_hidden.valid())
		dst.o_hidden = o_hidden;                               
	if (o_transmit.valid())
		dst.o_transmit = o_transmit;                             
	if (o_block_order.valid())
		dst.o_block_order = o_block_order;                          
	if (o_sweep_to_fill.valid())
		dst.o_sweep_to_fill = o_sweep_to_fill;                        
	if (o_continuous_update.valid())
		dst.o_continuous_update = o_continuous_update;                    
	if (o_whatif.valid())
		dst.o_whatif = o_whatif;                               
	if (o_not_held.valid())
		dst.o_not_held = o_not_held;
	if (o_opt_out_smart_routing.valid())
		dst.o_opt_out_smart_routing = o_opt_out_smart_routing;				
	if (o_scale_auto_reset.valid())
		dst.o_scale_auto_reset = o_scale_auto_reset;						
	if (o_scale_random_percent.valid())
		dst.o_scale_random_percent = o_scale_random_percent;					
}

void ib_order::cleanup_after_tws(tws::tws_instance_t *tws)
{
	if (!m_tws_data)
		return;

	struct tws::tr_order &dst = *m_tws_data;

	if (dst.o_algo_params)
	{
		for (int i = 0; i < dst.o_algo_params_count; i++)
		{
			tws::tr_tag_value_t &d = dst.o_algo_params[i];

			tws::tws_destroy_tag_value(tws, &d);
		}
		delete[] dst.o_algo_params;
	}

	if (dst.o_smart_combo_routing_params)
	{
		for (int i = 0; i < dst.o_smart_combo_routing_params_count; i++)
		{
			tws::tr_tag_value_t &d = dst.o_smart_combo_routing_params[i];

			tws::tws_destroy_tag_value(tws, &d);
		}
		delete[] dst.o_smart_combo_routing_params;
	}

	if (dst.o_combo_legs)
	{
		for (int i = 0; i < dst.o_combo_legs_count; i++)
		{
			tws::tr_order_combo_leg_t &d = dst.o_combo_legs[i];

			tws::tws_destroy_order_combo_leg(tws, &d);
		}
		delete[] dst.o_combo_legs;
	}

	tws::tws_destroy_order(tws, m_tws_data);

	delete m_tws_data;
	m_tws_data = NULL;
}









ib_order_status::ib_order_status() :
    ost_commission(0),
    ost_min_commission(0),
    ost_max_commission(0)
{
}
ib_order_status::ib_order_status(const tws::tr_order_status *s)
{
	ost_commission = s->ost_commission;
	ost_min_commission = s->ost_min_commission;
	ost_max_commission = s->ost_max_commission;
	
	ost_status = s->ost_status;
	ost_init_margin = s->ost_init_margin;
	ost_maint_margin = s->ost_maint_margin;
	ost_equity_with_loan = s->ost_equity_with_loan;
	ost_commission_currency = s->ost_commission_currency;
	ost_warning_text = s->ost_warning_text;
}
ib_order_status::~ib_order_status()
{
}






ib_execution::ib_execution() :
    e_price(0),
    e_avg_price(0),
    e_ev_multiplier(0),
    e_shares(0),
    e_permid(0),
    e_clientid(0),
    e_liquidation(0),
    e_orderid(0),
    e_cum_qty(0)
{
}
ib_execution::ib_execution(const tws::tr_execution *s)
{
	e_price = s->e_price;
	e_avg_price = s->e_avg_price;
	e_ev_multiplier = s->e_ev_multiplier;
	
	e_execid = s->e_execid;
	e_time = s->e_time;
	e_acct_number = s->e_acct_number;
	e_exchange = s->e_exchange;
	e_side = s->e_side;
	e_orderref = s->e_orderref;
	e_ev_rule = s->e_ev_rule;

	e_shares = s->e_shares;
	e_permid = s->e_permid;
	e_clientid = s->e_clientid;
	e_liquidation = s->e_liquidation;
	e_orderid = s->e_orderid;
	e_cum_qty = s->e_cum_qty;
}
ib_execution::~ib_execution()
{
}






ib_exec_filter::ib_exec_filter() : m_tws_data(NULL)
{
}
ib_exec_filter::ib_exec_filter(const tws::tr_exec_filter *s)
{
	m_tws_data = NULL;

    if (s->f_acctcode)
		f_acctcode = s->f_acctcode;
    if (s->f_time)
		f_time = s->f_time;
    if (s->f_symbol)
		f_symbol = s->f_symbol;
    if (s->f_sectype)
		f_sectype = s->f_sectype;
    if (s->f_exchange)
		f_exchange = s->f_exchange;
    if (s->f_side)
		f_side = s->f_side;

	if (s->f_clientid)
		f_clientid = s->f_clientid;
}
ib_exec_filter::~ib_exec_filter()
{
}

tws::tr_exec_filter *ib_exec_filter::to_tws(void) const
{
	return m_tws_data;
}

void ib_exec_filter::prep_for_tws(tws::tws_instance_t *tws)
{
	if (!m_tws_data)
	{
		m_tws_data = new struct tws::tr_exec_filter();
		if (!m_tws_data)
			return;
	}

	struct tws::tr_exec_filter &dst = *m_tws_data;

	tws::tws_init_exec_filter(tws, m_tws_data);

	if (f_acctcode.valid())
		tws_strcpy(dst.f_acctcode, f_acctcode);
	if (f_time.valid())
		tws_strcpy(dst.f_time, f_time);
	if (f_symbol.valid())
		tws_strcpy(dst.f_symbol, f_symbol);
	if (f_sectype.valid())
		tws_strcpy(dst.f_sectype, f_sectype);
	if (f_exchange.valid())
		tws_strcpy(dst.f_exchange, f_exchange);
	if (f_side.valid())
		tws_strcpy(dst.f_side, f_side);

	if (f_clientid.valid())
		dst.f_clientid = f_clientid;
}

void ib_exec_filter::cleanup_after_tws(tws::tws_instance_t *tws)
{
	if (!m_tws_data)
		return;

	tws::tws_destroy_exec_filter(tws, m_tws_data);

	delete m_tws_data;
	m_tws_data = NULL;
}





ib_scanner_subscription::ib_scanner_subscription() : m_tws_data(NULL)
{
}
ib_scanner_subscription::ib_scanner_subscription(const tws::tr_scanner_subscription *s)
{
	m_tws_data = NULL;

    if (s->scan_above_price)
		scan_above_price = s->scan_above_price;
    if (s->scan_below_price)
		scan_below_price = s->scan_below_price;
    if (s->scan_coupon_rate_above)
		scan_coupon_rate_above = s->scan_coupon_rate_above;
    if (s->scan_coupon_rate_below)
		scan_coupon_rate_below = s->scan_coupon_rate_below;
    if (s->scan_market_cap_above)
		scan_market_cap_above = s->scan_market_cap_above;
    if (s->scan_market_cap_below)
		scan_market_cap_below = s->scan_market_cap_below;
	
	if (s->scan_exclude_convertible)
		scan_exclude_convertible = s->scan_exclude_convertible;
    if (s->scan_instrument)
		scan_instrument = s->scan_instrument;
    if (s->scan_location_code)
		scan_location_code = s->scan_location_code;
    if (s->scan_maturity_date_above)
		scan_maturity_date_above = s->scan_maturity_date_above;
    if (s->scan_maturity_date_below)
		scan_maturity_date_below = s->scan_maturity_date_below;
    if (s->scan_moody_rating_above)
		scan_moody_rating_above = s->scan_moody_rating_above;
    if (s->scan_moody_rating_below)
		scan_moody_rating_below = s->scan_moody_rating_below;
    if (s->scan_code)
		scan_code = s->scan_code;
    if (s->scan_sp_rating_above)
		scan_sp_rating_above = s->scan_sp_rating_above;
    if (s->scan_sp_rating_below)
		scan_sp_rating_below = s->scan_sp_rating_below;
    if (s->scan_scanner_setting_pairs)
		scan_scanner_setting_pairs = s->scan_scanner_setting_pairs;
    if (s->scan_stock_type_filter)
		scan_stock_type_filter = s->scan_stock_type_filter;

	if (s->scan_above_volume)
		scan_above_volume = s->scan_above_volume;
    if (s->scan_number_of_rows)
		scan_number_of_rows = s->scan_number_of_rows;
    if (s->scan_average_option_volume_above)
		scan_average_option_volume_above = s->scan_average_option_volume_above;
}
ib_scanner_subscription::~ib_scanner_subscription()
{
}

tws::tr_scanner_subscription *ib_scanner_subscription::to_tws(void) const
{
	return m_tws_data;
}

void ib_scanner_subscription::prep_for_tws(tws::tws_instance_t *tws)
{
	if (!m_tws_data)
	{
		m_tws_data = new struct tws::tr_scanner_subscription();
		if (!m_tws_data)
			return;
	}

	struct tws::tr_scanner_subscription &dst = *m_tws_data;

	tws::tws_init_scanner_subscription(tws, m_tws_data);

	if (scan_above_price.valid())
		dst.scan_above_price = scan_above_price;
	if (scan_below_price.valid())
		dst.scan_below_price = scan_below_price;
	if (scan_coupon_rate_above.valid())
		dst.scan_coupon_rate_above = scan_coupon_rate_above;
	if (scan_coupon_rate_below.valid())
		dst.scan_coupon_rate_below = scan_coupon_rate_below;
	if (scan_market_cap_above.valid())
		dst.scan_market_cap_above = scan_market_cap_above;
	if (scan_market_cap_below.valid())
		dst.scan_market_cap_below = scan_market_cap_below;

	if (scan_exclude_convertible.valid())
		tws_strcpy(dst.scan_exclude_convertible, scan_exclude_convertible);
	if (scan_instrument.valid())
		tws_strcpy(dst.scan_instrument, scan_instrument);
	if (scan_location_code.valid())
		tws_strcpy(dst.scan_location_code, scan_location_code);
	if (scan_maturity_date_above.valid())
		tws_strcpy(dst.scan_maturity_date_above, scan_maturity_date_above);
	if (scan_maturity_date_below.valid())
		tws_strcpy(dst.scan_maturity_date_below, scan_maturity_date_below);
	if (scan_moody_rating_above.valid())
		tws_strcpy(dst.scan_moody_rating_above, scan_moody_rating_above);
	if (scan_moody_rating_below.valid())
		tws_strcpy(dst.scan_moody_rating_below, scan_moody_rating_below);
	if (scan_code.valid())
		tws_strcpy(dst.scan_code, scan_code);
	if (scan_sp_rating_above.valid())
		tws_strcpy(dst.scan_sp_rating_above, scan_sp_rating_above);
	if (scan_sp_rating_below.valid())
		tws_strcpy(dst.scan_sp_rating_below, scan_sp_rating_below);
	if (scan_scanner_setting_pairs.valid())
		tws_strcpy(dst.scan_scanner_setting_pairs, scan_scanner_setting_pairs);
	if (scan_stock_type_filter.valid())
		tws_strcpy(dst.scan_stock_type_filter, scan_stock_type_filter);

	if (scan_above_volume.valid())
		dst.scan_above_volume = scan_above_volume;
	if (scan_number_of_rows.valid())
		dst.scan_number_of_rows = scan_number_of_rows;
	if (scan_average_option_volume_above.valid())
		dst.scan_average_option_volume_above = scan_average_option_volume_above;
}

void ib_scanner_subscription::cleanup_after_tws(tws::tws_instance_t *tws)
{
	if (!m_tws_data)
		return;

	tws::tws_destroy_scanner_subscription(tws, m_tws_data);

	delete m_tws_data;
	m_tws_data = NULL;
}







ib_commission_report::ib_commission_report() :
    cr_commission(0),
    cr_realized_pnl(0),
    cr_yield(0),
    cr_yield_redemption_date(0)
{
}
ib_commission_report::ib_commission_report(const tws::tr_commission_report *s)
{
    cr_exec_id = s->cr_exec_id;
    cr_currency = s->cr_currency;

    cr_commission = s->cr_commission;
    cr_realized_pnl = s->cr_realized_pnl;
    cr_yield = s->cr_yield;

    cr_yield_redemption_date = s->cr_yield_redemption_date;
}
ib_commission_report::~ib_commission_report()
{
}






















ib_date_t::ib_date_t(const char *ts)
{
	m_t = mg_parse_date_string(ts);
}

ib_date_t::operator ib_string_t(void)
{
	char buf[40];

	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", gmtime(&m_t));
	
	return buf;
}

ib_date_t &ib_date_t::operator =(char const *ts)
{
	m_t = mg_parse_date_string(ts);

	return *this;
}

