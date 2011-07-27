
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
 * TODO / roadmap:
 *
 * - prioritizing outgoing requests, e.g. ORDERs have priority over historical data requests, and requests for recent historical data have priority over requests for older data.
 *
 * - use a priority queue for the above plus a 'idle time' delay to prevent hammering the TWS machines with historical data requests: only fire those when the interface has been 'quiet' for X seconds
 *
 * - store/cache historical data; use 'smart' code to request consecutive and _large_ chunks of historical data to be cached: one request, served many times (from local cache)
 *
 * - async TWS TX/RX: push requests asap, using a 'telnet' TCP setting (you don't want orders to wait for a TCP buffer fill timeout!): single thread/connection connected to TWS,
 *   all requests are posted in a 'response queue' (so we know which responses are for whom) upon transmission --> true full duplex communication instead of the standard TWS sample
 *   which uses the TCP connection as a half-duplex connect (as it waits for the response to the request before firing another).
 */


#include "mongoose_utils.h"

#include "tws_backend.h"




const char *mg_get_option_ex(const struct mg_context *ctx, const char *name, const char *default_value)
{
    const char *v = mg_get_option(ctx, name);

    if (!v)
        return default_value;
    return v;
}

int mg_get_option_int(const struct mg_context *ctx, const char *name, int default_value)
{
    const char *v = mg_get_option(ctx, name);

    if (!v)
        return default_value;
    return atol(v);
}






// --- user callback handlers for mongoose --

int option_decode(struct mg_context *ctx, const char *name, const char *value)
{
    struct tws_conn_cfg *tws_cfg = (struct tws_conn_cfg *)mg_get_user_data(ctx)->user_data;

    if (0 == strcmp("tws_ip_address", name))
    {
        tws_cfg->ip_address = mg_strdup(value);
        return 1;
    }
    else if (0 == strcmp("tws_ip_port", name))
    {
        int portno = atoi(value);
        if (portno > 0)
        {
            tws_cfg->port = portno;
            return 1;
        }
    }
    else if (0 == strcmp("tws_connect_id", name))
    {
        int tws_id = atoi(value);
        if (tws_id > 0)
        {
            tws_cfg->our_id_code = tws_id;
            return 1;
        }
    }
    else if (0 == strcmp("tws_poll_delay", name))
    {
        long poll_period = atol(value);
        if (poll_period > 0)
        {
            tws_cfg->backend_poll_period = poll_period;
            return 1;
        }
    }
    return 0;
}


int option_fill(struct mg_context *ctx)
{
    // don't do anything...
    return 1;
}

const char * option_get(const struct mg_context *ctx, const char *name)
{
    // we don't use this one, so keep it a dummy until we do...
    return NULL;
}



