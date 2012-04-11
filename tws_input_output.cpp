
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

#include <tws_c_api/twsapi.h>

#include "app_manager.h"
#include "tws_database_io.h"

#include <mongoose/mongoose_ex.h>





#define TWS_CONNECT_RETRY_DELAY     10000 // unit: milliseconds








static int tws_transmit_func(void *arg, const void *buf, unsigned int buflen)
{
    app_manager *mgr = (app_manager *)arg;
	mg_connection *conn = mgr->get_tws_ib_connection();

    if (conn)
    {
        return mg_write(conn, buf, buflen);
    }
    return -1;
}

static int tws_receive_func(void *arg, void *buf, unsigned int max_bufsize)
{
    app_manager *mgr = (app_manager *)arg;
	mg_connection *conn = mgr->get_tws_ib_connection();
	tier2_message_receiver *recvr = mgr->get_receiver(conn);

    if (conn && recvr)
    {
        // check whether there's anything available:
        fd_set read_set, except_set;
        struct timeval tv;
        int max_fd;

        tv.tv_sec = mgr->get_tws_ib_connection_config().backend_poll_period / 1000;
        tv.tv_usec = (mgr->get_tws_ib_connection_config().backend_poll_period % 1000) * 1000;

        while (mg_get_stop_flag(mg_get_context(conn)) == 0)
        {
            struct timeval tv2 = tv;
			int rv;

            FD_ZERO(&read_set);
			FD_ZERO(&except_set);
            max_fd = -1;

            // Add listening sockets to the read set
            mg_FD_SET(mg_get_client_socket(conn), &read_set, &max_fd);
			mgr->fd_set_4_interthread_messaging(recvr, &read_set, &except_set, &max_fd);

            if (select(max_fd + 1, &read_set, NULL, &except_set, &tv2) < 0)
            {
                break;
            }
            else
            {
                if (mg_FD_ISSET(mg_get_client_socket(conn), &read_set))
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
				rv = recvr->process_one_queued_tier2_request(mgr, &read_set, &except_set, max_fd);
				if (rv < 0)
				{
					// signal a fatal failure:
					max_bufsize = 0;
					break;
				}
            }
        }

		if (max_bufsize)
		{
			return mg_pull(conn, buf, max_bufsize);
		}
    }
    return -1;
}

/* 'flush()' marks the end of the outgoing message: it should be transmitted ASAP */
static int tws_flush_func(void *arg)
{
    //app_manager *mgr = (app_manager *)arg;

    return 0;
}

/* open callback is invoked when tws_connect is invoked and no connection has been established yet (tws_connected() == false); return 0 on success; a twsclient_error_codes error code on failure. */
static int tws_open_func(void *arg)
{
    app_manager *mgr = (app_manager *)arg;
    struct tws_conn_cfg &tws_cfg = mgr->get_tws_ib_connection_config();
    struct mg_context *ctx = mgr->get_tws_ib_context();
    struct mg_connection *conn = mg_connect_to_host(ctx, tws_cfg.ip_address, tws_cfg.port, 0);

    if (conn != NULL)
    {
		struct socket *sock = mg_get_client_socket(conn);

        // Disable Nagle - act a la telnet:
        mg_set_nodelay_mode(sock, 1);

		// enable keepalive + rx/tx timeouts:
		mg_set_socket_keepalive(sock, 1);
		mg_set_socket_timeout(sock, 10);
    }

    mgr->set_tws_ib_connection(conn);

    return (conn ? 0 : (int)tws::NOT_CONNECTED);
}


/* close callback is invoked on error or when tws_disconnect is invoked */
static int tws_close_func(void *arg)
{
    app_manager *mgr = (app_manager *)arg;
	mg_connection *conn = mgr->get_tws_ib_connection();

    if (conn)
    {
        mg_close_connection(conn);
        conn = NULL;
		mgr->set_tws_ib_connection(conn);
    }

    return 0;
}


const char *ib_tws_manager::strerror(int errcode)
{
	if (errcode >= 0)
	{
		const struct tws::twsclient_errmsg *einfo = tws::tws_strerror(errcode);

		return einfo->err_msg;
	}
	else
	{
		switch (errcode)
		{
		case -1:
			return "TWS API fatal initialization failure";

		default:
			return "Unidentified TWS API / MANAGER failure";
		}
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
	app_manager *mgr = (app_manager *)mg_get_user_data(ctx)->user_data;
	db_manager *dbm = mgr->get_db_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();

    // retry connecting to TWS as long as the server itself hasn't been stopped!
    while (mg_get_stop_flag(ctx) == 0)
    {
        int err;
		int abortus_provocatus = 0;

		err = ibm->init_tws_api();
		if (err >= 0)
		{
            if (err)
            {
                mg_cry4ctx(ctx, "tws connect returned error: %s", ibm->strerror(err));

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

				err = dbm->open_databases();
				if (err)
				{
					mg_cry4ctx(ctx, "FATAL ERROR: Cannot access the database: %d (%s)\n", err, dbm->strerror(err));
					abortus_provocatus = 1;
					goto fail_dramatically;
				}

                // request the valid set of scanner parameters first: this will trigger the requesting of several market scans from the msg receive handler:
				ib_req_scanner_parameters *scan = new ib_req_scanner_parameters(mgr->get_requester(ctx), NULL);
				err = scan->transmit();

                while (mg_get_stop_flag(ctx) == 0 && ibm->is_tws_connected())
                {
                    // process another message
                    if (0 != ibm->process_tws_event())
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
		err = ibm->exit_tws_api();
		err = dbm->close_databases();

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











int ib_tws_manager::init_tws_api(void)
{
	int err = 0;

	tws_handle = tws::tws_create(this, tws_transmit_func, tws_receive_func, tws_flush_func, tws_open_func, tws_close_func);
	if (tws_handle)
	{
		err = tws::tws_connect(tws_handle, tws_cfg.our_id_code);
	}
	else
	{
		err = -1;
	}
	return err;
}




int ib_tws_manager::is_tws_connected()
{
	return tws::tws_connected(tws_handle);
}


int ib_tws_manager::process_tws_event()
{
	return tws::tws_event_process(tws_handle);
}


int ib_tws_manager::exit_tws_api()
{
	tws::tws_disconnect(tws_handle);
	tws::tws_destroy(tws_handle);
	return 0;
}


