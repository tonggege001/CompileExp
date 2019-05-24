%{
	#include "fb4.tab.h"
	int yylval;
%}
%%
"+"	{return ADD;}
"-"	{return SUB;}
"*"	{return MUL;}
"/"	{return DIV;}
"|" {return ABS;}
[0-9]+	{yylval = atoi(yytext);printf("yylval=%d",yylval);return NUMBER;}
\n		{return EOL;}
[ \t]	{/*忽略空白*/}
.		{printf("Mystery character %s\n",yytext);}
%%
/*
int main(int argc, char ** argv){
	int tok;  
	while(tok = yylex()){
		printf("%d",tok);
		if (tok == NUMBER) printf(" = %d\n", yyval);
		else printf("\n");
	}
	return 0;
}
*/
int yywrap(){return 1;}