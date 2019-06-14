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

%}

%union{
	int type_int;
	float type_float;
	char type_id[32];
	struct node * ptr;
};


%type <ptr> Program CONST STMT DECLAREVAL DECLAREVALLIST FUNC FUNC_VAR FUNC_VAR_LIST EXTDEFLIST T_TYPE FUNC_ARG_LIST FUNC_ARG EXP BODY STMTLIST IF_STMT IF_ELSE_STMT WHILE_STMT T_ID

%token T_int T_char
%token T_float 

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
%token T_ls T_rs T_lb T_rb T_lm T_rm T_semi T_comma

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
Program: EXTDEFLIST {$1->parient = NULL;makeLevel($1,1);display($1,1);initTable();semanticAnalysisStatic($1,0);TACGen($1); PrintCode($1->code);}
		;

EXTDEFLIST:{$$=NULL;}
	| FUNC EXTDEFLIST	{$$=mknode(EXTDEFLIST, $1, $2 , NULL, yylineno);}
	| DECLAREVALLIST EXTDEFLIST {$$=mknode(EXTDEFLIST,$1,$2,NULL,yylineno);}
	;

FUNC: T_func T_ID T_ls FUNC_VAR_LIST T_rs T_TYPE BODY {$$=mknode(FUNC,$4,$7,NULL,yylineno); strcpy($$->type_id, $2->type_id);$$->type = $6->type;}
	;


FUNC_ARG_LIST:	{$$=NULL;}
	| EXP FUNC_ARG {$$=mknode(FUNC_ARG_LIST,$1,$2,NULL,yylineno); }
	;

FUNC_ARG: {$$=NULL;}
	| T_comma EXP FUNC_ARG {$$=mknode(FUNC_ARG_LIST,$2,$3,NULL,yylineno);}
	;

FUNC_VAR_LIST: {$$=NULL}
	| T_TYPE T_ID FUNC_VAR {
		struct node * n = mknode(T_TYPE,NULL,NULL,NULL,yylineno); strcpy(n->type_id,$2->type_id);n->type=$1->type;
		$$=mknode(FUNC_VAR_LIST,n, $3,$2, yylineno);
		strcpy($$->type_id,$2->type_id);
		}
	;

FUNC_VAR: {$$=NULL;}
	| 	T_comma T_TYPE T_ID FUNC_VAR{ 
			struct node * n = mknode(T_TYPE,NULL,NULL,NULL,yylineno);n->type = $2->type;
			$$=mknode(FUNC_VAR_LIST, n,$4,$3,yylineno);
		}
	;

T_TYPE:T_int T_lm T_cst_int T_rm 	{$$=mknode(T_TYPE, NULL,NULL,NULL,yylineno); $$->type = T_intArr; $$->arrlen = $3;}
	|  T_float T_lm T_cst_int T_rm	{$$=mknode(T_TYPE, NULL,NULL,NULL,yylineno); $$->type = T_floatArr; $$->arrlen = $3;}
	| T_float	{$$=mknode(T_TYPE, NULL,NULL,NULL,yylineno); $$->type = T_float; $$->arrlen = 0;}
	| T_char	{$$=mknode(T_TYPE, NULL,NULL,NULL,yylineno); $$->type = T_char;$$->arrlen = 0;}
	| T_int		{$$=mknode(T_TYPE, NULL,NULL,NULL,yylineno); $$->type = T_int; $$->arrlen = 0;}
	;

DECLAREVALLIST: DECLAREVAL DECLAREVALLIST {$$ = mknode(DECLAREVALLIST,$1,$2, NULL,yylineno);}
	| {$$=NULL;}
	;

DECLAREVAL: T_TYPE T_ID T_semi {$$=mknode(DECLAREVAL,NULL,NULL,NULL,yylineno); strcpy($$->type_id,$2->type_id);$$->type = $1->type; $$->arrlen = $1->arrlen;}
	| T_TYPE T_ID T_assign EXP T_semi {$$=mknode(DECLAREVAL,$4,NULL,NULL,yylineno); strcpy($$->type_id,$2->type_id);$$->type = $1->type; $$->arrlen = $1->arrlen;}
	;

BODY: T_lb DECLAREVALLIST STMTLIST T_rb	{$$=mknode(BODY,$2,$3,NULL,yylineno);}
	;

STMTLIST: {$$=NULL;}
	| STMT STMTLIST {$$=mknode(STMTLIST,$1,$2,NULL,yylineno);}
	;

STMT:BODY {$$=mknode(STMT,$1,NULL,NULL,yylineno); $$->type = BODY;}
	| EXP T_semi {$$=mknode(STMT,$1,NULL,NULL,yylineno);$$->type = EXP;}
	| T_return EXP T_semi {$$=mknode(STMT,$2,NULL,NULL,yylineno);$$->retStmt = 1; $$->type = T_return;}
	| T_break T_semi {$$=mknode(STMT,NULL,NULL,NULL,yylineno);$$->breakStmt=1; $$->type = T_break;}
	| T_continue T_semi {$$=mknode(STMT,NULL,NULL,NULL,yylineno);$$->ctnStmt = 1; $$->type = T_continue;}
	| IF_STMT {$$=mknode(STMT,$1,NULL,NULL,yylineno); $$->type = IF_STMT;}
	| IF_ELSE_STMT {$$=mknode(STMT,$1,NULL,NULL,yylineno); $$->type = IF_ELSE_STMT;}
	| WHILE_STMT {$$=mknode(STMT,$1,NULL,NULL,yylineno); $$->type = WHILE_STMT;}
	;	

IF_STMT:T_if T_ls EXP T_rs STMT {$$=mknode(IF_STMT,$3,$5,NULL,yylineno);}
	;

IF_ELSE_STMT:T_if T_ls EXP T_rs STMT T_else STMT {$$=mknode(IF_ELSE_STMT,$3,$5,$7,yylineno);}
	;

WHILE_STMT:T_while T_ls EXP T_rs STMT {$$=mknode(WHILE_STMT,$3,$5,NULL,yylineno);}



CONST: T_cst_char {$$ = mknode(CONST,NULL,NULL,NULL,yylineno); $$->type = T_char; $$->intval = $1;}
	| T_cst_int {$$ = mknode(CONST,NULL,NULL,NULL,yylineno); $$->type = T_int; $$->intval = $1;}
	| T_cst_float {$$ = mknode(CONST,NULL,NULL,NULL,yylineno); $$->type = T_float; $$->intval = $1;}
	;

EXP: CONST	{$$ = mknode(EXP, $1,NULL,NULL,yylineno);$$->op = CONST; $$->type = $1->type;$$->intval = $1->intval;}
	| T_ID {$$ = mknode(EXP, $1,NULL,NULL,yylineno); $$->op = T_ID;strcpy($$->type_id,$1->type_id);}
	| T_ls EXP T_rs	{$$ = mknode(EXP, $2,NULL,NULL,yylineno); $$->op = T_ls;}
	| EXP T_and EXP {$$=mknode(EXP,$1,$3,NULL,yylineno);$$->op = T_and;}
	| EXP T_assign EXP {$$=mknode(EXP,$1,$3,NULL,yylineno);$$->op = T_assign;}
	| EXP T_or EXP {$$=mknode(EXP,$1,$3,NULL,yylineno);$$->op = T_or;}
	| EXP T_ss {$$=mknode(EXP,$1,NULL,NULL,yylineno);$$->op = T_ss;}
	| EXP T_pp	{$$=mknode(EXP,$1,NULL,NULL,yylineno);$$->op = T_pp;}
	| T_not EXP {$$=mknode(EXP,$2,NULL,NULL,yylineno);$$->op = T_not;}
	| EXP T_land EXP {$$=mknode(EXP,$1,$3,NULL,yylineno);$$->op = T_land;}
	| EXP T_lor EXP {$$=mknode(EXP,$1,$3,NULL,yylineno);$$->op = T_lor;}
	| T_lnot EXP {$$=mknode(EXP,$2,NULL,NULL,yylineno);$$->op = T_lnot;}
	| EXP T_lxor EXP {$$=mknode(EXP,$1,$3,NULL,yylineno);$$->op = T_lxor;}
	| EXP T_add EXP {$$=mknode(EXP,$1,$3,NULL,yylineno);$$->op = T_add;}
	| EXP T_sub EXP {$$=mknode(EXP,$1,$3,NULL,yylineno);$$->op = T_sub;}
	| EXP T_mul  EXP {$$=mknode(EXP ,$1,$3,NULL,yylineno);$$->op = T_mul;}
	| EXP T_div EXP {$$=mknode(EXP,$1,$3,NULL,yylineno);$$->op = T_div;}
	| EXP T_equal EXP {$$=mknode(EXP,$1,$3,NULL,yylineno);$$->op = T_equal;}
	| EXP T_big EXP {$$=mknode(EXP,$1,$3,NULL,yylineno);$$->op = T_big;}
	| EXP T_bige EXP {$$=mknode(EXP,$1,$3,NULL,yylineno);$$->op = T_bige;}
	| EXP T_small EXP {$$=mknode(EXP,$1,$3,NULL,yylineno);$$->op = T_small;}
	| EXP T_smalle EXP {$$=mknode(EXP,$1,$3,NULL,yylineno);$$->op = T_smalle;}
	| EXP T_nequal EXP {$$=mknode(EXP,$1,$3,NULL,yylineno);$$->op = T_nequal;}
	| EXP T_leftmove EXP {$$=mknode(EXP,$1,$3,NULL,yylineno);$$->op = T_leftmove;}
	| EXP T_rightmove EXP {$$=mknode(EXP,$1,$3,NULL,yylineno);$$->op = T_rightmove;}
	| T_ID T_getarr EXP {$$=mknode(EXP, $1,$3,NULL,yylineno);$$->op = T_getarr;}
	| T_TYPE  T_ls EXP T_rs {$$=mknode(EXP,$1,$3,NULL,yylineno);$$->op = T_TYPE;}
	| EXP T_mod EXP {$$=mknode(EXP,$1,$3,NULL,yylineno); $$->op = T_mod;}
	| T_ID T_ls FUNC_ARG_LIST T_rs {$$=mknode(EXP,$1,$3,NULL,yylineno); $$->op = FUNC; strcpy($$->type_id,$1->type_id);}
	;

T_ID: T_identifier {$$ = mknode(T_ID,NULL,NULL,NULL,yylineno);strcpy($$->type_id, $1);}
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
	printf("����: %d  %s\n",yylineno,s);
}

	
	
	
	
	
	
	
	
	