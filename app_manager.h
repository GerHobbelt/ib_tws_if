
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
class ib_tws_manager;
class db_manager;
class sender_receiver_store;
class sender_receiver_links;



class app_manager
{
protected:
	ib_tws_manager *ib_tws;
	db_manager *dbi;

	sender_receiver_store *sr_store;
	sender_receiver_links *sr_links;

public:
	enum optional_requester_id_t
	{
		UNDEFINED = 0,
		IB_TWS_API_CONNECTION_THREAD,
	};

public:
	app_manager();
	virtual ~app_manager();

public:
	int register_frontend_thread(struct mg_connection *conn, tier2_message_processor *processor = NULL);
	int unregister_frontend_thread(struct mg_connection *conn);

	int register_backend_thread(struct mg_context *ctx, optional_requester_id_t optional_id = UNDEFINED, tier2_message_processor *processor = NULL);
	int unregister_backend_thread(struct mg_context *ctx, optional_requester_id_t optional_id = UNDEFINED);
	int register_backend_thread(struct mg_connection *conn, tier2_message_processor *processor = NULL);
	int unregister_backend_thread(struct mg_connection *conn);

	int register_communication_path(tier2_message_processor *requester, tier2_message_processor *receiver);

	tier2_message_processor *get_requester(struct mg_context *ctx, optional_requester_id_t optional_id = UNDEFINED);
	tier2_message_processor *get_requester(struct mg_connection *conn);

	tier2_message_processor *get_receiver(struct mg_context *ctx, optional_requester_id_t optional_id = UNDEFINED);
	tier2_message_processor *get_receiver(struct mg_connection *conn);

	struct tws_conn_cfg &get_tws_ib_connection_config(void);
    const struct database_cfg &get_db_config(void);

	db_manager *get_db_manager(void);
	ib_tws_manager *get_ib_tws_manager(void);
};



void tws_worker_thread(struct mg_context *ctx);


#endif // APPLICATION_MANAGER_HEADER_INCLUDED
