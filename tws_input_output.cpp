
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
#include "tws_backend_io.h"

#include <mongoose/mongoose_ex.h>





#define TWS_CONNECT_RETRY_DELAY     10000 // unit: milliseconds



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
    ib_tws_manager *ibm = mgr->get_ib_tws_manager();

    ibm->set_context(ctx);
    mgr->register_backend_thread(ctx, app_manager::IB_TWS_API_CONNECTION_THREAD, ibm);
    assert(mgr->get_requester(ctx, app_manager::IB_TWS_API_CONNECTION_THREAD) == ibm);
    
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
                    mg_cry4ctx(ctx, "Cannot establish a connection with the TWS application. Retrying every %d seconds...", TWS_CONNECT_RETRY_DELAY / 1000);
                    tws_app_is_down++;
                }
            }
            else
            {
                // reset the counter as we have a valid/working connection again now:
                tws_app_is_down = 0;

                // request the valid set of scanner parameters first: this will trigger the requesting of several market scans from the msg receive handler:
                assert(mgr->get_requester(ctx, app_manager::IB_TWS_API_CONNECTION_THREAD) == ibm);
#if 0  // won't work when IB doesn't have a data connection itself: SILENT FAILURE!
                ib_msg_req_scanner_parameters *scan = new ib_msg_req_scanner_parameters(ibm, NULL);
                scan->state(tier2_message::EXEC_COMMAND);
#endif

                while (mg_get_stop_flag(ctx) == 0 && ibm->is_tws_connected())
                {
                    // push messages across the thread barrier + destroy messages marked for destruction
                    if (0 != ibm->pulse_marked_messages())
                    {
                        break;
                    }

                    // process another message
                    if (0 != ibm->process_one_event())
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

        err = ibm->exit_tws_api();

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

    mgr->unregister_backend_thread(ctx, app_manager::IB_TWS_API_CONNECTION_THREAD);

    mg_signal_mgr_this_thread_is_done(ctx);

    DEBUG_TRACE(("exiting"));
}











int ib_tws_manager::init_tws_api(void)
{
    int err = 0;

    m_still_need_to_prime_the_pump = true;

    tws_handle = tws::tws_create(get_app_manager(), tws_transmit_func, tws_receive_func, tws_flush_func, tws_open_func, tws_close_func, tws_tx_elem_observe_func, tws_rx_elem_observe_func);
    if (tws_handle)
    {
        err = tws::tws_connect(tws_handle, m_tws_cfg.m_our_id_code);
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


int ib_tws_manager::process_one_event()
{
    return tws::tws_event_process(tws_handle);
}


int ib_tws_manager::exit_tws_api()
{
    tws::tws_disconnect(tws_handle);
    tws::tws_destroy(tws_handle);
    return 0;
}


