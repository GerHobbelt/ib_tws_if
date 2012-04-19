
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

#ifndef tier2_message_processor_HEADER_INCLUDED
#define tier2_message_processor_HEADER_INCLUDED

#include "system-includes.h"


// forward reference:
class tier2_message;
class app_manager;
class interthread_communicator;
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

  A processor is always a SENDER/REQUESTER as well, as a processor must be able
  to send the response back to the original requesting processor.
*/
class tier2_message_processor
{
protected:
	// the bits that make up the 'unique ID':
	requester_id *unique_id;
	// additional information attributes:
	app_manager *manager;

	virtual void register_interthread_connection(interthread_communicator *info);

	typedef std::vector<interthread_communicator *> sender_set_t;
	sender_set_t senders;

	typedef std::list<tier2_message *> msg_set_t;
	msg_set_t pending_msgs;

public:
	tier2_message_processor(requester_id *id, app_manager *mgr) :
		unique_id(id), manager(mgr)
	{
	}
	virtual ~tier2_message_processor()
	{
	}

public:
	requester_id *get_id(void) const
	{
		return unique_id;
	}
	app_manager *get_app_manager(void) const
	{
		return manager;
	}

public:
	virtual void register_sender(tier2_message_processor *sender);
	virtual interthread_communicator *get_interthread_communicator(tier2_message_processor *from, tier2_message_processor *to);

	virtual int prepare_fd_sets_for_reception(fd_set *read_set, fd_set *except_set, int &max_fd);
	virtual int process_one_queued_tier2_request(fd_set *read_set, fd_set *except_set, int max_fd);

	virtual int own(tier2_message *msg);
	virtual int release(tier2_message *msg);
	virtual bool does_own(tier2_message *msg) const;
};







#endif // tier2_message_processor_HEADER_INCLUDED
