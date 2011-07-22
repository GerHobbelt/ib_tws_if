
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

#if defined(_MSC_VER)
#define flockfile(x) (void) 0
#define funlockfile(x) (void) 0
#endif

#if defined(DEBUG)
#define DEBUG_TRACE(x) do { \
  flockfile(stdout); \
  printf("*** %lu.%p.%s.%d: ", \
         (unsigned long) time(NULL), (void *) pthread_self(), \
         __func__, __LINE__); \
  printf x; \
  putchar('\n'); \
  fflush(stdout); \
  funlockfile(stdout); \
} while (0)
#else
#define DEBUG_TRACE(x)
#endif // DEBUG








#define TWS_CONNECT_RETRY_DELAY     10 // unit: seconds





void init_tws_thread_exch(struct tws_thread_exch **ptr_ref)
{
  struct tws_thread_exch *ptr = (struct tws_thread_exch *)calloc(1, sizeof(*ptr));

  pthread_mutex_init(&ptr->tws_exch_mutex, NULL);
  pthread_cond_init(&ptr->tws_rx_signal, NULL);
  pthread_cond_init(&ptr->tws_tx_signal, NULL);

  // set up the queue:
  ptr->work_queue_size = 16;
  ptr->work_queue_fill = 0;
  ptr->work_queue_poppos = 0;
  ptr->work_queue_diane = 0;
  ptr->work_queue = (tier2_queue_item_t **)calloc(ptr->work_queue_size, sizeof(ptr->work_queue[0]));

  // init libxml, which is used to process the XML messages coming from TWS on various occassions:
  xmlInitParser();
  // xmlRegisterInputCallbacks(xmlTWSmatch, xmlTWSopen, xmlTWSread, xmlTWSclose);

  *ptr_ref = ptr;
}

void destroy_tws_thread_exch(struct tws_thread_exch **ptr_ref)
{
  if (*ptr_ref)
  {
    struct tws_thread_exch *ptr = *ptr_ref;
    size_t i;

    pthread_cond_destroy(&ptr->tws_rx_signal);
    pthread_cond_destroy(&ptr->tws_tx_signal);
    pthread_mutex_destroy(&ptr->tws_exch_mutex);

    // TODO: signal all pending requests as ABORTED:
    for (i = 0; i < ptr->work_queue_fill; i++)
    {
      size_t idx = (i + ptr->work_queue_poppos) % ptr->work_queue_size;
      tier2_queue_item_t *elem = ptr->work_queue[idx];

      elem->response.code = TIER2_ABORTED;

      // TODO: wait for the front-ends to recognize this change of affairs.
    }

    free(ptr->work_queue);
    free(ptr);

    xmlCleanupParser();
    xmlMemoryDump();

    *ptr_ref = NULL;
  }
}






// front-end invoked queue calls:
int tier2_push_request(tier2_queue_item_t *cmd);
int tier2_abort_request(tier2_queue_item_t *cmd);

// back-end invoked queue calls:
tier2_queue_item_t *tier2_pop_request(void);

// utility calls:
unsigned int tier2_queue_depth(void);
tier2_queue_item_t *tier2_peek_request(int optional_index);









static int tws_transmit_func(void *arg, const void *buf, unsigned int buflen)
{
  struct my_tws_io_info *info = (struct my_tws_io_info *)arg;

  return mg_write(info->conn, buf, buflen);
}

static int tws_receive_func(void *arg, void *buf, unsigned int max_bufsize)
{
  struct my_tws_io_info *info = (struct my_tws_io_info *)arg;
  struct tws_thread_exch *exch = info->tws_cfg->exch;

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
      more data to arrive. Meanwhile, we can process incomign requests from the front-end now:
      */
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
    }
  }

  return mg_pull(info->conn, buf, max_bufsize);
  // return mg_read(info->conn, buf, max_bufsize);
}

/* 'flush()' marks the end of the outgoing message: it should be transmitted ASAP */
static int tws_flush_func(void *arg)
{
  struct my_tws_io_info *info = (struct my_tws_io_info *)arg;

  return 0;
}

/* close callback is invoked on error or when tws_disconnect is invoked */
static int tws_close_func(void *arg)
{
  struct my_tws_io_info *info = (struct my_tws_io_info *)arg;

  mg_close_connection(info->conn);

  return 0;
}


static const char *tws_errcode2str(int errcode)
{
  const struct twsclient_errmsg *einfo = tws_strerror(errcode);

  return einfo->err_msg;
}



/* find top percentage gainers (US stocks) with price > 5 and volume > 2M */
static void scan_market(void *ti)
{
    /* illustrate new feature as of version 23 */
    tr_scanner_subscription_t s;

    tws_req_scanner_parameters(ti);

    memset(&s, 0, sizeof s);
    s.scan_number_of_rows = -1;
    s.scan_instrument = "STK";
    s.scan_location_code = "STK.AEB,STK.SBF";
    s.scan_code = "MOST_ACTIVE"; // "TOP_PERC_GAIN"; /* see xml returned by tws_req_scanner_parameters for more choices */

    s.scan_above_price = 0.05;
    s.scan_below_price = s.scan_coupon_rate_above = s.scan_coupon_rate_below = DBL_MAX;
    s.scan_above_volume = 1000;
    s.scan_market_cap_above = s.scan_market_cap_below = DBL_MAX;
    s.scan_moody_rating_above = s.scan_moody_rating_below = s.scan_sp_rating_above = s.scan_sp_rating_below = s.scan_maturity_date_above = s.scan_maturity_date_below = s.scan_exclude_convertible = "";
    s.scan_scanner_setting_pairs = "";
    s.scan_stock_type_filter = "";
    tws_req_scanner_subscription(ti, 1, &s);
}







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
    struct mg_connection *conn;
    struct tws_conn_cfg *tws_cfg = (struct tws_conn_cfg *)mg_get_user_data(ctx)->user_data;
    struct my_tws_io_info info = 
    { 
      NULL,
      tws_cfg,
      NULL
    };
    int err;

	conn = mg_connect_to_host(ctx, tws_cfg->ip_address, tws_cfg->port, 0);
    if (conn != NULL)
    {
      tws_app_is_down = 0;

      // Disable Nagle - act a la telnet:
      mg_set_nodelay_mode(mg_get_client_socket(conn), 1);
      
      info.conn = conn;
      info.tws_handle = tws_create(&info, tws_transmit_func, tws_receive_func, tws_flush_func, tws_close_func);
      err = tws_connect(info.tws_handle, tws_cfg->our_id_code);
      if (err) 
      {
        mg_cry(conn, "tws connect returned %s\n", tws_errcode2str(err)); 
      }
      else
      {
        // request the valid set of scanner parameters first: this will trigger the requesting of several market scans from the msg receive handler:
        tws_req_scanner_parameters(info.tws_handle);

        while (mg_get_stop_flag(ctx) == 0 && tws_connected(info.tws_handle)) 
        {
          if (0 != tws_event_process(info.tws_handle))
          {
            break;
          }
        }
      }

      tws_disconnect(info.tws_handle);
      tws_destroy(info.tws_handle);
      free(conn);
    }
    else 
    {
      if (!tws_app_is_down)
      {
        mg_cry4ctx(ctx, "Cannot establish a connection with the TWS application. Retrying in %d seconds...", TWS_CONNECT_RETRY_DELAY);
        tws_app_is_down++;
      }
    }

    // wait N seconds before retrying to connect to TWS:
    if (mg_get_stop_flag(ctx) == 0) 
    {
      sleep(TWS_CONNECT_RETRY_DELAY);
    }
  }

  mg_signal_mgr_this_thread_is_done(ctx);

  DEBUG_TRACE(("exiting"));
}






