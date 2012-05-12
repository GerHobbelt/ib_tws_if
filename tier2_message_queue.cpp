
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

#include "tier2_message_queue.h"


void tier2_queue_item::destroy(void)
{
	for (tier2_queue_item_visitor_set_t::iterator i = queue_item_visitors.begin();
		i != queue_item_visitors.end();
		i++)
	{
		tier2_queue_item_visitor *h = *i;

		h->on_item_destroy(*this);
	}
	//delete this;
}


tier2_message::state_change tier2_queue_item::process(tier2_message &msg, tier2_message::request_state_t new_state)
{
	// when the state changes, we might also need to notify the queue that this bugger is gonna go!
	tier2_message::state_change rv = tier2_message::PROCEED;

	if (new_state == tier2_message::ABORTED
		|| (new_state == tier2_message::TASK_COMPLETED
			&& exec_run_count <= 0))
	{
		exec_run_count = -1;

		for (tier2_queue_item_visitor_set_t::iterator i = queue_item_visitors.begin();
			i != queue_item_visitors.end();
			i++)
		{
			tier2_queue_item_visitor *h = *i;

			switch (h->process(*this, new_state))
			{
			default:
			case tier2_message::ERROR_OCCURRED:
				rv = tier2_message::ERROR_OCCURRED;
				break;

			case tier2_message::DONT_CHANGE:
				rv = tier2_message::DONT_CHANGE;
				break;

			case tier2_message::PROCEED:
				continue;
			}
			break;
		}
	}

	return tier2_message::PROCEED;
}



void tier2_queue_item::register_handler(tier2_queue_item_visitor *handler)
{
	for (tier2_queue_item_visitor_set_t::iterator i = queue_item_visitors.begin();
		i != queue_item_visitors.end(); 
		i++)
	{
		tier2_queue_item_visitor *h = *i;

		if (h == handler)
			return;
	}
	queue_item_visitors.push_back(handler);
}

void tier2_queue_item::unregister_handler(tier2_queue_item_visitor *handler)
{
	for (tier2_queue_item_visitor_set_t::iterator i = queue_item_visitors.begin();
		i != queue_item_visitors.end(); 
		i++)
	{
		tier2_queue_item_visitor *h = *i;

		if (h == handler)
		{
			queue_item_visitors.erase(i);
			return;
		}
	}
}
