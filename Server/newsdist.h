/**
 * $Id$
 */

#ifndef __NEWSDIST_H__
#define __NEWSDIST_H__

#include "../config.h"

#ifdef HAS_TYPES_H
#include <types.h>
#endif
#ifdef HAS_SYS_TYPES_H
#include <sys/types.h>
#endif

#define ND_IPV6_MASK 1
#define ND_SSL_MASK  2

struct nd_ssl_struct {
	void	       *ssl;
	void	       *ctx;
};

typedef struct nd_ssl_struct nd_ssl_t;

struct nd_pass_struct {
	int		serverindex;	/* server_sockets[n] */
	int		sock;
	nd_ssl_t       *ssl;
	int		do_ssl;
};

typedef struct nd_pass_struct nd_pass_t;

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
void		nd_close_socket(nd_pass_t * pass);
int		nd_accept_ssl(nd_pass_t * pass);

/* util.c */
char *		nd_strdup(const char *str);
char *		nd_get_system(void);

/* Config implementation */
#ifdef CONFIG_IMPLEMENTATION
#define CONFIG_DECL
#else
#define CONFIG_DECL extern
#endif

CONFIG_DECL int	ssl_port;
CONFIG_DECL int	plain_port;
CONFIG_DECL char *ssl_key;
CONFIG_DECL char *ssl_cert;

#define CONFIG_ASSIGN_DEFAULT \
	ssl_port = 563; \
	plain_port = 119; \
	ssl_key = NULL; \
	ssl_cert = NULL;

/* Include socket headers or not */
#ifdef INCLUDE_SOCKET
#if defined(HAS_WINSOCK)
#include <winsock2.h>
#include <ws2tcpip.h>

#ifndef ENETUNREACH
#define ENETUNREACH WSAENETUNREACH
#endif
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
#ifndef ENETUNREACH
#define ENETUNREACH 0
#endif
#endif

/* Include syslog headers or not */
#ifdef INCLUDE_SYSLOG
#ifdef HAS_SYSLOG
#include <syslog.h>
#endif

#if !defined(HAS_LOG_PID)
#define LOG_PID 0
#endif

#if defined(HAS_LOG_NEWS)
#define LOG_FACILITY LOG_NEWS
#elif defined(HAS_LOG_USER)
#define LOG_FACILITY LOG_USER
#endif

#if defined(HAS_LOG_NOTICE)
#define LOG_NOTICE_LEVEL LOG_NOTICE
#elif defined(HAS_LOG_WARNING)
#define LOG_NOTICE_LEVEL LOG_WARNING
#endif

#if defined(LOG_FACILITY) && defined(HAS_LOG_INFO) && defined(LOG_NOTICE_LEVEL)
#define USE_SYSTEM_SYSLOG
#endif
#endif

/* Include utsname headers or not */
#ifdef INCLUDE_UTSNAME
#ifdef HAS_UNAME
#include <sys/utsname.h>
#endif
#endif

#endif
