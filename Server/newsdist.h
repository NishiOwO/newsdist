/**
 * $Id$
 */

#ifndef __NEWSDIST_H__
#define __NEWSDIST_H__

/**
 * Memo
 *
 *         / INCLUDE_SOCKET  \             / socket \
 *        |  INCLUDE_ERRNO    |            | errno   |
 *        |  INCLUDE_SYSLOG   |            | syslog  |
 * Define |  INCLUDE_UTSNAME  | to include | errno   | headers
 *        |  INCLUDE_SIGNAL   |            | signal  |
 *         \ INCLUDE_BOOLEAN /             \ boolean /
 */

#define INCLUDE_BOOLEAN

#include "../config.h"
#include "compat.h"

#define ND_IPV6_MASK 1
#define ND_SSL_MASK 2

struct nd_ssl_struct {
	void* ssl;
};

typedef struct nd_ssl_struct nd_ssl_t;

struct nd_pass_struct {
	int	  serverindex; /* server_sockets[n] */
	int	  sock;
	nd_ssl_t* ssl;
	int	  do_ssl;
};

typedef struct nd_pass_struct nd_pass_t;

/*
 * log.c
 */
void nd_init_log(void);
void nd_log_info(const char* info);
void nd_log_notice(const char* info);

/*
 * server.c
 */
int nd_init_server(void);
int nd_loop_server(void);

/*
 * socket.c
 */
const void* nd_create_method(void);
const char* nd_get_ssl_version(void);
void	    nd_close_socket(nd_pass_t* pass);
int	    nd_accept_ssl(nd_pass_t* pass);
int	    nd_init_ssl(void);
int	    nd_read(nd_pass_t* pass, void* buffer, int size);
int	    nd_write(nd_pass_t* pass, void* buffer, int size);
int	    nd_timeout(nd_pass_t* pass, int time);
int	    nd_write_string(nd_pass_t* pass, const char* string);

/*
 * util.c
 */
char*  nd_strcat(const char* a, const char* b);
char*  nd_strdup(const char* str);
char*  nd_get_system(void);
char*  nd_gethostname(void);
char*  nd_format(const char* str);
NDBOOL nd_strcaseequ(const char* str1, const char* str2);
int    nd_wildcard_match(const char* wildcard, const char* target);

/*
 * nntpd.c
 */
void nd_nntpd_handle(nd_pass_t* p);

/*
 * Config implementation
 */
#ifdef CONFIG_IMPLEMENTATION
#	define CONFIG_DECL
#else
#	define CONFIG_DECL extern
#endif

CONFIG_DECL int	  ssl_port;
CONFIG_DECL int	  plain_port;
CONFIG_DECL char* ssl_key;
CONFIG_DECL char* ssl_cert;
CONFIG_DECL char* welcome_text;
CONFIG_DECL char* motd_path;

#define CONFIG_ASSIGN_DEFAULT \
	ssl_port     = 563; \
	plain_port   = 119; \
	ssl_key	     = NULL; \
	ssl_cert     = NULL; \
	welcome_text = NULL; \
	motd_path    = NULL;

#endif
