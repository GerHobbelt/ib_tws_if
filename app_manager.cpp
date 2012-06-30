
#include <tws_c_api/twsapi.h>


#include "app_manager.h"
#include "tws_database_io.h"
#include "tws_instance.h"
#include "interthread_comm.h"
#include "data_tracker.h"

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
        if (n1.ctx == n2.ctx)
            return n1.optional_id < n2.optional_id;
        return n1.ctx < n2.ctx;
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




#if 0
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
#endif






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
    // create m_dbi when it's not alive yet
    if (!m_dbi)
    {
        m_dbi = new db_manager(this);
    }
    assert(m_dbi);
    return m_dbi;
}

ib_tws_manager *app_manager::get_ib_tws_manager(void)
{
    return ib_tws_manager::get_instance(this, true);
}

data_tracker_manager *app_manager::get_data_tracker_manager(void)
{
    return data_tracker_manager::get_instance(this, true);
}

calculus_manager *app_manager::get_calculus_manager(void)
{
    //return data_tracker_manager::get_instance(this, true);
    return NULL;
}


app_manager::app_manager() :
    m_dbi(NULL),
    m_new_communicators_count(0)
{
    pthread_mutex_init(&m_comm_mutex, 0);

    m_transmitter_store = new sender_receiver_store();
}

app_manager::~app_manager()
{
    delete m_transmitter_store;

    pthread_mutex_destroy(&m_comm_mutex);
}



int app_manager::register_frontend_thread(struct mg_connection *conn, tier2_message_processor *processor)
{
    sendrecvr_connkey k = { conn };

    pthread_mutex_lock(&m_comm_mutex);

    tier2_message_processor *rv = m_transmitter_store->find(k);
    if (!rv)
    {
        if (processor)
            rv = processor;
        else
            rv = new tier2_message_processor(new requester_id(NULL, conn, 0), this);
        m_transmitter_store->store(k, rv);
    }

    pthread_mutex_unlock(&m_comm_mutex);

    return 0;
}
int app_manager::unregister_frontend_thread(struct mg_connection *conn)
{
    sendrecvr_connkey k = { conn };

    pthread_mutex_lock(&m_comm_mutex);

    m_transmitter_store->erase(k);

    pthread_mutex_unlock(&m_comm_mutex);

    return 0;
}

int app_manager::register_backend_thread(struct mg_connection *conn, tier2_message_processor *processor)
{
    sendrecvr_connkey k = { conn };

    pthread_mutex_lock(&m_comm_mutex);

    tier2_message_processor *rv = m_transmitter_store->find(k);
    if (!rv)
    {
        if (processor)
            rv = processor;
        else
            rv = new tier2_message_processor(new requester_id(NULL, conn, 0), this);
        m_transmitter_store->store(k, rv);
    }

    pthread_mutex_unlock(&m_comm_mutex);

    return 0;
}
int app_manager::unregister_backend_thread(struct mg_connection *conn)
{
    sendrecvr_connkey k = { conn };

    pthread_mutex_lock(&m_comm_mutex);

    m_transmitter_store->erase(k);

    pthread_mutex_unlock(&m_comm_mutex);

    return 0;
}

int app_manager::register_backend_thread(struct mg_context *ctx, app_manager::optional_requester_id_t optional_id, tier2_message_processor *processor)
{
    sendrecvr_ctxkey k = { ctx, optional_id };

    pthread_mutex_lock(&m_comm_mutex);

    tier2_message_processor *rv = m_transmitter_store->find(k);
    if (!rv)
    {
        if (processor)
            rv = processor;
        else
            rv = new tier2_message_processor(new requester_id(NULL, NULL, 0), this);
        m_transmitter_store->store(k, rv);
    }

    pthread_mutex_unlock(&m_comm_mutex);

    return 0;
}
int app_manager::unregister_backend_thread(struct mg_context *ctx, app_manager::optional_requester_id_t optional_id)
{
    sendrecvr_ctxkey k = { ctx, optional_id };

    pthread_mutex_lock(&m_comm_mutex);

    m_transmitter_store->erase(k);

    pthread_mutex_unlock(&m_comm_mutex);

    return 0;
}

interthread_communicator *app_manager::create_communication_path(tier2_message_processor *requester, tier2_message_processor *receiver)
{
    ib_tws_manager *ibm = get_ib_tws_manager();
    mg_connection *conns[2];
    interthread_communicator *comm[2];
    interthread_communicator *rv = NULL;
    
    if (!mg_socketpair(conns, ibm->get_context()))
    {
        comm[0] = new interthread_communicator(requester, receiver, conns);
        mg_connection *tmp = conns[1];
        conns[1] = conns[0];
        conns[0] = tmp;
        comm[1] = new interthread_communicator(receiver, requester, conns);
        comm[0]->set_reverse(comm[1]);
        comm[1]->set_reverse(comm[0]);

        /*
        we've been doing a bit of optimistic locking there, so make sure,
        inside the critial section, that nobody overtook us and created
        the same interconnect while we weren't watching...
        */
        pthread_mutex_lock(&m_comm_mutex);

        for (int i = 0; i < (int)m_communicators.size(); i++)
        {
            interthread_communicator *c = m_communicators[i];

            if (c->matches(requester, receiver))
            {
                // oops; someone else was a wee bit earlier than us; ride with them
                rv = c;
                break;
            }
        }

        if (!rv)
        {
            m_new_communicators_count++;
            m_communicators.push_back(comm[0]);
            m_communicators.push_back(comm[1]);
            rv = comm[0];
        }

        pthread_mutex_unlock(&m_comm_mutex);

        if (rv != comm[0])
        {
            delete comm[0];
            delete comm[1];
        }

        return rv;
    }
    assert(!"Should not get here!");
    return NULL;
}

interthread_communicator *app_manager::get_interthread_communicator(tier2_message_processor *from, tier2_message_processor *to)
{
    interthread_communicator *rv = NULL;

    pthread_mutex_lock(&m_comm_mutex);

    for (int i = 0; i < (int)m_communicators.size(); i++)
    {
        interthread_communicator *comm = m_communicators[i];

        if (comm->matches(from, to))
        {
            rv = comm;
            break;
        }
    }

    pthread_mutex_unlock(&m_comm_mutex);

    if (!rv)
    {
        rv = create_communication_path(from, to);
    }

    return rv;
}

// may only be invoked by 'receiver':
int app_manager::fetch_new_interthread_communicators(tier2_message_processor *receiver)
{
    pthread_mutex_lock(&m_comm_mutex);

    /*
    We know that for each new communicator added, two entries are pushed at the end of the array,
    but we don't know in which order those entries are 'resolved' by the various invocations
    of this method.

    Nevertheless, we're probably done the fastest when we scan from the end towards to the
    start of the list.
    */
    if (m_new_communicators_count)
    {
        assert(m_new_communicators_count > 0);

        for (int i = m_communicators.size(); i-- > 0 && m_new_communicators_count > 0; )
        {
            interthread_communicator *comm = m_communicators[i];

            if (comm->has_receiver(receiver))
            {
                // see if this one is already in our own list:
                for (int j = receiver->m_senders.size(); j-- > 0; )
                {
                    interthread_communicator *cachedcomm = receiver->m_senders[j];

                    if (cachedcomm == comm)
                    {
                        comm = NULL;
                        break;
                    }
                }
                if (comm)
                {
                    // truely a new entry:
                    receiver->register_interthread_connection(comm);
                    // and as receivers can only be one per interconnect, we can count down the number of pending interconnects by one too:
                    m_new_communicators_count--;
                }
            }
        }
    }

    pthread_mutex_unlock(&m_comm_mutex);

    return 0;
}

tier2_message_processor *app_manager::get_requester(struct mg_context *ctx, app_manager::optional_requester_id_t id)
{
    return get_receiver(ctx, id);
}

tier2_message_processor *app_manager::get_requester(struct mg_connection *conn)
{
    return get_receiver(conn);
}

tier2_message_processor *app_manager::get_receiver(struct mg_context *ctx, app_manager::optional_requester_id_t id)
{
    sendrecvr_ctxkey k = { ctx, id };

    pthread_mutex_lock(&m_comm_mutex);

    tier2_message_processor *rv = m_transmitter_store->find(k);

    pthread_mutex_unlock(&m_comm_mutex);

    return rv;
}

tier2_message_processor *app_manager::get_receiver(struct mg_connection *conn)
{
    sendrecvr_connkey k = { conn };

    pthread_mutex_lock(&m_comm_mutex);

    tier2_message_processor *rv = m_transmitter_store->find(k);

    pthread_mutex_unlock(&m_comm_mutex);

    return rv;
}


int app_manager::set_next_order_id(int id)
{
    // only allow the ID to be bumped UPWARDS
    return m_next_order_id.update_unique_id(id);
}
int app_manager::get_next_order_id(void)
{
    return m_next_order_id.obtain_unique_id();
}

int app_manager::get_next_ticker_id(void)
{
    return m_next_order_id.obtain_unique_id();
}

