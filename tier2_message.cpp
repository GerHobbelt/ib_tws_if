
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

/*
 * Mongoose-based server app which interfaces with a TWS station and displays data in HTML form,
 * suitable for loading by, for example, 64-bit Excel 2010, using web queries.
 */

#include "tier2_message.h"

#include "app_manager.h"
#include "tier2_message_processor.h"
#include "interthread_comm.h"

#include <mongoose/mongoose_ex.h>






unique_type_id_manager tier2_msg_typeid_mgr(0);




tier2_message::state_change tier2_message::handle_state_change(tier2_message::request_state_t new_state)
{
	state_change rv = PROCEED;

	for (state_change_handler_set_t::iterator i = state_change_handlers.begin();
		i != state_change_handlers.end(); 
		i++)
	{
		tier2_message_state_change_handler *h = *i;

		switch (h->process(*this, new_state))
		{
		default:
		case ERROR_OCCURRED:
			rv = ERROR_OCCURRED;
			break;

		case DONT_CHANGE:
			rv = DONT_CHANGE;
			break;

		case PROCEED:
			continue;
		}
		break;
	}
	return rv;
}




void tier2_message::register_handler(tier2_message_state_change_handler *handler)
{
	for (state_change_handler_set_t::iterator i = state_change_handlers.begin();
		i != state_change_handlers.end(); 
		i++)
	{
		tier2_message_state_change_handler *h = *i;

		if (h == handler)
			return;
	}
	state_change_handlers.push_back(handler);
}

void tier2_message::unregister_handler(tier2_message_state_change_handler *handler)
{
	for (state_change_handler_set_t::iterator i = state_change_handlers.begin();
		i != state_change_handlers.end(); 
		i++)
	{
		tier2_message_state_change_handler *h = *i;

		if (h == handler)
		{
			state_change_handlers.erase(i);
			return;
		}
	}
}






// set up the defaults; perform any necessary registration with the app_manager, etc...
void tier2_message::resolve_requester_and_receiver_issues(void)
{
	assert(requester || receiver);

	/*
	Default: receiver == requester.
	*/
	if (!receiver)
	{
		assert(typeid(*requester) == typeid(tier2_message_processor));
		receiver = requester;
		assert(receiver);
	}
	if (!requester)
	{
		requester = receiver;
	}

	if (!owner)
	{
		current_owner(requester);
	}

	/*
	Register the communication path with the app_manager so it can help all receivers to track their incoming message paths.

	Ignore the 'loopback' path to the node itself.
	*/
	receiver->register_sender(requester);
}












tier2_message::tier2_message(tier2_message_processor *from, tier2_message_processor *to, request_state_t s) :
	requester(from),
	receiver(to),
	now_state(s),
	previous_state(INIT4PREV),
	owner(NULL)
{
	unique_msgID = obtain_next_unique_msgID();

	resolve_requester_and_receiver_issues();
}

tier2_message::~tier2_message()
{
	// state(DESTRUCTION); -- can't do that here as derived classes will already have destructed themselves! Hence protected destructor!
	
	owner = NULL;
	current_owner(owner);

	release_unique_msgID();
}

void tier2_message::destroy(void)
{
	// this way, the state change will make it through /just before/ the destructor sequence is executed!
	state(DESTRUCTION);

	delete this;
}

tier2_message::request_state_t tier2_message::state(request_state_t new_state)
{
	for (int i = 2; i > 0 && new_state != now_state; i--)
	{
		tier2_message::state_change rv = handle_state_change(new_state);
		switch (rv)
		{
		default:
		case ERROR_OCCURRED:
			previous_state = now_state;
			now_state = new_state = FAILED;
			break;

		case DONT_CHANGE:
			new_state = now_state;
			break;

		case PROCEED:
			previous_state = now_state;
			now_state = new_state;
			break;
		}

		interthread_communicator *comm = NULL;
		int err = 0;

		switch (new_state)
		{
		case EXEC_COMMAND:
			// send message to receiver ~ handler
			if (owner != receiver)
			{
				comm = receiver->get_interthread_communicator(owner, receiver);

				// push message across the pond:
				err = comm->post_message(this);
			}
			else
			{
				// transmit message to 'beyond / outside world':
				err = f_exec_command();
			}
			break;

		case WAIT_FOR_TRANSMIT:
			// queue message at receiver for forwarding transmission to 'abroad'
			if (owner != receiver)
			{
				assert(!"Should not get here");

				comm = receiver->get_interthread_communicator(owner, receiver);

				// push message across the pond:
				err = comm->post_message(this);
			}
			else
			{
				// transmit message to 'beyond / outside world':
				err = f_wait_for_transmit();
			}
			break;

		case COMMENCE_TRANSMIT:
			// at receiver: forward message to 'abroad'
			if (owner != receiver)
			{
				assert(!"Should not get here");

				comm = receiver->get_interthread_communicator(owner, receiver);

				// push message across the pond:
				err = comm->post_message(this);
			}
			else
			{
				// transmit message to 'beyond / outside world':
				err = f_commence_transmit();
			}
			break;

		case READY_TO_RECEIVE_RESPONSE:
			// at receiver: wait for reception of corresponding response(s) from 'abroad'
			if (owner != receiver)
			{
				assert(!"Should not get here");

				comm = receiver->get_interthread_communicator(owner, receiver);

				// push message across the pond:
				err = comm->post_message(this);
			}
			else
			{
				// transmit message to 'beyond / outside world':
				err = f_ready_to_receive_response();
			}
			break;

		case RESPONSE_PENDING:
			// send message to receiver ~ handler
			if (owner != receiver)
			{
				assert(!"Should not get here");

				comm = receiver->get_interthread_communicator(owner, receiver);

				// push message across the pond:
				err = comm->post_message(this);
			}
			break;

		default:
			// return message to requester
			if (owner != requester)
			{
				comm = requester->get_interthread_communicator(owner, requester);

				// push message across the pond:
				err = comm->post_message(this);
			}
			break;
		}

		// changing to FAILED state is only useful when we're still in charge here, otherwise it's only cause to collisions!
		if (err != 0 && owner)
		{
			new_state = FAILED;
			continue;
		}
		break;
	}

	return new_state;
}



tier2_message_processor *tier2_message::current_owner(tier2_message_processor *new_owner)
{
	if (owner != new_owner)
	{
		if (owner)
		{
			owner->release(this);
		}
		if (new_owner)
		{
			new_owner->own(this);
		}
	}

	owner = new_owner;
	return owner;
}













int tier2_message::cancel_request(tier2_message_processor *transmitter)
{
	// create a CANCEL message:
	cancel_message *cancel = new cancel_message(transmitter, this);

	// push message across the pond:
	cancel->state(EXEC_COMMAND);

	return 0;
}


int tier2_message::wait_for_response(void)
{
	interthread_communicator *comm = receiver->get_interthread_communicator(requester, receiver);

	while (this->state() < RESPONSE_COMPLETE)
	{
		tier2_message *msg = comm->pop_one_message();

		// if the popped message is our response (and not some book-keeping sort of thing), we know we're done.

		if (!msg)
		{
			return -1;
		}
	}
	
	return 0;
}

int tier2_message::process_response(tier2_message *resp_msg)
{
	assert(!"Should never get here!");
	return 0;
}

void tier2_message::release_unique_msgID(void)
{
	return;
}

unique_id_t tier2_message::obtain_next_unique_msgID(void)
{
	return 0;
}




















int tier2_message::f_exec_command(void)
{
	assert(!"Should never get here");
	return 0;
}
int tier2_message::f_task_completed(void)
{
	return 0;
}
int tier2_message::f_response_complete(void)
{
	return 0;
}
int tier2_message::f_response_pending(void)
{
	return 0;
}
int tier2_message::f_ready_to_receive_response(void)
{
	return 0;
}
int tier2_message::f_commence_transmit(void)
{
	return 0;
}
int tier2_message::f_wait_for_transmit(void)
{
	return 0;
}









int cancel_message::f_exec_command(void)
{
	assert(refd_msg);
	assert(owner == receiver);

	/*
	prevent race condition where original msg already got sent back to the requester while a cancel request (this one) was still pending in the receiver queue:

	--> check if referenced message is still owned by our owner/its target.

	When it isn't, the target is either 'in limbo' or already completely processed and destroyed. We're trying to prevent illegal memory access core dumps due to the
	latter, while we decided not to bother treating the former as a special case: when the target IS 'in limbo', it means the target message has probably already
	completed and is returning to the requester; meanwhile it ALSO means that the target's STATE isn't anymore what we think it is and that clashes a wee bit
	with the expectation when the CANCEL was fired.

	The way out of this conundrum about the 'in limbo' ambiguity (has the targeted message arrived yet or has it already left again?) is to have a non-applicable 
	cancel request remain 'active' until the incoming (receive) queue of the targeted processor is empty: when that happens we are certain that the targeted
	message has truly already left again as it has been sent to this processor before we ('the cancel request') got sent there, so it should have popped up in
	the queue a little while after we arrived, at the very latest.
	*/
	if (receiver->does_own(refd_msg))
	{
		tier2_message::request_state_t rv = refd_msg->state(tier2_message::ABORTED);

		return rv == tier2_message::ABORTED;
	}
	else
	{
		// TODO: push cancel message in the active cancel set for the receiver/processor.
	}
	return -1;
}












