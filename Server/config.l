/**
 * $Id$
 */

%{
#include "yconf.tab.h"

#include <stdlib.h>
#include <string.h>

char* yyconftmpbuf;

int yyconfwrap(void){
	return 1;
}
int yyconferror(const char* str){
	extern char* yyconftext;
	fprintf(stderr, "Syntax error near `%s': %s\n", yyconftext, str);
	return 0;
}

%}

%x STR

%%

\"			{
	BEGIN(STR);
	yyconftmpbuf = malloc(1);
	yyconftmpbuf[0] = 0;
}

<STR>[^\\"]*		{
	char* old = yyconftmpbuf;
	yyconftmpbuf = malloc(strlen(old) + strlen(yyconftext) + 1);
	strcpy(yyconftmpbuf, old);
	strcpy(yyconftmpbuf + strlen(old), yyconftext);
	free(old);
}

<STR>\\.		{
	char* old = yyconftmpbuf;
	yyconftmpbuf = malloc(strlen(old) + 2);
	strcpy(yyconftmpbuf, old);
	strcpy(yyconftmpbuf + strlen(old), yyconftext + 1);
	free(old);
}

<STR>\"			{
	BEGIN(0);
	yyconflval.string = yyconftmpbuf;
	return STRING;
}

([ \t]*#[^\n]+)		;
([ \t]*#)		;
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

SSLKey			{
	return SSLKEY;
}

SSLCertificate		{
	return SSLCERT;
}

[ \t]+			{
	return SPACES;
}

[0-9]+			{
	yyconflval.number = atoi(yytext);
	return NUMBER;
}

.			{
	return yyconftext[0];
}

%%
