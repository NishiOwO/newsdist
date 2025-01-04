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
	return st;
}

char *
nd_nntpd_read_line(nd_pass_t *p)
{
	char	       *buffer = malloc(1);
	unsigned long	len = 0;

	buffer[0] = 0;

	while (1) {
		if (nd_timeout(p, 10000) < 0) {
			free(buffer);
			return NULL;
		} else {
			char		c;

			if (nd_read(p, &c, 1) < 0) {
				free(buffer);
				return NULL;
			}
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
		free(fmt);
		return;
	}
	free(fmt);
	while (1) {
		char	       *l = nd_nntpd_read_line(p);

		if (l == NULL)
			break;
		if (nd_strcaseequ(l, "CAPABILITIES")) {
			nd_write_string(p, "101 Here:\r\n");
			nd_write_string(p, "VERSION 2\r\n");
			nd_write_string(p, "IMPLEMENTATION NewsDist " NEWSDIST_VERSION "\r\n");
			nd_write_string(p, "AUTHINFO\r\n");
			nd_write_string(p, "LIST ACTIVE MOTD NEWSGROUPS\r\n");
			nd_write_string(p, "READER\r\n");
			nd_write_string(p, ".\r\n");
		} else if (nd_strcaseequ(l, "MODE READER")) {
			fmt = nd_format(welcome_text);

			if (nd_nntpd_status(p, 200, fmt)) {
				free(fmt);
				return;
			}
			free(fmt);
		} else if (nd_strcaseequ(l, "LIST MOTD")) {

		} else if (nd_strcaseequ(l, "QUIT")) {
			nd_nntpd_status(p, 205, "Sayonara");
			free(l);
			break;
		} else {
			printf("[%s]\n", l);
			nd_write_string(p, "500 Pardon?\r\n");
		}
		free(l);
	}
}
