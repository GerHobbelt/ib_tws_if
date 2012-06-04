
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

#ifndef MONGOOSE_UTILS_HEADER_INCLUDED
#define MONGOOSE_UTILS_HEADER_INCLUDED

#include "mongoose_headers.h"




/*
===============================================================================

custom handlers (callbacks) for extending the mongoose command line / config file 
configurable parameter processing so we can detect and process our own parameters 
through the regular mongoose approach:

===============================================================================
*/
extern "C" int option_decode(struct mg_context *ctx, const char *name, const char *value);
extern "C" int option_fill(struct mg_context *ctx);
extern "C" const char * option_get(struct mg_context *ctx, struct mg_connection *conn, const char *name);



#endif // MONGOOSE_UTILS_HEADER_INCLUDED
