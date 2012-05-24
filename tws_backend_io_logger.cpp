
#include "tws_backend_io_logger.h"

#include <tws_c_api/twsapi.h>
#include <mongoose/mongoose_ex.h>




ib_backend_io_observer::ib_backend_io_observer()
{
}
ib_backend_io_observer::~ib_backend_io_observer()
{
}




ib_backend_io_logger::ib_backend_io_logger()
	: m_pending_tx_msg(tws::TWS_NO_TX_MESSAGE)
	, m_pending_rx_msg(tws::TWS_NO_RX_MESSAGE)
	, m_tx_msg_row(0)
	, m_rx_msg_row(0)
	, m_tx_timestamp(0)
	, m_rx_timestamp(0)
	, m_init_phase(true)
{
}

ib_backend_io_logger::~ib_backend_io_logger()
{
}

int ib_backend_io_logger::io_transmit(int &return_value, ib_backend_io_channel *originator, const void *buf, unsigned int buflen)
{
	assert(originator || 1);

	return 0;
}

int ib_backend_io_logger::io_receive(int &return_value, ib_backend_io_channel *originator, void *buf, unsigned int max_bufsize)
{
	assert(originator);

	return 0;
}

/* 'flush()' marks the end of the outgoing message: it should be transmitted ASAP */
int ib_backend_io_logger::io_flush(int &return_value, ib_backend_io_channel *originator)
{
	if (!m_init_phase)
	{
		write_tx_msg(originator);
	}

	return 0;
}

/* open callback is invoked when tws_connect is invoked and no connection has been established yet (tws_connected() == false); return 0 on success; a twsclient_error_codes error code on failure. */
int ib_backend_io_logger::io_open(int &return_value, ib_backend_io_channel *originator)
{
	m_pending_tx_msg = tws::TWS_NO_TX_MESSAGE;
	m_pending_rx_msg = tws::TWS_NO_RX_MESSAGE;

	m_tx_timestamp = m_rx_timestamp = time(NULL);
	
	return 0;
}

/* close callback is invoked on error or when tws_disconnect is invoked */
int ib_backend_io_logger::io_close(int &return_value, ib_backend_io_channel *originator)
{
	assert(originator || 2);

	if (m_tx_msg_buffer.size() > 0)
	{
		write_tx_msg(originator);
	}
	if (m_rx_msg_buffer.size() > 0)
	{
		write_rx_msg(originator);
	}

	return 0;
}

int ib_backend_io_logger::io_tx_elem_observe(ib_backend_io_channel *originator, const char *elem, unsigned int elem_size, ib_outgoing_id_t start_of_message)
{
	if (start_of_message != tws::TWS_NO_TX_MESSAGE)
	{
		assert(!elem);

		if (m_init_phase)
		{
			write_tx_msg(originator);
			write_rx_msg(originator);
		}
		m_init_phase = false;

		struct mg_connection *conn = originator->get_connection();
		assert(conn);
		struct mg_request_info *rinfo = mg_get_request_info(conn);
		assert(rinfo);
		rinfo->uri = NULL;
		rinfo->request_method = NULL;

		m_tx_msg_id = "TX.";
		m_tx_msg_id += tws_outgoing_msg_name(start_of_message);
		m_pending_tx_msg = start_of_message;
		m_tx_timestamp = time(NULL);
		m_tx_log_file.clear();
	}

	if (elem)
	{
		m_tx_msg_buffer += elem;
		m_tx_msg_buffer += "\n";
	}

	return 0;
}

int ib_backend_io_logger::io_rx_elem_observe(ib_backend_io_channel *originator, const char *elem, unsigned int elem_size, int return_value)
{
	if (!m_init_phase)
	{
		if (!elem && !elem_size)
		{
			if (!m_rx_msg_buffer.empty())
			{
				write_rx_msg(originator);
			}

			m_pending_rx_msg = tws::TWS_NO_RX_MESSAGE;
		}
		else if (m_pending_rx_msg == tws::TWS_NO_RX_MESSAGE)
		{
			struct mg_connection *conn = originator->get_connection();
			assert(conn);
			struct mg_request_info *rinfo = mg_get_request_info(conn);
			assert(rinfo);
			rinfo->uri = NULL;
			rinfo->request_method = NULL;

			m_pending_rx_msg = (ib_incoming_id_t)atoi(elem);
			m_rx_msg_id = "RX.";
			m_rx_msg_id += tws_incoming_msg_name(m_pending_rx_msg);
			m_rx_timestamp = time(NULL); 
			m_rx_log_file.clear();
		}
	}

	if (elem)
	{
		m_rx_msg_buffer += elem;
		m_rx_msg_buffer += "\n";
	}

	return 0;
}

int ib_backend_io_logger::destroy(ib_backend_io_channel *originator)
{
	assert(originator);

	return 0;
}

int ib_backend_io_logger::init(void)
{
	m_init_phase = true;
	m_tx_timestamp = m_rx_timestamp = time(NULL);

	m_tx_msg_id = m_rx_msg_id = "init";

	return 0;
}


int ib_backend_io_logger::write_tx_msg(ib_backend_io_channel *originator)
{
	struct tws_conn_cfg &tws_cfg = originator->get_config();

	if (tws_cfg.m_tws_log_traffic)
	{
		struct mg_connection *conn = originator->get_connection();

		assert(conn);
		if (m_tx_log_file.empty())
		{
			char path[MAX_PATH];
			struct mg_request_info *rinfo;

			rinfo = mg_get_request_info(conn);
			rinfo->uri = const_cast<char *>(m_tx_msg_id.c_str());
			rinfo->request_method = "TX";

			m_tx_log_file = mg_get_logfile_path(path, sizeof(path), tws_cfg.m_tws_traffic_log_file, conn, m_tx_timestamp);
		}

		mg_write2log(conn, m_tx_log_file.c_str(), m_tx_timestamp, "trace", "TX: %s", m_tx_msg_buffer.c_str());

		m_tx_msg_buffer.clear();
	}

	return 0;
}
int ib_backend_io_logger::write_rx_msg(ib_backend_io_channel *originator)
{
	struct tws_conn_cfg &tws_cfg = originator->get_config();

	if (tws_cfg.m_tws_log_traffic)
	{
		struct mg_connection *conn = originator->get_connection();

		assert(conn);
		if (m_rx_log_file.empty())
		{
			char path[MAX_PATH];
			struct mg_request_info *rinfo;

			rinfo = mg_get_request_info(conn);
			rinfo->uri = const_cast<char *>(m_rx_msg_id.c_str());
			rinfo->request_method = "RX";

			m_rx_log_file = mg_get_logfile_path(path, sizeof(path), tws_cfg.m_tws_traffic_log_file, conn, m_rx_timestamp);
		}

		mg_write2log(originator->get_connection(), m_rx_log_file.c_str(), m_rx_timestamp, "trace", "RX: %s", m_rx_msg_buffer.c_str());

		m_rx_msg_buffer.clear();
	}

	return 0;
}
