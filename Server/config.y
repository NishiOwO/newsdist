/**
 * $Id$
 */

%{
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

component	: WELCOME SPACES STRING NEWLINE
		| PORT SPACES NUMBER NEWLINE {
	plain_port = $<number>3;
}
		| SSLPORT SPACES NUMBER NEWLINE {
	ssl_port = $<number>3;
}
		| SSLKEY SPACES STRING NEWLINE {
}
		| SSLCERT SPACES STRING NEWLINE {
	printf("%s\n", $<string>3);
}
		| NEWLINE;
		;

%%
