
#include "interthread_comm.h"

#include "tier2_message_processor.h"
#include "tier2_message.h"

#include <mongoose/mongoose_ex.h>




interthread_communicator::interthread_communicator(tier2_message_processor *requester, tier2_message_processor *receiver, struct mg_connection *conns[2]) :
    m_receiving_socket(conns[0]),
    m_sending_socket(conns[1]),
    m_sender(requester),
    m_receipient(receiver),
    m_reverse(NULL)
{
}

interthread_communicator::~interthread_communicator()
{
    mg_close_connection(m_receiving_socket);
}



int interthread_communicator::prepare_fd_sets_for_reception(struct fd_set *read_set, struct fd_set *except_set, int &max_fd)
{
    mg_FD_SET(m_receiving_socket, read_set, &max_fd);
    mg_FD_SET(m_receiving_socket, except_set, &max_fd);
    return 0;
}


interthread_communicator::msg_pending_mode_t interthread_communicator::is_message_pending(fd_set *read_set, fd_set *except_set, int max_fd)
{
    // only check when there's actually something to be expected at all:
    if (max_fd >= 0)
    {
        if (mg_FD_ISSET(m_receiving_socket, read_set))
            return interthread_communicator::MSG_PENDING;
        if (mg_FD_ISSET(m_receiving_socket, except_set))
            return interthread_communicator::CONNECTION_DROPPED;
    }
    return interthread_communicator::NO_MSG;
}


/*
All 'messages' passed across the interthread connection are merely pointers (references) to tier2_message instances.

Those instances are what it's really all about; they also interact with one another, e.g. when you transmit/post
a 'request' message, you might expect to receive a 'response' message a while later: that one is processed by the
(queued/pending) request message.

The same goes for a 'cancel' operation where the requester wishes to cancel the pending request: this is done
by transmitting/posting a 'cancel' message which references the original request message. The 'cancel' request
itself is handled by the referenced message-to-be-canceled at the receiver's end of the interthread connection.
The 'cancel' message is a fire-and-forget sort of message, as there won't be any response to it: you just have
to assume that the cancel succeeded.

As everything is done through tier2_messages, the interthread connection is merely the means to provide thread-safety
around here and the actual data piped through the connection is just a reference/pointer to the tier2_message itself,
thus 'passing the buck' to the receiving thread.

'Pending' messages (and 'schedules', which are messages themselves) are kept in a queue at the receiving end:
hence we have one queue per thread.

IMPORTANT: anything passing a thread boundary implies the use of the corresponding interthread communicator instance
           which MUST be used to pass a reference to the appropriate tier2_message instance to 'the other side'.

Request-response message series can be 'asynchronous' when you use the event-oriented coding scheme of
select() / is_message_pending() / pop_one_message() or 'synchronous' when you use the pop_one_message()
method. In the latter case, you MUST verify that the message actually received is indeed the message you
wished to receive; if it isn't, you are, at worst, mixing the async and sync models in the same interthread
pipeline, or, at best, observing a cancellation or connection failure, all of which are fatal issues as the
receiver/sender threads' state and/or interconnection is untrustworthy.
*/


int interthread_communicator::post_message(tier2_message *msg)
{
    tier2_message_processor *prev_owner = msg->current_owner();
    assert(prev_owner == m_sender);

    msg->current_owner(NULL); // put message 'in limbo'

    int rv = mg_write(m_sending_socket, &msg, sizeof(msg));

    rv = (rv > 0 ? rv != sizeof(msg) : rv);
    if (rv)
    {
        // failed to pass the buck; re-take ownership
        msg->current_owner(prev_owner);
    }
    return rv;
}

/*
We don't select/wait for the socket to cough up a message when the input mode
is MSG_PENDING; hence one should only use this mode when he is absolutely certain
a message is waiting in the socket buffer or mg_pull() will lock up, unless you've
non-blocking socket I/O turned on.
*/
tier2_message *interthread_communicator::pop_one_message(msg_pending_mode_t &mode)
{
    tier2_message *msg = NULL;
    int rv = -1;
    bool good_to_go;

    if (mode != MSG_PENDING)
    {
        // check whether there's anything available:
        fd_set read_set;
        struct timeval tv;
        int max_fd;

        FD_ZERO(&read_set);
        max_fd = -1;

        tv.tv_sec = 1;
        tv.tv_usec = 0;

        while (mg_get_stop_flag(mg_get_context(m_receiving_socket)) == 0)
        {
            struct timeval tv2 = tv;

            FD_ZERO(&read_set);
            max_fd = -1;

            // Add listening sockets to the read set
            mg_FD_SET(m_receiving_socket, &read_set, &max_fd);
            if (select(max_fd + 1, &read_set, NULL, NULL, &tv2) < 0)
            {
                // signal a fatal failure:
                // clear the handles sets to prevent 'surprises' from processing these a second time (below):
                FD_ZERO(&read_set);
                max_fd = -1;
                assert(!"Should never get here");
                break;
            }
            else
            {
                if (mg_FD_ISSET(m_receiving_socket, &read_set))
                {
                    break;
                }
                max_fd = -1;
            }
        }
    
        good_to_go = (max_fd >= 0);
    }
    else
    {
        good_to_go = true;
    }

    if (good_to_go)
    {
        // don't use mg_read() as that depends upon decoded HTTP header info
        rv = mg_pull(m_receiving_socket, &msg, sizeof(msg));
    }

    if (rv != sizeof(msg))
    {
        mode = (rv == 0 ? NO_MSG : CONNECTION_DROPPED);
        rv = 0;
    }
    else
    {
        mode = MSG_PENDING;

        msg->current_owner(m_receipient); // and attach the message to the new owner ~ receiver
    }

    return (rv ? msg : NULL);
}
