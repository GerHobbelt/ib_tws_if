

#include "tier2_message_requester.h"

#include "interthread_comm.h"
#include "app_manager.h"
#include "tws_instance.h"

#include <mongoose/mongoose_ex.h>




void tier2_message_receiver::register_sender(tier2_message_requester *sender)
{
	// do not add the sender to the list a second time:
	int count = senders.size();

	for (int i = 0; i < count; i++)
	{
		if (senders[i].sender == sender)
			return;
	}

	app_manager *mgr = this->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	mg_connection *conns[2];
	struct sender_info inf;
	
	if (!mg_socketpair(conns, ibm->get_context()))
	{
		interthread_communicator *comm = new interthread_communicator(conns);

		inf.link = comm;
	}
	else
	{
		assert(0);
		inf.link = NULL;
	}
	inf.sender = sender;

	senders.push_back(inf);
}

interthread_communicator *tier2_message_receiver::get_interthread_communicator(tier2_message_requester *sender)
{
	int count = senders.size();

	for (int i = 0; i < count; i++)
	{
		if (senders[i].sender == sender)
			return senders[i].link;
	}
	return NULL;
}


int tier2_message_receiver::prepare_fd_sets_for_reception(fd_set *read_set, fd_set *except_set, int &max_fd)
{
	int count = senders.size();

	for (int i = 0; i < count; i++)
	{
		interthread_communicator *comm = senders[i].link;

		assert(comm);
		comm->prepare_fd_sets_for_reception(read_set, except_set, max_fd);
	}
	return 0;
}

int tier2_message_receiver::process_one_queued_tier2_request(fd_set *read_set, fd_set *except_set, int max_fd)
{
	int count = senders.size();

	for (int i = 0; i < count; i++)
	{
		struct sender_info inf = senders[i];

		assert(inf.link);
		switch (inf.link->is_message_pending(read_set, except_set, max_fd))
		{
		default:
			continue;

		case 1:
			// fetch message from socket ~ queue
			break;

		case -1:
			// connection has been dropped or other fatality:
			break;
		}
		break;
	}
	return 0;
}

