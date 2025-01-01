/**
 * $Id$
 */

#define INCLUDE_SYSLOG

#include <stdio.h>
#include <time.h>

#include "newsdist.h"

#ifdef HAS_FORK
int		enable_stderr_log = 0;
int		enable_syslog = 1;
#else
int		enable_stderr_log = 1;
int		enable_syslog = 0;
#endif

void
nd_init_log(void)
{
	printf("Initializing logger\n");
#ifdef USE_SYSTEM_SYSLOG
	if (enable_syslog)
		openlog("newsdist", LOG_PID, LOG_FACILITY);
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

	sprintf(out, "%s %.d %.2d:%02d:%02d", mons[tm->tm_mon],
		tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
}

void
nd_log_info(const char *info)
{
	char		timestr[512];

#ifdef USE_SYSTEM_SYSLOG
	if (enable_syslog)
		syslog(LOG_INFO, info);
#endif
	if (enable_stderr_log) {
		nd_log_string(timestr);
		fprintf(stderr, "%s [ info ] %s\n", timestr, info);
	}
}

void
nd_log_notice(const char *info)
{
	char		timestr[512];

#ifdef USE_SYSTEM_SYSLOG
	if (enable_syslog)
		syslog(LOG_NOTICE_LEVEL, info);
#endif
	if (enable_stderr_log) {
		nd_log_string(timestr);
		fprintf(stderr, "%s [notice] %s\n", timestr, info);
	}
}
