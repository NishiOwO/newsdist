/**
 * $Id$
 */

#define INCLUDE_UTSNAME
#define INCLUDE_SOCKET

#include <stdlib.h>
#include <string.h>

#include "newsdist.h"

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
#elif defined(IS_OS2)
	return nd_strdup("OS2");
#elif defined(IS_NETWARE)
	return nd_strdup("NetWare");
#else
	return nd_strdup("Unknown");
#endif
}

#ifndef HAS_HTONS
uint16_t
htons(uint16_t n) {
	return ((n >> 8) & 0xff) | ((n << 8) & 0xff00);
}
#endif

char *
nd_gethostname(void)
{
	char	       *host = malloc(513);
#if defined(HAS_GETHOSTNAME)
	gethostname(host, 512);
#elif defined(HAS_UNAME)
	struct utsname	u;

	uname(&u);
	strcpy(host, u.nodename);
#else
	strcpy(host, "unknown");
#endif
	return host;
}


