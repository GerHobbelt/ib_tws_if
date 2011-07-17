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

#ifndef TWS_COMM_THREAD_HEADER_INCLUDED
#define TWS_COMM_THREAD_HEADER_INCLUDED

#include <stddef.h>

#include "mongoose/mongoose.h"


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	
struct tws_thread_exch;  // forward reference: structure which contains all front-end to back-end thread comminucation stuff


struct tws_conn_cfg
{
  /* configuration parameters: how to connect to TWS */
	const char *ip_address;
	int port;
	int our_id_code;

  long backend_poll_period; // unit: milliseconds

  /* internal communication stuff between mongoose threads and the tws back-end thread goes here: */
  struct tws_thread_exch *exch;
};


void init_tws_thread_exch(struct tws_thread_exch **ptr);
void destroy_tws_thread_exch(struct tws_thread_exch **ptr);

void *event_handler(enum mg_event event_id, struct mg_connection *conn, const struct mg_request_info *request_info);

int option_decode(struct mg_context *ctx, const char *name, const char *value);
int option_fill(struct mg_context *ctx);
const char * option_get(const struct mg_context *ctx, const char *name);



void die(const char *fmt, ...);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // TWS_COMM_THREAD_HEADER_INCLUDED
