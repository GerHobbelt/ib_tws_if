
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

#include <tws_c_api/twsapi.h>

#include "system-includes.h"

#include "tws_instance.h"

#include "app_manager.h"
#include "tier2_message_processor.h"
#include "tws_request.h"
#include "tws_response.h"

#include <mongoose/mongoose_ex.h>


static bool in_server_negotiation = false;
static int client_id = 0;

static char EOM_magick[4] = "\xFA\xC7!";
static char EOT_magick[4] = { '\0', '\xEB', '\xD6', '~' };



static char *strtok0(char *s = NULL)
{
	static char *p = NULL;

	if (s)
	{
		p = s;
	}

	if (p)
	{
		char *rv = p;

		p += strlen(p) + 1;
		if (0 == memcmp(EOT_magick, p, sizeof(EOT_magick)))
		{
			p = NULL;
		}
		return rv;
	}
	return NULL;
}

static int inttok0(char *s = NULL)
{
	const char *v = strtok0(s);

	if (v)
		return atoi(v);
	return 0;
}

static int respond_with(struct mg_connection *conn, const char *elem)
{
	int len = strlen(elem);
	int rv = mg_send_data(conn, elem, len + 1);
	return rv;
}
static int respond_with(struct mg_connection *conn, int elem)
{
	char buf[40];
	mg_snprintf(conn, buf, sizeof(buf), "%d", elem);
	int len = strlen(buf);
	int rv = mg_send_data(conn, buf, len + 1);
	return rv;
}
static int respond_with(struct mg_connection *conn, double elem)
{
	char buf[40];
	mg_snprintf(conn, buf, sizeof(buf), "%g", elem);
	int len = strlen(buf);
	int rv = mg_send_data(conn, buf, len + 1);
	return rv;
}
static int respond_with(struct mg_connection *conn, time_t elem)
{
	char buf[60];
	strftime(buf, sizeof(buf), "%Y%m%d %H:%M:%S %z", gmtime(&elem));
	buf[59] = 0;
	int len = strlen(buf);
	int rv = mg_send_data(conn, buf, len + 1);
	return rv;
}

static int respond_with_file(struct mg_connection *conn, ib_backend_io_channel *ibm, const char *uri)
{
	FILE *in;
	char buf[2048];
	char path[PATH_MAX];
	struct mg_context *ctx = mg_get_context(conn);
	struct mg_request_info *info = mg_get_request_info(conn);
	int rv = 0;

	mg_snprintf(conn, path, sizeof(path), "%s/%s", mg_get_option(ctx, "document_root"), uri);

	in = mg_fopen(path, "rb");
	if (in)
	{
		while (!feof(in))
		{
			int len = fread(buf, 1, sizeof(buf), in);
			if (len > 0)
			{
				rv += mg_send_data(conn, buf, len);
			}
			else
			{
				break;
			}
		}
		mg_fclose(in);
	}
	buf[0] = 0;
	rv += mg_send_data(conn, buf, 1);
	return rv;
}

static int respond_with_messages_file(struct mg_connection *conn, ib_backend_io_channel *ibm, const char *uri, const char *uri_param, ...)
{
	va_list args;
	FILE *in;
	char *buf = NULL;
	char path[PATH_MAX];
	struct mg_context *ctx = mg_get_context(conn);
	struct mg_request_info *info = mg_get_request_info(conn);
	struct mgstat st;
	int rv = 0;

	mg_asprintf(conn, &buf, 0, uri, uri_param);
	mg_snprintf(conn, path, sizeof(path), "%s/%s", mg_get_option(ctx, "document_root"), buf);

	if (mg_stat(path, &st))
		return -1;

	free(buf);
	buf = (char *)malloc(st.size + 1 /* extra space for NUL sentinel */);
	if (!buf)
		return -1;

	in = mg_fopen(path, "rb");
	if (in)
	{
		int len = fread(buf, 1, st.size, in);
		if (len > 0)
		{
			char *procd_buf = NULL;
			char *s;
			char *d;

			buf[len] = 0;

			va_start(args, uri_param);
			mg_vasprintf(conn, &procd_buf, 0, buf, args);
			va_end(args);

			// now we have the message(s) ready for transmission, only we need to replace any CRLF to NUL:
			d = s = procd_buf;
			for (size_t i = strlen(procd_buf); i > 0; i--)
			{
				switch (*s++)
				{
				case '\r':
					if (*s == '\n')
						continue;
					*d++ = '\0';
					continue;

				case '\n':
					*d++ = '\0';
					continue;

				default:
					*d++ = s[-1];
					continue;
				}
			}
			*d = 0;

			rv += mg_send_data(conn, procd_buf, d - procd_buf);
		}
		mg_fclose(in);
	}
	return rv;
}




/*
mode:

0: flush on transmit to server
1: wait for async server response(s) in waiting loop
*/
void ib_backend_io_channel::fake_ib_tws_server(int mode)
{
	if (!fake_ib_tws_connection)
		return;

	if (mode == 2)
	{
		if (!tws_conn)
		{
			// open a new fake server connection
			int sp_rv = mg_socketpair(fake_conn, tws_ctx);

			if (!sp_rv)
			{
				int tcpbuflen = 1 * 1024 * 1024;

				tws_conn = fake_conn[0];
			
				mg_setsockopt(mg_get_client_socket(fake_conn[0]), SOL_SOCKET, SO_RCVBUF, (const void *)&tcpbuflen, sizeof(tcpbuflen));
				mg_setsockopt(mg_get_client_socket(fake_conn[0]), SOL_SOCKET, SO_SNDBUF, (const void *)&tcpbuflen, sizeof(tcpbuflen));
				mg_setsockopt(mg_get_client_socket(fake_conn[1]), SOL_SOCKET, SO_RCVBUF, (const void *)&tcpbuflen, sizeof(tcpbuflen));
				mg_setsockopt(mg_get_client_socket(fake_conn[1]), SOL_SOCKET, SO_SNDBUF, (const void *)&tcpbuflen, sizeof(tcpbuflen));

				faking_the_ib_tws_connection = true;
			}
		}
		return;
	}
	if (mode == 3)
	{
		// close a server connection
		if (fake_conn[1])
		{
			mg_close_connection(fake_conn[1]);
		}
		fake_conn[0] = NULL;
		fake_conn[1] = NULL;
		faking_the_ib_tws_connection = false;
		return;
	}

	if (!faking_the_ib_tws_connection)
		return;

	/*
	A message may be pending at conn[1]; when it does, we play back a suitable response at conn[1]
	*/
	mg_connection *conn = fake_conn[1];
	mg_request_info *request_info = mg_get_request_info(conn);
	int64_t old_num_bytes_sent = mg_get_num_bytes_sent(conn);

    if (conn)
    {
		if (mode == 0)
		{
			// send 'magic' to uniquely mark the end of the message (this simplifies the fake server code)
			mg_send_data(fake_conn[0], EOM_magick, sizeof(EOM_magick));
		}

        // check whether there's anything available:
        fd_set read_set;
        struct timeval tv;
        int max_fd;

        tv.tv_sec = 0;
        tv.tv_usec = 1000;

        for (int loopcount = 3; loopcount > 0 && mg_get_stop_flag(mg_get_context(conn)) == 0; loopcount--)
        {
            struct timeval tv2 = tv;

			/*
			stop sending [so we don't overflow socketpair I/O buffers] when we've sent one message; 
			as we send one complete message in here each time we merely need to check whether we've
			sent any data at all.
			*/
			if (old_num_bytes_sent < mg_get_num_bytes_sent(conn))
				break;

            FD_ZERO(&read_set);
            max_fd = -1;

            // Add listening sockets to the read set
            mg_FD_SET(mg_get_client_socket(conn), &read_set, &max_fd);

            if (select(max_fd + 1, &read_set, NULL, NULL, &tv2) < 0)
            {
				// signal a fatal failure:
                break;
            }
            else
            {
                if (mg_FD_ISSET(mg_get_client_socket(conn), &read_set))
                {
                    /*
                    Mongoose mg_read() does NOT fetch any pending data from the TCP/IP stack when the 'content length' isn't set yet.

                    We, however, desire to load an unknown and arbitrary amount of data here to fill a buffer and our protocol doesn't
                    have something like a 'content length' to guide us along, so we'll have to use another method to make sure
                    the read operation actually delivers DATA!
                    */
					char buf[4096 + 4];
					int len = mg_pull(conn, buf, sizeof(buf) - 4);
					if (len > 0)
					{
						loopcount = 3;

						memcpy(buf + len, EOT_magick, sizeof(EOT_magick));

						char *t = strtok0(buf);
						int mcode = atoi(t);
						t = strtok0();
						int mversion = (t ? atoi(t) : 0);
						bool eom = (!t || 0 == memcmp(EOM_magick, t, sizeof(EOM_magick)));

						if (mcode == TWSCLIENT_VERSION && eom && !in_server_negotiation)
						{
							in_server_negotiation = true;

							/* send back server version and timestamp */
							respond_with(conn, 666);
							respond_with(conn, time(NULL));
						}
						else if (eom && in_server_negotiation)
						{
							client_id = mcode;

							/* 
							act like my European TWS with second test/paper account
							*/
							respond_with(conn, tws::MANAGED_ACCTS);
							respond_with(conn, 1);
							respond_with(conn, "FAKE12345");

							respond_with(conn, tws::NEXT_VALID_ID);
							respond_with(conn, 1);
							respond_with(conn, 1);

							static const struct
							{
								tws::twsclient_error_code_t code;
								char *text;
							} farm_msgs[] = 
							{
								{
									tws::FAIL_MARKET_DATA_FARM_CONNECTED,
									"Market data farm connection is OK:eurofarm",
								},
								{
									tws::FAIL_HISTORICAL_DATA_FARM_CONNECTED,
									"HMDS data farm connection is OK:euhmds",
								},
								{
									tws::FAIL_HISTORICAL_DATA_FARM_CONNECTED,
									"HMDS data farm connection is OK:hkhmds",
								},
								{
									tws::FAIL_HISTORICAL_DATA_FARM_CONNECTED,
									"HMDS data farm connection is OK:ushmds",
								},
							};

							for (int fi = 0; fi < ARRAY_SIZE(farm_msgs); fi++)
							{
								respond_with(conn, tws::ERR_MSG);
								respond_with(conn, 2);
								respond_with(conn, -1);
								respond_with(conn, farm_msgs[fi].code);
								respond_with(conn, farm_msgs[fi].text);
							}

							in_server_negotiation = false;
						}
						else
						{
							ib_outgoing_id_t msgcode = (ib_outgoing_id_t)mcode;

							in_server_negotiation = false;
							switch (msgcode)
							{
							case tws::REQ_MKT_DATA :
								break;

							case tws::CANCEL_MKT_DATA :
								break;

							case tws::PLACE_ORDER :
								break;

							case tws::CANCEL_ORDER :
								break;

							case tws::REQ_OPEN_ORDERS :
								break;

							case tws::REQ_ACCOUNT_DATA :
								break;

							case tws::REQ_EXECUTIONS :
								break;

							case tws::REQ_IDS :
								break;

							case tws::REQ_CONTRACT_DATA :
								break;

							case tws::REQ_MKT_DEPTH :
								break;

							case tws::CANCEL_MKT_DEPTH :
								break;

							case tws::REQ_NEWS_BULLETINS :
								break;

							case tws::CANCEL_NEWS_BULLETINS :
								break;

							case tws::SET_SERVER_LOGLEVEL :
								break;

							case tws::REQ_AUTO_OPEN_ORDERS :
								break;

							case tws::REQ_ALL_OPEN_ORDERS :
								break;

							case tws::REQ_MANAGED_ACCTS :
								break;

							case tws::REQ_FA :
								break;

							case tws::REPLACE_FA :
								break;

							case tws::REQ_HISTORICAL_DATA :
								break;

							case tws::EXERCISE_OPTIONS :
								break;

							case tws::REQ_SCANNER_SUBSCRIPTION :
								t = strtok0();
								if (t)
								{
									int ticker_id = atoi(t);
									int maxrows = inttok0();
									const char *instrument = strtok0();
									const char *scanloc = strtok0();
									const char *scancode = strtok0();

									if (mg_strcasecmp("HIGH_DIVIDEND_YIELD_IB", scancode))
									{
										// reject subscription:
										respond_with(conn, tws::ERR_MSG);
										respond_with(conn, 2);
										respond_with(conn, ticker_id);
										respond_with(conn, tws::FAIL_HISTORICAL_MARKET_DATA_SERVICE);
										respond_with(conn, "Historical Market Data Service error message:duplicate scan subscription");
									}
									else
									{
										// accept subscription: send 2 messages based on file templte (quicker to code here ;-) )
										respond_with_messages_file(conn, this, "faking_it/RX.scanner_data.%s.txt", scancode, ticker_id, ticker_id);
									}
								}
								break;

							case tws::CANCEL_SCANNER_SUBSCRIPTION :
								break;

							case tws::REQ_SCANNER_PARAMETERS :
								respond_with(conn, tws::SCANNER_PARAMETERS);
								respond_with(conn, 1);
								respond_with_file(conn, this, "faking_it/scanner-parameters.xml");
								break;

							case tws::CANCEL_HISTORICAL_DATA :
								break;

							case tws::REQ_CURRENT_TIME :
								break;

							case tws::REQ_REAL_TIME_BARS :
								break;

							case tws::CANCEL_REAL_TIME_BARS :
								break;

							case tws::REQ_FUNDAMENTAL_DATA :
								break;

							case tws::CANCEL_FUNDAMENTAL_DATA :
								break;

							case tws::REQ_CALC_IMPLIED_VOLAT :
								break;

							case tws::REQ_CALC_OPTION_PRICE :
								break;

							case tws::CANCEL_CALC_IMPLIED_VOLAT :
								break;

							case tws::CANCEL_CALC_OPTION_PRICE :
								break;

							case tws::REQ_GLOBAL_CANCEL :
								break;

							case tws::REQ_MARKET_DATA_TYPE :
								break;

							default:
								assert(0);
								break;
							}

							// slurp until eom:
							while (!(eom = (!t || 0 == memcmp(EOM_magick, t, sizeof(EOM_magick)))))
							{
								t = strtok0();
							}
						}
					}
                }
				else
				{
					/*
					done faking for now; wait for more request data...

					we MAY fake async responses here based on pending subscriptions...
					*/
					break;
				}
            }
        }
    }
}
