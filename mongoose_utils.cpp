
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

#include "system-includes.h"

#include "mongoose_utils.h"
#include "tws_backend.h"
#include "tws_instance.h"
#include "tws_database_io.h"
#include "app_manager.h"




const char *mg_get_option_ex(struct mg_context *ctx, const char *name, const char *default_value)
{
    const char *v = mg_get_option(ctx, name);

    if (!v)
        return default_value;
    return v;
}

int mg_get_option_int(struct mg_context *ctx, const char *name, int default_value)
{
    const char *v = mg_get_option(ctx, name);

    if (!v)
        return default_value;
    return atol(v);
}






// --- user callback handlers for mongoose --

int option_decode(struct mg_context *ctx, const char *name, const char *value)
{
    app_manager *mgr = (app_manager *)mg_get_user_data(ctx)->user_data;
    struct tws_conn_cfg &tws_cfg = mgr->get_tws_ib_connection_config();

    if (0 == strcmp("tws_ip_address", name))
    {
        tws_cfg.m_ip_address = mg_strdup(value);
        return 1;
    }
    else if (0 == strcmp("tws_ip_port", name))
    {
        int portno = atoi(value);
        if (portno > 0)
        {
            tws_cfg.m_port = portno;
            return 1;
        }
    }
    else if (0 == strcmp("tws_connect_id", name))
    {
        int tws_id = atoi(value);
        if (tws_id > 0)
        {
            tws_cfg.m_our_id_code = tws_id;
            return 1;
        }
    }
    else if (0 == strcmp("tws_poll_delay", name))
    {
        long poll_period = atol(value);
        if (poll_period > 0)
        {
            tws_cfg.m_backend_poll_period = poll_period;
            return 1;
        }
    }
    else if (0 == strcmp("tws_reconnect_delay", name))
    {
        long reconnect_delay = atol(value);
        if (reconnect_delay > 0)
        {
            tws_cfg.m_backend_reconnect_delay = reconnect_delay;
            return 1;
        }
    }
    else if (0 == strcmp("tws_log_traffic", name))
    {
        bool tws_log_traffic = (atoi(value) != 0);
        if (!mg_strncasecmp(value, "t", 1) || !mg_strncasecmp(value, "f", 1))
        {
            tws_log_traffic = !mg_strncasecmp(value, "t", 1);
        }
        tws_cfg.m_tws_log_traffic = tws_log_traffic;
        return 1;
    }
    else if (0 == strcmp("tws_traffic_log_file", name))
    {
        tws_cfg.m_tws_traffic_log_file = mg_strdup(value);
        return 1;
    }
    else if (0 == strcmp("database_file", name))
    {
        mgr->get_db_manager()->set_database_path(value);
        return 1;
    }
	else if (0 == strcmp("gists_document_root", name))
	{
		tws_cfg.m_gists_document_root = mg_strdup(value);
		return 1;
	}
    return 0;
}


int option_fill(struct mg_context *ctx)
{
    // don't do anything...
    return 1;
}

const char * option_get(struct mg_context *ctx, struct mg_connection *conn, const char *name)
{
	// when something for a GIST is requested, we assume a different document root:
	const struct mg_request_info *ri = mg_get_request_info(conn);
	app_manager *mgr = (app_manager *)mg_get_user_data(ctx)->user_data;
	struct tws_conn_cfg &tws_cfg = mgr->get_tws_ib_connection_config();

	if (ri && ri->uri && !strncmp("/gist-", ri->uri, 6) && !strcmp(name, "document_root"))
	{
		return tws_cfg.m_gists_document_root;
	}
	return NULL;
}






