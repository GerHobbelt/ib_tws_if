/*
  Copyright (C) 2012 Ger Hobbelt, www.hebbut.net

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

*/


#ifndef ION_SUPPORT_CODE_H__
#define ION_SUPPORT_CODE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include <assert.h>
#if defined(_MSC_VER)
#define _WIN32_WINNT 0x0601 // Win7
#include <io.h>
#include <windows.h>
#else
#include <unistd.h>
#endif

#ifndef PATH_MAX            
#define PATH_MAX            1024
#endif

#define PATH_MAXSIZE		(PATH_MAX + 2)





const char *filename(const char *path);
char *dirname(char *path, char **end);

char *readfile(const char *path, size_t *read_length);
int writefile(const char *path, const char *buf, size_t size, int append_mode);

int create_directories(const char *path);
int movefile(const char *spath, const char *dpath);

char *strtolower(char *s);
char *strntolower(char *d, size_t len, const char *s);

char *concat_path(char *buf, size_t bufsize, const char *path, ...);

static __inline int striEndsWith(const char *str, const char *end)
{
	size_t l = strlen(str);
	size_t el = strlen(end);

	if (l >= el)
	{
		str += l - el;
		return stricmp(str, end) == 0;
	}
	return 0;
}



#endif /* ION_SUPPORT_CODE_H__ */
