#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"

//定义符号表的大小
#define MAXTABLELENGTH 1000

enum node_kind{
    Program,CONST,EXP, BODY, STMT, LOOPSTMT, DECLAREVAL, FUNC, FUNC_VAR, FUNC_VAR_LIST,EXTDEFLIST, T_TYPE,DECLAREVALLIST,FUNC_ARG_LIST,FUNC_ARG,STMTLIST,
    IF_STMT,IF_ELSE_STMT,WHILE_STMT,T_ID
};


struct node{
    int kind;       //结点类型
    int type;       //T_int, T_float, T_char 3:T_func表示结点对应值的类型 4:形参列表
    union{
        int intval;
        float floatval;
        char type_id[33]; 
    };
    int lineno;     //行号
    int retStmt;    //含有return语句
    int breakStmt;  //含有break语句
    int ctnStmt;    //含有continue语句
    int op;  //表达式的运算符
    int arrlen;     //数组元素的个数
    int offset;                     //偏移量当偏移量为-1时说明是右值表达式
    int left;                       //判断是否是左值表达式
    int level;                      //层号


    struct node * parient;          //父节点指针
    struct node *child[3];          //子树指针，由kind确定有多少棵子树
    
    int place;                      //表示结点对应的变量在符号表的位置
    char Etrue[15], Efalse[15];     //表示布尔表达式的翻译时，真假转移目标的标号
    char Snext[15];                 //表示结点对应语句执行后的下一语句位置标号
    
    
    int width;                      //各种数据占用的字节数
};


struct symbol{
    char name[32];      //变量名或函数名
    int level;
    int type;
    struct node * addr;     //函数调用入口地址
    int arrlen;             //数组长度
    int offset;             //偏移量（函数不存在偏移量）
};



struct node * mknode(int kind, struct node * first, struct node * second, struct node * third, int llno);
void display(struct node * tree,int offset);
void makeLevel(struct node * tree, int level);
void semanticAnalysisStatic(struct node * tree, int level);


//符号表调用
void initTable();
struct symbol * find(char * id);
void enter(char * id, int type, int level, struct node * addr, int arrlen);

extern int LEVEL;           //当前解析的层号




