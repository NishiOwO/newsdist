/**
 * $Id$
 * SPDX-License-Identifier: Unlicense 
 */

%{
int yyconflex();
int yyconferror(const char*);
%}

%start statement_list
%token NEWLINE STRING SPACES

%%

statement_list	: statement
		| statement_list statement_list;

%%
