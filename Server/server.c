/**
 * $Id$
 */

#define INCLUDE_SOCKET
#define INCLUDE_ERRNO

#include <stdlib.h>
#include <string.h>

#include "newsdist.h"

#ifdef HAS_NW_BEGINTHREAD
#include <nwthread.h>
#endif
#ifdef HAS_BEGINTHREAD
#include <process.h>
#endif

#define PLAIN	0
#define PLAIN6	1
#define SECURE	2
#define SECURE6	3

/*
 * I hope your socket() implementation does not return -0xdead
 */
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
#ifdef IS_OS2
	sock_init();
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
	server_sockets[PLAIN6] =
		socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
#endif

#ifdef HAS_SSL
#ifdef HAS_IPV4
	if (ssl_key != NULL && ssl_cert != NULL)
		server_sockets[SECURE] =
			socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#endif
#ifdef HAS_IPV6
	if (ssl_key != NULL && ssl_cert != NULL)
		server_sockets[SECURE6] =
			socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
#endif
#endif

	for (i = 0; i < sizeof(server_sockets) / sizeof(server_sockets[0]);
	     i++) {
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
		if (setsockopt
		    (server_sockets[i], SOL_SOCKET, SO_REUSEADDR,
		     (void *)&yes, sizeof(yes)) < 0) {
			CLOSE_SOCKET(server_sockets[i]);
			nd_log_notice("setsockopt fail (SO_REUSEADDR)");
			return 1;
		}
#endif
#ifdef HAS_TCP_NODELAY
		if (setsockopt
		    (server_sockets[i], IPPROTO_TCP, TCP_NODELAY,
		     (void *)&yes, sizeof(yes)) < 0) {
			CLOSE_SOCKET(server_sockets[i]);
			nd_log_notice("setsockopt fail (TCP_NODELAY)");
			return 1;
		}
#endif
		if (i & 1) {
			/*
			 * IPv6
			 */
#ifdef HAS_IPV6
			memset(&inet6, 0, sizeof(inet6));
			inet6.sin6_family = AF_INET6;
			inet6.sin6_addr = in6addr_any;
			inet6.sin6_port =
				htons((i & 2) ? ssl_port : plain_port);
#ifdef HAS_IPV6_V6ONLY

			/*
			 * Better set this explicitly, Linux seems to have
			 * this disabled by default
			 */
			setsockopt(server_sockets[i], IPPROTO_IPV6,
				   IPV6_V6ONLY, (void *)&yes,
				   sizeof(yes));
#endif
			if (bind
			    (server_sockets[i], (struct sockaddr *)&inet6,
			     sizeof(inet6)) < 0 && errno != ENETUNREACH) {
				/*
				 * ENETUNREACH if there is no IPv6 assigned,
				 * confirmed on UnixWare 7
				 */
				CLOSE_SOCKET(server_sockets[i]);
				nd_log_notice("bind fail");
				return 1;
			}
#endif
		} else {
			/*
			 * IPv4
			 */
#ifdef HAS_IPV4
			memset(&inet4, 0, sizeof(inet4));
			inet4.sin_family = AF_INET;
			inet4.sin_addr.s_addr = INADDR_ANY;
			inet4.sin_port =
				htons((i & 2) ? ssl_port : plain_port);
			if (bind
			    (server_sockets[i], (struct sockaddr *)&inet4,
			     sizeof(inet4)) < 0 && errno != ENETUNREACH) {
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

void
nd_pass(void *tptr)
{
	nd_pass_t      *ptr = (nd_pass_t *) tptr;

	if (nd_accept_ssl(ptr) < 0) {
		nd_close_socket(ptr);
		free(ptr);
	} else {
		nd_nntpd_handle(ptr);
	}
#ifdef HAS_NW_BEGINTHREAD
	ExitThread(EXIT_THREAD, 0);
#endif
#ifdef HAS_ENDTHREAD
	_endthread();
#endif
}

int
nd_loop_server(void)
{
	int		i;

#if defined(HAS_POLL)

	/*
	 * This is preferred way
	 */
	int		count = 0;
	struct pollfd  *fds;

	for (i = 0; i < sizeof(server_sockets) / sizeof(server_sockets[0]);
	     i++) {
		if (server_sockets[i] != NO_SOCKET)
			count++;
	}
	fds = malloc(sizeof(*fds) * count);
	count = 0;
	for (i = 0; i < sizeof(server_sockets) / sizeof(server_sockets[0]);
	     i++) {
		if (server_sockets[i] == NO_SOCKET)
			continue;
		fds[i].fd = server_sockets[i];
		fds[i].events = POLLIN | POLLPRI;
		count++;
	}
#elif defined(HAS_SELECT)
	fd_set		fdset;
	struct timeval	tv;
#endif
	while (1) {
		int		n;

#if defined(HAS_POLL)
		n = poll(fds, count, 1000);
#elif defined(HAS_SELECT)
		FD_ZERO(&fdset);
		for (i = 0;
		     i <
		     sizeof(server_sockets) / sizeof(server_sockets[0]);
		     i++) {
			if (server_sockets[i] == NO_SOCKET)
				continue;
			FD_SET(server_sockets[i], &fdset);
		}
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		n = select(FD_SETSIZE, &fdset, NULL, NULL, &tv);
#endif
		if (n < 0) {
			if (errno == EINTR)
				continue;
			nd_log_notice("select/poll fail");
			break;
		} else if (n > 0) {
			/*
			 * Connection
			 */
			int		incr = 0;

			for (i = 0;
			     i <
			     sizeof(server_sockets) /
			     sizeof(server_sockets[0]); i++) {
				int		go = 0;

				if (server_sockets[i] == NO_SOCKET)
					continue;
#if defined(HAS_POLL)
				if (fds[incr].revents & POLLIN)
					go = 1;
#elif defined(HAS_SELECT)
				if (FD_ISSET(server_sockets[i], &fdset))
					go = 1;
#endif
				if (go) {
					int		sock;

#ifdef HAS_IPV4
					struct sockaddr_in inet4;
					socklen_t	cl4 =
					sizeof(inet4);
#endif
#ifdef HAS_IPV6
					struct sockaddr_in6 inet6;
					socklen_t	cl6 =
					sizeof(inet6);
#endif
#ifdef HAS_IPV4

					/*
					 * IPv4 connection
					 */
					if (!(i & ND_IPV6_MASK)) {
						sock =
							accept(server_sockets
							       [i],
							    (struct sockaddr
							     *)&inet4,
							       &cl4);
					}
#endif
#ifdef HAS_IPV6

					/*
					 * IPv6 connection
					 */
					if (i & ND_IPV6_MASK) {
						sock =
							accept(server_sockets
							       [i],
							    (struct sockaddr
							     *)&inet6,
							       &cl6);
					}
#endif
					nd_log_info("New connection");
					if (sock >= 0) {
						/*
						 * Process socket here
						 */
						nd_pass_t      *ptr =
						malloc(sizeof(*ptr));

						ptr->do_ssl =
							i & ND_SSL_MASK;
						ptr->sock = sock;
						ptr->serverindex = i;
						ptr->ssl = NULL;
#if defined(HAS_FORK)
						if (fork() == 0) {
							for (i = 0;
							     i <
							     sizeof
							     (server_sockets)
							     /
							     sizeof
							     (server_sockets
							      [0]); i++) {
								if (server_sockets[i] == NO_SOCKET)
									continue;
								CLOSE_SOCKET
									(server_sockets
								       [i]);
							}

							nd_pass(ptr);
							_exit(0);
						} else {
							free(ptr);
						}
						CLOSE_SOCKET(sock);
#elif defined(HAS_NW_BEGINTHREAD)
						BeginThread(nd_pass, NULL,
							    0, ptr);
#elif defined(HAS_BEGINTHREAD)
#ifdef IS_OS2
						_beginthread(nd_pass, 0, 0,
							     ptr);
#else
						_beginthread(nd_pass, 0,
							     ptr);
#endif
#else
						nd_pass(ptr);
#endif
					}
				}
				incr++;
			}
		}
	}

	/*
	 * NOTREACHED
	 */
	return 0;
}
