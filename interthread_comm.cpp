
#include "interthread_comm.h"

#include "tier2_message_processor.h"
#include "tier2_message.h"

#include <mongoose/mongoose_ex.h>




int interthread_communicator::prepare_fd_sets_for_reception(struct fd_set *read_set, struct fd_set *except_set, int &max_fd)
{
	struct socket *sock = mg_get_client_socket(outgoing);

	mg_FD_SET(sock, read_set, &max_fd);
	mg_FD_SET(sock, except_set, &max_fd);
	return 0;
}


interthread_communicator::msg_pending_mode_t interthread_communicator::is_message_pending(fd_set *read_set, fd_set *except_set, int max_fd)
{
	struct socket *sock = mg_get_client_socket(outgoing);

	if (mg_FD_ISSET(sock, read_set))
		return interthread_communicator::MSG_PENDING;
	if (mg_FD_ISSET(sock, except_set))
		return interthread_communicator::CONNECTION_DROPPED;
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
	msg->current_owner(NULL); // put message 'in limbo'

	int rv = mg_write(incoming, &msg, sizeof(msg));

	return rv;
}

tier2_message *interthread_communicator::pop_one_message(msg_pending_mode_t *mode_ref)
{
	tier2_message *msg = NULL;
	msg_pending_mode_t mode;
	int rv = mg_read(outgoing, &msg, sizeof(msg));

	if (rv != sizeof(msg))
	{
		mode = (rv == 0 ? NO_MSG : CONNECTION_DROPPED);
		rv = 0;
	}
	else
	{
		mode = MSG_PENDING;

		msg->current_owner(receipient); // and attach the message to the new owner ~ receiver
	}

	if (mode_ref)
	{
		*mode_ref = mode;
	}

	return (rv ? msg : NULL);
}
