
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

#include <libxml/parser.h>
#include <tre/regex.h>
#include <upskirt/src/markdown.h>
#include <upskirt/html/html.h>


using namespace upskirt;



int serve_a_markdown_page(struct mg_connection *conn)
{
#define SD_READ_UNIT 1024
#define SD_OUTPUT_UNIT 64

	struct mg_request_info *ri = mg_get_request_info(conn);

	struct mgstat st;

	assert(ri->phys_path);
	if (0 == mg_stat(ri->phys_path, &st) && !st.is_directory)
	{
		struct sd_buf *ib, *ob;
		int ret;
		unsigned int enabled_extensions = MKDEXT_TABLES | MKDEXT_FENCED_CODE | MKDEXT_EMAIL_FRIENDLY;
		unsigned int render_flags = 0; // HTML_SKIP_HTML | HTML_SKIP_STYLE | HTML_HARD_WRAP;

		struct sd_callbacks callbacks;
		struct html_renderopt options;
		struct sd_markdown *markdown;

		/* opening the file */
		FILE *in = fopen(ri->phys_path, "r");
		if (!in)
		{
			mg_send_http_error(conn, 404, NULL, "Unable to open input file: [%s] %s", ri->uri, mg_strerror(errno));
			return -1;
		}

		/* reading everything */
		ib = sd_bufnew(SD_READ_UNIT);
		if (SD_BUF_OK != sd_bufgrow(ib, (size_t)st.size))
		{
			mg_send_http_error(conn, 500, NULL, "Out of memory while loading Markdown input file: [%s]", ri->uri);
			fclose(in);
			sd_bufrelease(ib);
			return -1;
		}
		ret = fread(ib->data, 1, ib->asize, in);
		if (ret > 0) 
		{
			ib->size += ret;
			fclose(in);
		}
		else
		{
			mg_send_http_error(conn, 500, NULL, "Cannot read from input file: [%s] %s", ri->uri, mg_strerror(errno));
			fclose(in);
			sd_bufrelease(ib);
			return -1;
		}

		/* performing markdown parsing */
		ob = sd_bufnew(SD_OUTPUT_UNIT);

		sdhtml_renderer(&callbacks, &options, render_flags);
		markdown = sd_markdown_new(enabled_extensions, 16, &callbacks, &options);
		if (!markdown)
		{
			mg_send_http_error(conn, 500, NULL, "Out of memory while processing Markdown input file: [%s]", ri->uri);
			sd_bufrelease(ib);
			sd_bufrelease(ob);
			return -1;
		}
		sd_markdown_render(ob, ib->data, ib->size, markdown);
		sd_markdown_free(markdown);

		/* write the appropriate headers */
		char date[64], lm[64], etag[64], range[64];
		time_t curtime = time(NULL);
		const char *hdr;
		int64_t cl, r1, r2;
		int n;

		ri->status_code = 200;
		range[0] = '\0';

		cl = ob->size;

#if 0
		// If Range: header specified, act accordingly
		r1 = r2 = 0;
		hdr = mg_get_header(conn, "Range");
		if (hdr != NULL && (n = parse_range_header(hdr, &r1, &r2)) > 0) {
			conn->request_info.status_code = 206;
			(void) fseeko(fp, (off_t) r1, SEEK_SET);
			cl = n == 2 ? r2 - r1 + 1: cl - r1;
			(void) mg_snprintf(conn, range, sizeof(range),
				"Content-Range: bytes "
				"%" INT64_FMT "-%"
				INT64_FMT "/%" INT64_FMT "\r\n",
				r1, r1 + cl - 1, stp->size);
		}
#endif

		// Prepare Etag, Date, Last-Modified headers. Must be in UTC, according to
		// http://www.w3.org/Protocols/rfc2616/rfc2616-sec3.html#sec3.3
		mg_gmt_time_string(date, sizeof(date), &curtime);
		mg_gmt_time_string(lm, sizeof(lm), &st.mtime);
		(void) mg_snprintf(conn, etag, sizeof(etag), "%lx.%lx", (unsigned long) st.mtime, (unsigned long) st.size);

		(void) mg_printf(conn,
			"HTTP/1.1 %d %s\r\n"
			"Date: %s\r\n"
			"Last-Modified: %s\r\n"
			"Etag: \"%s\"\r\n"
			"Content-Type: text/html\r\n"
			"Content-Length: %" INT64_FMT "\r\n"
			"Connection: %s\r\n"
			// "Accept-Ranges: bytes\r\n"
			// "%s\r\n"
			, ri->status_code, mg_get_response_code_text(ri->status_code)
			, date, lm, etag
			, cl
			, mg_suggest_connection_header(conn)
			// , range
			);

		ret = (int)cl;
		if (strcmp(ri->request_method, "HEAD") != 0) {
			ret = mg_send_data(conn, ob->data, (size_t)cl);
		}

		/* cleanup */
		sd_bufrelease(ib);
		sd_bufrelease(ob);

		return ret;
	}
	else
	{
		mg_send_http_error(conn, 404, NULL, "File not found: [%s]", ri->uri);
		return -1;
	}
}


void *event_handler(enum mg_event event_id, struct mg_connection *conn)
{
    void *processed = "yes";
    struct mg_context *ctx = mg_get_context(conn);
    app_manager *mgr = (app_manager *)mg_get_user_data(ctx)->user_data;
	const struct mg_request_info *ri = mg_get_request_info(conn);

    switch (event_id)
    {
    case MG_NEW_REQUEST:
		// process gists a la bl.ocks.org, with a few twists:

		// process Markdown files
		if (mg_match_prefix("**.md$", 6, ri->phys_path) > 0)
		{
			serve_a_markdown_page(conn);
			break;
		}
        if (strncmp(ri->uri, "/tws/", 5) == 0)
        {
			assert(mgr->get_requester(conn));
			assert(mgr->get_requester(ctx, app_manager::IB_TWS_API_CONNECTION_THREAD));
			ib_msg_req_current_time *tws_req = new ib_msg_req_current_time(mgr->get_requester(conn), mgr->get_requester(ctx, app_manager::IB_TWS_API_CONNECTION_THREAD));
            struct timespec poll_time;
			int err;
            poll_time.tv_sec = mgr->get_tws_ib_connection_config().backend_poll_period / 1000;
            poll_time.tv_nsec = (mgr->get_tws_ib_connection_config().backend_poll_period % 1000) * 1000000;

            // pass the request to the backend; block & wait for the response...
			tws_req->state(tier2_message::EXEC_COMMAND);
			err = tws_req->wait_for_response();

#if 0
			mg_printf(conn, "<h1>TWS says the time is: %s</h1>\n", ctime(&tws_req->current_time));
#endif
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

        // kickstart the TWS backend thread now:
        if (mg_start_thread(ctx, (mg_thread_func_t) tws_worker_thread, ctx) != 0) {
            mg_cry4ctx(ctx, "Cannot start TWS connection thread: %d", mg_strerror(mg_get_lasterror()));
            processed = NULL;
        }
        break;

	case MG_INIT_CLIENT_CONN:  // Mongoose has opened a connection to a client.
		mgr->register_frontend_thread(conn);
		processed = NULL;
		break;

	case MG_EXIT_CLIENT_CONN:  // Mongoose is going to close the client connection.
		mgr->unregister_frontend_thread(conn);
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



