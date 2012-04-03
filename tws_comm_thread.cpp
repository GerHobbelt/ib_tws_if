
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

#include "tws_comm_thread.h"






#define TWS_CONNECT_RETRY_DELAY     10000 // unit: milliseconds






/*
replace TWSAPI debug printf call.
*/
void tws_debug_printf(void *opaque, const char *fmt, ...)
{
	my_tws_io_info *info = (my_tws_io_info *)opaque;
	va_list ap;

	va_start(ap, fmt);
	//mg_vlog((info ? info->conn : NULL), "debug", fmt, ap);
	va_end(ap);
}












static int tws_transmit_func(void *arg, const void *buf, unsigned int buflen)
{
    my_tws_io_info *info = (my_tws_io_info *)arg;

    if (info->conn)
    {
        return mg_write(info->conn, buf, buflen);
    }
    return -1;
}

static int tws_receive_func(void *arg, void *buf, unsigned int max_bufsize)
{
    my_tws_io_info *info = (my_tws_io_info *)arg;
    tws_thread_exch *exch = info->tws_cfg->exch;

    if (info->conn)
    {
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
                    have something like a 'content length' to guide us along, so we'll have to use another method to make sure
                    the read operation actually delivers DATA!
                    */
                    // conn->content_len = MAX_INT;
                    break;
                }

                /*
                When there's no pending incoming data from TWS itself, we'll be running around in this loop while waiting for
                more data to arrive. Meanwhile, we can process queued requests from the front-end now:
                */
				process_one_queued_tier2_request(info);
            }
        }

        return mg_pull(info->conn, buf, max_bufsize);
    }
    return -1;
}
8
/* 'flush()' marks the end of the outgoing message: it should be transmitted ASAP */
static int tws_flush_func(void *arg)
{
    //my_tws_io_info *info = (my_tws_io_info *)arg;

    return 0;
}

/* open callback is invoked when tws_connect is invoked and no connection has been established yet (tws_connected() == false); return 0 on success; a twsclient_error_codes error code on failure. */
static int tws_open_func(void *arg)
{
    my_tws_io_info *info = (my_tws_io_info *)arg;
    struct tws_conn_cfg *tws_cfg = info->tws_cfg;
    struct mg_context *ctx = info->ctx;
    struct mg_connection *conn = mg_connect_to_host(ctx, tws_cfg->ip_address, tws_cfg->port, 0);

    if (conn != NULL)
    {
		struct socket *sock = mg_get_client_socket(conn);

        // Disable Nagle - act a la telnet:
        mg_set_nodelay_mode(sock, 1);

		// enable keepalive + rx/tx timeouts:
		mg_set_socket_keepalive(sock, 1);
		mg_set_socket_timeout(sock, 10);
    }

    info->conn = conn;

    return (conn ? 0 : NOT_CONNECTED);
}


/* close callback is invoked on error or when tws_disconnect is invoked */
static int tws_close_func(void *arg)
{
    my_tws_io_info *info = (my_tws_io_info *)arg;

    if (info->conn)
    {
        mg_close_connection(info->conn);
        info->conn = NULL;
    }

    return 0;
}


static const char *tws_errcode2str(int errcode)
{
    const struct tws::twsclient_errmsg *einfo = tws::tws_strerror(errcode);

    return einfo->err_msg;
}




















/*
Check whether there are any queued requests which are 'activated' and if there
are, process one.
*/
static void process_one_queued_tier2_request(my_tws_io_info *info)
{
	struct tws_conn_cfg *tws_cfg = info->tws_cfg;
	tier2_queue_item cmd;
	
	if (tier2_pop_request(tws_cfg, &cmd) > 0)
	{


	}
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
        struct tws_conn_cfg *tws_cfg = (struct tws_conn_cfg *)mg_get_user_data(ctx)->user_data;
		tws_thread_exch *exch = tws_cfg->exch;
        my_tws_io_info info = {0};
        int err;
		int abortus_provocatus = 0;

		tws_cfg->tws_thread_info = &info;

        info.tws_cfg = tws_cfg;
        info.ctx = ctx;

		info.tws_handle = tws_create(&info, tws_transmit_func, tws_receive_func, tws_flush_func, tws_open_func, tws_close_func);
        if (info.tws_handle)
        {
            err = tws_connect(info.tws_handle, tws_cfg->our_id_code);
            if (err)
            {
                mg_cry4ctx(ctx, "tws connect returned error: %s", tws_errcode2str(err));

                if (!tws_app_is_down)
                {
                    mg_cry4ctx(ctx, "Cannot establish a connection with the TWS application. Retrying every %d seconds...", TWS_CONNECT_RETRY_DELAY);
                    tws_app_is_down++;
                }
            }
            else
            {
                // reset the counter as we have a valid/working connection again now:
                tws_app_is_down = 0;

				// make sure the databases have been opened and are ready to receive data:
				err = ib_open_databases(&info);
				if (err)
				{
					mg_cry4ctx(ctx, "FATAL ERROR: Cannot access the database: %d (%s)\n", err, ib_strerror(err));
					abortus_provocatus = 1;
					goto fail_dramatically;
				}

                // request the valid set of scanner parameters first: this will trigger the requesting of several market scans from the msg receive handler:
				pthread_mutex_lock(&exch->tws_tx_mutex);
                tws_req_scanner_parameters(info.tws_handle);
				pthread_mutex_unlock(&exch->tws_tx_mutex);

                while (mg_get_stop_flag(ctx) == 0 && tws_connected(info.tws_handle))
                {
                    // process another message
                    if (0 != tws_event_process(info.tws_handle))
                    {
                        break;
                    }

					/*
					The 'tws_receive_func' takes care of processing front-end messages 
					while waiting for incoming TWS traffic...
					*/
				}
            }
        }
        else
        {
            mg_cry4ctx(ctx, "FATAL ERROR: memory error in tws_create: aborting thread\n");
			abortus_provocatus = 1;
        }

fail_dramatically:
        tws_disconnect(info.tws_handle);
		ib_close_databases(&info);
        tws_destroy(info.tws_handle);
		info.tws_handle = NULL;
		tws_cfg->tws_thread_info = NULL;
		if (abortus_provocatus)
		{
			mg_signal_stop(ctx);
			break;
		}

        // wait N seconds before retrying to connect to TWS:
        if (mg_get_stop_flag(ctx) == 0)
        {
            mg_sleep(TWS_CONNECT_RETRY_DELAY);
        }
    }

    mg_signal_mgr_this_thread_is_done(ctx);

    DEBUG_TRACE(("exiting"));
}






