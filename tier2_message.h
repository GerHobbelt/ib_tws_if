
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


// forward reference:
class my_tws_io_info;





class tier2_message;
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

public:
	tier2_message(request_state_t s = MSG_INITIALIZED) :
		now_state(s),
		previous_state(INIT4PREV)
	{
	}
protected:
	virtual ~tier2_message()
	{
		// state(DESTRUCTION); -- can't do that here as derived classes will already have destructed themselves! Hence protected destructor!
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
	virtual int transmit(my_tws_io_info *info) = 0;
	/* invoke this method to cancel a long-running (repetitive) request. */
	virtual int cancel_request(my_tws_io_info *info) = 0;
	/* this method is invoked by the backend when a matching response message is received: */
	virtual int process_response(my_tws_io_info *info, tier2_message &response) = 0;

	virtual int store(my_tws_io_info *info) = 0;
	virtual int load(my_tws_io_info *info) = 0;

	virtual bool equal(const tier2_message &alt) const = 0;
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
