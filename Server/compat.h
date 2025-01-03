/**
 * $Id$
 */

#ifndef __NEWSDIST_COMPAT_H__
#define __NEWSDIST_COMPAT_H__

#include "../config.h"

#ifdef HAS_TYPES_H
#include <types.h>
#endif
#ifdef HAS_SYS_TYPES_H
#include <sys/types.h>
#endif

/*
 * Include socket headers or not
 */
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
#ifdef IS_OS2
#include <tcpustd.h>
#endif
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

/*
 * Include errno headers or not
 */
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

/*
 * Include syslog headers or not
 */
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

/*
 * Include utsname headers or not
 */
#ifdef INCLUDE_UTSNAME
#ifdef HAS_UNAME
#include <sys/utsname.h>
#endif
#endif

/*
 * Include signal headers or not
 */
#ifdef INCLUDE_SIGNAL
#ifdef HAS_SIGNAL_H
#include <signal.h>
#endif
#endif

/*
 * Include boolean or not
 */
#ifdef INCLUDE_BOOLEAN
#if defined(HAS_STDBOOL_H) && !defined(__lint__)
#include <stdbool.h>
typedef bool NDBOOL;
#define NDTRUE ((NDBOOL)1)
#define NDFALSE ((NDBOOL)0)
#else
typedef unsigned char NDBOOL;
#define NDTRUE ((NDBOOL)1)
#define NDFALSE ((NDBOOL)0)
#endif
#endif

#endif
