/**
 * $Id$
 * SPDX-License-Identifier: Unlicense
 */

#ifndef __NEWSDIST_H__
#define __NEWSDIST_H__

#include "../config.h"

/* log.c */
void		nd_init_log(void);
void		nd_log_info(const char *info);
void		nd_log_notice(const char *info);

/* server.c */
int		nd_init_server(void);
int		nd_loop_server(void);

/* socket.c */
const void *	nd_create_method(void);
const char *	nd_get_ssl_version(void);

/* util.c */
char *		nd_strdup(const char *str);
char *		nd_get_system(void);

struct nd_pass_struct {
	int		sock;
};

typedef struct nd_pass_struct nd_pass_t;

/* Config implementation */
#ifdef CONFIG_IMPLEMENTATION
#define CONFIG_DECL
#else
#define CONFIG_DECL extern
#endif

CONFIG_DECL int	ssl_port;
CONFIG_DECL int	plain_port;

#define CONFIG_ASSIGN_DEFAULT \
	ssl_port = 563; \
	plain_port = 119;

/* Include socket headers or not */
#ifdef INCLUDE_SOCKET
#if defined(HAS_WINSOCK)
#include <winsock2.h>
#include <ws2tcpip.h>
#elif defined(IS_NETWARE)
#include <sys/bsdskt.h>
#include <sys/socket.h>

#ifndef HAS_INADDR
struct in_addr {
	uint32_t	s_addr;
};
#endif
#ifndef HAS_SOCKADDR_IN
struct sockaddr_in {
	uint16_t	sin_family;
	uint16_t	sin_port;
	struct in_addr	sin_addr;
	uint8_t		sin_zero[8];
};
#endif
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#ifdef HAS_POLL
#include <sys/poll.h>
#endif
#ifdef HAS_SELECT
#include <sys/select.h>
#endif
#endif
#include <unistd.h>

#ifndef HAS_SOCKLEN_T
typedef int socklen_t;
#endif

#ifndef HAS_IPPROTO_TCP
#define IPPROTO_TCP 0
#endif

#ifndef HAS_INADDR_ANY
#define INADDR_ANY 0
#endif

#ifndef HAS_HTONS
uint16_t	htons(uint16_t n);
#endif
#endif

/* Include errno headers or not */
#ifdef INCLUDE_ERRNO
#ifdef HAS_ERRNO
#include <errno.h>
#else
extern int	errno;
#endif
#endif

#endif
