
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


#if defined(_WIN32)
#define _CRT_SECURE_NO_WARNINGS  // Disable deprecation warning in VS2005
#else
#define _XOPEN_SOURCE 600  // For PATH_MAX on linux
#endif

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdarg.h>


#ifdef _WIN32

#include <windows.h>
#include <winsvc.h>

#if defined(_MSC_VER)

#define PATH_MAX MAX_PATH
#define S_ISDIR(x) ((x) & _S_IFDIR)

#endif // _MSC_VER

#define DIRSEP '\\'
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#define sleep(x) Sleep((x) * 1000)
#define WINCDECL __cdecl

#else

#include <sys/wait.h>
#include <unistd.h>

#define DIRSEP '/'
#define WINCDECL

#endif // _WIN32

#include <assert.h>
#include <time.h>
#include <float.h>
#include <math.h>

#include <pthread.h>

#include <mongoose/mongoose.h>
#include <mongoose/mongoose_ex.h>


#if defined(_MSC_VER)
#pragma warning(disable: 4100)  // shut up MSVC about 'unreferenced formal parameter'
#endif



#ifdef __cplusplus
extern "C" {
#endif // __cplusplus






/*
===============================================================================

--- utility functions ---

===============================================================================
*/

/*
log a message and terminate the application
*/
void die(const char *fmt, ...);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // TWS_MONGOOSE_PORTING_HEADER_INCLUDED
