
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

#ifndef INTERTHREAD_COMMUNICATIONS_HEADER_INCLUDED
#define INTERTHREAD_COMMUNICATIONS_HEADER_INCLUDED

#include "tier2_message_processor.h"


struct mg_connection;
class tier2_message;
class tier2_queue_item;



/*
A wrapper for the socketpair used for passing messages to & from the
thread to a (predetermined) destination thread.
*/
class interthread_communicator
{
protected:
	// the socket pair:
	struct mg_connection *m_receiving_socket;
	struct mg_connection *m_sending_socket;

	tier2_message_processor *m_sender;
	tier2_message_processor *m_receipient;

	interthread_communicator *m_reverse;

public:
	interthread_communicator(tier2_message_processor *requester, tier2_message_processor *receiver, struct mg_connection *conns[2]);
	virtual ~interthread_communicator();

public:
	enum msg_pending_mode_t
	{
	NO_MSG = 0,                             // we'll have to wait for a message to arrive, shan't we?
	CONNECTION_DROPPED = -1,				// connection has been dropped or other fatality:
	MSG_PENDING = 1,						// fetch message from socket ~ queue
	MSG_CANCELED = 2,						// the referenced message is to be canceled
	};

	virtual int prepare_fd_sets_for_reception(struct fd_set *read_set, struct fd_set *except_set, int &max_fd);
	virtual msg_pending_mode_t is_message_pending(fd_set *read_set, fd_set *except_set, int max_fd);
	virtual int post_message(tier2_message *msg);
	virtual tier2_message *pop_one_message(msg_pending_mode_t &mode /* in/out */);

	bool has_sender(tier2_message_processor *messager) const
	{
		return messager == m_sender;
	}
	bool has_receiver(tier2_message_processor *messager) const
	{
		return messager == m_receipient;
	}
	bool matches(tier2_message_processor *from, tier2_message_processor *to) const 
	{
		return from == m_sender && to == m_receipient;
	}
	tier2_message_processor *receiver(void) const
	{
		return m_receipient;
	}
	tier2_message_processor *transmitter(void) const
	{
		return m_sender;
	}
	struct mg_connection *receiver_socket(void)
	{
		return m_receiving_socket;
	}

	interthread_communicator *reverse(void) const
	{
		return m_reverse;
	}
	void set_reverse(interthread_communicator *comm) 
	{
		m_reverse = comm;
	}
};




/*
Carries the 'globals' relevant for a particular front-end thread, 
such as:
 - the socketpair to use for message passing to/from the back-end
 - the thread-specific connection
 - ...
*/
class frontend_info_manager
{
protected:
	struct mg_connection *m_thread_conn;

	// the socket pair:
	interthread_communicator *m_comm;

public:
	frontend_info_manager(struct mg_connection *conn);
	virtual ~frontend_info_manager();

public:
	int send(tier2_message *message);
	tier2_message *receive(void);

	// and these are used with select() to make threads notice they're being invoked:
	void fd_set(struct fd_set *set, int *max_fd);
	int fd_isset(struct fd_set *set);
};




#endif // INTERTHREAD_COMMUNICATIONS_HEADER_INCLUDED
