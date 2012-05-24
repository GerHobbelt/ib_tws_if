
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

#ifndef TIER2_MESSAGE_QUEUE_HEADER_INCLUDED
#define TIER2_MESSAGE_QUEUE_HEADER_INCLUDED

#include "system-includes.h"
#include "tier2_message.h"



// forward reference:
class app_manager;
class tier2_queue_item_visitor;



class tier2_queue_item: public tier2_message_state_change_handler
{
protected:
	tier2_message *m_request;

	int m_priority;							// higher is more important

	// The moment this request should become 'active', i.e. should be executed
	time_t m_activation_time;
	// and the number of times this command should be executed at the given interval (seconds)
	int m_exec_run_count;
	unsigned int m_exec_time_interval;

	// The last time this request has been sent
	time_t m_last_transmit_time;
	// The last time a response for this request has been received
	time_t m_last_response_time;

public:
	tier2_queue_item(tier2_message *msg, 
			time_t activate = 0, 
			int run_count = 1,
			int interval = 3600,
			int prio = 0) :
		m_request(msg),
		m_activation_time(activate),
		m_exec_run_count(run_count <= 0 ? 1 : run_count), // make sure the message is run at least once
		m_exec_time_interval(interval),
		m_last_transmit_time(0),
		m_last_response_time(0),
		m_priority(prio)
	{
		m_request->register_handler(this);
	}

private:
	virtual ~tier2_queue_item()
	{
		m_request->unregister_handler(this);
	}
public:
	void destroy(void);

protected:
	typedef std::vector<tier2_queue_item_visitor *> tier2_queue_item_visitor_set_t;
	friend tier2_queue_item_visitor;

	tier2_queue_item_visitor_set_t queue_item_visitors;

public:
	virtual void register_handler(tier2_queue_item_visitor *handler);
	virtual void unregister_handler(tier2_queue_item_visitor *handler);

public:
	virtual tier2_message::state_change process(tier2_message &msg, tier2_message::request_state_t new_state);

public:
	virtual int abort(void)
	{
		m_exec_run_count = -1;
		m_request->state(tier2_message::ABORTED);
	}

	tier2_message *message(void) const
	{
		return m_request;
	}

public:
	/*
	Check if this queue item is eligible for execution and when it is,
	also check/update the reference to the eligible one (if available)
	with the highest priority.
	*/
	virtual bool is_eligible_for_exec(tier2_queue_item *&chosen, time_t timestamp)
	{
		if (m_exec_run_count > 0
			&& (m_activation_time == 0
				|| m_activation_time >= timestamp)
			&& (!chosen 
				|| m_priority >= chosen->m_priority)
			)
		{
			chosen = this;
			return true;
		}
		return false;
	}

	virtual void calc_next_invocation(time_t timestamp)
	{
		// update the periodical when it's a repeat performance request:
		if (--m_exec_run_count > 0)
		{
			m_activation_time += m_exec_time_interval;

			// and cope with the situation where the original request had an activation timestamp of zero or pickup was much delayed
			if (m_activation_time < timestamp)
			{
				m_activation_time = timestamp + m_exec_time_interval;
			}
		}
	}
};








/*
Visitor
*/
class tier2_queue_item_visitor
{
public:
	virtual tier2_message::state_change process(tier2_queue_item &msg, tier2_message::request_state_t new_state) = 0;

	// invoked when queue item is destroyed:
	virtual void on_item_destroy(tier2_queue_item &msg) = 0;
};







/* 
Each thread keeps its own queue of incoming/pending messages. 

This also serves as part of a 'scheduler' for messages which 
represent a 'scheduled', i.e. recurring operation.

The cross-border travel of messages is regulated through the 
socketpairs which represent the allowed message paths in this
application. The socketpairs are managed/tracked elsewhere; we
are merely the queue bit here...
*/
class tier2_message_queue: public tier2_queue_item_visitor
{
protected:
	typedef std::vector<tier2_queue_item *> work_queue_t;

protected:
    work_queue_t m_work_queue;

    size_t m_poppos;         // the position of the queue 'head' for popping

public:
	tier2_message_queue() :
		m_poppos(0)
	{
	}

	virtual ~tier2_message_queue() 
	{
		while (!m_work_queue.empty())
		{
			tier2_queue_item *i = m_work_queue.back();
			m_work_queue.pop_back();

			i->abort();

			// TODO: wait for the front-ends to recognize this change of affairs.
		}

		m_work_queue.clear();
		m_poppos = 0;
	}

public:
	virtual tier2_message::state_change process(tier2_queue_item &msg, tier2_message::request_state_t new_state)
	{
		return tier2_message::PROCEED;
	}

	// invoked when queue item is destroyed:
	virtual void on_item_destroy(tier2_queue_item &msg)
	{
		for (work_queue_t::iterator i = m_work_queue.begin();
			i != m_work_queue.end(); 
			i++)
		{
			tier2_queue_item *e = *i;

			if (e == &msg)
			{
				msg.unregister_handler(this);
				m_work_queue.erase(i);
			}
		}
	}

public:
	void push(tier2_queue_item *cmd)
	{
		cmd->message()->state(tier2_message::EXEC_COMMAND);

		cmd->register_handler(this);
		m_work_queue.push_back(cmd);
	}

	/*
	Check which queued request has become active (round robin!) and 
	return a reference to the queue entry. 
	
	Return NULL when there is no pending entry.
	*/
	tier2_queue_item *fetch(void)
	{
		time_t timestamp = time(NULL);
		tier2_queue_item *result = NULL;
		size_t i;
		size_t candidate = 0;
		size_t work_queue_size = m_work_queue.size();

		for (i = (m_poppos + 1) % work_queue_size; ; i = (i + 1) % work_queue_size)
		{
			tier2_queue_item *item = m_work_queue[i];
			if (item->is_eligible_for_exec(result, timestamp))
			{
				// round robin polling of the queue:
				candidate = i;
			}
			if (i == m_poppos)
				break;
		}
		m_poppos = candidate;

		if (result)
		{
			result->calc_next_invocation(timestamp);
		}
		return result;
	}

	// utility calls:
	size_t get_queue_depth(void)
	{
		size_t result = m_work_queue.size();

		return result;
	}
};



#endif // TIER2_MESSAGE_QUEUE_HEADER_INCLUDED
