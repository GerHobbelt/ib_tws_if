
#ifndef IB_BACKEND_IO_LOGGER_H
#define IB_BACKEND_IO_LOGGER_H

#include "tws_backend_io.h"

class app_manager;
namespace tws
{
    enum tws_outgoing_ids;
    enum tws_incoming_ids;
}
typedef tws::tws_outgoing_ids ib_outgoing_id_t;
typedef tws::tws_incoming_ids ib_incoming_id_t;




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
class ib_backend_io_logger : public ib_backend_io_observer
{
public:
    ib_backend_io_logger();
    virtual ~ib_backend_io_logger();

public:
    virtual int io_transmit(int &return_value, ib_backend_io_channel *originator, const void *buf, unsigned int buflen);
    virtual int io_receive(int &return_value, ib_backend_io_channel *originator, void *buf, unsigned int max_bufsize);
    /* 'flush()' marks the end of the outgoing message: it should be transmitted ASAP */
    virtual int io_flush(int &return_value, ib_backend_io_channel *originator);
    /* open callback is invoked when tws_connect is invoked and no connection has been established yet (tws_connected() == false); return 0 on success; a twsclient_error_codes error code on failure. */
    virtual int io_open(int &return_value, ib_backend_io_channel *originator);
    /* close callback is invoked on error or when tws_disconnect is invoked */
    virtual int io_close(int &return_value, ib_backend_io_channel *originator);

    virtual int io_tx_elem_observe(ib_backend_io_channel *originator, const char *elem, unsigned int elem_size, ib_outgoing_id_t start_of_message);
    virtual int io_rx_elem_observe(ib_backend_io_channel *originator, const char *elem, unsigned int elem_size, int return_value);

    virtual int destroy(ib_backend_io_channel *originator);

protected:
    app_manager *m_app_manager;

    std::string m_tx_msg_id;
    std::string m_tx_msg_buffer;
    ib_outgoing_id_t m_pending_tx_msg;
    int m_tx_msg_row;
    time_t m_tx_timestamp;
    std::string m_tx_log_file;

    std::string m_rx_msg_id;
    std::string m_rx_msg_buffer;
    ib_incoming_id_t m_pending_rx_msg;
    int m_rx_msg_row;
    time_t m_rx_timestamp;
    std::string m_rx_log_file;

    bool m_init_phase;

public:
    virtual int init(void);

    virtual int write_tx_msg(ib_backend_io_channel *originator);
    virtual int write_rx_msg(ib_backend_io_channel *originator);
};




#endif
