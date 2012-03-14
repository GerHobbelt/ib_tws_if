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


#include "system-includes.h"

#include "support-code.h"


static __inline char D(char dirsep)
{
	if (dirsep == '\\')
		return '/';
	return dirsep;
}

static __inline char *strnmov_D(char *dst, const char *src, size_t dstlen)
{
	dstlen--;
	while (dstlen > 0 && *src)
	{
		*dst++ = D(*src++);
		dstlen--;
	}
	*dst = 0;
	return dst;
}



const char *filename(const char *path)
{
    const char *delims = "/\\:";

    for ( ; *delims; delims++)
    {
        const char *p = strrchr(path, *delims);
        if (p) path = p + 1;
    }
    return path;
}

char *dirname(char *path, char **end)
{
	char * fnp = (char *)filename(path);
	fnp--;
	// now strip off any trailing '/' or '\\':
	while (fnp >= path && strchr("/\\", *fnp))
		fnp--;
	if (fnp > path && *fnp != ':')
	{
		if (end)
			*end = fnp + 1;
		else
			fnp[1] = 0;
		return path;
	}
	return NULL;
}


char *readfile(const char *path, size_t *read_length)
{
    char *buf;
    char *dst;
    size_t size = 1024 * 1024 * 8;
    size_t fill = 0;
    int len;
    const size_t CHUNKSIZE = 1024;
	FILE *fin = ((path && path[0]) ? fopen(path, "rb") : stdin);

    dst = buf = (char *)malloc(size + 4);
    if (!buf) goto fail_dramatically;
    while (!feof(fin))
    {
        if (size - fill < CHUNKSIZE)
        {
            size *= 2;
            buf = (char *)realloc(buf, size + 4);
            if (!buf) goto fail_dramatically;
            dst = buf + fill;
        }
        len = fread(dst, 1, size - fill, fin);
        if (len > 0)
        {
            dst += len;
            fill += len;
        }
        else if (len < 0)
        {
            goto fail_dramatically;
        }
    }
    *read_length = fill;
    *dst++ = 0;
    *dst++ = 0;
    *dst++ = 0;
    *dst++ = 0;
	if (fin != stdin) fclose(fin);
    return buf;

fail_dramatically:
	free(buf);
	if (fin) if (fin != stdin) fclose(fin);
	return NULL;
}

int writefile(const char *path, const char *buf, size_t size, int append_mode)
{
    int len = -1;
	FILE *fout;

	// make sure the subdir exists:
	char dir[PATH_MAXSIZE];
	char *fnp;
	int rv;

	strnmov_D(dir, path, PATH_MAXSIZE);
	fnp = dirname(dir, NULL);
	if (fnp)
	{
		rv = create_directories(dir);
		if (rv)
			return rv;
	}

	fout = ((path && path[0]) ? fopen(path, (append_mode ? "a" : "w")) : stdout);
	if (fout)
	{
		len = fwrite(buf, 1, size, fout);
		if (fout != stdout) fclose(fout);
	}
    return len;
}

char *strtolower(char *s)
{
	while (*s)
	{
		if (*s >= 'A' && *s <= 'Z')
		{
			*s += 'a' - 'A'; // (char)tolower(*s);
		}
		s++;
	}
	return s;
}

char *strntolower(char *d, size_t len, const char *s)
{
	len--;
	while (*s && len)
	{
		*d++ = *s++;
		if (s[-1] >= 'A' && s[-1] <= 'Z')
		{
			d[-1] += 'a' - 'A'; // (char)tolower(s[-1]);
		}
		len--;
	}
	*d = 0;
	return d;
}



char *concat_path(char *buf, size_t bufsize, const char *path, ...)
{
	va_list a;
	const char *p;
	char *e;
	char *d;
	size_t remaining_bufsize;

	if (!buf || bufsize < 2)
		return NULL;

	d = strnmov_D(buf, path, bufsize);
	e = buf + bufsize;
	remaining_bufsize = bufsize + buf - d - 1;
	if (d == buf || !buf)
		return NULL;

	va_start(a, path);
	while ((p = va_arg(a, const char *)) && d + 2 < e)
	{
		// skip empty path section specs
		if (!p[0])
			continue;

		if (D(d[-1]) != '/' && D(p[0]) != '/')
		{
			*d++ = '/';
			remaining_bufsize--;
		}
		d = strnmov_D(d, p, remaining_bufsize);
	}
	va_end(a);

	if (d >= e)
		return NULL;

	*d = 0;

	// reduce path?
	d = buf;
	for (;;)
	{
		char *r;

		r = strstr(d, "/.");
		if (!r)
			break;

		if (r[2] == '/')
		{
			// reduce '/./':
			memmove(r + 1, r + 3, strlen(r + 3) + 1);
			d = r;
		}
		else if (r[2] == '.' && r[3] == '/')
		{
			// reduce '/../':
			char *o = r - 1;
			while (o > buf && *o != '/' && *o != ':')
				o--;
			// only reduce when a parent directory has been specified; 
			// since we reduce from left to right, we might assume that said
			// parent wouldn't be '../' itself, but it /might/ when you start
			// out with a sequence like '../../../' as the initial '../' will
			// be skipped.
			if (o >= buf && *o == '/' && strncmp(o + 1, "../", 3) != 0)
			{
				memmove(o + 1, r + 4, strlen(r + 4) + 1);
				d = o;
			}
			else
			{
				d = r + 3;
			}
		}
		else
		{
			d = r + 2;
		}
	}
	
	return buf;
}



int create_directories(const char *path)
{
	int rv = CreateDirectoryA(path, NULL);
	if (!rv)
	{
		DWORD err = GetLastError();
		if (err == ERROR_ALREADY_EXISTS)
			return 0;
		if (err == ERROR_PATH_NOT_FOUND)
		{
			char *sep;
			char *fnp = dirname((char *)path, &sep);

			if (fnp)
			{
				char c = *sep;
				*sep = 0;

				rv = create_directories(path);
				*sep = c;
				if (rv == 0)
				{
					int rv = CreateDirectoryA(path, NULL);
					return (!rv ? GetLastError() : 0);
				}
				return rv;
			}
			return 0;
		}
		return err;
	}
	return 0;
}


int movefile(const char *spath, const char *dpath)
{
	int rv;

	// make sure the subdir exists:
	char dir[PATH_MAXSIZE];
	char *fnp;

	strncpy(dir, dpath, PATH_MAXSIZE);
	fnp = (char *)filename(dir);
	fnp--;
	// now strip off any trailing '/' or '\\':
	while (fnp >= dir && strchr("/\\", *fnp))
		fnp--;
	if (fnp > dir && *fnp != ':')
	{
		fnp[1] = 0;
		rv = create_directories(dir);
		if (rv)
			return rv;
	}

	rv = MoveFileA(spath, dpath);
	if (!rv)
	{
		rv = GetLastError();
		if (rv == ERROR_FILE_EXISTS || rv == ERROR_ALREADY_EXISTS)
		{
			fprintf(stderr, "WARNING: Skipped existing target, as it already exists: [%s]\n", dpath);
			rv = 0;
		}
	}
	else
	{
		rv = 0;
	}
	return rv;
}

