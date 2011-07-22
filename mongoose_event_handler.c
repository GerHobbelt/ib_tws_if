
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


#include "mongoose_event_handler.h"

#include "tws_backend.h"
#include "tws_comm_thread.h"




void *event_handler(enum mg_event event_id, struct mg_connection *conn, const struct mg_request_info *request_info) 
{
	void *processed = "yes";
	struct mg_context *ctx = mg_get_context(conn);
	struct tws_conn_cfg *tws_cfg = (struct tws_conn_cfg *)mg_get_user_data(ctx)->user_data;
	struct tws_thread_exch *exch = tws_cfg->exch;

	switch (event_id) 
	{
	case MG_NEW_REQUEST:
#if 0
		if (!request_info->is_ssl) 
		{
      redirect_to_ssl(conn, request_info);
      processed = NULL;
		} 
		else if (!is_authorized(conn, request_info)) 
		{
      redirect_to_login(conn, request_info);
      processed = NULL;
		} 
		else if (strcmp(request_info->uri, authorize_url) == 0) 
		{
			do_authorize(conn, request_info);
		} 
		else if (strcmp(request_info->uri, "/ajax/get_messages") == 0) 
		{
			ajax_get_messages(conn, request_info);
		} 
		else if (strcmp(request_info->uri, "/ajax/send_message") == 0) 
		{
			ajax_send_message(conn, request_info);
		} 
    else
#endif
    
    if (strncmp(request_info->uri, "/tws/", 5) == 0)
    {
      struct timespec poll_time;
      poll_time.tv_sec = tws_cfg->backend_poll_period / 1000;
      poll_time.tv_nsec = (tws_cfg->backend_poll_period % 1000) * 1000000;

      // raw TWS backend requests: decode the request and pass the request to the backend in a serialized fashion; block & wait for the response...
      pthread_mutex_lock(&exch->tws_exch_mutex);

      // block & wait until we can go and submit the request:
      while (mg_get_stop_flag(ctx) == 0 && ETIMEOUT == pthread_cond_timedwait(&exch->tws_tx_signal, &exch->tws_exch_mutex, &poll_time))
        ;

      // send the request
      mg_cry(conn, "frontend request handler for url '%s': command count = %d", request_info->uri, exch->command);

      exch->command++;

      // block & wait until we can go and fetch the response:
      while (mg_get_stop_flag(ctx) == 0 && ETIMEOUT == pthread_cond_timedwait(&exch->tws_rx_signal, &exch->tws_exch_mutex, &poll_time))
        ;

      // receive the response
      mg_cry(conn, "frontend request handler for url '%s': response count = %d", request_info->uri, exch->response);

      mg_printf(conn, "<h1>TWS says the time is: %s</h1>\n", ctime(&exch->current_time));

      pthread_mutex_unlock(&exch->tws_exch_mutex);
    }
    else
		{
			// No suitable handler found, mark as not processed. Mongoose will
			// try to serve the request.
			processed = NULL;
		}
		break;

  case MG_EXIT0:
    // threads have already shut down; discard our custom mutexes, etc.:
    destroy_tws_thread_exch(&tws_cfg->exch);
    break;

	case MG_INIT0:
    // set up the 'front-end to back-end communication serialization' mutexes:
    init_tws_thread_exch(&tws_cfg->exch);

    // kickstart the TWS backend thread now:
    if (mg_start_thread(ctx, (mg_thread_func_t) tws_worker_thread, ctx) != 0) {
      mg_cry4ctx(ctx, "Cannot start TWS connection thread: %d", mg_strerror(mg_get_lasterror()));
      processed = NULL;
    }
		break;

  case MG_EVENT_LOG:
    // dump log to stderr as well:
    fprintf(stderr, "%s\n", request_info->log_message);
    // and let the default file logging do its own magic as well:
		processed = NULL;
    break;

	default:
		processed = NULL;
		break;
	}

	return processed;
}


