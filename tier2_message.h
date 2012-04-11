
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

#include "tier2_message_requester.h"



// forward references:
class tier2_message_state_change_handler;



class tier2_message
{
public:
	enum request_state_t
	{
		INIT4PREV = INT_MIN,		// special state so state change handler always sees a state change when we want it to
		DESTRUCTION = -3,			// just before the destructor is invoked: last call!
		FAILED = -2,				// when an error occurred
		ABORTED = -1,
		MSG_INITIALIZED = 0,		// start value
		EXEC_COMMAND,				// before the message is processed
		WAIT_FOR_RESPONSE,			// once the message is processed and a response is expected
		RESPONSE_PENDING,	        // when a response is constructed of multiple messages itself: we're still waiting for a few more...
		RESPONSE_COMPLETE,			// The entire response has been collected (requester must still process it though)
		TASK_COMPLETED,				// The message (and optional response) has been completely processed
	};

protected:
	request_state_t previous_state;
	request_state_t now_state;

	typedef unsigned int unique_id_t;
	unique_id_t unique_msgID;

	tier2_message_requester *requester;
	tier2_message_receiver *receiver;

public:
	tier2_message(tier2_message_requester *from = NULL, tier2_message_receiver *to = NULL, request_state_t s = MSG_INITIALIZED) :
		requester(from),
		receiver(to),
		now_state(s),
		previous_state(INIT4PREV)
	{
		unique_msgID = obtain_next_unique_msgID();

		resolve_requester_and_receiver_issues();
	}

protected:
	virtual ~tier2_message()
	{
		// state(DESTRUCTION); -- can't do that here as derived classes will already have destructed themselves! Hence protected destructor!
		release_unique_msgID();
	}

protected:
	int obtain_next_unique_msgID(void);
	void release_unique_msgID(void);

public:
	tier2_message_requester *get_requester(void) const
	{
		return requester;
	}

	tier2_message_requester *get_receiver(void) const
	{
		return receiver;
	}

	unique_id_t get_uniq_msg_id(void) const
	{
		return unique_msgID;
	}

	// set up the defaults; perform any necessary registration with the app_manager, etc...
	virtual void resolve_requester_and_receiver_issues(void);

	bool matches(unique_id_t id) const
	{
		return id == unique_msgID;
	}

public:
	virtual void destroy(void)
	{
		// this way, the state change will make it through /just before/ the destructor sequence is executed!
		state(DESTRUCTION);

		delete this;
	}

public:
	enum state_change
	{
		ERROR_OCCURRED = -1,
		DONT_CHANGE = 0,
		PROCEED = 1
	};

	request_state_t state(request_state_t new_state)
	{
		if (new_state != now_state)
		{
			state_change rv = handle_state_change(new_state);
			switch (rv)
			{
			default:
			case ERROR_OCCURRED:
				new_state = FAILED;
				break;

			case DONT_CHANGE:
				new_state = now_state;
				break;

			case PROCEED:
				break;
			}
			previous_state = now_state;
			now_state = new_state;
		}
		return now_state;
	}
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

public:
	virtual int transmit(void);
	/* this method is invoked by the back-end when a matching response message is received: */
	virtual int process_response(tier2_message &received_response);

	virtual int transmit_and_wait_for_response(app_manager *mgr);

	/* 
	Invoke this method to cancel a long-running (repetitive) request or...
	Abort the mission:  http://www.menagea3.net/strips-ma3/coop_lungeuhil%EF%BC%9F%EF%BC%9F
	*/
	virtual int cancel_request(void);

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







#endif // TIER2_REQUEST_HEADER_INCLUDED
