/**
 * $Id$
 * SPDX-License-Identifier: Unlicense 
 */

%{
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
		| PORT SPACES NUMBER NEWLINE
		| SSLPORT SPACES NUMBER NEWLINE
		| NEWLINE;
		;

%%
