

#include "tier2_message_processor.h"

#include "interthread_comm.h"
#include "app_manager.h"
#include "tws_instance.h"

#include <mongoose/mongoose_ex.h>




void tier2_message_processor::register_sender(tier2_message_processor *sender)
{
	// ignore the 'loopback' path:
	if (this == sender)
		return;

	// do not add the sender to the list a second time:
	int count = senders.size();

	for (int i = 0; i < count; i++)
	{
		if (senders[i]->has_sender(sender))
			return;
	}

	ib_tws_manager *ibm = m_app_manager->get_ib_tws_manager();
	mg_connection *conns[2];
	interthread_communicator *comm = NULL;
	
	if (!mg_socketpair(conns, ibm->get_context()))
	{
		comm = new interthread_communicator(sender, this, conns);
	}
	else
	{
		assert(0);
	}
	senders.push_back(comm);

	if (sender)
	{
		sender->register_interthread_connection(comm);
	}
}

void tier2_message_processor::register_interthread_connection(interthread_communicator *comm)
{
	assert(comm->receiver() == this || comm->transmitter() == this);

	// do not add the sender to the list a second time:
	int count = senders.size();

	for (int i = 0; i < count; i++)
	{
		if (senders[i]->matches(comm))
			return;
	}

	senders.push_back(comm);
}

interthread_communicator *tier2_message_processor::get_interthread_communicator(tier2_message_processor *from, tier2_message_processor *to)
{
	int count = senders.size();

	for (int i = 0; i < count; i++)
	{
		if (senders[i]->matches(from, to))
			return senders[i];
	}
	return NULL;
}


int tier2_message_processor::prepare_fd_sets_for_reception(fd_set *read_set, fd_set *except_set, int &max_fd)
{
	int count = senders.size();

	for (int i = 0; i < count; i++)
	{
		if (senders[i]->has_receiver(this))
		{
			interthread_communicator *comm = senders[i];

			assert(comm);
			comm->prepare_fd_sets_for_reception(read_set, except_set, max_fd);
		}
	}
	return 0;
}

int tier2_message_processor::process_one_queued_tier2_request(fd_set *read_set, fd_set *except_set, int max_fd)
{
	int requests_handled_counter = 0;

	// we only need to check the client connections when there's actually something to expected there:
	if (max_fd >= 0)
	{
		int count = senders.size();

		for (int i = 0; i < count; i++)
		{
			interthread_communicator *comm = senders[i];
			assert(comm);

			if (comm->has_receiver(this))
			{
				interthread_communicator::msg_pending_mode_t mode = comm->is_message_pending(read_set, except_set, max_fd);
				tier2_message *msg = NULL;

				switch (mode)
				{
				default:
					continue;

				case interthread_communicator::MSG_PENDING:
					// fetch message from socket ~ queue
					requests_handled_counter++;
					msg = comm->pop_one_message(&mode);
					if (msg)
					{
						// ???
					}
					break;

				case interthread_communicator::CONNECTION_DROPPED:
					// connection has been dropped or other fatality:
					break;
				}
				break;
			}
		}
	}

	return 0;
}







int tier2_message_processor::own(tier2_message *msg)
{
	if (does_own(msg))
	{
		return 1;
	}
	m_msgs_i_own.push_back(msg);
	assert(msg->current_owner() == this);

	return 0;
}

int tier2_message_processor::release(tier2_message *msg)
{
	for (msg_set_t::iterator i = m_msgs_i_own.begin(); i != m_msgs_i_own.end(); i++)
	{
		if (*i == msg)
		{
			m_msgs_i_own.erase(i);
			return 0;
		}
	}
	return -1;
}

bool tier2_message_processor::does_own(tier2_message *msg) const
{
	for (msg_set_t::const_iterator i = m_msgs_i_own.cbegin(); i != m_msgs_i_own.cend(); i++)
	{
		if (*i == msg)
			return true;
	}
	return true;
}

int tier2_message_processor::queue_msg_for_pulsing(tier2_message *msg)
{
	assert(does_own(msg));
	for (msg_set_t::iterator i = m_msgs_pending_for_pulsing.begin(); i != m_msgs_pending_for_pulsing.end(); i++)
	{
		if (*i == msg)
		{
			return 1;
		}
	}
	m_msgs_pending_for_pulsing.push_back(msg);

	return 0;
}

int tier2_message_processor::pulse_marked_messages(void)
{
	int rv = 0;

	for (msg_set_t::iterator i = m_msgs_pending_for_pulsing.begin(); i != m_msgs_pending_for_pulsing.end(); i++)
	{
		tier2_message *msg = *i;

		if (msg->current_owner() == this)
		{
			rv |= msg->pulse();
		}
	}
	m_msgs_pending_for_pulsing.clear();

	return rv;
}

















requester_id::requester_id(mg_connection *thread, mg_connection *client, int seq_id) :
	client_connection(client),
	calling_thread_id(thread),
	sequence_id(seq_id)
{
}

requester_id::~requester_id()
{
}

int requester_id::hash(void) const
{
	return 0;
}

int requester_id::get_next_sequence_value(requester_id *optional_id)
{
	return 0;
}

