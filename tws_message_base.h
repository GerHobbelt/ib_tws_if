
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

#ifndef TWS_REQ_RESP_BASE_MSG_HEADER_INCLUDED
#define TWS_REQ_RESP_BASE_MSG_HEADER_INCLUDED

#include "tier2_message.h"



class tws_reqresp_message: public tier2_message
{
	UNIQUE_TYPE_ID_CLASSDEF();

public:
	tws_reqresp_message(tier2_message_processor *from, tier2_message_processor *to = NULL) :
		tier2_message(from, to)
	{
	}
protected:
	virtual ~tws_reqresp_message()
	{
	}

public:
	virtual bool equal(const tier2_message &alt) const;
	virtual bool equal(const tws_reqresp_message &alt) const;
};






#endif // TWS_REQ_RESP_BASE_MSG_HEADER_INCLUDED
