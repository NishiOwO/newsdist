
/**
 * $Id$
 */

#define INCLUDE_UTSNAME
#define INCLUDE_SOCKET

#include <stdlib.h>
#include <string.h>

#include "newsdist.h"

char *
nd_strcat(const char *a, const char *b)
{
	char	       *str = malloc(strlen(a) + strlen(b) + 1);

	strcpy(str, a);
	strcpy(str + strlen(a), b);
	return str;
}

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

char *
nd_format(const char *str)
{
	char	       *r = malloc(1);
	int		i;

	r[0] = 0;
	for (i = 0; str[i] != 0; i++) {
		if (str[i] == '%') {
			i++;
			if (str[i] == 'n') {
				char	       *h = nd_gethostname();
				char	       *tmp = r;

				r = nd_strcat(r, h);
				free(tmp);
				free(h);
			} else if (str[i] == 'v') {
				char	       *tmp = r;

				r = nd_strcat(r, NEWSDIST_VERSION);
				free(tmp);
			} else if (str[i] == 'p') {
				char	       *t = nd_get_system();
				char	       *tmp = r;

				r = nd_strcat(r, t);
				free(tmp);
				free(t);
			} else {
				char		cbuf[2];
				char	       *tmp;

				cbuf[0] = str[i];
				cbuf[1] = 0;
				tmp = r;
				r = nd_strcat(r, cbuf);
				free(tmp);
			}
		} else {
			char		cbuf[2];
			char	       *tmp;

			cbuf[0] = str[i];
			cbuf[1] = 0;
			tmp = r;
			r = nd_strcat(r, cbuf);
			free(tmp);
		}
	}
	return r;
}
