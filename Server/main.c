/**
 * $Id$
 * SPDX-License-Identifier: Unlicense
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "newsdist.h"

#include "../config.h"

char	       *confpath = PREFIX "/etc/newsdist.conf";

int		yyconfparse(void);
extern FILE    *yyconfin;

int
main(int argc, char **argv)
{
	int		i;
	char	       *buffer;

	printf("NewsDist NNTP daemon - NewsDist/%s, system = %s\n", NEWSDIST_VERSION, (buffer = nd_get_system()));
	free(buffer);
	printf("Under public-domain, original by Nishi <nishi@nishi.boats>.\n");
	printf("\n");
	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-V") == 0) {
				const char     *defines[] = DEFINES;
				int		j;
				int		k;
				int		maxlen = 0;

				printf("Configuration:\n");
				for (j = 0; j < sizeof(defines) / sizeof(defines[0]); j++) {
					if ((strlen(defines[j]) + 1) > maxlen)
						maxlen = strlen(defines[j]) + 1;
				}
				for (j = 0; j < sizeof(defines) / sizeof(defines[0]); j++) {
					if ((j % 3) == 0)
						printf("    ");
					printf("%s", defines[j]);
					for (k = 0; k < maxlen - strlen(defines[j]); k++)
						printf(" ");
					if ((j + 1) % 3 == 0)
						printf("\n");
				}
				if (j % 3 != 0)
					printf("\n");
				return 0;
			} else if (strcmp(argv[i], "--config") == 0 || strcmp(argv[i], "-C") == 0) {
				confpath = argv[(long)i + 1];
				if (confpath == NULL) {
					fprintf(stderr, "%s requires an argument\n", argv[i]);
					return 1;
				}
			} else {
				fprintf(stderr, "Invalid flag: %s\n", argv[i]);
				return 1;
			}
		}
	}
	yyconfin = fopen(confpath, "r");
	if (yyconfin == NULL) {
		fprintf(stderr, "Could not open the config: %s\n", confpath);
		return 1;
	}
	if (yyconfparse() != 0) {
		fclose(yyconfin);
		return 1;
	}
	fclose(yyconfin);
	nd_init_log();
	if (nd_init_server() != 0)
		return 1;
	return 0;
}
