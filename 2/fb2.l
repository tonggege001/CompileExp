%{
#include<string.h>
int chars = 0;
int words = 0;
int lines = 0;
%}
%%
"colour"	{printf("color");}
"flavour"	{printf("flavor");}
"clever"	{printf("smart");}
"smart"		{printf("elegant");}
"conservative"	{printf("liberal");}
.	{printf("%s",yytext);}
%%
int main(int argc, char ** argv){yylex();printf("OK");return 0;}
int yywrap(){return 1;}
