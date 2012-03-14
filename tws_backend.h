
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

#ifndef TWS_BACKEND_GENERIC_HEADER_INCLUDED
#define TWS_BACKEND_GENERIC_HEADER_INCLUDED

#include "tws_mongoose_porting.h"

#include <tws_c_api/twsapi.h>



#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))





struct tws_thread_exch;  // forward reference: structure which contains all front-end to back-end thread comminucation stuff


struct tws_conn_cfg
{
    /* configuration parameters: how to connect to TWS */
    const char *ip_address;
    int port;
    int our_id_code;

    long backend_poll_period; // unit: milliseconds

	char *database_path;

    /* internal communication stuff between mongoose threads and the tws back-end thread goes here: */
    struct tws_thread_exch *exch;

	/*
	to be filled in by the backend ('tier2/TWS') thread:
	*/
    struct my_tws_io_info *tws_thread_info;
};



#ifdef __cplusplus


static __inline void tws_copy(char *dst, const char *src)
{
	tws_strcpy(dst, src);
}
static __inline void tws_copy(double &dst, double src)
{
	if (src != DBL_MAX)
		dst = src;
}
static __inline void tws_copy(int &dst, int src)
{
	if (src != INT_MAX)
		dst = src;
}

#endif


#endif // TWS_BACKEND_GENERIC_HEADER_INCLUDED
