
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

#ifndef TIER2_MESSAGE_REQUESTER_HEADER_INCLUDED
#define TIER2_MESSAGE_REQUESTER_HEADER_INCLUDED

#include "system-includes.h"


// forward reference:
class app_manager;
struct mg_connection;




/*
This is the ID/'SOFI-number' of the one who made the request: 
'he' may be a thread, a client connection, etc.
*/
class requester_id 
{
protected:
	/* optional: the client connection, serving as (part of) the ID */
	mg_connection *client_connection;
	/*
		optional: the thread identifier; as we know we're working 
		on top of mongoose, the thread's conn reference serves this 
		purpose, since we do NOT have a reliable thread number (int)
		there: the only bit that's thread-safe and thread-specific 
		in mongoose is the 'conn' pointer passed to the thread upon
		instantiation.
	*/
	mg_connection *calling_thread_id;
	/* 
		optional: issuer-specified sequence ID; need only be unique for 
		this particular client/thread combo.
	*/
	int sequence_id;

public:
	requester_id(mg_connection *thread, mg_connection *client, int seq_id = 0);
	virtual ~requester_id();

public:
	/* produce the hash value for this id */
	virtual int hash(void) const;

	/* 
	   Produce a new, unique-per-requester, sequence number value.
	   Use this call when you, as a requester, need to generate
	   multiple unique /message/ IDs, i.e. in situations where
	   you are to be identified by multiple IDs, dependent on the
	   context.

	   Also use this to get an initial unique ID number for yourself
	   when you obtain your first ID.
	*/
	static int get_next_sequence_value(requester_id *optional_id = NULL);

	/* getters: */
	mg_connection *get_client_connection(void) const
	{
		return client_connection;
	}
	mg_connection *get_calling_thread_id(void) const
	{
		return calling_thread_id;
	}
	int get_my_sequence_id(void) const
	{
		return sequence_id;
	}
};





/*
This class has a function similar to a 'passport/Ausweis'
as it not only acts as a unique identifier but also carries
all 'requester' related info that's relevant for the message(s)
it is referenced by.
*/
class tier2_message_requester
{
protected:
	// the bits that make up the 'unique ID':
	requester_id *unique_id;
	// additional information attributes:
	app_manager *manager;

public:
	tier2_message_requester(requester_id *id, app_manager *mgr) :
		unique_id(id), manager(mgr)
	{
	}
	virtual ~tier2_message_requester()
	{
	}

public:
	requester_id *get_id(void) const
	{
		return unique_id;
	}
	app_manager *get_appmanager(void) const
	{
		return manager;
	}
};





/*
  A receiver is always a SENDER/REQUESTER as well, as a receiver must be able
  to send the response back to the original requester.
*/
class tier2_message_receiver: public tier2_message_requester
{
public:
	tier2_message_receiver(requester_id *id, app_manager *mgr) :
		tier2_message_requester(id, mgr)
	{
	}
	virtual ~tier2_message_receiver()
	{
	}

public:
	virtual int process_one_queued_tier2_request(app_manager *mgr, fd_set *read_set, fd_set *except_set, int max_fd);
};




#endif // TIER2_MESSAGE_REQUESTER_HEADER_INCLUDED
