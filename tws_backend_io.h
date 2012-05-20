
#ifndef IB_BACKEND_IO_H
#define IB_BACKEND_IO_H

#include "system-includes.h"
#include "tws_config_struct.h"
#include "tier2_message_processor.h"


class ib_backend_io_channel;
namespace tws
{
	enum tws_outgoing_ids;
	typedef struct tws_instance tws_instance_t;
}
typedef tws::tws_outgoing_ids ib_outgoing_id_t;
struct mg_connection;
struct mg_context;




/*
For all observer methods this rule applies:

If you want to abort executing the remaining observers and the invoking function itself,
return a negative value.

If you want to abort executing the current chain (before/after) chain of observers,
return a non-negative positive value.

Otherwise, return 0;


For all observers this applies as well: you may choose to modify the 'return_value'
parameter; it will have been initialized to 0(zero) at the onset and it will be updated
by the invoking function when applicable.

When you modify the 'return_value' value in any 'before' observer, the invoking function
will skip the internal process. One may use this feature to replace the internal operation
of the invoker by any functionality in the observer.

Generally, this parameter represents the (semi)final return value of the invoking function.
*/
class ib_backend_io_observer
{
public:
	ib_backend_io_observer();
	virtual ~ib_backend_io_observer();

public:
	virtual int io_transmit(int &return_value, ib_backend_io_channel *originator, const void *buf, unsigned int buflen) = 0;
	virtual int io_receive(int &return_value, ib_backend_io_channel *originator, void *buf, unsigned int max_bufsize) = 0;
	/* 'flush()' marks the end of the outgoing message: it should be transmitted ASAP */
	virtual int io_flush(int &return_value, ib_backend_io_channel *originator) = 0;
	/* open callback is invoked when tws_connect is invoked and no connection has been established yet (tws_connected() == false); return 0 on success; a twsclient_error_codes error code on failure. */
	virtual int io_open(int &return_value, ib_backend_io_channel *originator) = 0;
	/* close callback is invoked on error or when tws_disconnect is invoked */
	virtual int io_close(int &return_value, ib_backend_io_channel *originator) = 0;

	virtual int io_tx_elem_observe(ib_backend_io_channel *originator, const char *elem, unsigned int elem_size, ib_outgoing_id_t start_of_message) = 0;
	virtual int io_rx_elem_observe(ib_backend_io_channel *originator, const char *elem, unsigned int elem_size, int return_value) = 0;

	virtual int destroy(ib_backend_io_channel *originator) = 0;
};



class ib_backend_io_channel : public tier2_message_processor
{
protected:
	typedef std::vector<ib_backend_io_observer *> io_observer_list_t;

	io_observer_list_t m_before;
	io_observer_list_t m_after;

	bool fake_ib_tws_connection;
	bool faking_the_ib_tws_connection;
	struct mg_connection *fake_conn[2];

	virtual void fake_ib_tws_server(int mode);

public:
	bool is_faking_the_ib_tws_connection(void) const
	{
		return faking_the_ib_tws_connection && tws_conn;
	}

protected:
	struct tws_conn_cfg tws_cfg;
    struct mg_connection *tws_conn;
    struct mg_context *tws_ctx;
    tws::tws_instance_t *tws_handle;

protected:
	ib_backend_io_channel(app_manager *mgr);
public:
	virtual ~ib_backend_io_channel();
	static ib_backend_io_channel *get_instance(app_manager *mgr, bool instantiate_singleton = false);
protected:
	static void set_instance(ib_backend_io_channel *instance);

public:
	static int tws_transmit_func(void *arg, const void *buf, unsigned int buflen);
	static int tws_receive_func(void *arg, void *buf, unsigned int max_bufsize);
	/* 'flush()' marks the end of the outgoing message: it should be transmitted ASAP */
	static int tws_flush_func(void *arg);
	/* open callback is invoked when tws_connect is invoked and no connection has been established yet (tws_connected() == false); return 0 on success; a twsclient_error_codes error code on failure. */
	static int tws_open_func(void *arg);
	/* close callback is invoked on error or when tws_disconnect is invoked */
	static int tws_close_func(void *arg);

	static int tws_tx_elem_observe_func(void *arg, const char *elem, unsigned int elem_size, ib_outgoing_id_t start_of_message);
	static int tws_rx_elem_observe_func(void *arg, const char *elem, unsigned int elem_size, int return_value);

public:
	virtual int push_before(ib_backend_io_observer *listener);
	virtual int push_after(ib_backend_io_observer *listener);
	virtual int pop_before(ib_backend_io_observer *listener);
	virtual int pop_after(ib_backend_io_observer *listener);
	virtual int clean_before(void);
	virtual int clean_after(void);

protected:
	virtual int io_transmit(const void *buf, unsigned int buflen);
	virtual int io_receive(void *buf, unsigned int max_bufsize);
	/* 'flush()' marks the end of the outgoing message: it should be transmitted ASAP */
	virtual int io_flush(void);
	/* open callback is invoked when tws_connect is invoked and no connection has been established yet (tws_connected() == false); return 0 on success; a twsclient_error_codes error code on failure. */
	virtual int io_open(void);
	/* close callback is invoked on error or when tws_disconnect is invoked */
	virtual int io_close(void);

	virtual int destroy(void);

public:
	struct tws_conn_cfg &get_config(void)
	{
		return tws_cfg;
	}

	tws::tws_instance_t *get_tws_instance(void)
	{
		return tws_handle;
	}

	// helper function: produce the IB/TWS app connection. (Used by the TWS back-end communication thread / TWS API callbacks)
	struct mg_connection *get_connection(void)
	{
		return tws_conn;
	}
	struct mg_context *get_context(void)
	{
		return tws_ctx;
	}
	struct tws_conn_cfg &get_tws_ib_connection_config(void)
	{
		return tws_cfg;
	}

	void set_context(struct mg_context *ctx)
	{
		assert(ctx);
		tws_ctx = ctx;
	}

	virtual int pulse_pending_issues(void);
};


#endif
