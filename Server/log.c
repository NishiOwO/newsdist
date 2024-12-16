/**
 * $Id$
 * SPDX-License-Identifier: Unlicense
 */

#include "newsdist.h"

#include "../config.h"

#include <stdio.h>
#ifdef HAS_SYSLOG
#include <syslog.h>
#endif

#ifndef HAS_LOG_PID
#define LOG_PID 0
#endif
#ifndef HAS_LOG_PERROR
#define LOG_PERROR 0
#endif

#ifdef HAS_LOG_NEWS
#define LOG_FACILITY LOG_NEWS
#else
#ifdef HAS_LOG_USER
#define LOG_FACILITY LOG_USER
#endif
#endif

#ifdef HAS_LOG_NOTICE
#define LOG_NOTICE_LEVEL LOG_NOTICE
#else
#ifdef HAS_LOG_WARNING
#define LOG_NOTICE_LEVEL LOG_WARNING
#endif
#endif

#ifdef LOG_FACILITY
#ifdef HAS_LOG_INFO
#ifdef LOG_NOTICE_LEVEL
#define USE_SYSTEM_SYSLOG
#endif
#endif
#endif

void
nd_init_log(void)
{
	printf("Initializing logger\n");
#ifdef USE_SYSTEM_SYSLOG
	openlog("newsdist", LOG_PID | LOG_PERROR, LOG_FACILITY);
#endif
}

void
nd_log_info(const char *info)
{
#ifdef USE_SYSTEM_SYSLOG
	syslog(LOG_INFO, info);
#else
#endif
}

void
nd_log_notice(const char *info)
{
#ifdef USE_SYSTEM_SYSLOG
	syslog(LOG_NOTICE_LEVEL, info);
#else
#endif
}
