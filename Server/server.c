/**
 * $Id$
 * SPDX-License-Identifier: Unlicense
 */

#include "newsdist.h"

#include "../config.h"

#ifdef HAS_WINSOCK
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif

#define PLAIN	0
#define PLAIN6	1
#define SECURE	2
#define SECURE6	3

int		server_sockets[4];

int
nd_init_server(void)
{
#ifdef HAS_WINSOCK
	WSADATA		wsa;

	WSAStartup(MAKEWORD(2, 0), &wsa);
#endif
	server_sockets[PLAIN] = -1;
	server_sockets[PLAIN6] = -1;
	server_sockets[SECURE] = -1;
	server_sockets[SECURE6] = -1;
#ifdef HAS_SSL
	nd_log_info("TLS/SSL support enabled");
	nd_log_info(nd_get_ssl_version());
#endif
	return 0;
}
