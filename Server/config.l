/**
 * $Id$
 * SPDX-License-Identifier: Unlicense 
 */

%{
#include "yconf.tab.h"

int yyconfwrap(void){
	return 1;
}
int yyconferror(const char* str){
	extern char* yytext;
	fprintf(stderr, "Syntax error near `%s': %s\n", yytext, str);
	return 0;
}

%}

%x STR

%%

\"			{
	BEGIN(STR);
}

<STR>[^\\"]*		{
}

<STR>\\.		{
}

<STR>\"			{
	BEGIN(0);
	return STRING;
}

([ \t]*#[^\n]+)		;
([ \t]*\n[ \t]*)	{
        return NEWLINE;
}

Welcome			{
	return WELCOME;
}

[ \t]+			{
	return SPACES;
}

.			{
	return yytext[0];
}

%%
