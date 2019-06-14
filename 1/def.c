#include "def.h"

struct node * mknode(int kind, struct node * first, struct node * second, struct node * third,struct node * four, struct node * five, int pos){
    struct node * p = (struct node *)malloc(sizeof(struct node));
    if(p == -1 || p==NULL){
        printf("struct node * mknode malloc error");
        return NULL;
    }

    p->kind = kind;
    p->child[0] = first;
    p->child[1] = second;
    p->child[2] = third;
    p->child[3] = four;
    p->child[4] = five;
    p->place = pos;
    return p;
}

void display(struct node * tree,int offset){
    if(tree == NULL) return;
    switch(tree->kind){
        case Program:
            printf("%*sln:%d  程序\n",offset," ",tree->place);
            break;
        case EXTDEFLIST:
            printf("%*sln:%d  定义列表\n",offset," ",tree->place);
            break;
        case FUNC:
            printf("%*sln:%d  函数定义  函数名称：%s\n",offset," ",tree->place,tree->type_id);
            break;
        case DECLAREVAL:
            printf("%*sln:%d  变量声明\n",offset," ",tree->place);
            break;
        case T_identifier:
            if(tree->type < 3)
                printf("%*sln:%d  T_identifier id=%s\n",offset," ",tree->place, tree->type_id);
            break;
        case FUNC_VAR_LIST:
            printf("%*sln:%d  函数形参列表\n",offset," ",tree->place);
            break;
        /*case T_TYPE:
            printf("%*sln:%d  T_TYPE id=%s\n",offset," ",tree->place, tree->type_id);
            break;
            */
        case BODY:
            printf("%*sln:%d  语句体\n",offset," ",tree->place);
            break;
        case FUNC_VAR:
            printf("%*sln:%d  形参名：%s\n",offset," ",tree->place,tree->type_id);
            break;
        case STMT:
            printf("%*sln:%d  语句\n",offset," ",tree->place);
            break;
        case IF_STMT:
            printf("%*sln:%d  IF语句\n",offset," ",tree->place);
            break;
        case IF_ELSE_STMT:
            printf("%*sln:%d  IF_ELSE语句\n",offset," ",tree->place);
            break;
        case WHILE_STMT:
            printf("%*sln:%d  WHILE语句\n",offset," ",tree->place);
            break;
        case EXP:
            printf("%*sln:%d  表达式\n",offset," ",tree->place);
            break;
        case T_return:
            printf("%*sln:%d  返回语句\n",offset," ",tree->place);
            break;
        case T_if:
            printf("%*sln:%d  if语句\n",offset," ",tree->place);
            break;
        case T_else:
            printf("%*sln:%d  else语句\n",offset," ",tree->place);
            break;  
        case T_while:
            printf("%*sln:%d  while语句\n",offset," ",tree->place);
            break;  
        case LOOPSTMT:
            printf("%*sln:%d  循环体\n",offset," ",tree->place);
            break;

        /*case CONST:
            printf("%*sln:%d  常量\n",offset," ",tree->place);
            break;
        */
        case T_cst_int:
            printf("%*sln:%d  整型常量, value=%d\n",offset," ",tree->place,tree->intval);
            break;
        case T_cst_float:
            printf("%*sln:%d  浮点型常量, value=%f\n",offset," ",tree->place,tree->floatval);
            break;
        case T_cst_char:
            printf("%*sln:%d  字符型常量, value=%c\n",offset," ",tree->place,tree->intval);
            break;
        case T_assign:
            printf("%*sln:%d  赋值语句\n",offset," ",tree->place);break; 
        case T_add:printf("%*sln:%d  加法\n",offset," ",tree->place);break; 
        case T_sub:printf("%*sln:%d  减法\n",offset," ",tree->place);break; 
        case T_mul:printf("%*sln:%d  乘法\n",offset," ",tree->place);break; 
        case T_div:printf("%*sln:%d  除法\n",offset," ",tree->place);break; 
        case T_mod:printf("%*sln:%d  T_mod\n",offset," ",tree->place);break; 
        case T_equal:printf("%*sln:%d  T_equal\n",offset," ",tree->place);break; 
        case T_nequal:printf("%*sln:%d  T_nequal, \n",offset," ",tree->place);break; 
        case T_big:printf("%*sln:%d  T_big \n",offset," ",tree->place);break; 
        case T_bige:printf("%*sln:%d  T_bige \n",offset," ",tree->place);break; 
        case T_small:printf("%*sln:%d  T_small \n",offset," ",tree->place);break; 
        case T_smalle:printf("%*sln:%d  T_smalle \n",offset," ",tree->place);break; 
        case T_leftmove:printf("%*sln:%d  T_leftmove \n",offset," ",tree->place);break; 
        case T_rightmove:printf("%*sln:%d  T_assign \n",offset," ",tree->place);break; 
        case T_getarr:printf("%*sln:%d  T_getarr\n",offset," ",tree->place);break; 
        case T_or:printf("%*sln:%d  T_or\n",offset," ",tree->place);break; 
        case T_not:printf("%*sln:%d  T_not\n",offset," ",tree->place);break; 
        case T_land:printf("%*sln:%d  T_land\n",offset," ",tree->place);break; 
        case T_lor:printf("%*sln:%d  T_lor\n",offset," ",tree->place);break; 
        case T_lnot:printf("%*sln:%d  T_lnot\n",offset," ",tree->place);break; 
        case T_lxor:printf("%*sln:%d  T_lxor\n",offset," ",tree->place);break; 
        case DECLAREVALLIST:break;
        case FUNC_ARG_LIST:
            printf("%*sln:%d  函数实参列表\n",offset," ",tree->place);
            break;
        case FUNC_ARG:
            printf("%*sln:%d  函数实参:%s\n",offset," ",tree->place,tree->type_id);
            break;
    }

    display(tree->child[0],offset+2);
    display(tree->child[1],offset+2);
    display(tree->child[2],offset+2);
    display(tree->child[3],offset+2);
    display(tree->child[4],offset+2);
    return;
}


void makeLevel(struct node * tree, int level){
    if(tree == NULL) return;
    tree->level = level;
    makeLevel(tree->child[0], level+1);
    makeLevel(tree->child[1], level+1);
    makeLevel(tree->child[2],level+1);
    makeLevel(tree->child[3],level+1);
    makeLevel(tree->child[4],level+1);

    return;
}










