/**
 * $Id$
 * SPDX-License-Identifier: Unlicense
 */

#include <stdio.h>
#include <string.h>

char	       *confpath = PREFIX "/etc/newsdist.conf";

int		yyconfparse(void);
extern FILE    *yyconfin;

int
main(int argc, char **argv)
{
	int		i;

	printf("NewsDist NNTP daemon - NewsDist/%s\n", NEWSDIST_VERSION);
	printf("Under public-domain, original by Nishi <nishi@nishi.boats>.\n");
	printf("\n");
	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-V") == 0) {
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
	return 0;
}
