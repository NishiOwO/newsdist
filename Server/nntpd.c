/**
 * $Id$
 */

#define INCLUDE_SOCKET

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "newsdist.h"

int
nd_nntpd_status(nd_pass_t *p, int status, const char *message)
{
	char	       *r = malloc(4 + strlen(message) + 2 + 1);
	int		st;

	sprintf(r, "%d %s\r\n", status, message);
	st = nd_write(p, r, strlen(r));
	free(r);
	return st <= 0 ? -1 : 0;
}

char *
nd_nntpd_read_line(nd_pass_t *p)
{
	char	       *buffer = malloc(1);
	unsigned long	len = 0;

	buffer[0] = 0;

	while (1) {
		if (nd_timeout(p, 1000) < 0) {
			free(buffer);
			return NULL;
		} else {
			char		c;

			nd_read(p, &c, 1);
			if (c == '\n') {
				break;
			} else if (c != '\r') {
				char	       *old = buffer;

				buffer = malloc(len + 2);
				strcpy(buffer, old);
				buffer[len] = c;
				buffer[len + 1] = 0;
				len++;
				free(old);
			}
		}
	}
	return buffer;
}

void
nd_nntpd_handle(nd_pass_t *p)
{
	char	       *fmt = nd_format(welcome_text);

	if (nd_nntpd_status(p, 200, fmt)) {
		printf("!\n");
		free(fmt);
		return;
	}
	free(fmt);
	while (1) {
		char	       *l = nd_nntpd_read_line(p);

		if (l == NULL)
			break;
		if (nd_strcaseequ(l, "CAPABILITIES")) {
			nd_write(p, "101 Here:\r\n", 3 + 1 + 4 + 1 + 2);
			nd_write(p, ".\r\n", 3);
		} else {
			nd_write(p, "500 Pardon?\r\n", 3 + 1 + 6 + 1 + 2);
		}
		free(l);
	}
}
