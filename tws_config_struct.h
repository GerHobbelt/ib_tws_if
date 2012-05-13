
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

#ifndef TWS_CONFIGURATION_STRUCT_H
#define TWS_CONFIGURATION_STRUCT_H



struct tws_conn_cfg
{
    /* configuration parameters: how to connect to TWS */
    const char *ip_address;
    int port;
    int our_id_code;

    long backend_poll_period; // unit: milliseconds
	long backend_reconnect_delay; // unit: milliseconds

    bool tws_log_traffic;
	const char *tws_traffic_log_file;

public:
	tws_conn_cfg() :
		ip_address(0), port(0), our_id_code(0),
		backend_poll_period(0), backend_reconnect_delay(0)
	    , tws_log_traffic(false), tws_traffic_log_file(0)
	{
	}
	~tws_conn_cfg()
	{
	}
};




#endif // TWS_CONFIGURATION_STRUCT_H
