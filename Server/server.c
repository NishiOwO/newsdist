/**
 * $Id$
 * SPDX-License-Identifier: Unlicense
 */

#define INCLUDE_SOCKET

#include "newsdist.h"

#include <string.h>

#define PLAIN	0
#define PLAIN6	1
#define SECURE	2
#define SECURE6	3

/* I hope your socket() implementation does not return -0xdead */
#define NO_SOCKET -0xdead

int		server_sockets[4];

int
nd_init_server(void)
{
	int		i;
#ifdef HAS_WINSOCK
	WSADATA		wsa;

	WSAStartup(MAKEWORD(2, 0), &wsa);
#endif
	server_sockets[PLAIN] = NO_SOCKET;
	server_sockets[PLAIN6] = NO_SOCKET;
	server_sockets[SECURE] = NO_SOCKET;
	server_sockets[SECURE6] = NO_SOCKET;
#ifdef HAS_SSL
	nd_log_info("TLS/SSL support enabled");
	nd_log_info(nd_get_ssl_version());
#endif

#ifdef HAS_IPV4
	server_sockets[PLAIN] = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#endif
#ifdef HAS_IPV6
	server_sockets[PLAIN6] = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
#endif

#ifdef HAS_SSL
#ifdef HAS_IPV4
	server_sockets[SECURE] = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#endif
#ifdef HAS_IPV6
	server_sockets[SECURE6] = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
#endif
#endif

	for (i = 0; i < sizeof(server_sockets) / sizeof(server_sockets[0]); i++) {
		int		yes = 1;
#ifdef HAS_IPV4
		struct sockaddr_in inet4;
#endif
#ifdef HAS_IPV6
		struct sockaddr_in6 inet6;
#endif
		if (server_sockets[i] == NO_SOCKET)
			continue;
#ifdef HAS_SO_REUSEADDR
		if (setsockopt(server_sockets[i], SOL_SOCKET, SO_REUSEADDR, (void *)&yes, sizeof(yes)) < 0) {
			CLOSE_SOCKET(server_sockets[i]);
			nd_log_notice("setsockopt fail (SO_REUSEADDR)");
			return 1;
		}
#endif
#ifdef HAS_TCP_NODELAY
		if (setsockopt(server_sockets[i], IPPROTO_TCP, TCP_NODELAY, (void *)&yes, sizeof(yes)) < 0) {
			CLOSE_SOCKET(server_sockets[i]);
			nd_log_notice("setsockopt fail (TCP_NODELAY)");
			return 1;
		}
#endif
		if (i & 1) {
			/* IPv6 */
#ifdef HAS_IPV6
			memset(&inet6, 0, sizeof(inet6));
			inet6.sin6_family = AF_INET6;
			inet6.sin6_addr = in6addr_any;
			inet6.sin6_port = htons((i & 2) ? ssl_port : plain_port);
			if (bind(server_sockets[i], (struct sockaddr *)&inet6, sizeof(inet6)) < 0) {
				CLOSE_SOCKET(server_sockets[i]);
				nd_log_notice("bind fail");
				return 1;
			}
#endif
		} else {
			/* IPv4 */
#ifdef HAS_IPV4
			memset(&inet4, 0, sizeof(inet4));
			inet4.sin_family = AF_INET;
			inet4.sin_addr.s_addr = INADDR_ANY;
			inet4.sin_port = htons((i & 2) ? ssl_port : plain_port);
			if (bind(server_sockets[i], (struct sockaddr *)&inet4, sizeof(inet4)) < 0) {
				CLOSE_SOCKET(server_sockets[i]);
				nd_log_notice("bind fail");
				return 1;
			}
#endif
		}
		if (listen(server_sockets[i], 128) < 0) {
			CLOSE_SOCKET(server_sockets[i]);
			nd_log_notice("listen fail");
			return 1;
		}
	}

	return 0;
}

int
nd_loop_server(void)
{
	return 0;
}
