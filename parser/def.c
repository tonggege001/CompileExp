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
            printf("%*sln:%d  Program\n",offset," ",tree->place);
            break;
        case EXTDEFLIST:
            printf("%*sln:%d  EXTDEFLIST\n",offset," ",tree->place);
            break;
        case FUNC:
            printf("%*sln:%d  FUNC\n",offset," ",tree->place);
            break;
        case DECLAREVAL:
            printf("%*sln:%d  DECLAREVAL\n",offset," ",tree->place);
            break;
        case T_func:
            printf("%*sln:%d  T_func\n",offset," ",tree->place);
            break;
        case T_identifier:
            printf("%*sln:%d  T_identifier id=%s\n",offset," ",tree->place, tree->type_id);
            break;
        case FUNC_VAR_LIST:
            printf("%*sln:%d  FUNC_VAR_LIST\n",offset," ",tree->place);
            break;
        case T_TYPE:
            printf("%*sln:%d  T_TYPE id=%s\n",offset," ",tree->place, tree->type_id);
            break;
        case BODY:
            printf("%*sln:%d  BODY\n",offset," ",tree->place);
            break;
        case FUNC_VAR:
            printf("%*sln:%d  FUNC_VAR\n",offset," ",tree->place);
            break;
        case STMT:
            printf("%*sln:%d  STMT\n",offset," ",tree->place);
            break;
        case EXP:
            printf("%*sln:%d  EXP\n",offset," ",tree->place);
            break;
        case T_return:
            printf("%*sln:%d  T_return\n",offset," ",tree->place);
            break;
        case T_if:
            printf("%*sln:%d  T_if\n",offset," ",tree->place);
            break;
        case T_else:
            printf("%*sln:%d  T_else\n",offset," ",tree->place);
            break;  
        case T_while:
            printf("%*sln:%d  T_while\n",offset," ",tree->place);
            break;  
        case LOOPSTMT:
            printf("%*sln:%d  LOOPSTMT\n",offset," ",tree->place);
            break;
        case CONST:
            printf("%*sln:%d  CONST\n",offset," ",tree->place);
            break;
        case T_cst_int:
            printf("%*sln:%d  T_cst_int, value=%d\n",offset," ",tree->place,tree->intval);
            break;
        case T_cst_float:
            printf("%*sln:%d  T_cst_float, value=%f\n",offset," ",tree->place,tree->floatval);
            break;
        case T_cst_char:
            printf("%*sln:%d  T_cst_char, value=%d\n",offset," ",tree->place,tree->intval);
            break;
        case T_assign:
            printf("%*sln:%d  T_assign, value=%d\n",offset," ",tree->place);break; 
        case T_add:printf("%*sln:%d  T_add, value=%d\n",offset," ",tree->place);break; 
        case T_sub:printf("%*sln:%d  T_sub, value=%d\n",offset," ",tree->place);break; 
        case T_mul:printf("%*sln:%d  T_mul, value=%d\n",offset," ",tree->place);break; 
        case T_div:printf("%*sln:%d  T_div, value=%d\n",offset," ",tree->place);break; 
        case T_mod:printf("%*sln:%d  T_mod, value=%d\n",offset," ",tree->place);break; 
        case T_equal:printf("%*sln:%d  T_equal, value=%d\n",offset," ",tree->place);break; 
        case T_nequal:printf("%*sln:%d  T_nequal, value=%d\n",offset," ",tree->place);break; 
        case T_big:printf("%*sln:%d  T_big, value=%d\n",offset," ",tree->place);break; 
        case T_bige:printf("%*sln:%d  T_bige, value=%d\n",offset," ",tree->place);break; 
        case T_small:printf("%*sln:%d  T_small, value=%d\n",offset," ",tree->place);break; 
        case T_smalle:printf("%*sln:%d  T_smalle, value=%d\n",offset," ",tree->place);break; 
        case T_leftmove:printf("%*sln:%d  T_leftmove, value=%d\n",offset," ",tree->place);break; 
        case T_rightmove:printf("%*sln:%d  T_assign, value=%d\n",offset," ",tree->place);break; 
        case T_getarr:printf("%*sln:%d  T_getarr, value=%d\n",offset," ",tree->place);break; 
        case T_or:printf("%*sln:%d  T_assign, value=%d\n",offset," ",tree->place);break; 
        case T_not:printf("%*sln:%d  T_not, value=%d\n",offset," ",tree->place);break; 
        case T_land:printf("%*sln:%d  T_land, value=%d\n",offset," ",tree->place);break; 
        case T_lor:printf("%*sln:%d  T_lor, value=%d\n",offset," ",tree->place);break; 
        case T_lnot:printf("%*sln:%d  T_lnot, value=%d\n",offset," ",tree->place);break; 
        case T_lxor:printf("%*sln:%d  T_lxor, value=%d\n",offset," ",tree->place);break; 
        case DECLAREVALLIST:break;
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










