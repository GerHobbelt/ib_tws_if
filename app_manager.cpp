
#include <tws_c_api/twsapi.h>


#include "app_manager.h"
#include "tws_database_io.h"
#include "tws_instance.h"
#include "interthread_comm.h"

#include <mongoose/mongoose_ex.h>





struct sendrecvr_ctxkey
{
	mg_context *ctx;
	app_manager::optional_requester_id_t optional_id;
};

class sendrecvr_ctxkey_less: public std::less<sendrecvr_ctxkey>
{
public:
	bool operator()(const sendrecvr_ctxkey &n1, const sendrecvr_ctxkey &n2) const
	{
		return n1.ctx < n2.ctx ? n1.optional_id < n2.optional_id : false;
	}
};

struct sendrecvr_connkey
{
	mg_connection *conn;
};

class sendrecvr_connkey_less: public std::less<sendrecvr_connkey>
{
public:
	bool operator ()(const sendrecvr_connkey &n1, const sendrecvr_connkey &n2) const
	{
		return n1.conn < n2.conn;
	}
};

class sender_receiver_store
{
protected:
	typedef std::map<sendrecvr_ctxkey, int, sendrecvr_ctxkey_less> ctx_index_t;
	typedef std::pair<sendrecvr_ctxkey, int> ctx_index_pair_t;
	ctx_index_t ctx_index;
	typedef std::map<sendrecvr_connkey, int, sendrecvr_connkey_less> conn_index_t;
	typedef std::pair<sendrecvr_connkey, int> conn_index_pair_t;
	conn_index_t conn_index;
	typedef std::vector<tier2_message_processor *> recvr_set_t;
	recvr_set_t receiver_set;

public:
	sender_receiver_store()
	{
	}
	virtual ~sender_receiver_store()
	{
	}

public:
	tier2_message_processor *find(sendrecvr_ctxkey &key)
	{
		ctx_index_t::const_iterator idx = ctx_index.find(key);
		if (idx != ctx_index.end())
		{
			int i = idx->second;
			return receiver_set[i];
		}
		return NULL;
	}

	tier2_message_processor *find(sendrecvr_connkey &key)
	{
		conn_index_t::const_iterator idx = conn_index.find(key);
		if (idx != conn_index.end())
		{
			int i = idx->second;
			return receiver_set[i];
		}
		return NULL;
	}

	int store(sendrecvr_ctxkey &key, tier2_message_processor *value)
	{
		receiver_set.push_back(value);
		int idx = receiver_set.size() - 1;
		ctx_index_pair_t v(key, idx);
		ctx_index.insert(v);
		return 0;
	}

	int store(sendrecvr_connkey &key, tier2_message_processor *value)
	{
		receiver_set.push_back(value);
		int idx = receiver_set.size() - 1;
		conn_index_pair_t v(key, idx);
		conn_index.insert(v);
		return 0;
	}

	int erase(sendrecvr_ctxkey &key)
	{
		ctx_index_t::const_iterator idx = ctx_index.find(key);
		if (idx != ctx_index.end())
		{
			int i = idx->second;
			receiver_set[i] = NULL;
		}
		return 0;
	}

	int erase(sendrecvr_connkey &key)
	{
		conn_index_t::const_iterator idx = conn_index.find(key);
		if (idx != conn_index.end())
		{
			int i = idx->second;
			receiver_set[i] = NULL;
		}
		return 0;
	}
};




struct link_connkey
{
	mg_connection *front;
	mg_connection *back;
};

class link_connkey_less: public std::less<link_connkey>
{
public:
	bool operator ()(const link_connkey &n1, const link_connkey &n2) const
	{
		return n1.front < n2.front ? n1.back < n2.back : false;
	}
};

class sender_receiver_links
{
protected:
	typedef std::map<link_connkey, interthread_communicator *, link_connkey_less> link_map_t;
	typedef std::pair<link_connkey, interthread_communicator *> link_pair_t;
	link_map_t mapping;

public:
	sender_receiver_links()
	{
	}
	virtual ~sender_receiver_links()
	{
	}

public:
	interthread_communicator *find(const link_connkey &key)
	{
		link_map_t::const_iterator idx = mapping.find(key);
		if (idx != mapping.end())
		{
			return idx->second;
		}
		return NULL;
	}

	int store(link_connkey &key, interthread_communicator *value)
	{
		link_map_t::const_iterator idx = mapping.find(key);
		if (idx != mapping.end())
		{
			link_pair_t v(key, value);
			mapping.insert(v);
		}
		return 0;
	}

	int erase(link_connkey &key)
	{
		mapping.erase(key);
		return 0;
	}
};






struct tws_conn_cfg &app_manager::get_tws_ib_connection_config(void)
{
	ib_tws_manager *ibm = get_ib_tws_manager();

	return ibm->get_config();
}

const struct database_cfg &app_manager::get_db_config(void)
{
	db_manager *dbm = get_db_manager();

	return dbm->get_config();
}

db_manager *app_manager::get_db_manager(void)
{
	// create dbi when it's not alive yet
	if (!dbi)
	{
		dbi = new db_manager(this);
	}
	assert(dbi);
	return dbi;
}

class ib_tws_manager *app_manager::get_ib_tws_manager(void)
{
	// create ib_tws when it's not alive yet
	if (!ib_tws)
	{
		ib_tws = ib_tws_manager::get_instance(this, true);
	}
	assert(ib_tws);
	return ib_tws;
}



app_manager::app_manager() :
	ib_tws(NULL),
	dbi(NULL)
{
	sr_store = new sender_receiver_store();
	sr_links = new sender_receiver_links();
}

app_manager::~app_manager()
{
	delete sr_store;
}



int app_manager::register_frontend_thread(struct mg_connection *conn, tier2_message_processor *processor)
{
	sendrecvr_connkey k = { conn };
	tier2_message_processor *rv = sr_store->find(k);
	if (!rv)
	{
		if (processor)
			rv = processor;
		else
			rv = new tier2_message_processor(new requester_id(NULL, conn, 0), this);
		sr_store->store(k, rv);
	}
	return 0;
}
int app_manager::unregister_frontend_thread(struct mg_connection *conn)
{
	sendrecvr_connkey k = { conn };
	sr_store->erase(k);
	return 0;
}

int app_manager::register_backend_thread(struct mg_connection *conn, tier2_message_processor *processor)
{
	sendrecvr_connkey k = { conn };
	tier2_message_processor *rv = sr_store->find(k);
	if (!rv)
	{
		if (processor)
			rv = processor;
		else
			rv = new tier2_message_processor(new requester_id(NULL, conn, 0), this);
		sr_store->store(k, rv);
	}
	return 0;
}
int app_manager::unregister_backend_thread(struct mg_connection *conn)
{
	sendrecvr_connkey k = { conn };
	sr_store->erase(k);
	return 0;
}

int app_manager::register_backend_thread(struct mg_context *ctx, app_manager::optional_requester_id_t optional_id, tier2_message_processor *processor)
{
	sendrecvr_ctxkey k = { ctx, optional_id };
	tier2_message_processor *rv = sr_store->find(k);
	if (!rv)
	{
		if (processor)
			rv = processor;
		else
			rv = new tier2_message_processor(new requester_id(NULL, NULL, 0), this);
		sr_store->store(k, rv);
	}
	return 0;
}
int app_manager::unregister_backend_thread(struct mg_context *ctx, app_manager::optional_requester_id_t optional_id)
{
	sendrecvr_ctxkey k = { ctx, optional_id };
	sr_store->erase(k);
	return 0;
}

int app_manager::register_communication_path(tier2_message_processor *requester, tier2_message_processor *receiver)
{
	return 0;
}

tier2_message_processor *app_manager::get_requester(struct mg_context *ctx, app_manager::optional_requester_id_t optional_id)
{
	return get_receiver(ctx, optional_id);
}

tier2_message_processor *app_manager::get_requester(struct mg_connection *conn)
{
	return get_receiver(conn);
}

tier2_message_processor *app_manager::get_receiver(struct mg_context *ctx, app_manager::optional_requester_id_t optional_id)
{
	sendrecvr_ctxkey k = { ctx, optional_id };
	tier2_message_processor *rv = sr_store->find(k);
	return rv;
}

tier2_message_processor *app_manager::get_receiver(struct mg_connection *conn)
{
	sendrecvr_connkey k = { conn };
	tier2_message_processor *rv = sr_store->find(k);
	return rv;
}

