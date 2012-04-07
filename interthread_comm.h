
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

#ifndef INTERTHREAD_COMMUNICATIONS_HEADER_INCLUDED
#define INTERTHREAD_COMMUNICATIONS_HEADER_INCLUDED

struct mg_connection;
class tier2_message;
class tier2_queue_item;



/*
A wrapper for the socketpair used for passing messages to & from the
thread to a (predetermined) destination thread.
*/
class interthread_communicator
{
protected:
	// the socket pair:
	struct mg_connection *outgoing;
	struct mg_connection *incoming;

	interthread_communicator *slave;

public:
	/*
		each inter-thread connection is master-slave.
	*/
	interthread_communicator(struct mg_connection *conns[2]) :
		outgoing(conns[0]),
		incoming(conns[1]),
		slave(0)
	{
	}
protected:
	interthread_communicator(struct mg_connection *out, struct mg_connection *in) :
		outgoing(out),
		incoming(in),
		slave(0)
	{
	}
public:
	virtual ~interthread_communicator()
	{
	}

public:
	int send(tier2_queue_item *message);
	tier2_queue_item *receive(void);

	// and these are used with select() to make threads notice they're being invoked:
	void fd_set(struct fd_set *set, int *max_fd);
	int fd_isset(struct fd_set *set);

	// and a helper which provides us with the connected slave instance:
	interthread_communicator *get_slave(void)
	{
		if (!slave)
		{
			slave = new interthread_communicator(incoming, outgoing);
		}
		return slave;
	}
};




/*
Carries the 'globals' relevant for a particular front-end thread, 
such as:
 - the socketpair to use for message passing to/from the backend
 - the thread-specific connection
 - ...
*/
class frontend_info_manager
{
protected:
	struct mg_connection *thread_conn;

	// the socket pair:
	interthread_communicator *comm;

public:
	frontend_info_manager(struct mg_connection *conn);
	virtual ~frontend_info_manager();

public:
	int send(tier2_message *message);
	tier2_message *receive(void);

	// and these are used with select() to make threads notice they're being invoked:
	void fd_set(struct fd_set *set, int *max_fd);
	int fd_isset(struct fd_set *set);
};




#endif // INTERTHREAD_COMMUNICATIONS_HEADER_INCLUDED
