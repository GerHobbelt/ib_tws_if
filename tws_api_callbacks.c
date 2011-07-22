
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
 * suitable for loading by, for example, 64-bit Excel 2010, using web queries.
 */

/*
TODO / roadmap:

- prioritizing outgoing requests, e.g. ORDERs have priority over historical data requests, and requests for recent historical data have priority over requests for older data.

- use a priority queue for the above plus a 'idle time' delay to prevent hammering the TWS machines with historical data requests: only fire those when the interface has been 'quiet' for X seconds

- store/cache historical data; use 'smart' code to request consecutive and _large_ chunks of historical data to be cached: one request, served many times (from local cache)

- async TWS TX/RX: push requests asap, using a 'telnet' TCP setting (you don't want orders to wait for a TCP buffer fill timeout!): single thread/connection connected to TWS,
  all requests are posted in a 'response queue' (so we know which responses are for whom) upon transmission --> true full duplex communication instead of the standard TWS sample
  which uses the TCP connection as a half-duplex connect (as it waits for the response to the request before firing another).
*/


#include "tws_comm_thread.h"

#include "tws_stocklist_loading_thread.h"





void event_tick_price(void *opaque, int ticker_id, long field, double price, int can_auto_execute)
{
    printf("tick_price: opaque=%p, ticker_id=%d, type=%ld, price=%.2lf, can_auto=%d\n",
           opaque, ticker_id, field, price, can_auto_execute);
}

void event_tick_size(void *opaque, int ticker_id, long type, int size)
{
    printf("tick_size: opaque=%p, ticker_id=%d, type=%ld, size=%d\n",
           opaque, ticker_id, type, size);
}

void event_tick_option_computation(void *opaque, int ticker_id, int type, double implied_vol, double delta, double opt_price, double pv_dividend, double gamma, double vega, double theta, double und_price)
{
    printf("tick option computation: opaque=%p, ticker_id=%d, type=%d, implied_vol=%f, delta=%f, opt_price=%f, pv_dividend=%f, gamma=%f, vega=%f, theta=%f, und_price=%f\n",
           opaque, ticker_id, type, implied_vol, delta, opt_price, pv_dividend, gamma, vega, theta, und_price);
}

void event_tick_generic(void *opaque, int ticker_id, int type, double value)
{
    printf("tick_generic: opaque=%p, ticker_id=%d, type=%d, ...\n", opaque, ticker_id, type);
}

void event_tick_string(void *opaque, int ticker_id, int type, const char value[])
{
    printf("tick_string: opaque=%p, ticker_id=%d, type=%d, ...\n", opaque, ticker_id, type);
}

void event_tick_efp(void *opaque, int ticker_id, int tick_type, double basis_points, const char formatted_basis_points[], double implied_futures_price, int hold_days, const char future_expiry[], double dividend_impact, double dividends_to_expiry)
{
    printf("tick_efp: opaque=%p, ticker_id=%d, type=%d, ...\n", opaque, ticker_id, tick_type);
}

void event_order_status(void *opaque, long order_id, const char status[],
                        int filled, int remaining, double avg_fill_price, int perm_id,
                        int parent_id, double last_fill_price, int client_id, const char why_held[])
{
    printf("order_status: opaque=%p, order_id=%ld, filled=%d remaining %d, avg_fill_price=%lf, last_fill_price=%lf, why_held=%s\n", opaque, order_id, filled, remaining, avg_fill_price, last_fill_price, why_held);
}

void event_open_order(void *opaque, long order_id, const tr_contract_t *contract, const tr_order_t *order, const tr_order_status_t *ostatus)
{
    /* commission values might be DBL_MAX */
    if(fabs(ostatus->ost_commission - DBL_MAX) < DBL_EPSILON)
        printf("open_order: commission not reported\n");
    else
        printf("open_order: commission for %ld was %.4lf\n", order_id, ostatus->ost_commission);

    printf("open_order: opaque=%p, order_id=%ld, sym=%s\n", opaque, order_id, contract->c_symbol);
}

void event_connection_closed(void *opaque)
{
    printf("connection_closed: opaque=%p\n", opaque);
}

void event_update_account_value(void *opaque, const char key[], const char val[],
                                const char currency[], const char account_name[])
{
    printf("update_account_value: %p, key=%s val=%s, currency=%s, name=%s\n",
           opaque, key, val, currency, account_name);
}

void event_update_portfolio(void *opaque, const tr_contract_t *contract, int position,
                            double mkt_price, double mkt_value, double average_cost,
                            double unrealized_pnl, double realized_pnl, const char account_name[])
{
    printf("update_portfolio: %p, sym=%s, position=%d, mkt_price=%.4lf, mkt_value=%.4lf, avg_cost=%.4lf, unrealized_pnl=%.4lf, realized pnl=%.4lf name=%s\n",
           opaque, contract->c_symbol, position, mkt_price, mkt_value, average_cost, unrealized_pnl, realized_pnl, account_name);
}

void event_update_account_time(void *opaque, const char time_stamp[])
{
    printf("update_account_time: opaque=%p, ...\n", opaque);
}

void event_next_valid_id(void *opaque, long order_id)
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

    /* invoked once at connection establishment
     * the scope of this variable is not program wide, instance wide,
     * or even system wide. It has the same scope as the TWS account itself.
     * It appears to be persistent across TWS restarts.
     * Well behaved human and automatic TWS clients shall increment
     * this order_id atomically and cooperatively
     */
    info->next_order_id = order_id;

    printf("next_valid_id for order placement %ld\n", order_id);
}

void event_contract_details(void *opaque, int req_id, const tr_contract_details_t *contract_details)
{
    printf("contract_details: opaque=%p, ...\n", opaque);
}

void event_bond_contract_details(void *opaque, int req_id, const tr_contract_details_t *contract_details)
{
    printf("bond_contract_details: opaque=%p, ...\n", opaque);
}

void event_exec_details(void *opaque, long order_id, const tr_contract_t *contract, const tr_execution_t *execution)
{
    printf("exec_details: opaque=%p, ...\n", opaque);
}

void event_error(void *opaque, int id, int error_code, const char error_string[])
{
    printf("error: opaque=%p, error_code=%d, msg=%s\n", opaque, error_code, error_string);
}

void event_update_mkt_depth(void *opaque, int ticker_id, int position, int operation, int side, double price, int size)
{
    printf("update_mkt_depth: opaque=%p, ticker_id=%d, ...\n", opaque, ticker_id);
}

void event_update_mkt_depth_l2(void *opaque, int ticker_id, int position, char *market_maker, int operation, int side, double price, int size)
{
    printf("update_mkt_depth_l2: opaque=%p, ticker_id=%d, ...\n", opaque, ticker_id);
}

void event_update_news_bulletin(void *opaque, int msgid, int msg_type, const char news_msg[], const char origin_exch[])
{
    printf("update_news_bulletin: opaque=%p, ...\n", opaque);
}

void event_managed_accounts(void *opaque, const char accounts_list[])
{
    printf("managed_accounts: opaque=%p, ...\n", opaque);
}

void event_receive_fa(void *opaque, long fa_data_type, const char cxml[])
{
    printf("receive_fa: opaque=%p, fa_data_type=%ld, xml='%s'\n", opaque, fa_data_type, cxml);
}

void event_historical_data(void *opaque, int reqid, const char date[], double open, double high, double low, double close, int volume, int bar_count, double wap, int has_gaps)
{
    printf("historical: opaque=%p, reqid=%d, date=%s, %.3lf, %.3lf, %.3lf, %.3lf, %d, wap=%.3lf, has_gaps=%d\n", opaque, reqid, date, open, high, low, close, volume, wap, has_gaps);
}

void event_scanner_parameters(void *opaque, const char xml[])
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;
    int rv;
    xmlTextReaderPtr reader;
    xmlNodePtr tree;
    xmlDocPtr doc;
    const char *URL = "tws://dummy";

#if defined(_DEBUG)
    mg_cry(info->conn, "INFO: dumping TWS scanner parameters to log file:");
    mg_cry_raw(info->conn, xml);
#endif

    rv = 0;

    doc = xmlReadDoc((const xmlChar *)xml, URL, "UTF-8", XML_PARSE_NOENT | XML_PARSE_NONET | XML_PARSE_NOCDATA);
  	xmlFreeDoc(doc);

    // process the XML response using libxml:
    xmlResetLastError();
    reader = xmlReaderForDoc((const xmlChar *)xml, URL, "UTF-8", XML_PARSE_NOENT | XML_PARSE_NONET | XML_PARSE_NOCDATA);

    rv = xmlTextReaderRead(reader);

    // signal the xmlreader to 'preserve' all XML nodes in there; we'll free them when their time is due...
    doc = xmlTextReaderCurrentDoc(reader);

    tree = xmlTextReaderExpand(reader);

    doc = xmlTextReaderCurrentDoc(reader);

    xmlFreeTextReader(reader);

  	xmlFreeDoc(doc);

    printf("scanner_parameters: opaque=%p, xml=%s, parse result=%d\n", opaque, xml, rv);
}

void event_scanner_data(void *opaque, int ticker_id, int rank, tr_contract_details_t *cd, const char distance[], const char benchmark[], const char projection[], const char legs_str[])
{
    printf("scanner_data: opaque=%p, ticker_id=%d, rank=%d, distance=%s, benchmark=%s, projection=%s\n",
           opaque, ticker_id, rank, distance, benchmark, projection);
    printf("scanner_data details: sym=%s, sectype=%s, expiry=%s, strike=%.3lf, right=%s, exch=%s, currency=%s, local_sym=%s, market_name=%s, trading_class=%s\n", cd->d_summary.s_symbol, cd->d_summary.s_sectype, cd->d_summary.s_expiry, cd->d_summary.s_strike, cd->d_summary.s_right, cd->d_summary.s_exchange, cd->d_summary.s_currency, cd->d_summary.s_local_symbol, cd->d_market_name, cd->d_trading_class);

    /* it seems that NYSE traded stocks have different market_name and trading_class from NASDAQ */
}

void event_scanner_data_end(void *opaque, int ticker_id)
{
    printf("scanner_data_end: opaque=%p, ticker_id=%d\n", opaque, ticker_id);
}

void event_current_time(void *opaque, long time)
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;
    struct tws_thread_exch *exch = info->tws_cfg->exch;

    printf("current_time: opaque=%p, time=%ld, ...\n", opaque, time);

    /*
    Pass response on to front-end now:
    */
    pthread_mutex_lock(&exch->tws_exch_mutex);
    if (0 != pthread_cond_signal(&exch->tws_rx_signal))
    {
      pthread_mutex_unlock(&exch->tws_exch_mutex);
    }
    else
    {
      // process incoming TX command/request from one of the front-end threads:
      mg_cry(info->conn, "backend CURRENT TIME message handler: response count = %d", exch->response);

      exch->response++;
      exch->current_time = time;

      pthread_mutex_unlock(&exch->tws_exch_mutex);
    }
}

void event_realtime_bar(void *opaque, int reqid, long time, double open, double high, double low, double close, long volume, double wap, int count)
{
    printf("realtime_bar: %p reqid=%d time=%ld, ohlc=%.4lf/%.4lf/%.4lf/%.4lf, vol=%ld, wap=%.4lf, count=%d\n", opaque, reqid, time, open, high, low, close, volume, wap, count);
}

void event_fundamental_data(void *opaque, int reqid, const char data[])
{
    printf("fundamental_data: opaque=%p, reqid=%d, ...\n", opaque, reqid);
}

void event_contract_details_end(void *opaque, int reqid)
{
    printf("contract_details_end: opaque=%p, ...\n", opaque);
}

void event_open_order_end(void *opaque)
{
    printf("open_order_end: opaque=%p\n", opaque);
}

void event_delta_neutral_validation(void *opaque, int reqid, under_comp_t *und)
{
    printf("delta_neutral_validation: opaque=%p, reqid=%d, ...\n", opaque, reqid);
}

void event_acct_download_end(void *opaque, char acct_name[])
{
    printf("acct_download_end: opaque=%p, account name='%s'\n", opaque, acct_name);
}

void event_exec_details_end(void *opaque, int reqid)
{
    printf("exec_details_end: opaque=%p, reqid=%d\n", opaque, reqid);
}

void event_tick_snapshot_end(void *opaque, int reqid)
{
    printf("tick_snapshot_end: opaque=%p, reqid=%d\n", opaque, reqid);
}
