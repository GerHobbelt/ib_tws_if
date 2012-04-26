
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

#ifndef IBSRV_SYSTEM_INCLUDES_WRAPPER_H
#define IBSRV_SYSTEM_INCLUDES_WRAPPER_H

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601 // Win7
#endif

/*
make sure we load system headers etc FIRST as windows.h and a few others have the 
very irritating idiosyncracy of loading winsock.h which clashes fataly with
winsock2.h -- and we want the latter to make it through where we need anything
'socky'.
*/
#include <mongoose/mongoose_sys_porting.h>


#ifdef _WIN32
#include <winsvc.h>
#endif // _WIN32

#include <assert.h>
#include <float.h>
#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#if defined(_MSC_VER)
#include <io.h>
#include <windows.h>
#else
#include <unistd.h>
#endif

#ifndef PATH_MAX            
#define PATH_MAX            1024
#endif

#ifdef __cplusplus
// blow away a couple of colliding mongoose porting defines:
#undef write
#undef read

#include <vector>
#include <list>
#include <map>
#include <string>

// typeid / RTTI
#include <typeinfo>

#endif


#if defined(_MSC_VER)
#pragma warning(disable: 4100)  // shut up MSVC about 'unreferenced formal parameter'
#endif


#endif // IBSRV_SYSTEM_INCLUDES_WRAPPER_H
