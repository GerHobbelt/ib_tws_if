
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

#include "mongoose_event_handler.h"
#include "tws_request.h"
#include "data_tracker.h"
#include "app_manager.h"

#include <libxml/parser.h>


void *event_handler(enum mg_event event_id, struct mg_connection *conn)
{
    void *processed = "yes";
    struct mg_context *ctx = mg_get_context(conn);
    app_manager *mgr = (app_manager *)mg_get_user_data(ctx)->user_data;
    const struct mg_request_info *ri = mg_get_request_info(conn);

    switch (event_id)
    {
    case MG_NEW_REQUEST:
        if (!strncmp("/tws/", ri->uri, 5)
            && (!strcmp(ri->request_method, "GET") || !strcmp(ri->request_method, "POST")))
        {
            assert(mgr->get_requester(conn));
            assert(mgr->get_requester(ctx, app_manager::IB_TWS_API_CONNECTION_THREAD));
            assert(mgr->get_requester(ctx, app_manager::IB_TWS_API_CONNECTION_THREAD) == mgr->get_ib_tws_manager());
            tier2_message_processor *from = mgr->get_requester(conn);
            tier2_message_processor *to = mgr->get_ib_tws_manager();
            assert(from);
            assert(to);
            tws_request_message *req_msg = NULL;
            const char *req_uri = ri->uri + 5;
            if (!strncmp("proxy/", req_uri, 6))
            {
                req_uri += 6;

                if (!strcmp("server_time", req_uri))
                {
                    req_msg = new ib_msg_req_current_time(mgr->get_requester(conn), mgr->get_ib_tws_manager());
                }
            }

            if (!req_msg)
            {
                processed = NULL;
            }
            else
            {
                interthread_communicator *comm = req_msg->get_interthread_communicator();
                int err;

                // pass the request to the back-end; block & wait for the response...
                req_msg->state(tier2_message::EXEC_COMMAND);
                req_msg->pulse();
                err = req_msg->wait_for_response(comm);

                // we don't set the required 'Content-Length' header, so it's close after we're done here:
                mg_connection_must_close(conn);

                (void) mg_printf(conn,
                      "HTTP/1.1 200 OK\r\n"
                      "Content-Type: application/json\r\n"  // http://stackoverflow.com/questions/477816/the-right-json-content-type
                      "Cache-Control: no-cache\r\n"
                      //"Content-Length: %d\r\n"
                      "Connection: close\r\n\r\n");
                mg_mark_end_of_header_transmission(conn);

                json_output channel(conn);
                channel.start();
                req_msg->save_response(&channel);
                channel.finish();
            }
            break;
        }
        // No suitable handler found, mark as not processed. Mongoose will
        // try to serve the request.
        processed = NULL;
        break;

    case MG_REQUEST_COMPLETE:
        processed = NULL;
        break;

    case MG_EXIT0:
        // threads have already shut down; discard our custom mutexes, etc.:
        xmlCleanupParser();
        xmlMemoryDump();
        break;

    case MG_INIT0:
        // init libxml, which is used to process the XML messages coming from TWS on various occasions:
        xmlInitParser();
        // xmlRegisterInputCallbacks(xmlTWSmatch, xmlTWSopen, xmlTWSread, xmlTWSclose);

        // set up the 'front-end to back-end communication serialization' mutexes:

        // kickstart the TWS and Calculus/DB back-end threads now:
        if (mg_start_thread(ctx, (mg_thread_func_t) tws_worker_thread, ctx) != 0) {
            mg_cry4ctx(ctx, "Cannot start TWS connection thread: %d", mg_strerror(mg_get_lasterror()));
            processed = NULL;
        }
        if (mg_start_thread(ctx, (mg_thread_func_t) data_tracker_thread, ctx) != 0) {
            mg_cry4ctx(ctx, "Cannot start Data Tracker thread: %d", mg_strerror(mg_get_lasterror()));
            processed = NULL;
        }
        break;

    case MG_INIT_CLIENT_CONN:  // Mongoose has opened a connection to a client.
        mgr->register_frontend_thread(conn);
        processed = NULL;
        break;

    case MG_EXIT_CLIENT_CONN:  // Mongoose is going to close the client connection.
//      mgr->unregister_frontend_thread(conn);
        processed = NULL;
        break;

    case MG_EVENT_LOG:
        // dump log to stderr as well:
        fprintf(stderr, "%s: %s\n", ri->log_severity, ri->log_message);
        // and let the default file logging do its own magic as well:
        processed = NULL;
        break;

    case MG_HTTP_ERROR:
        processed = NULL;
        break;

    default:
        processed = NULL;
        break;
    }

    return processed;
}



