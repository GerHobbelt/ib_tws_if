
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

#ifndef TWS_MONGOOSE_PORTING_HEADER_INCLUDED
#define TWS_MONGOOSE_PORTING_HEADER_INCLUDED

#include <mongoose/mongoose_sys_porting.h>

#ifdef _WIN32
#include <winsvc.h>
#endif // _WIN32

#include <assert.h>
#include <float.h>
#include <math.h>

#include <mongoose/mongoose.h>
#include <mongoose/mongoose_ex.h>


#if defined(_MSC_VER)
#pragma warning(disable: 4100)  // shut up MSVC about 'unreferenced formal parameter'
#endif







/*
===============================================================================

--- utility functions ---

===============================================================================
*/

/*
log a message and terminate the application
*/
void die(const char *fmt, ...);


#endif // TWS_MONGOOSE_PORTING_HEADER_INCLUDED
