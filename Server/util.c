/**
 * $Id$
 * SPDX-License-Identifier: Unlicense
 */

#include "newsdist.h"

#ifdef HAS_UNAME
#include <sys/utsname.h>
#endif
#include <stdlib.h>
#include <string.h>

char *
nd_strdup(const char *str)
{
#ifdef HAS_STRDUP
	return strdup(str);
#else
	char	       *r = malloc(strlen(str) + 1);

	strcpy(r, str);
	return r;
#endif
}

char *
nd_get_system(void)
{
#if defined(HAS_UNAME)
	struct utsname	u;
	char	       *name = malloc(512);

	name[0] = 0;
	uname(&u);
	strcat(name, u.sysname);
	strcat(name, "/");
	strcat(name, u.release);
	return name;
#elif defined(__MINGW32__)
	return nd_strdup("Windows");
#else
	return nd_strdup("Unknown");
#endif
}

#ifndef HAS_HTONS
uint16_t htons(uint16_t n) { return ((n >> 8) & 0xff) | ((n << 8) & 0xff00); }
#endif
