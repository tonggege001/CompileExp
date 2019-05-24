%{
#include <stdio.h>

extern int yyval;
%}

/*声明token*/
%token NUMBER
%token ADD SUB MUL DIV ABS
%token EOL

%%

calclist: /*空规则*/
	| calclist exp EOL {printf("= %d\n",$2);}
	;

exp: factor {$$ = $1;}
	| exp ADD factor { $$ = $1 + $3;printf("AAAA%d  %d  %d",$$,$1, $2);}
	| exp SUB factor { $$ = $1 - $3;}
	;
	
factor: term {$$ = $1;}
	| factor MUL term {$$ = $1 * $3;}
	| factor DIV term {$$ = $1 / $3;}
	;
	
term: NUMBER {printf("BBBB  %d",yylval);}
	| ABS term {$$ = $2 >= 0? $2 : -$2;}
	;
%%

int main(int argc, char ** argv){
	yyparse();
}

int yyerror(char * s){
	printf("error: %s\n",s);
}
	
	
	
	
	
	
	
	
	