
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
