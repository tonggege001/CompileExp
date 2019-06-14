%error-verbose
%locations
%{
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "def.h"
extern int yylineno;
extern char * yytext;
extern FILE * yyin;

void yyerror(const char * s);
void display(struct node *, int);

/**
1、暂不支持字符串和数组类型
2、暂不支持-1 取负这种操作
*/
%}

%union{
	int type_int;
	float type_float;
	char type_id[32];
	struct node * ptr;
};

// %type定义非终结符的语义值的类型
%type <ptr> Program CONST STMT DECLAREVAL DECLAREVALLIST FUNC FUNC_VAR FUNC_VAR_LIST EXTDEFLIST T_TYPE FUNC_ARG_LIST FUNC_ARG EXP BODY STMTLIST IF_STMT IF_ELSE_STMT WHILE_STMT
// %token定义终结符的语义值类型
%token T_int T_char
%token T_float 
/* TODO 暂不支持字符串和数组类型 */
%token T_string T_intArr T_floatArr 

%token <type_id> T_identifier 

%token T_func T_alloc T_free
%token T_if T_else T_while T_break T_continue T_return 
%token T_getarr
%token T_not T_lnot
%token T_div T_mul T_mod 
%token T_sub T_add
%token T_leftmove T_rightmove
%token T_big T_bige T_small T_smalle
%token T_equal T_nequal
%token T_land
%token T_lxor
%token T_lor
%token T_and
%token T_or
%token T_assign
%token T_pp T_ss
%token <type_int> T_cst_int 
%token <type_float>T_cst_float
%token <type_int> T_cst_char
%token T_ls T_rs T_lb T_rb T_semi T_comma

//不完整
%nonassoc T_identifier
%nonassoc T_comma
%nonassoc T_semi
%left T_assign
%left T_or
%left T_and
%left T_lor 
%left T_lxor
%left T_land
%left T_equal T_nequal
%left T_big T_bige T_small T_smalle
%left T_leftmove T_rightmove
%left T_add T_sub
%left T_mul T_div T_mod
%right T_not T_lnot T_pp T_ss
%left T_getarr
%nonassoc T_int T_char T_float
%left T_ls T_rs T_lb T_rb
%nonassoc T_func T_alloc T_free 
%nonassoc T_if T_else T_while T_break T_continue T_return 




%%
Program: EXTDEFLIST {makeLevel($1,1);display($1,1);}
		;

EXTDEFLIST:{$$=NULL;}
	| FUNC EXTDEFLIST	{$$=mknode(EXTDEFLIST, $1, $2, NULL,NULL,NULL, yylineno);}
	| DECLAREVALLIST EXTDEFLIST {$$=mknode(EXTDEFLIST,$1,$2,NULL,NULL,NULL,yylineno);}
	;

FUNC: T_func T_identifier T_ls FUNC_VAR_LIST T_rs T_TYPE BODY {
	struct node * n = mknode(T_identifier,NULL,NULL,NULL,NULL,NULL,yylineno); strcpy(n->type_id,yylval.type_id);
	$$=mknode(FUNC,n,$4,$6,$7,NULL,yylineno);
	strcpy($$->type_id,$2);
	n->type = 3;
	}
	;

FUNC_ARG_LIST:	{$$=mknode(FUNC_ARG_LIST,NULL,NULL,NULL,NULL,NULL,yylineno);}
	| EXP FUNC_ARG {
		$$=mknode(FUNC_ARG_LIST,$1,$2,NULL,NULL,NULL,yylineno);
	}
	;

FUNC_ARG: 
	{$$=mknode(FUNC_ARG,NULL,NULL,NULL,NULL,NULL,yylineno);}
	| T_comma EXP FUNC_ARG {$$=mknode(FUNC_ARG,$2,$3,NULL,NULL,NULL,yylineno);}
	;

FUNC_VAR_LIST: {$$=mknode(FUNC_VAR_LIST,NULL,NULL,NULL,NULL,NULL,yylineno);}
	| T_TYPE T_identifier FUNC_VAR {
		struct node * n = mknode(FUNC_VAR,NULL,NULL,NULL,NULL,NULL,yylineno); strcpy(n->type_id,yylval.type_id);
		$$=mknode(FUNC_VAR_LIST,$1, n,$3,NULL,NULL, yylineno);
		}
	;

FUNC_VAR: {$$=NULL;}
	| T_comma T_TYPE T_identifier { 
			struct node * n = mknode(T_identifier,NULL,NULL,NULL,NULL,NULL,yylineno); strcpy(n->type_id,yylval.type_id);
			$$=mknode(FUNC_VAR, $2,n,NULL,NULL,NULL,yylineno);
		}
	;

T_TYPE: T_int	{$$=mknode(T_TYPE, NULL,NULL, NULL,NULL,NULL,yylineno); $$->kind = T_int;}
	| T_float	{$$=mknode(T_TYPE, NULL,NULL, NULL,NULL,NULL,yylineno); $$->kind = T_float;}
	| T_char	{$$=mknode(T_TYPE, NULL,NULL, NULL,NULL,NULL,yylineno); $$->kind = T_char;}
	;

DECLAREVALLIST: DECLAREVAL DECLAREVALLIST {$$ = mknode(DECLAREVALLIST,$1,$2, NULL,NULL,NULL,yylineno);}
	| {$$=NULL;}
	;

DECLAREVAL: T_TYPE T_identifier T_semi {struct node * n = mknode(T_identifier,NULL,NULL,NULL,NULL,NULL,yylineno); strcpy(n->type_id,yylval.type_id);$$=mknode(DECLAREVAL,$1,n,NULL,NULL,NULL,yylineno);}
	| T_TYPE T_identifier T_assign EXP T_semi {
		struct node * n = mknode(T_identifier,NULL,NULL,NULL,NULL,NULL,yylineno); 
		strcpy(n->type_id,yylval.type_id);
		struct node * n2 = mknode(T_assign,NULL,NULL,NULL,NULL,NULL,yylineno); 
		$$=mknode(DECLAREVAL,$1,n,n2,$4,NULL,yylineno);}
	;

BODY: T_lb DECLAREVALLIST STMTLIST T_rb	{$$=mknode(BODY,$2,$3,NULL,NULL,NULL,yylineno);}
	;
	
STMTLIST: {$$=NULL;}
	| STMT STMTLIST {$$=mknode(STMTLIST,$1,$2,NULL,NULL,NULL,yylineno);}
	;

STMT:BODY {$$=mknode(STMT,$1,NULL,NULL,NULL,NULL,yylineno);}
	| EXP T_semi {$$=mknode(STMT,$1,NULL,NULL,NULL,NULL,yylineno);}
	| T_return EXP T_semi {struct node * n = mknode(T_return,NULL,NULL,NULL,NULL,NULL,yylineno);$$=mknode(STMT,n,$2,NULL,NULL,NULL,yylineno);}
	| T_break T_semi {$$=mknode(T_break,NULL,NULL,NULL,NULL,NULL,yylineno);}
	| T_continue T_semi {$$=mknode(T_continue,NULL,NULL,NULL,NULL,NULL,yylineno);}
	| IF_STMT {$$=$1;}
	| IF_ELSE_STMT {$$=$1;}
	| WHILE_STMT {$$=$1;}
	;	

IF_STMT:T_if T_ls EXP T_rs STMT {$$=mknode(IF_STMT,$3,$5,NULL,NULL,NULL,yylineno);}
	;

IF_ELSE_STMT:T_if T_ls EXP T_rs STMT T_else STMT {$$=mknode(IF_ELSE_STMT,$3,$5,$7,NULL,NULL,yylineno);}
	;

WHILE_STMT:T_while T_ls EXP T_rs STMT {$$=mknode(WHILE_STMT,$3,$5,NULL,NULL,NULL,yylineno);}



CONST: T_cst_int 
	{
	struct node * n = mknode(T_cst_int,NULL,NULL,NULL,NULL,NULL,yylineno);n->intval = yylval.type_int;
	$$ = mknode(CONST, n,NULL,NULL,NULL,NULL,yylineno);}
	| T_cst_float 
	{struct node * n = mknode(T_cst_float,NULL,NULL,NULL,NULL,NULL,yylineno);n->floatval = yylval.type_float;
	$$ = mknode(CONST, n,NULL,NULL,NULL,NULL,yylineno);}
	| T_cst_char
	{struct node * n = mknode(T_cst_char,NULL,NULL,NULL,NULL,NULL,yylineno);n->intval = yylval.type_int;
	$$ = mknode(CONST, n,NULL,NULL,NULL,NULL,yylineno);}
	;	

EXP: CONST	{$$ = mknode(EXP, $1,NULL,NULL,NULL,NULL,yylineno);}
	| T_identifier {
	struct node * n = mknode(T_identifier,NULL,NULL,NULL,NULL,NULL,yylineno);strcpy(n->type_id,yylval.type_id);
	$$ = mknode(EXP, n,NULL,NULL,NULL,NULL,yylineno);}
	| T_ls EXP T_ls	{$$ = mknode(EXP, $2,NULL,NULL,NULL,NULL,yylineno);}
	| EXP T_and EXP {$$=mknode(T_and,$1,$3,NULL,NULL,NULL,yylineno);}
	| EXP T_assign EXP {$$=mknode(T_assign,$1,$3,NULL,NULL,NULL,yylineno);}
	| EXP T_or EXP {$$=mknode(T_or,$1,$3,NULL,NULL,NULL,yylineno);}
	| EXP T_ss {$$=mknode(T_ss,$1,NULL,NULL,NULL,NULL,yylineno);}
	| EXP T_pp	{$$=mknode(T_pp,$1,NULL,NULL,NULL,NULL,yylineno);}
	| T_not EXP {$$=mknode(T_not,$2,NULL,NULL,NULL,NULL,yylineno);}
	| EXP T_land EXP {$$=mknode(T_land,$1,$3,NULL,NULL,NULL,yylineno);}
	| EXP T_lor EXP {$$=mknode(T_lor,$1,$3,NULL,NULL,NULL,yylineno);}
	| T_lnot EXP {$$=mknode(T_lnot,$2,NULL,NULL,NULL,NULL,yylineno);}
	| EXP T_lxor EXP {$$=mknode(T_lxor,$1,$3,NULL,NULL,NULL,yylineno);}
	| EXP T_add EXP {$$=mknode(T_add,$1,$3,NULL,NULL,NULL,yylineno);}
	| EXP T_sub EXP {$$=mknode(T_sub,$1,$3,NULL,NULL,NULL,yylineno);}
	| EXP T_mul  EXP {$$=mknode(T_mul ,$1,$3,NULL,NULL,NULL,yylineno);}
	| EXP T_div EXP {$$=mknode(T_div,$1,$3,NULL,NULL,NULL,yylineno);}
	| EXP T_equal EXP {$$=mknode(T_equal,$1,$3,NULL,NULL,NULL,yylineno);}
	| EXP T_big EXP {$$=mknode(T_big,$1,$3,NULL,NULL,NULL,yylineno);}
	| EXP T_bige EXP {$$=mknode(T_bige,$1,$3,NULL,NULL,NULL,yylineno);}
	| EXP T_small EXP {$$=mknode(T_small,$1,$3,NULL,NULL,NULL,yylineno);}
	| EXP T_smalle EXP {$$=mknode(T_smalle,$1,$3,NULL,NULL,NULL,yylineno);}
	| EXP T_nequal EXP {$$=mknode(T_nequal,$1,$3,NULL,NULL,NULL,yylineno);}
	| EXP T_leftmove EXP {$$=mknode(T_leftmove,$1,$3,NULL,NULL,NULL,yylineno);}
	| EXP T_rightmove EXP {$$=mknode(T_rightmove,$1,$3,NULL,NULL,NULL,yylineno);}
	| T_identifier T_getarr EXP {
		struct node * n = mknode(T_identifier,NULL,NULL,NULL,NULL,NULL,yylineno); strcpy(n->type_id,yylval.type_id);
		$$=mknode(EXP, n,$3,NULL,NULL ,NULL,yylineno);
	}
	| T_TYPE  T_ls EXP T_rs {$$=mknode(EXP,$1,$3,NULL,NULL,NULL,yylineno);}
	| EXP T_mod EXP {$$=mknode(T_mod,$1,$3,NULL,NULL,NULL,yylineno);}
	| T_identifier T_ls FUNC_ARG_LIST T_rs {
		struct node * n = mknode(T_getarr,NULL,NULL,NULL,NULL,NULL,yylineno); strcpy(n->type_id,yylval.type_id);
		$$=mknode(EXP,n,$3,NULL,NULL,NULL,yylineno);
	}
	;



%%
	
	

	
int main(int argc, char ** argv){
	yyin=fopen(argv[1],"r");
	if (!yyin) return 0;
	yylineno=1;
	yyparse();
	return 0;
}

void yyerror(const char * s){
	printf("行号: %d  %s\n",yylineno,s);
}

	
	
	
	
	
	
	
	
	