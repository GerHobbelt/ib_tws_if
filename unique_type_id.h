
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


class unique_type_id
{
protected:
	unique_id_t id_nr;

private:
	static unique_id_t prev_id;

public:
	unique_type_id()
	{
		id_nr = get_next_id();
	}
	virtual ~unique_type_id()
	{
	}

protected:
	unique_id_t get_next_id(void)
	{
		return ++prev_id;
	}

public:
	unique_id_t id(void) const
	{
		return id_nr;
	}
	operator unsigned int(void) const
	{
		return (unsigned int)(id_nr);
	}
};







#endif // UNIQUE_TYPE_ID_CALCULATOR_HEADER_INCLUDED
