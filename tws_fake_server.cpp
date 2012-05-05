
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


static int respond_with(struct mg_connection *conn, const char *elem)
{
	int len = strlen(elem);
	int rv = mg_write(conn, elem, len + 1);
	return rv;
}
static int respond_with(struct mg_connection *conn, int elem)
{
	char buf[40];
	mg_snprintf(conn, buf, sizeof(buf), "%d", elem);
	int len = strlen(buf);
	int rv = mg_write(conn, buf, len + 1);
	return rv;
}
static int respond_with(struct mg_connection *conn, double elem)
{
	char buf[40];
	mg_snprintf(conn, buf, sizeof(buf), "%g", elem);
	int len = strlen(buf);
	int rv = mg_write(conn, buf, len + 1);
	return rv;
}
static int respond_with(struct mg_connection *conn, time_t elem)
{
	char buf[60];
	strncpy(buf, asctime(gmtime(&elem)), sizeof(buf));
	buf[59] = 0;
	int len = strlen(buf);
	int rv = mg_write(conn, buf, len + 1);
	return rv;
}

static int respond_with_file(struct mg_connection *conn, ib_tws_manager *ibm, const char *uri)
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
				rv += mg_write(conn, buf, len);
			}
			else
			{
				break;
			}
		}
		mg_fclose(in);
	}
	buf[0] = 0;
	rv += mg_write(conn, buf, 1);
	return rv;
}



/*
mode:

0: flush on transmit to server
1: wait for async server response(s) in waiting loop
*/
void ib_tws_manager::fake_ib_tws_server(int mode)
{
	if (!fake_ib_tws_connection)
		return;

	/*
	A message may be pending at conn[1]; when it does, we play back a suitable response at conn[1]
	*/
	mg_connection *conn = this->fake_conn[1];

    if (conn)
    {
		if (mode == 0)
		{
			// send 'magic' to uniquely mark the end of the message (this simplifies the fake server code)
			mg_write(this->fake_conn[0], EOM_magick, sizeof(EOM_magick));
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
							in_server_negotiation = false;
						}
						else
						{
							in_server_negotiation = false;
							switch (mcode)
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

                /*
                Meanwhile, we can process queued requests, such as from the front-end now:
                */
            }
        }
    }
}

