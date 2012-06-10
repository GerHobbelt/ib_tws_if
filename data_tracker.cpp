
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
#include "tws_backend_io.h"
#include "interthread_comm.h"
#include "data_tracker.h"

#include <mongoose/mongoose_ex.h>

// kill mongoose/OTL collisions due to mongoose #defines:
#undef close

#include <otl/otlv4_h/otlv4.h>






#define DATA_TRACKER_INIT_RETRY_DELAY     10000 // unit: milliseconds



data_tracker_manager::data_tracker_manager(app_manager *mgr)
    : tier2_message_processor(new requester_id(NULL, NULL, 0), mgr)
    , m_global_ctx(NULL)
{
}

data_tracker_manager::~data_tracker_manager()
{
}




int data_tracker_manager::process_one_event(void)
{
#if 0
    // check whether there's anything available:
    fd_set read_set, except_set;
    struct timeval tv;
    int max_fd;
    int rv;

    rv = -1;

    while (mg_get_stop_flag(get_context()) == 0)
    {
        struct timeval tv2 = tv;
        int proc_rv;
        // determine how many messages are waiting to be 'pulsed':
        size_t pulse_count = m_msgs_pending_for_pulsing.size();

        tv.tv_sec = m_tws_cfg.m_backend_poll_period / 1000;
        tv.tv_usec = (m_tws_cfg.m_backend_poll_period % 1000) * 1000;
        // when there are messages waiting to be pulsed, we don't wait long to do so
        // if there's no incoming data:
        if (pulse_count)
        {
            tv.tv_sec = 0;
            tv.tv_usec = 0;
        }

        FD_ZERO(&read_set);
        FD_ZERO(&except_set);
        max_fd = -1;

        // Add listening sockets to the read set
        mg_FD_SET(mg_get_client_socket(m_tws_conn), &read_set, &max_fd);
        prepare_fd_sets_for_reception(&read_set, &except_set, max_fd);

        if (select(max_fd + 1, &read_set, NULL, &except_set, &tv2) < 0)
        {
            // signal a fatal failure:
            // clear the handles sets to prevent 'surprises' from processing these a second time (below):
            FD_ZERO(&read_set);
            FD_ZERO(&except_set);
            max_fd = -1;
            assert(!"Should never get here");

            max_bufsize = 0;
            break;
        }
        else
        {
            if (mg_FD_ISSET(mg_get_client_socket(m_tws_conn), &read_set))
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
            proc_rv = process_one_queued_tier2_request(&read_set, &except_set, max_fd);
            if (proc_rv < 0)
            {
                // signal a fatal failure:
                // clear the handles sets to prevent 'surprises' from processing these a second time (below):
                FD_ZERO(&read_set);
                FD_ZERO(&except_set);
                max_fd = -1;
                assert(!"Should never get here");

                // prevent the mg_read() from locking up due to no incoming data:
                max_bufsize = 0;
                break;
            }

            // check whether any new client interconnects have been set up?
            m_app_manager->fetch_new_interthread_communicators(this);

            /*
            Also 'tickle' the pending queue in round-robin fashion to ensure that
            scheduled / postponed requests get serviced.
            */
            proc_rv = pulse_pending_issues();
            if (proc_rv < 0)
            {
                // signal a fatal failure:
                // clear the handles sets to prevent 'surprises' from processing these a second time (below):
                FD_ZERO(&read_set);
                FD_ZERO(&except_set);
                max_fd = -1;
                assert(!"Should never get here");

                // prevent the mg_read() from locking up due to no incoming data:
                max_bufsize = 0;
                break;
            }
        }
    }

    if (mg_get_stop_flag(mg_get_context(m_tws_conn)) == 0)
    {
        int proc_rv;

        /*
        Even when there's pending incoming data from TWS itself, we'll need to process queued
        requests from the front-end and 'pending' queue or we would be experiencing lockup:
        */
        proc_rv = process_one_queued_tier2_request(&read_set, &except_set, max_fd);
        if (proc_rv < 0)
        {
            // signal a fatal failure:
            rv = proc_rv;
            assert(!"Should never get here");
        }
        else
        {
            proc_rv = pulse_pending_issues();
            if (proc_rv < 0)
            {
                // signal fatal failure:
                rv = proc_rv;
                assert(!"Should never get here");
            }
            else if (max_bufsize)
            {
                rv = mg_pull(m_tws_conn, buf, max_bufsize);
                if (rv < 0)
                {
                    assert(!"Should never get here");
                }
                //assert(rv != 0);
            }
        }
    }
#endif

    return 0;
}




/*
Track any 'registered' stocks:

- use this thread to run any calculations based on the incoming data / user activity.

- issue historical data requests when the data is lacking

- run what-if scenarios, etc.etc.
*/
void data_tracker_thread(struct mg_context *ctx)
{
    app_manager *mgr = (app_manager *)mg_get_user_data(ctx)->user_data;
    data_tracker_manager *clm = mgr->get_data_tracker_manager();
    db_manager *dbm = mgr->get_db_manager();

    clm->set_context(ctx);
    mgr->register_backend_thread(ctx, app_manager::DATA_TRACKER_THREAD, clm);
    
    // retry connecting to the database as long as the server itself hasn't been stopped!
    while (mg_get_stop_flag(ctx) == 0)
    {
        int err;
        int abortus_provocatus = 0;

        err = dbm->open_databases();
        if (err)
        {
            mg_cry4ctx(ctx, "FATAL ERROR: Cannot access the database: %d (%s)\n", err, dbm->strerror(err));
            abortus_provocatus = 1;
            goto fail_dramatically;
        }

        assert(mgr->get_requester(ctx, app_manager::DATA_TRACKER_THREAD) == clm);

        while (mg_get_stop_flag(ctx) == 0)
        {
            // push messages across the thread barrier + destroy messages marked for destruction
            if (0 != clm->pulse_marked_messages())
            {
                break;
            }

            // process another message
            if (0 != clm->process_one_event())
            {
                break;
            }
        }

fail_dramatically:
        err = dbm->close_databases();

        if (abortus_provocatus)
        {
            mg_signal_stop(ctx);
            break;
        }

        // wait N seconds before retrying to connect to TWS:
        if (mg_get_stop_flag(ctx) == 0)
        {
            mg_sleep(DATA_TRACKER_INIT_RETRY_DELAY);
        }
    }

    mgr->unregister_backend_thread(ctx, app_manager::DATA_TRACKER_THREAD);

    mg_signal_mgr_this_thread_is_done(ctx);

    DEBUG_TRACE(("exiting"));
}







static data_tracker_manager *singleton = NULL;


data_tracker_manager *data_tracker_manager::get_instance(app_manager *mgr, bool instantiate_singleton)
{
    if (!singleton && instantiate_singleton)
    {
        data_tracker_manager *obj = new data_tracker_manager(mgr);

        set_instance(obj);
        return obj;
    }
    return singleton;
}

void data_tracker_manager::set_instance(data_tracker_manager *instance)
{
    if (singleton)
    {
        delete singleton;
    }
    singleton = instance;
}



