/**
 * $Id$
 */

%{
#include <stdio.h>
#include <stdlib.h>

#define CONFIG_IMPLEMENTATION
#include "newsdist.h"

int yyconflex(void);
int yyconferror(const char*);
%}

%union {
	int number;
	char* string;
}

%start list
%token NEWLINE STRING NUMBER SPACES
%token WELCOME PORT SSLPORT SSLKEY SSLCERT

%%

list		: component
		| list component
		;

component	: WELCOME SPACES STRING NEWLINE {
	char* s;
	char* tmp;
	if(welcome_text != NULL) free(welcome_text);
	welcome_text = nd_strdup($<string>3);
	tmp = nd_format(welcome_text);
	s = nd_strcat("Setting welcome text: ", tmp);
	free(tmp);
	nd_log_info(s);
	free(s);
}
		| PORT SPACES NUMBER NEWLINE {
	plain_port = $<number>3;
}
		| SSLPORT SPACES NUMBER NEWLINE {
	ssl_port = $<number>3;
}
		| SSLKEY SPACES STRING NEWLINE {
	if(ssl_key != NULL) free(ssl_key);
	ssl_key = $<string>3;
}
		| SSLCERT SPACES STRING NEWLINE {
	if(ssl_cert != NULL) free(ssl_cert);
	ssl_cert = $<string>3;
}
		| NEWLINE;
		;

%%
