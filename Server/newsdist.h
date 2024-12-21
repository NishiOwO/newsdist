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
#ifdef HAS_WINSOCK
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#endif
#include <unistd.h>
#endif

#endif
