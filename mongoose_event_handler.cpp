
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
#include "app_manager.h"



void *event_handler(enum mg_event event_id, struct mg_connection *conn)
{
    void *processed = "yes";
    struct mg_context *ctx = mg_get_context(conn);
    app_manager *mgr = (app_manager *)mg_get_user_data(ctx)->user_data;
	const struct mg_request_info *ri = mg_get_request_info(conn);

    switch (event_id)
    {
    case MG_NEW_REQUEST:
        if (strncmp(ri->uri, "/tws/", 5) == 0)
        {
			ib_req_current_time *tws_req = new ib_req_current_time(mgr->get_requester(conn));
            struct timespec poll_time;
			int err;
            poll_time.tv_sec = mgr->get_tws_ib_connection_config().backend_poll_period / 1000;
            poll_time.tv_nsec = (mgr->get_tws_ib_connection_config().backend_poll_period % 1000) * 1000000;

            // pass the request to the backend; block & wait for the response...
			err = mgr->tws_cfg->push(tws_req);

#if 0
			mg_printf(conn, "<h1>TWS says the time is: %s</h1>\n", ctime(&tws_req->current_time));
#endif
		}
        else
        {
            // No suitable handler found, mark as not processed. Mongoose will
            // try to serve the request.
            processed = NULL;
        }
        break;

	case MG_REQUEST_COMPLETE:
		processed = NULL;
		break;

    case MG_EXIT0:
        // threads have already shut down; discard our custom mutexes, etc.:
        delete tws_cfg->exch;
		tws_cfg->exch = NULL;

		xmlCleanupParser();
        xmlMemoryDump();
        break;

    case MG_INIT0:
		// init libxml, which is used to process the XML messages coming from TWS on various occasions:
		xmlInitParser();
		// xmlRegisterInputCallbacks(xmlTWSmatch, xmlTWSopen, xmlTWSread, xmlTWSclose);

        // set up the 'front-end to back-end communication serialization' mutexes:

        // kickstart the TWS backend thread now:
        if (mg_start_thread(ctx, (mg_thread_func_t) tws_worker_thread, ctx) != 0) {
            mg_cry4ctx(ctx, "Cannot start TWS connection thread: %d", mg_strerror(mg_get_lasterror()));
            processed = NULL;
        }
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



