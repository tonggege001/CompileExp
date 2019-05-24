%{
    #include "flex.tab.h"
    #include <string.h>
    
    //状态
    const int NORMAL = 0,
    const int COMMENT = 1;


    int yycolumn=1;
    int state = NORMAL;
    #define YY_USER_ACTION yylloc.first_line=yylloc.last_line=yylineno; \
	yylloc.first_column=yycolumn;	yylloc.last_column=yycolumn+yyleng-1; yycolumn+=yyleng;
    
    

    

    
}%
