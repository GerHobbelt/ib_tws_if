
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

#ifndef APPLICATION_MANAGER_HEADER_INCLUDED
#define APPLICATION_MANAGER_HEADER_INCLUDED

#include "tws_instance.h"

class tier2_message;
class ib_instance;
class db_manager;




class app_manager
{
protected:
	ib_instance *ib_tws;
	db_manager *dbi;

public:
	app_manager()
	{
	}
	virtual ~app_manager()
	{
	}

public:
	int register_frontend_thread(struct mg_connection *conn);
	int unregister_frontend_thread(struct mg_connection *conn);

	int register_backend_thread(struct mg_context *ctx, int optional_id = 0);
	int unregister_backend_thread(struct mg_context *ctx, int optional_id = 0);

	tier2_message_requester *get_requester(struct mg_context *ctx, int optional_id = 0);
	tier2_message_requester *get_requester(struct mg_connection *conn);

	tier2_message_requester *get_ib_tws_manager(void);

	// helper function: produce the IB/TWS app connection. (Used by the TWS backend communication thread / TWS API callbacks)
	struct mg_connection *get_tws_ib_connection(void);
	struct mg_context *get_tws_ib_context(void);
	struct tws_conn_cfg &get_tws_ib_connection_config(void);
    const struct database_cfg &get_db_config(void);
	void fd_set_4_interthread_messaging(tier2_message_requester *requester, fd_set *read_set, fd_set *exception_set, int *max_fd);

	void set_tws_ib_connection(struct mg_connection *conn);

	db_manager *get_db_manager(void);

	void process_response_message(tier2_message *response);
};


#endif // APPLICATION_MANAGER_HEADER_INCLUDED
