/**
 * $Id$
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

Port			{
	return PORT;
}

SSLPort			{
	return SSLPORT;
}

[ \t]+			{
	return SPACES;
}

[0-9]+			{
	yyconflval.number = atoi(yytext);
	return NUMBER;
}

.			{
	return yytext[0];
}

%%
