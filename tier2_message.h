
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

#ifndef TIER2_REQUEST_HEADER_INCLUDED
#define TIER2_REQUEST_HEADER_INCLUDED

#include "system-includes.h"

#include "tier2_message_processor.h"
#include "unique_type_id.h"


/*
Messages are sent between threads (and sometimes a message is sent from a thread to itself).

Rules:

- messages are ALWAYS returned to their requester for destruction ~ the one who created the message is the one who must destroy it at the end.

- the above applies under ALL circumstances, i.e. also when message processing failed or was canceled: that's what state() is for.

- There's always one owner of a message, UNLESS a message is being transferred to another thread, in which case the message has NO OWNER: it is 'in limbo'.


Considerations:

We might have gone and made the effort to track 'in limbo' messages in order to clean them up / recover them when the interconnect they're 'in limbo' on fails
for whatever reason (IP stack hickup, threads crashing, ...) but any message that REMAINS in limbo for a significant amount of time is

a) a clear indication that something is very rotten indeed and the application/server should have power-cycled already

b) adding this 'tracking in limbo messages' feature would imply adding locks around this common/shared code, while using the socketpair()s' point is 
   precisely the opposite, i.e. the intent that we WON'T need any interthread locks around our message traffic code at all, for the purposes of maximum
   scalability.

*/


// forward references:
class tier2_message_state_change_handler;



class tier2_message
{
public:
	enum request_state_t
	{
		INIT4PREV = INT_MIN,		// T: special state so state change handler always sees a state change when we want it to
		DESTRUCTION = -3,			// T: just before the destructor is invoked: last call!
		FAILED = -2,				// T: when an error occurred
		ABORTED = -1,				// T: when the request has been canceled
		MSG_INITIALIZED = 0,		// T: start value
		EXEC_COMMAND,				// R: before the message is processed, i.e. send the message off to the designated processor (task)
		WAIT_FOR_TRANSMIT,          // R: message has been received by the 'processor'/'end node' and is waiting for clearance to be transmitted / executed
		COMMENCE_TRANSMIT,          // R: message is been processed / transmitted to entity outside this application
		READY_TO_RECEIVE_RESPONSE,	// R: once the message is processed and a response is expected
		RESPONSE_PENDING,	        // R: when a response is constructed of multiple messages itself: we're still waiting for a few more...
		RESPONSE_COMPLETE,			// T: The entire response has been collected (requester must still process it though)
		TASK_COMPLETED,				// T: The message (and optional response) has been completely processed
	};

protected:
	request_state_t previous_state;
	request_state_t now_state;

	unique_id_t unique_msgID;

	tier2_message_processor *requester;
	tier2_message_processor *receiver;
	tier2_message_processor *owner;

protected:
	typedef std::vector<unique_id_t> unique_id_list_t;
	unique_id_list_t acceptable_responses;

public:
	tier2_message(tier2_message_processor *from = NULL, tier2_message_processor *to = NULL, request_state_t s = MSG_INITIALIZED);

protected:
	virtual ~tier2_message();

protected:
	unique_id_t obtain_next_unique_msgID(void);
	void release_unique_msgID(void);

public:
	tier2_message_processor *get_requester(void) const
	{
		return requester;
	}

	tier2_message_processor *get_receiver(void) const
	{
		return receiver;
	}

	tier2_message_processor *current_owner(tier2_message_processor *new_owner);
	tier2_message_processor *current_owner(void) const
	{
		return owner;
	}

	// set up the defaults; perform any necessary registration with the app_manager, etc...
	virtual void resolve_requester_and_receiver_issues(void);

	unique_id_t get_uniq_msg_id(void) const
	{
		return unique_msgID;
	}

	virtual bool matches(unique_id_t id) const
	{
		return id == unique_msgID;
	}

	virtual bool matches(tier2_message *msg) const
	{
		return msg == this;
	}

	virtual bool response_is_meant_for_us(tier2_message *resp_msg) const
	{
		/* accept NO responses by default */
		return false;
	}

public:
	virtual void destroy(void);

public:
	enum state_change
	{
		ERROR_OCCURRED = -1,
		DONT_CHANGE = 0,
		PROCEED = 1
	};

	request_state_t state(request_state_t new_state);
	request_state_t state(void) const
	{
		return now_state;
	}

protected:
	typedef std::vector<tier2_message_state_change_handler *> state_change_handler_set_t;
	friend tier2_message_state_change_handler;

	state_change_handler_set_t state_change_handlers;

public:
	virtual state_change handle_state_change(request_state_t new_state);
	virtual void register_handler(tier2_message_state_change_handler *handler);
	virtual void unregister_handler(tier2_message_state_change_handler *handler);

protected:
	// before the message is processed, i.e. send the message off to the designated processor (task)
	virtual int f_exec_command(void); 
	// message has been received by the 'processor'/'end node' and is waiting for clearance to be transmitted / executed
	virtual int f_wait_for_transmit(void);
	// message is been processed / transmitted to entity outside this application
	virtual int f_commence_transmit(void);
	// once the message is processed and a response is expected
	virtual int f_ready_to_receive_response(void);
	// when a response is constructed of multiple messages itself: we're still waiting for a few more...
	virtual int f_response_pending(void);
	// The entire response has been collected (requester must still process it though)
	virtual int f_response_complete(void);
	// The message (and optional response) has been completely processed
	virtual int f_task_completed(void);
	
public:
	/* this method is invoked by the back-end when a matching response message is received: */
	virtual int process_response(tier2_message *received_response);

	virtual int wait_for_response(void);

	/* 
	Invoke this method to cancel a long-running (repetitive) request or...
	Abort the mission:  http://www.menagea3.net/strips-ma3/coop_lungeuhil%EF%BC%9F%EF%BC%9F
	*/
	virtual int cancel_request(tier2_message_processor *transmitter);

	virtual int store(void);
	virtual int load(void);

	virtual bool equal(const tier2_message &alt) const;
};





/*
Visitor
*/
class tier2_message_state_change_handler
{
public:
	virtual tier2_message::state_change process(tier2_message &msg, tier2_message::request_state_t new_state) = 0;
};







class cancel_message: public tier2_message
{
protected:
	tier2_message *refd_msg;

public:
	cancel_message(tier2_message_processor *from, tier2_message *referenced_msg) :
	  tier2_message(from, referenced_msg->get_receiver()),
		  refd_msg(referenced_msg)
	  {
	  }
protected:
	virtual ~cancel_message()
	{
	}

protected:
	virtual int f_exec_command(void);

};







class schedule_message: public tier2_message
{
protected:
	tier2_message *refd_msg;

	int priority;							// higher is more important

	// The moment this request should become 'active', i.e. should be executed
	time_t activation_time;
	// and the number of times this command should be executed at the given interval (seconds)
	int exec_run_count;
	unsigned int exec_time_interval;

	// The last time this request has been sent
	time_t last_transmit_time;
	// The last time a response for this request has been received
	time_t last_response_time;

public:
	schedule_message(tier2_message_processor *from, tier2_message *referenced_msg, time_t activate = 0, int run_count = 1, int interval = 3600, int prio = 0) :
		tier2_message(from, referenced_msg->get_receiver()),
		refd_msg(referenced_msg),
		activation_time(activate),
		exec_run_count(run_count <= 0 ? 1 : run_count), // make sure the message is run at least once
		exec_time_interval(interval),
		last_transmit_time(0),
		last_response_time(0),
		priority(prio)
	{
	}
protected:
	virtual ~schedule_message()
	{
	}

protected:
	virtual int f_exec_command(void);

};







#endif // TIER2_REQUEST_HEADER_INCLUDED
