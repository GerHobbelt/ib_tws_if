
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

#ifndef UNIQUE_TYPE_ID_CALCULATOR_HEADER_INCLUDED
#define UNIQUE_TYPE_ID_CALCULATOR_HEADER_INCLUDED

#include "system-includes.h"



typedef unsigned int unique_id_t;


class unique_type_id_manager
{
private:
	unique_id_t prev_id;

public:
	unique_type_id_manager(unique_id_t start_id = 1)
		: prev_id(start_id - 1)
	{
	}
	virtual ~unique_type_id_manager()
	{
	}

public:
	virtual unique_id_t obtain_unique_id(void)
	{
		return ++prev_id;
	}
	virtual unique_id_t reset_unique_id(unique_id_t new_start_value)
	{
		prev_id = new_start_value;
		return prev_id;
	}
	virtual unique_id_t update_unique_id(unique_id_t new_start_value)
	{
		if (prev_id < new_start_value)
		{
			prev_id = new_start_value;
		}
		return prev_id;
	}
};




class unique_type_id_threadsafe_manager : public unique_type_id_manager
{
private:
	pthread_spinlock_t m_lock;

public:
	unique_type_id_threadsafe_manager(unique_id_t start_id = 1)
		: unique_type_id_manager(start_id - 1)
	{
		pthread_spin_init(&m_lock, false);
	}
	virtual ~unique_type_id_threadsafe_manager()
	{
		pthread_spin_destroy(&m_lock);
	}

public:
	virtual unique_id_t obtain_unique_id(void)
	{
		unique_id_t rv;

		pthread_spin_lock(&m_lock);
		rv = __super::obtain_unique_id();
		pthread_spin_unlock(&m_lock);
		return rv;
	}
	virtual unique_id_t reset_unique_id(unique_id_t new_start_value)
	{
		unique_id_t rv;

		pthread_spin_lock(&m_lock);
		rv = __super::reset_unique_id(new_start_value);
		pthread_spin_unlock(&m_lock);
		return rv;
	}
	virtual unique_id_t update_unique_id(unique_id_t new_start_value)
	{
		unique_id_t rv;

		pthread_spin_lock(&m_lock);
		rv = __super::update_unique_id(new_start_value);
		pthread_spin_unlock(&m_lock);
		return rv;
	}
};





#define UNIQUE_TYPE_ID_CLASSDEF()								\
public:															\
	virtual bool type_matches(const tier2_message *msg) const;	\
	virtual unique_id_t get_type_id(void) const;				\
private:														\
	static unique_id_t m_type_id


#if 0

#define UNIQUE_TYPE_ID_CLASSIMPL(class, manager)				\
bool class::type_matches(const tier2_message *msg)				\
{																\
	return msg->get_type_id() == get_type_id();					\
}																\
unique_id_t class::get_type_id(void)							\
{																\
	if (!m_type_id)												\
	{															\
		m_type_id = manager.obtain_unique_id();					\
	}															\
	return m_type_id;											\
}																\
/* 																\
	uninitialized; 												\
	will be set up as soon as anyone invokes get_type_id() 		\
 */																\
unique_id_t class::m_type_id = manager.obtain_unique_id()

#endif




#endif // UNIQUE_TYPE_ID_CALCULATOR_HEADER_INCLUDED
