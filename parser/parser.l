%{
    
    int yycolumn = 0;
    typedef union{
        int type_int;
        float type_float;
        char type_id[32];       //返回标识符名称
        struct node * ptr;
    }YYLVAL;

    #define YYSTYPE YYLVAL


    #include <string.h>
    #include "parser.tab.h"
    #include "def.h"
    void output(char * str);

    YYSTYPE yylval;

    /*
{int}           {yycolumn++;yylval.type_int=atoi(yytext);printf("T_cst_int, value=%d\n",yylval.type_int);return T_cst_int;}
{float}         {yycolumn++;yylval.type_float=atof(yytext);printf("T_cst_float, value=%f\n",yylval.type_float);return T_cst_float;}
{char}          {yycolumn++;yylval.type_int=yytext[0]-'0';printf("T_cst_char,value=%c\n",yylval.type_int);return T_cst_char;}
{T_identifier}  {yycolumn++;strcpy(yylval.type_id,yytext);printf("T_identifier, value=%s\n",yylval.type_id);return T_identifier;}
*/
%}

%option yylineno

T_identifier    [A-Z_a-z][A-Z_a-z0-9]*
int             [0-9]+
float           [0-9]+\.[0-9]+
char            \\'[0-9a-zA-Z]?\\'
commentSingle   (\/\/[^n]*) 

%%
{commentSingle} {}
"int"           {yycolumn++;output("T_int");return T_int;}
"float"         {yycolumn++;output("T_float");return T_float;}
"char"          {yycolumn++;output("T_char");return T_char;}
"string"        {yycolumn++;output("T_string");return T_string;}
"int[]"         {yycolumn++;output("T_intArr");return T_intArr;}
"float[]"       {yycolumn++;output("T_floatArr");return T_floatArr;}
"func"          {yycolumn++;output("T_func");return T_func;}
"alloc"         {yycolumn++;output("T_alloc");return T_alloc;}
"free"          {yycolumn++;output("T_free");return T_free;}

"if"            {yycolumn++;output("T_if");return T_if;}
"else"          {yycolumn++;output("T_else");return T_else;}
"while"         {yycolumn++;output("T_while");return T_while;}
"break"         {yycolumn++;output("T_break");return T_break;}
"continue"      {yycolumn++;output("T_continue");return T_continue;}
"return"        {yycolumn++;output("T_return");return T_return;}


{int}           {yycolumn++;yylval.type_int=atoi(yytext);return T_cst_int;}
{float}         {yycolumn++;yylval.type_float=atof(yytext);return T_cst_float;}
{char}          {yycolumn++;yylval.type_int=yytext[0]-'0';return T_cst_char;}
{T_identifier}  {yycolumn++;strcpy(yylval.type_id,yytext);return T_identifier;}

"("             {yycolumn++;output("T_ls");return T_ls;}
")"             {yycolumn++;output("T_rs");return T_rs;}
"{"             {yycolumn++;output("T_lb");return T_lb;}
"}"             {yycolumn++;output("T_rb");return T_rb;}
";"             {yycolumn++;output("T_semi");return T_semi;}
","             {yycolumn++;output("T_comma");return T_comma;}
"+"             {yycolumn++;output("T_add");return T_add;}
"-"             {yycolumn++;output("T_sub");return T_sub;}
"*"             {yycolumn++;output("T_mul");return T_mul;}
"/"             {yycolumn++;output("T_div");return T_div;}
"%"             {yycolumn++;output("T_mod");return T_mod;}
"=="            {yycolumn++;output("T_equal");return T_equal;}
">"             {yycolumn++;output("T_big");return T_big;}
">="            {yycolumn++;output("T_bige");return T_bige;}
"<"             {yycolumn++;output("T_small");return T_small;}
"<="            {yycolumn++;output("T_smalle");return T_smalle;}
"!="            {yycolumn++;output("T_nequal");return T_nequal;}
"<<"            {yycolumn++;output("T_leftmove");return T_leftmove;}
">>"            {yycolumn++;output("T_rightmove");return T_rightmove;}
"&&"            {yycolumn++;output("T_and");return T_and;}
"||"            {yycolumn++;output("T_or");return T_or;}
"!"             {yycolumn++;output("T_not");return T_not;}
"&"             {yycolumn++;output("T_land");return T_land;}
"|"             {yycolumn++;output("T_lor");return T_lor;}
"~"             {yycolumn++;output("T_lnot");return T_lnot;}
"^"             {yycolumn++;output("T_lxor");return T_lxor;}
"$"             {yycolumn++;output("T_getarr");return T_getarr;}
"="             {yycolumn++;output("T_assign");return T_assign;}   

[\n]            {yycolumn=0;}
[ \r\t]         {}
.               {printf("Error type A : Mysterious character \"%s\"\n\t at Line %d\n",yytext,yylineno);}

%%
/*
int main(){
    do{
        yylex();

    }while(1);
    
    return 0;
}
*/

int yywrap(){
    return 1;
}

void output(char * str){
    //printf("line:%d\tcolumn:%d\t\t%s\n",yylineno,yycolumn,str);
    return;
}