
#include "tws_backend_io.h"

#include <tws_c_api/twsapi.h>

#include "app_manager.h"
#include "tws_database_io.h"

#include <mongoose/mongoose_ex.h>




ib_backend_io_channel::ib_backend_io_channel(app_manager *mgr)
	: m_app_manager(mgr)
	, tws_conn(NULL), tws_ctx(NULL), tws_handle(NULL)
	, fake_ib_tws_connection(true)
	, faking_the_ib_tws_connection(false)
{
	fake_conn[0] = NULL;
	fake_conn[1] = NULL;
}

ib_backend_io_channel::~ib_backend_io_channel()
{
	destroy();
}


static ib_backend_io_channel *singleton = NULL;

ib_backend_io_channel *ib_backend_io_channel::get_instance(app_manager *mgr, bool instantiate_singleton)
{
	if (!singleton && instantiate_singleton)
	{
		singleton = new ib_backend_io_channel(mgr);
	}
	return singleton;
}

void ib_backend_io_channel::set_instance(ib_backend_io_channel *obj)
{
	assert(!singleton);
	singleton = obj;
}





int ib_backend_io_channel::push_before(ib_backend_io_observer *listener)
{
	if (!listener)
		return -1;

	for (size_t i = 0; i < m_before.size(); i++)
	{
		if (m_before[i] == listener)
		{
			return 1;
		}
	}

	m_before.push_back(listener);
	return 0;
}
int ib_backend_io_channel::push_after(ib_backend_io_observer *listener)
{
	if (!listener)
		return -1;

	for (size_t i = 0; i < m_after.size(); i++)
	{
		if (m_after[i] == listener)
		{
			return 1;
		}
	}

	m_after.push_back(listener);
	return 0;
}
int ib_backend_io_channel::pop_before(ib_backend_io_observer *listener)
{
	if (!listener)
		return -1;

	for (size_t i = 0; i < m_before.size(); i++)
	{
		if (m_before[i] == listener)
		{
			m_before.erase(m_before.begin() + i);
			return 1;
		}
	}
	return 0;
}
int ib_backend_io_channel::pop_after(ib_backend_io_observer *listener)
{
	if (!listener)
		return -1;

	for (size_t i = 0; i < m_after.size(); i++)
	{
		if (m_after[i] == listener)
		{
			m_after.erase(m_after.begin() + i);
			return 1;
		}
	}
	return 0;
}
int ib_backend_io_channel::clean_before(void)
{
	for (size_t i = 0; i < m_before.size(); i++)
	{
		m_before[i]->destroy(this);
	}
	m_before.clear();
	return 0;
}
int ib_backend_io_channel::clean_after(void)
{
	for (size_t i = 0; i < m_after.size(); i++)
	{
		m_after[i]->destroy(this);
	}
	m_after.clear();
	return 0;
}






int ib_backend_io_channel::destroy(void)
{
	int rv = clean_before();
	rv |= clean_after();
	return rv;
}





int ib_backend_io_channel::tws_transmit_func(void *arg, const void *buf, unsigned int buflen)
{
	app_manager *mgr = (app_manager *)arg;
	ib_backend_io_channel *self = get_instance(mgr);

	return self->io_transmit(buf, buflen);
}

int ib_backend_io_channel::io_transmit(const void *buf, unsigned int buflen)
{
	int rv = 0;

	for (size_t i = 0; i < m_before.size(); i++)
	{
		int obs_rv = m_before[i]->io_transmit(rv, this, buf, buflen);
		if (obs_rv < 0)
			return obs_rv;
		if (obs_rv > 0)
			break;
	}

	if (!rv)
	{
		if (tws_conn)
		{
			rv = mg_write(tws_conn, buf, buflen);
		}
		else 
		{
			rv = -1;
		}
	}

	for (size_t i = 0; i < m_after.size(); i++)
	{
		int obs_rv = m_after[i]->io_transmit(rv, this, buf, buflen);
		if (obs_rv < 0)
			return obs_rv;
		if (obs_rv > 0)
			break;
	}

    return rv;
}

int ib_backend_io_channel::tws_receive_func(void *arg, void *buf, unsigned int max_bufsize)
{
	app_manager *mgr = (app_manager *)arg;
	ib_backend_io_channel *self = get_instance(mgr);

	return self->io_receive(buf, max_bufsize);
}

int ib_backend_io_channel::io_receive(void *buf, unsigned int max_bufsize)
{
	int rv = 0;

	for (size_t i = 0; i < m_before.size(); i++)
	{
		int obs_rv = m_before[i]->io_receive(rv, this, buf, max_bufsize);
		if (obs_rv < 0)
			return obs_rv;
		if (obs_rv > 0)
			break;
	}

	if (!rv)
	{
		tier2_message_processor *recvr = m_app_manager->get_receiver(tws_conn);

		if (tws_conn && recvr)
		{
			// check whether there's anything available:
			fd_set read_set, except_set;
			struct timeval tv;
			int max_fd;

			rv = -1;

			FD_ZERO(&read_set);
			FD_ZERO(&except_set);
			max_fd = -1;

			tv.tv_sec = tws_cfg.backend_poll_period / 1000;
			tv.tv_usec = (tws_cfg.backend_poll_period % 1000) * 1000;

			while (mg_get_stop_flag(mg_get_context(tws_conn)) == 0)
			{
				struct timeval tv2 = tv;
				int proc_rv;

				FD_ZERO(&read_set);
				FD_ZERO(&except_set);
				max_fd = -1;

				// Add listening sockets to the read set
				mg_FD_SET(mg_get_client_socket(tws_conn), &read_set, &max_fd);
				recvr->prepare_fd_sets_for_reception(&read_set, &except_set, max_fd);
				if (fake_ib_tws_connection)
				{
					fake_ib_tws_server(1);
				}

				if (select(max_fd + 1, &read_set, NULL, &except_set, &tv2) < 0)
				{
					// signal a fatal failure:
					// clear the handles sets to prevent 'surprises' from processing these a second time (below):
					FD_ZERO(&read_set);
					FD_ZERO(&except_set);
					max_fd = -1;

					max_bufsize = 0;
					break;
				}
				else
				{
					if (mg_FD_ISSET(mg_get_client_socket(tws_conn), &read_set))
					{
						/*
						Mongoose mg_read() does NOT fetch any pending data from the TCP/IP stack when the 'content length' isn't set yet.

						We, however, desire to load an unknown and arbitrary amount of data here to fill a buffer and our protocol doesn't
						have something like a 'content length' to guide us along, so we'll have to use another method to make sure
						the read operation actually delivers DATA!
						*/
						// conn->content_len = MAX_INT;
						break;
					}

					// prevent the mg_read() from locking up due to no incoming data:
					max_bufsize = 0;

					/*
					When there's no pending incoming data from TWS itself, we'll be running around in this loop while waiting for
					more data to arrive. Meanwhile, we can process queued requests from the front-end now:
					*/
					proc_rv = recvr->process_one_queued_tier2_request(&read_set, &except_set, max_fd);
					if (proc_rv < 0)
					{
						// signal a fatal failure:
						// clear the handles sets to prevent 'surprises' from processing these a second time (below):
						FD_ZERO(&read_set);
						FD_ZERO(&except_set);
						max_fd = -1;
						break;
					}

					/*
					Also 'tickle' the pending queue in round-robin fashion to ensure that 
					scheduled / postponed requests get serviced.
					*/
					proc_rv = pulse_pending_issues();
				}
			}

			/*
			Even when there's pending incoming data from TWS itself, we'll need to process queued 
			requests from the front-end and 'pending' queue or we would be experiencing lockup:
			*/
			rv = recvr->process_one_queued_tier2_request(&read_set, &except_set, max_fd);
			if (rv < 0)
			{
				// signal a fatal failure:
			}
			else 
			{
				rv = pulse_pending_issues();
				if (rv < 0)
				{
					// signal fatal failure:
				}
				else if (max_bufsize)
				{
					rv = mg_pull(tws_conn, buf, max_bufsize);
				}
			}
		}
		else 
		{
			rv = -1;
		}
	}

	for (size_t i = 0; i < m_after.size(); i++)
	{
		int obs_rv = m_after[i]->io_receive(rv, this, buf, max_bufsize);
		if (obs_rv < 0)
			return obs_rv;
		if (obs_rv > 0)
			break;
	}

    return rv;
}

int ib_backend_io_channel::tws_flush_func(void *arg)
{
	app_manager *mgr = (app_manager *)arg;
	ib_backend_io_channel *self = get_instance(mgr);

	return self->io_flush();
}

/* 'flush()' marks the end of the outgoing message: it should be transmitted ASAP */
int ib_backend_io_channel::io_flush(void)
{
	int rv = 0;

	for (size_t i = 0; i < m_before.size(); i++)
	{
		int obs_rv = m_before[i]->io_flush(rv, this);
		if (obs_rv < 0)
			return obs_rv;
		if (obs_rv > 0)
			break;
	}

	if (!rv)
	{
		if (fake_ib_tws_connection)
		{
			// fetch data from fake socket[1] and push a response back:
			fake_ib_tws_server(0);
		}
	}

	for (size_t i = 0; i < m_after.size(); i++)
	{
		int obs_rv = m_after[i]->io_flush(rv, this);
		if (obs_rv < 0)
			return obs_rv;
		if (obs_rv > 0)
			break;
	}

	return rv;
}

int ib_backend_io_channel::tws_open_func(void *arg)
{
	app_manager *mgr = (app_manager *)arg;
	ib_backend_io_channel *self = get_instance(mgr);

	return self->io_open();
}

/* open callback is invoked when tws_connect is invoked and no connection has been established yet (tws_connected() == false); return 0 on success; a twsclient_error_codes error code on failure. */
int ib_backend_io_channel::io_open(void)
{
	int rv = 0;

	for (size_t i = 0; i < m_before.size(); i++)
	{
		int obs_rv = m_before[i]->io_open(rv, this);
		if (obs_rv < 0)
			return obs_rv;
		if (obs_rv > 0)
			break;
	}

	if (!rv)
	{
		assert(!tws_conn);
		tws_conn = mg_connect_to_host(tws_ctx, tws_cfg.ip_address, tws_cfg.port, 0);

		faking_the_ib_tws_connection = false;

		if (tws_conn == NULL && fake_ib_tws_connection)
		{
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

		if (tws_conn != NULL)
		{
			struct socket *sock = mg_get_client_socket(tws_conn);

			// Disable Nagle - act a la telnet:
			mg_set_nodelay_mode(sock, 1);

			// enable keepalive + rx/tx timeouts:
			mg_set_socket_keepalive(sock, 1);
			mg_set_socket_timeout(sock, 10);

			m_app_manager->register_backend_thread(tws_conn);
		}

		rv = (tws_conn ? 0 : (int)tws::NOT_CONNECTED);
	}

	for (size_t i = 0; i < m_after.size(); i++)
	{
		int obs_rv = m_after[i]->io_open(rv, this);
		if (obs_rv < 0)
			return obs_rv;
		if (obs_rv > 0)
			break;
	}

	return rv;
}


int ib_backend_io_channel::tws_close_func(void *arg)
{
	app_manager *mgr = (app_manager *)arg;
	ib_backend_io_channel *self = get_instance(mgr);

	return self->io_close();
}

/* close callback is invoked on error or when tws_disconnect is invoked */
int ib_backend_io_channel::io_close(void)
{
	int rv = 0;

	for (size_t i = 0; i < m_before.size(); i++)
	{
		int obs_rv = m_before[i]->io_close(rv, this);
		if (obs_rv < 0)
			return obs_rv;
		if (obs_rv > 0)
			break;
	}

	if (!rv)
	{
		if (tws_conn)
		{
			mg_close_connection(tws_conn);
			tws_conn = NULL;
		}
	}

	for (size_t i = 0; i < m_after.size(); i++)
	{
		int obs_rv = m_after[i]->io_close(rv, this);
		if (obs_rv < 0)
			return obs_rv;
		if (obs_rv > 0)
			break;
	}

	return rv;
}







int ib_backend_io_channel::tws_tx_elem_observe_func(void *arg, const char *elem, unsigned int elem_size, ib_outgoing_id_t start_of_message)
{
	app_manager *mgr = (app_manager *)arg;
	ib_backend_io_channel *self = get_instance(mgr);

	for (size_t i = 0; i < self->m_before.size(); i++)
	{
		int obs_rv = self->m_before[i]->io_tx_elem_observe(self, elem, elem_size, start_of_message);
		if (obs_rv < 0)
			return obs_rv;
		if (obs_rv > 0)
			break;
	}

	for (size_t i = 0; i < self->m_after.size(); i++)
	{
		int obs_rv = self->m_after[i]->io_tx_elem_observe(self, elem, elem_size, start_of_message);
		if (obs_rv < 0)
			return obs_rv;
		if (obs_rv > 0)
			break;
	}

	return 0;
}

int ib_backend_io_channel::tws_rx_elem_observe_func(void *arg, const char *elem, unsigned int elem_size, int return_value)
{
	app_manager *mgr = (app_manager *)arg;
	ib_backend_io_channel *self = get_instance(mgr);

	for (size_t i = 0; i < self->m_before.size(); i++)
	{
		int obs_rv = self->m_before[i]->io_rx_elem_observe(self, elem, elem_size, return_value);
		if (obs_rv < 0)
			return obs_rv;
		if (obs_rv > 0)
			break;
	}

	for (size_t i = 0; i < self->m_after.size(); i++)
	{
		int obs_rv = self->m_after[i]->io_rx_elem_observe(self, elem, elem_size, return_value);
		if (obs_rv < 0)
			return obs_rv;
		if (obs_rv > 0)
			break;
	}

	return 0;
}









tier2_message_processor *ib_backend_io_channel::get_receiver(void)
{
	tier2_message_processor *rv = m_app_manager->get_receiver(tws_ctx, app_manager::IB_TWS_API_CONNECTION_THREAD);

	assert(rv);
	return rv;
}


int ib_backend_io_channel::pulse_pending_issues(void)
{
	assert(!"Should never get here!");

	return 0;
}