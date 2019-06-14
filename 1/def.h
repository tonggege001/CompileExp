#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"

//定义符号表的大小
#define MAXTABLELENGTH 1000

enum node_kind{
    Program,CONST,EXP, BODY, STMT, LOOPSTMT, DECLAREVAL, FUNC, FUNC_VAR, FUNC_VAR_LIST,EXTDEFLIST, T_TYPE,DECLAREVALLIST,FUNC_ARG_LIST,FUNC_ARG,STMTLIST,
    IF_STMT,IF_ELSE_STMT,WHILE_STMT

};


struct node{
    int kind;       //结点类型
    int type;       //T_int, T_float, T_char 3:T_func表示结点对应值的类型 4:形参列表
    union{
        int intval;
        float floatval;
        char type_id[33]; 
    };
    struct node * parient;
    struct node *child[5];          //子树指针，由kind确定有多少棵子树
    int level;                      //层号
    int place;                      //表示结点对应的变量在符号表的位置
    char Etrue[15], Efalse[15];     //表示布尔表达式的翻译时，真假转移目标的标号
    char Snext[15];                 //表示结点对应语句执行后的下一语句位置标号
    
    int offset;                     //偏移量
    int width;                      //各种数据占用的字节数
};


struct symbol{
    char name[32];      //变量名或函数名
    int level;
    int type;           
};


struct node * mknode(int kind, struct node * first, struct node * second, struct node * third, struct node * four, struct node * five, int pos);
void display(struct node * tree,int offset);
void makeLevel(struct node * tree, int level);




