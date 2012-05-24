
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
    const char *m_ip_address;
    int m_port;
    int m_our_id_code;

    long m_backend_poll_period; // unit: milliseconds
	long m_backend_reconnect_delay; // unit: milliseconds

    bool m_tws_log_traffic;
	const char *m_tws_traffic_log_file;

public:
	tws_conn_cfg() :
		m_ip_address(0), m_port(0), m_our_id_code(0),
		m_backend_poll_period(0), m_backend_reconnect_delay(0)
	    , m_tws_log_traffic(false), m_tws_traffic_log_file(0)
	{
	}
	~tws_conn_cfg()
	{
	}
};




#endif // TWS_CONFIGURATION_STRUCT_H
