/**
 * $Id$
 * SPDX-License-Identifier: Unlicense
 */

#include "newsdist.h"

#include "../config.h"

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
#else
#define LOG_FACILITY 0
#endif
#endif

void
nd_init_log(void)
{
#ifdef HAS_SYSLOG
	syslog("newsdist", LOG_PID | LOG_PERROR, LOG_FACILITY);
#endif
}
