/**
 * $Id$
 */

#define INCLUDE_SIGNAL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "newsdist.h"

#ifdef HAS_NW_BEGINTHREAD
#	include <nwconio.h>
#	include <nwnamspc.h>
#	include <nwthread.h>
#endif

char* confpath = PREFIX "/etc/newsdist.conf";

int	     yyconfparse(void);
extern FILE* yyconfin;
extern int   enable_stderr_log;
extern int   enable_syslog;

#ifdef HAS_NW_BEGINTHREAD
#	define EXIT(x) return
#else
#	define EXIT(x) return x
#endif

#ifdef HAS_NW_BEGINTHREAD
void thread_stuff(void* pargs);
#else
int thread_stuff(void* pargs);
#endif
struct arg_struct {
	int    argc;
	char** argv;
};

int main(int argc, char** argv) {
	struct arg_struct* parg = malloc(sizeof(*parg));

	parg->argc = argc;
	parg->argv = argv;
#ifdef HAS_NW_BEGINTHREAD
	SetCurrentNameSpace(NW_NS_LONG);
	DestroyScreen(GetCurrentScreen());
	SetCurrentScreen(CreateScreen("NewsDist Cosnole", 0));
	BeginThread(thread_stuff, NULL, 0, parg);
	ThreadSwitch();
	return 0;
#else
	return thread_stuff(parg);
#endif
}

#ifdef HAS_NW_BEGINTHREAD
void thread_stuff(void* pargs) {
#else
int thread_stuff(void* pargs) {
#endif
	int    argc = ((struct arg_struct*)pargs)->argc;
	char** argv = ((struct arg_struct*)pargs)->argv;
	int    i;
	char*  buffer;

#ifdef HAS_FORK
	int daemonize = 1;
#else
	int daemonize = 0;
#endif

	CONFIG_ASSIGN_DEFAULT;

	printf("NewsDist NNTP daemon - NewsDist/%s, system = %s\n", NEWSDIST_VERSION, (buffer = nd_get_system()));
	free(buffer);
	printf("\n");
	for(i = 1; i < argc; i++) {
		if(argv[i][0] == '-') {
			if(strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-V") == 0) {
				const char* defines[] = DEFINES;
				int	    j;
				int	    k;
				int	    maxlen = 0;

				printf("Configuration:\n");
				for(j = 0; j < sizeof(defines) / sizeof(defines[0]); j++) {
					if((strlen(defines[j]) + 1) > maxlen) maxlen = strlen(defines[j]) + 1;
				}
				for(j = 0; j < sizeof(defines) / sizeof(defines[0]); j++) {
					if((j % 3) == 0) printf("    ");
					printf("%s", defines[j]);
					for(k = 0; k < maxlen - strlen(defines[j]); k++) printf(" ");
					if((j + 1) % 3 == 0) printf("\n");
				}
				if(j % 3 != 0) printf("\n");
				EXIT(0);
			} else if(strcmp(argv[i], "--config") == 0 || strcmp(argv[i], "-C") == 0) {
				confpath = argv[(long)i + 1];
				if(confpath == NULL) {
					fprintf(stderr, "%s requires an argument\n", argv[i]);
					EXIT(1);
				}
			} else if(strcmp(argv[i], "--stderr-log") == 0 || strcmp(argv[i], "-S") == 0) {
				enable_stderr_log = 1;
			} else if(strcmp(argv[i], "--no-stderr-log") == 0 || strcmp(argv[i], "-s") == 0) {
				enable_stderr_log = 0;
			} else if(strcmp(argv[i], "--syslog") == 0 || strcmp(argv[i], "-L") == 0) {
				enable_syslog = 1;
			} else if(strcmp(argv[i], "--no-syslog") == 0 || strcmp(argv[i], "-l") == 0) {
				enable_syslog = 0;
			} else if(strcmp(argv[i], "--daemon") == 0 || strcmp(argv[i], "-d") == 0) {
				daemonize = 1;
			} else if(strcmp(argv[i], "--foreground") == 0 || strcmp(argv[i], "-f") == 0) {
				daemonize = 0;
			} else {
				fprintf(stderr, "Invalid flag: %s\n", argv[i]);
				EXIT(1);
			}
		}
	}
	yyconfin = fopen(confpath, "r");
	if(yyconfin == NULL) {
		fprintf(stderr, "Could not open the config: %s\n", confpath);
		EXIT(1);
	}
	nd_init_log();
	if(yyconfparse() != 0) {
		fclose(yyconfin);
		EXIT(1);
	}
	fclose(yyconfin);
#ifdef HAS_SIGCHLD
	signal(SIGCHLD, SIG_IGN);
#endif
#ifdef HAS_SIGPIPE
	signal(SIGPIPE, SIG_IGN);
#endif
	if(nd_init_ssl() < 0) {
		nd_log_notice("SSL initialization fail");
		EXIT(1);
	}
	if(nd_init_server() != 0) EXIT(1);
#ifdef HAS_FORK
	if(daemonize) {
		if(fork() != 0) {
			printf("Daemon started\n");
			EXIT(0);
		}
	} else {
		nd_log_info("Not daemonizing");
	}
#endif
#ifdef HAS_NW_BEGINTHREAD
	nd_loop_server();
#else
	return nd_loop_server();
#endif
}
