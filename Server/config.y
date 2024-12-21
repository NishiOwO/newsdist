/**
 * $Id$
 * SPDX-License-Identifier: Unlicense 
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
%token WELCOME PORT SSLPORT

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
		| NEWLINE;
		;

%%
