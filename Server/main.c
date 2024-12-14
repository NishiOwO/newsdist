/* $Id$ */

#include <stdio.h>
#include <string.h>

int main(int argc, char** argv){
	int i;
	printf("NewsDist NNTP daemon - NewsDist/%s\n", NEWSDIST_VERSION);
	printf("Under public-domain, original by Nishi <nishi@nishi.boats>.\n");
	for(i = 1; i < argc; i++){
		if(argv[i][0] == '-'){
			if(strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-V") == 0){
				return 0;
			}else{
				fprintf(stderr, "Invalid flag: %s\n", argv[i]);
				return 1;
			}
		}
	}
}
