/**
 * $Id$
 * SPDX-License-Identifier: Unlicense
 */

#include "newsdist.h"

#include <stdio.h>
#include <time.h>
#ifdef HAS_SYSLOG
#include <syslog.h>
#endif

#if !defined(HAS_LOG_PID)
#define LOG_PID 0
#elif !defined(HAS_LOG_PERROR)
#define LOG_PERROR 0
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

void
nd_init_log(void)
{
	printf("Initializing logger\n");
#ifdef USE_SYSTEM_SYSLOG
	openlog("newsdist", LOG_PID | LOG_PERROR, LOG_FACILITY);
#endif
	nd_log_info("NewsDist/" NEWSDIST_VERSION " starting up");
}

void
nd_log_string(char *out)
{
	time_t		t = time(NULL);
	struct tm      *tm = localtime(&t);
	const char     *mons[] = {
		"Jan",
		"Feb",
		"Mar",
		"Apr",
		"May",
		"Jun",
		"Jul",
		"Aug",
		"Sep",
		"Oct",
		"Nov",
		"Dec"
	};

	sprintf(out, "%s %.d %.2d:%02d:%02d", mons[tm->tm_mon], tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
}

void
nd_log_info(const char *info)
{
#ifdef USE_SYSTEM_SYSLOG
	syslog(LOG_INFO, info);
#else
	char		timestr[512];

	nd_log_string(timestr);
	fprintf(stderr, "%s [ info ] %s\n", timestr, info);
#endif
}

void
nd_log_notice(const char *info)
{
#ifdef USE_SYSTEM_SYSLOG
	syslog(LOG_NOTICE_LEVEL, info);
#else
	char		timestr[512];

	nd_log_string(timestr);
	fprintf(stderr, "%s [notice] %s\n", timestr, info);
#endif
}
