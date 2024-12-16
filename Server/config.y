/**
 * $Id$
 * SPDX-License-Identifier: Unlicense 
 */

%{
int yyconflex(void);
int yyconferror(const char*);
%}

%start list
%token NEWLINE STRING SPACES
%token WELCOME

%%

list		: component
		| list component;

component	: WELCOME SPACES STRING NEWLINE
		| NEWLINE;

%%
