#include "def.h"
#include <stdarg.h>

struct node * mknode(int kind, struct node * first, struct node * second, struct node * third, int llno){
    struct node * p = (struct node *)malloc(sizeof(struct node));
    if(p == -1 || p==NULL){
        printf("struct node * mknode malloc error");
        return NULL;
    }
    p->retStmt = 0;
    p->breakStmt = 0;
    p->ctnStmt = 0;
    p->kind = kind;
    p->child[0] = first;
    p->child[1] = second;
    p->child[2] = third;
    p->lineno = llno;
    return p;
}

void display(struct node * tree,int offset){
    if(tree == NULL) return;
    switch(tree->kind){
        case Program:
            printf("%*sln:%d  ����\n",offset," ",tree->lineno);
            break;
        case EXTDEFLIST:
            display(tree->child[0],offset);
            display(tree->child[1],offset);
            break;
        case FUNC:
            printf("%*sln:%d  ��������  �������ƣ�%s\n",offset," ",tree->lineno,tree->type_id);
            printf("�β��б�\n");
            display(tree->child[0],offset+2);
            display(tree->child[1],offset+2);
            break;
        case DECLAREVAL:
            printf("%*sln:%d  ���� %s ���������ͣ�",offset," ",tree->lineno, tree->type_id);
            if(tree->type == T_int) printf("int\n");
            else if(tree->type == T_float) printf("float\n");
            else if(tree->type == T_char) printf("char\n");
            else if(tree->type == T_intArr) printf("int[]�����鳤�ȣ�%d\n",tree->arrlen);
            else if(tree->type == T_floatArr) printf("float[]�����鳤�ȣ�%d\n",tree->arrlen);
            break;
        case FUNC_VAR_LIST:
            display(tree->child[0],offset);
            display(tree->child[1],offset);
            break;
        case BODY:
            printf("%*sln:%d  �����\n",offset," ",tree->lineno);
            display(tree->child[0],offset+2);
            display(tree->child[1],offset+2);
            break;
        case T_TYPE:
            printf("%*sln:%d  ���ͣ�",offset," ",tree->lineno);
            if(tree->type == T_int) printf("int");
            else if(tree->type == T_float) printf("float");
            else if(tree->type == T_char) printf("char");
            else if(tree->type == T_intArr) printf("int[]");
            else if(tree->type == T_floatArr) printf("float[]");
            printf("\n");
        case STMTLIST:
            display(tree->child[0],offset);
            display(tree->child[1],offset);
            break;
        case STMT:
            if(tree->type == BODY){
                display(tree->child[0],offset);
                break;
            }
            if(tree->child[0] == NULL  && tree->breakStmt == 1)
                printf("%*sln:%d  break���\n",offset," ",tree->lineno);
            else if(tree->child[0] == NULL && tree->ctnStmt == 1)
                printf("%*sln:%d  continue���\n",offset," ",tree->lineno);
            else if(tree->child[0]->kind == EXP && tree->type == T_return){
                printf("%*sln:%d  return���",offset," ",tree->lineno);
                printf("����ֵ���ʽ��");
                display(tree->child[0],offset);
                printf("\n");
            }
            else if(tree->child[0]->kind == EXP){
                printf("%*sln:%d  ���ʽ���",offset," ",tree->lineno);
                printf("  ���ʽ��"," ");
                display(tree->child[0],offset);
                printf("\n");
            }
            else if(tree->child[0]->kind == IF_STMT){
                printf("%*sln:%d  IF ���\n",offset," ",tree->lineno);
                display(tree->child[0],offset+2);
            }
            else if(tree->child[0]->kind == IF_ELSE_STMT){
                printf("%*sln:%d  IF_ELSE ���\n",offset," ",tree->lineno);
                display(tree->child[0],offset+2);
            }
            else if(tree->child[0]->kind == WHILE_STMT){
                printf("%*sln:%d  WHILE ���\n",offset," ",tree->lineno);
                display(tree->child[0],offset+2);
            }
            break;
        case IF_STMT:
                printf("%*sln:%d  �������ʽ��",offset," ",tree->lineno);
                display(tree->child[0],offset);printf("\n");
                printf("%*sln:%d  ����ִ���壺\n",offset," ",tree->lineno);
                display(tree->child[1],offset+2);
            break;
        case IF_ELSE_STMT:
                printf("%*sln:%d  �������ʽ��",offset," ",tree->lineno);
                display(tree->child[0],offset);printf("\n");
                printf("%*sln:%d  ������ִ���壺\n",offset," ",tree->lineno);
                display(tree->child[1],offset+2);
                printf("%*sln:%d  ������ִ���壺\n",offset," ",tree->lineno);
                display(tree->child[2],offset+2);
            break;
        case WHILE_STMT:
            printf("%*sln:%d  �������ʽ��",offset," ",tree->lineno);
            display(tree->child[0],offset);printf("\n");
            printf("%*sln:%d  ѭ��ִ���壺\n",offset," ",tree->lineno);
            display(tree->child[1],offset+2);
            break;
        case EXP:
            //if(tree->op == CONST) printf("%")
            printf("<expression>");
            break;
        case DECLAREVALLIST:
            display(tree->child[0],offset);
            display(tree->child[1],offset);
            break;
        case FUNC_ARG_LIST:
            printf("%*sln:%d  ����ʵ���б�\n",offset," ",tree->place);
            break;
        case FUNC_ARG:
            printf("%*sln:%d  ����ʵ��:%s\n",offset," ",tree->place,tree->type_id);
            break;
            
    }
    return;
}

void makeLevel(struct node * tree, int level){
    if(tree == NULL) return;
    if(tree->child[0] != NULL) tree->child[0]->parient = tree;      //����˫��ָ��
    if(tree->child[1] != NULL) tree->child[1]->parient = tree;
    if(tree->child[2] != NULL) tree->child[2]->parient = tree;
    tree->level = level;
    makeLevel(tree->child[0], level+1);
    makeLevel(tree->child[1], level+1);
    makeLevel(tree->child[2],level+1);
    return;
}

int LEVEL = 0;

struct symbol * table;
int top;

void initTable(){
    printf("initTable begin\n");
    table = (struct symbol *)malloc(MAXTABLELENGTH * sizeof(struct symbol));
    top = 0;
    printf("initTable end\n");
}

void enter(char * id, int type, int level, struct node * addr, int arrlen){
    strcpy(table[top].name,id);
    printf("%s������ű�\n",table[top].name);
    table[top].type = type;
    table[top].level = level;
    table[top].addr = addr;
    table[top].arrlen = arrlen;
    if(top == 0){
        table[top].offset = 0;
    }else{
        if(type == FUNC){
            table[top].offset = table[top-1].offset;
        }else{
            switch(table[top-1].type){
                case FUNC:
                    table[top].offset = table[top -1].offset + 4;
                    break;
                case T_int:
                    table[top].offset = table[top -1].offset + 4;
                    break;
                case T_float:
                    table[top].offset = table[top-1].offset + 4;
                    break;
                case T_intArr:
                    table[top].offset = table[top-1].offset + table[top-1].arrlen * 4;
                    break;
                case T_floatArr:
                    table[top].offset = table[top-1].offset + table[top-1].arrlen * 4;
                    break;
                case T_char:
                    table[top].offset = table[top-1].offset + 1;
                    break;
            }
        }
 
    }
    top++;
}

struct symbol * find(char * id){
    for(int i = top-1; i>=0;i--){
        if(strcmp(id,table[i].name) == 0){
            return &table[i];
        }
    }
    return NULL;
}

void rremove(int level){
    int t = top-1;
    while(t >= 0){
        if(table[t].level > level){
            printf("%s ���Ƴ���level=%d\n",table[t].name,table[t].level);
            t--;
            
        }else{
            break;
        }
    }
    top = t+1;
}

void semantic_error(int line,char *msg1,char *msg2){
    printf("<error> ��%d��,%s %s\n",line,msg1,msg2);
}

void semanticFuncVarArgCheck(struct node * varlist, struct node * arglist, int level){
    if(varlist == NULL && arglist == NULL) return;
    if(varlist == NULL && arglist != NULL){
        semantic_error(arglist->lineno,"���������������","���������������");
        return;
    }
    if(varlist != NULL && arglist == NULL){
        semantic_error(varlist->lineno, "���������������","���������������");
        return;
    }
    if(varlist->child[0] == NULL && arglist->child[0] == NULL){
        return;
    }
    if(varlist->child[0] == NULL && arglist->child[0] != NULL){
        semantic_error(arglist->lineno,"���������������","���������������");
        return;
    }

    if(varlist->child[0] != NULL && arglist->child[0] == NULL){
        semantic_error(arglist->lineno, "���������������","���������������");
        return;
    }

    semanticExp(arglist->child[0],level);
    if(varlist->child[0]->type != arglist->child[0]->type){
        printf("var type:%d, arg type:%d\n",varlist->child[0]->type, arglist->child[0]->type);
        semantic_error(arglist->lineno,"�����������Ͳ�ƥ��","�����������Ͳ�ƥ��");
        return;
    }

    semanticFuncVarArgCheck(varlist->child[1],arglist->child[1],level);
    return;

}

//�����ȸ�����
int semanticExp(struct node * tree, int level){             //�Ա��ʽ�������ͷ����ͷ��ŷ���
    printf("op = %d\n",tree->op);
    if(tree == NULL) return 0;
    if(tree->kind != EXP) return 0;
    if(tree->op == T_ID){
        struct symbol * s = find(tree->type_id);
        if(s == NULL){
            semantic_error(tree->lineno,tree->type_id,"1 û���ҵ������Ķ���");
        }else{
            tree->type = s->type;                     //�õ�����������
            tree->offset = s->offset;                   //�õ�������ַ
            tree->left = 1;
            tree->level = s->level;
        }
    }
    else if(tree->op == CONST){tree->left = 0;}
    else if(tree->op == T_ls){                      //��������
        semanticExp(tree->child[0],level);
        tree->type = tree->child[0]->type;
        tree->left = 0;
    }
    else if(tree->op == T_assign){          //TODO assign type check and leftvalue check
        semanticExp(tree->child[0],level);
        semanticExp(tree->child[1],level);
        if(!tree->child[0]->left){
            semantic_error(tree->lineno,"2 ������ֵ���ʽ","�������ܸ�ֵ");
            return -1;
        }
        if(tree->child[0]->type != tree->child[1]->type){
            printf("tree->child[0] type:%d, tree->child[1] type:%d\n");
            semantic_error(tree->lineno,"3 ����","��ֵ����������Ͳ�ƥ��");
        }

        tree->type = tree->child[0]->type;
        tree->left = 0;
    }
    
    switch(tree->op){
        case T_and:
        case T_or:
        case T_lor:
        case T_land:
        case T_lxor:
        case T_mod:
        case T_leftmove:
        case T_rightmove:
            semanticExp(tree->child[0],level);
            semanticExp(tree->child[1],level);
            if(tree->child[0]->type != T_int || tree->child[1]->type != T_int){
                semantic_error(tree->lineno,"4 ���ʽ���ʹ���","�߼��������ʽ�����Ϊint����");
                return -1;
            }else{
                tree->type = T_int;
                tree->left = 0;
            }
            break;
        
        case T_ss:
        case T_pp:
            semanticExp(tree->child[0],level);
            if(tree->child[0]->type != T_int &&  tree->child[0]->type != T_char){
                semantic_error(tree->lineno,"5 ���ʽ���ʹ���","++ or --���������������");
                return -1;
            }
            if(!tree->child[0]->left){
                semantic_error(tree->lineno,"6 ���������","++ or --������Ĳ�������������ֵ���ʽ");
            }
            tree->type = T_int;
            tree->left = 0;
            break;
        case T_not:
        case T_lnot:
            semanticExp(tree->child[0],level);
            if(tree->child[0]->type != T_int && tree->child[0]->type != T_char){
                semantic_error(tree->lineno,"7 ���ʽ���ʹ���","�߼��������ʽ�����Ϊint��char����");
                return -1;
            }
            tree->left = 0;
            tree->type = T_int;
            break;
        case T_add:
        case T_sub:
        case T_mul:
        case T_div:
            semanticExp(tree->child[0],level);
            semanticExp(tree->child[1],level);
            if(tree->child[0]->type || tree->child[1]->type == T_float){
                tree->type = T_float;
            }
        case T_equal:
        case T_nequal:
        case T_big:
        case T_bige:
        case T_small:
        case T_smalle:
            semanticExp(tree->child[0],level);
            semanticExp(tree->child[1],level);
            if(tree->child[0]->type != tree->child[1]->type){
                semantic_error(tree->lineno,"8 ���������ߵı��ʽ���Ͳ�ƥ��","���������ߵı��ʽ���Ͳ�ƥ��");
            }
            if(tree->child[0]->type!= T_int && tree->child[0]->type != T_char && tree->child[0]->type != T_float){
                semantic_error(tree->lineno,"9 ��һ��������","��һ����������������");
            }
            if(tree->child[1]->type != T_int && tree->child[1]->type != T_char && tree->child[1]->type != T_float){
                printf("�ڶ���������type:%d",tree->child[1]->type);
                semantic_error(tree->lineno,"�ڶ���������","�ڶ�����������������");
            }
            tree->type = T_int;
            tree->left = 0;
            break;
    }

    if(tree->op == T_getarr){
        struct symbol * s = find(tree->child[0]->type_id);
        if(s == NULL){
            semantic_error(tree->lineno,tree->type_id,"10 û���ҵ������Ķ���");
        }else{
            semanticExp(tree->child[1],level);
            if(s->type != T_intArr && s->type != T_floatArr){
                semantic_error(tree->lineno, tree->type_id,"11 ����������������");
                return -1;
            }
            if(tree->child[1]->type != T_int){
                semantic_error(tree->lineno,"12 ������������Ϊ����","������������Ϊ����");
                return -1;
            }
            tree->offset = s->offset;
            tree->left = 1;
            tree->level = s->level;
            if(s->type == T_intArr){
                tree->type = T_int;
            }else if(s->type == T_floatArr){
                tree->type = T_float;
            }
        }
    }
    if(tree->op == T_TYPE){           //ǿ��ת��
        semanticExp(tree->child[1],level);
        if(tree->child[0]->type == T_floatArr || tree->child[0]->type == T_intArr){
            semantic_error(tree->lineno,"13 ǿ������ת������ȷ","ǿ������ת������ȷ");
            return -1;
        }
        tree->type = tree->child[0]->type;
        tree->left = 0;
    }
    if(tree->op == FUNC){             //��������
        //���Һ���
        struct symbol * s = find(tree->child[0]->type_id);
        if(s == NULL){
            semantic_error(tree->lineno, "","14 ����������");
            return -1;
        }
        if(s->type != FUNC){
            semantic_error(tree->lineno,tree->child[0]->type_id,"����������");
            return -1;
        }
        printf("�������ã���������%s,��������ֵ��%d",s->addr->type_id, s->addr->type);
        //��鴫��Ĳ�������
        semanticFuncVarArgCheck(s->addr->child[0],tree->child[1],level);
        
        tree->type = s->addr->type;     //���ű�����func
        return 0;
    }
}


int semanticFindWhile(struct node * brk){                   //��ѯwhileѭ��
    struct node * w = brk;
    while(w){
        if(w->kind == WHILE_STMT){
            break;
        }
        w = w->parient;
    }
    if(w == NULL){  //��whileѭ��
        semantic_error(brk->lineno,"15 û��whileѭ��","û��whileѭ��������ʹ��break �� continue");
        return -1;
    }
    return 0;
}


int printSymbolTable(char * title){
    printf("%s\n\n",title);
    int t = top-1;
    printf("%10s%10s%10s%10s\n","������","����","level","ƫ��");
    while(t >= 0){
        printf("%10s%10d%10d%10d\n",table[t].name,table[t].type,table[t].level, table[t].offset);
        t--;
    }
    printf("\n\n\n\n\n");
}

void semanticAnalysisStatic(struct node * tree, int level){
    if(tree == NULL) return;
    struct symbol * s = NULL;
    switch(tree->kind){
        case EXTDEFLIST:
            printSymbolTable("�ⲿ��������֮ǰ��EXTDEFLIST");
            semanticAnalysisStatic(tree->child[0],level);
            semanticAnalysisStatic(tree->child[1],level);
            printSymbolTable("�ⲿ����飺EXTDEFLIST");
            break;
        case FUNC:
            s = find(tree->type_id);
            if(s == NULL){
                enter(tree->type_id,FUNC,level,tree,0);       //������ű�
            }
            else if(s->level == level){
                semantic_error(tree->lineno,tree->type_id,"16 ������������ͻ");
            }
            semanticAnalysisStatic(tree->child[0],level+1);
            semanticAnalysisStatic(tree->child[1],level);
            rremove(level);
            break;
        case DECLAREVALLIST:
            semanticAnalysisStatic(tree->child[0],level);
            semanticAnalysisStatic(tree->child[1],level);
            break;
        case DECLAREVAL:
            s = find(tree->type_id);
            if(s != NULL) printf("DECLAREVAL  level=%d, s->level=%d, name=%s, s->name=%s\n",level, s->level, tree->type_id,s->name);
            if(s == NULL || s->level < level){
                enter(tree->type_id, tree->type,level,NULL,0);
            }else{
                semantic_error(tree->lineno,tree->type_id,"17 �����ظ����壡");
            }
            if(tree->child[0]!= NULL){
                semanticExp(tree->child[0],level);
                if(tree->child[0]->type != tree->type){
                    semantic_error(tree->lineno,"","18 ��ʼ��������Ͳ�ƥ��\n");
                }
            }
            s = find(tree->type_id);
            tree->level = s->level;
            break;
        case BODY:
            printSymbolTable("BODY��");
            semanticAnalysisStatic(tree->child[0],level+1);         //������������
            semanticAnalysisStatic(tree->child[1],level+1);         //�������
            printSymbolTable("BODY�����");
            printf("BODY׼���Ƴ����ű�, level = %d\n",level);
            rremove(level);                                          //�Ƴ����ű�
            break;
        case STMTLIST:
            semanticAnalysisStatic(tree->child[0],level);
            semanticAnalysisStatic(tree->child[1],level);
            //rremove(level);
            break;
        case STMT:
            if(tree->type == BODY) semanticAnalysisStatic(tree->child[0],level);
            if(tree->type == EXP){                              //�жϱ����Ƿ�δ�����ʹ��
                semanticExp(tree->child[0],level);
                tree->level = level;
            }
            if(tree->type == T_return) semanticAnalysisStatic(tree->child[0],level);
            if(tree->type == T_break){
                if(semanticFindWhile(tree) == -1){
                    semantic_error(tree->lineno,"","19 break���û���ҵ���Ӧ��while���");
                    break;
                }
            }
            if(tree->type == T_continue){
                if(semanticFindWhile(tree) == -1){
                    semantic_error(tree->lineno,"","20 continue���û���ҵ���Ӧ��while���");
                    break;
                }
            }
            if(tree->type == IF_STMT || tree->type == IF_ELSE_STMT || tree->type == WHILE_STMT){
                semanticAnalysisStatic(tree->child[0],level);
                break;
            }
            break;
        case IF_STMT:
            semanticExp(tree->child[0],level);
            semanticAnalysisStatic(tree->child[1],level);
            break;
        case IF_ELSE_STMT:
            semanticExp(tree->child[0],level);
            semanticAnalysisStatic(tree->child[1],level);
            semanticAnalysisStatic(tree->child[2],level);
            break;
        case WHILE_STMT:
            semanticExp(tree->child[0],level);
            semanticAnalysisStatic(tree->child[1],level);
            break;
        case FUNC_VAR_LIST:
            if(tree->child[0] == NULL || tree->child[2] == NULL) return;
            enter(tree->child[2]->type_id,tree->child[0]->type,level,NULL,0);
        
            tree->child[2]->level = level;
            tree->level = level;
            if(tree->child[1] != NULL){
                strcpy(tree->child[1]->type_id, tree->type_id);
                semanticAnalysisStatic(tree->child[1],level);
            }
    }
    return;
}






char * newtmp(){                //����һ���µ���ʱ������
    static int tmp = 0;
    static char tmparr[32];
    tmparr[0] = 't';
    itoa(tmp,tmparr+1,10);
    tmp++;
    return tmparr;
}

char * newAlias(char * origin,int level){
    static char alias[32];
    static char l[4];
    itoa(level,l,10);
    strcpy(alias,origin);
    strcat(alias,l);
    return alias;
}
char *newAliasArr(char * arrname, char * _alias, int level){
    static char alias[32];
    static char l[4];
    itoa(level,l,10);
    strcpy(alias,arrname);
    strcat(alias,l);
    sprintf(alias,"%s%d[%s]",arrname,level,_alias);
    return alias;
}

char * newAliasCONST(int cst_int,float cst_float, int type){
    static char ret[32];
    strcpy(ret,"");
    switch(type){
        case T_int:
            itoa(cst_int,ret,10);
            break;
        case T_float:
            strcpy(ret,"ffff");
            break;

    }
    return ret;
}

char * newLabel(){
    static int tmp = 0;
    static char tmparr[32];
    strcpy(tmparr,"LABEL");
    tmp++;
    itoa(tmp,tmparr+5,10);
    return tmparr;
}

struct codenode * gencode(int num, ...){
    printf("gencode\n");
    va_list valist;
    va_start(valist,num);
    struct codenode * n = (struct codenode *)malloc(sizeof(struct codenode));
    n->next = NULL;
    n->prior = NULL;
    strcpy(n->code,"");
    for(int i = 0;i<num;i++){
        char * cc = va_arg(valist,char *);
        strcat(n->code,cc);
        printf("%s\n",cc);
    }
    return n;
}

char * toString(int op){
    static char oop[10];
    strcpy(oop,"");
    switch(op){
        case T_and:
            strcpy(oop,"AND");
            break;
        case T_not:
            strcpy(oop,"NOT");
            break;
        case T_lnot:
            strcpy(oop,"LNOT");
            break;
        case T_div:
            strcpy(oop,"DIV");
            break;
        case T_mul:
            strcpy(oop,"MUL");
            break;
        case T_mod:
            strcpy(oop,"MOD");
            break;
        case T_sub:
            strcpy(oop,"SUB");
            break;
        case T_add:
            strcpy(oop,"ADD");
            break;
        case T_leftmove:
            strcpy(oop,"LEFTMOVE");
            break;
        case T_rightmove:
            strcpy(oop,"RIGHTMOVE");
            break;
        case T_big:
            strcpy(oop,"BIG");
            break;
        case T_bige:
            strcpy(oop,"BIGE");
            break;
        case T_small:
            strcpy(oop,"SMALL");
            break;
        case T_smalle:
            strcpy(oop,"SMALLE");
            break;
        case T_equal:
            strcpy(oop,"EQUAL");
            break;
        case T_nequal:
            strcpy(oop,"NEQUAL");
            break;
        case T_land:
            strcpy(oop,"LAND");
            break;
        case T_lxor:
            strcpy(oop,"LXOR");
            break;
        case T_lor:
            strcpy(oop,"LOR");
            break;
        case T_or:
            strcpy(oop,"OR");
            break;
        case T_pp:
            strcpy(oop,"PLUSPLUS");
            break;
        case T_ss:
            strcpy(oop,"SUBSUB");
            break;
    }
    return oop;
}

struct codenode * NNext(struct codenode * nn){
    if(nn == NULL) return NULL;
    while(nn->next != NULL){
        nn = nn->next;
    }
    return nn;
}

void PrintCode(struct codenode * n){
    printf("�м�������ɽ׶Σ�\n");
    while(n != NULL){
        if(strcmp(n->code,"") == 0){
            n = n->next;
            continue;
        }
        printf("%s\n",n->code);
        n = n->next;
    }
    printf("�м�������ɽ���\n");
}

void TACGen_FUNCARG(struct node * arglist,struct node * varlist){
    if(arglist == NULL && varlist == NULL) return;
    if(arglist->child[0] == NULL) return;
    arglist->code = gencode(1,"");
    strcpy(varlist->alias,newAlias(varlist->type_id,varlist->level));
    TACGen(arglist->child[0]);                  //��һ�����ʽ
    NNext(arglist->code)->next = arglist->child[0]->code;
    NNext(arglist->code)->next = gencode(4,"MOV ",arglist->child[0]->alias," ",varlist->alias);
    TACGen_FUNCARG(arglist->child[1], varlist->child[1]);
    if(arglist->child[1] != NULL)
        NNext(arglist->code)->next = arglist->child[1]->code;

    return;
}

//�м��������
void TACGen(struct node * tree){
    static char code[32];
    struct symbol * s;
    struct node * n;
    if(tree == NULL) return;
    struct codenode * cn = NULL;
    if(tree->kind == EXP){      //ר�Ŵ�����ʽ
        switch(tree->op){
            case T_ID:
                strcpy(tree->alias,newAlias(tree->type_id,tree->level));
                tree->code = gencode(1,"");
                break;
            case CONST:
                strcpy(tree->alias,newAliasCONST(tree->intval, tree->floatval, tree->type));
                printf("CONST  tree->type=%s, tree->intval = %d",tree->type == T_int?"T_int":"other",tree->intval);
                tree->code = gencode(1,"");
                break;
            case T_ls:
                TACGen(tree->child[0]);
                tree->code = tree->child[0]->code;
                break;
            case T_getarr:
                TACGen(tree->child[1]);
                strcpy(tree->alias,newAliasArr(tree->child[0]->type_id,tree->child[1]->alias,tree->level));
                tree->code = gencode(1,"");
                NNext(tree->code)->next = tree->child[1]->code;
                break;
            case T_assign:
                TACGen(tree->child[0]);
                TACGen(tree->child[1]);
                tree->code = gencode(1,"");
                NNext(tree->code)->next = tree->child[0]->code;
                NNext(tree->code)->next = tree->child[1]->code;
                NNext(tree->code)->next = gencode(4,"MOV ",tree->child[1]->alias," ",tree->child[0]->alias);
                break;
            case T_not:
            case T_lnot:
                strcpy(tree->alias,newtmp());               //�����
                TACGen(tree->child[0]);                     
                sprintf(code,"%s",toString(tree->op));
                cn = gencode(7,code," ",tree->child[0]->alias," " ,"_" , " ",tree->alias);
                tree->code = tree->child[0]->code;
                NNext(tree->child[0]->code)->next = cn;
                //tree->code->prior = tree->child[0]->code;
                break;
            case T_and:
            case T_div:
            case T_mul:
            case T_mod: 
            case T_sub: 
            case T_add:
            case T_leftmove: 
            case T_rightmove:
            case T_big:
            case T_bige: 
            case T_small: 
            case T_smalle:
            case T_equal:
            case T_nequal:
            case T_land:
            case T_lxor:
            case T_lor:
            case T_or:
                printf("EXP double OP\n");
                strcpy(tree->alias,newtmp());           // �����±�����
                TACGen(tree->child[0]);
                TACGen(tree->child[1]);
                tree->code = gencode(1,"");
                NNext(tree->code)->next = tree->child[0]->code;
                NNext(tree->code)->next = tree->child[1]->code;
                sprintf(code,"%s",toString(tree->op));
                cn = gencode(7,code," ",tree->child[0]->alias, " ", tree->child[1]->alias, " ", tree->alias);
                NNext(tree->code)->next = cn;
                break;
            case T_pp:
                TACGen(tree->child[0]);
                tree->code = gencode(6,"ADD ",tree->child[0]->alias," ","1"," ",tree->child[0]->alias);
                break;
            case T_ss:
                TACGen(tree->child[0]);
                tree->code = gencode(6,"SUB ",tree->child[0]->alias," ","1"," ",tree->child[0]->alias);
                break;
            case FUNC:              //��������
                printf("EXP FUNC tree->type_id=%s\n", tree->type_id);
                s = find(tree->type_id);            //��ȫ�ַ��ű��в��Һ���
                n = s->addr;                        //�ҵ��������ڵĳ����﷨���Ľ��λ��
                tree->code = gencode(1,"");
                TACGen_FUNCARG(tree->child[1],n->child[0]);     //���ɺ������ε��м����
                if(tree->child[1] != NULL){
                    NNext(tree->code)->next = tree->child[1]->code;
                }
                NNext(tree->code)->next = gencode(2,"CALL ",tree->type_id);
                break;
        }
        return ;
    }
    switch(tree->kind){
        case EXTDEFLIST:
            printf("in EXDEFLIST\n");
            tree->code = gencode(1,"");
            TACGen(tree->child[0]);
            TACGen(tree->child[1]);
            if(tree->child[0] != NULL){
                NNext(tree->code)->next = tree->child[0]->code;
                if(tree->child[1] != NULL){
                    NNext(tree->child[0]->code)->next = tree->child[1]->code;
                }
            }
            break;
        case FUNC:
            TACGen(tree->child[0]);
            TACGen(tree->child[1]);
            tree->code = gencode(2,tree->type_id,":");
            if(tree->child[0] == NULL){             //�޲κ���
                NNext(tree->code)->next = tree->child[1]->code;
            }else{                                  //�вκ���
                NNext(tree->code)->next = tree->child[0]->code;
                NNext(tree->child[0]->code)->next = tree->child[1]->code;
            }
            break;
        case FUNC_VAR_LIST:
            TACGen(tree->child[1]);
            if(tree->child[0] == NULL || tree->child[2] == NULL) {
                return;
            }
            tree->code = gencode(2,"param: ", newAlias(tree->child[2]->type_id, tree->level));
            if(tree->child[1] != NULL){
                NNext(tree->code)->next = tree->child[1]->code;
            }
            break;
        case BODY:
            printf("BODY\n");
            TACGen(tree->child[0]);             //DECLAREVALLIST
            TACGen(tree->child[1]);             //STMTLIST
            tree->code = gencode(1,"");
            if(tree->child[0] == NULL && tree->child[1] == NULL){       //����������Ϊ��
                break;
            }
            if(tree->child[0]== NULL && tree->child[1] != NULL){              //����Ϊ��
                NNext(tree->code)->next = tree->child[1]->code;          
            }else if(tree->child[0] != NULL && tree->child[1] == NULL){         //���Ϊ��
                NNext(tree->code)->next = tree->child[0]->code;
            }else{                                                              //����Ϊ��
                NNext(tree->code)->next = tree->child[0]->code;
                NNext(tree->code)->next = tree->child[1]->code;
            }
            break;
        case DECLAREVALLIST:                //û����������
            printf("DECLAREVALLIST\n");
            TACGen(tree->child[0]);
            TACGen(tree->child[1]);
            tree->code = gencode(1,"");
            if(tree->child[0] != NULL){
                NNext(tree->code)->next = tree->child[0]->code;
                if(tree->child[1] != NULL){
                    NNext(tree->child[0]->code)->next = tree->child[1]->code;
                }
            }
            break;
        case DECLAREVAL:
            strcpy(tree->alias, newAlias(tree->type_id, tree->level));
            tree->code = gencode(2,"param: ",tree->alias);
            if(tree->child[0] != NULL){             //�г�ֵ
                TACGen(tree->child[0]);     
                NNext(tree->code)->next = tree->child[0]->code; 
                NNext(tree->code)->next = gencode(4,"MOV ", tree->child[0]->alias ," ",tree->alias);  
            }
            break;
        case STMTLIST:
            printf("STMTLIST\n");
            tree->code = gencode(1,"");
            TACGen(tree->child[0]);
            TACGen(tree->child[1]);
            if(tree->child[0] == NULL){         //û���κ����
                break;
            }
            if(tree->child[1] == NULL){         //���һ�����
                NNext(tree->code)->next = tree->child[0]->code;
            }else{
                NNext(tree->code)->next = tree->child[0]->code;
                NNext(tree->child[0]->code)->next = tree->child[1]->code;
            }
            break;
        case STMT:
            tree->code = gencode(1,"");
            if(tree->type == BODY){             //���������body
                TACGen(tree->child[0]);
                NNext(tree->code)->next = tree->child[0]->code;
            }else if(tree->type == EXP){
                printf("STMT EXP\n");
                TACGen(tree->child[0]);
                NNext(tree->code)->next = tree->child[0]->code;
            }else if(tree->type == T_return){
                NNext(tree->code)->next = gencode(1,"RETURN");
            }else if(tree->type == T_break){
                NNext(tree->code)->next = gencode(1,"BREAK");
            }else if(tree->type == T_continue){
                NNext(tree->code)->next = gencode(1,"CONTINUE");
            }else if(tree->type == IF_STMT){
                TACGen(tree->child[0]);
                NNext(tree->code)->next = tree->child[0]->code;
            }else if(tree->type == IF_ELSE_STMT){
                TACGen(tree->child[0]);
                NNext(tree->code)->next = tree->child[0]->code;
            }else if(tree->type == WHILE_STMT){
                TACGen(tree->child[0]);
                NNext(tree->code)->next = tree->child[0]->code;
            }
            break;
        case IF_STMT:
            strcpy(tree->Etrue, newLabel());
            strcpy(tree->Efalse,newLabel());
            tree->code = gencode(1,"");
            TACGen(tree->child[0]);                 //EXP����
            TACGen(tree->child[1]);                 //BODY����
            NNext(tree->code)->next = tree->child[0]->code;         //EXP
            NNext(tree->code)->next = gencode(5,"IF ", tree->child[0]->alias,"> 0 "," GOTO ", tree->Etrue);
            NNext(tree->code)->next = gencode(2, "GOTO ",tree->Efalse);
            NNext(tree->code)->next = gencode(2,tree->Etrue," : ");
            NNext(tree->code)->next = tree->child[1]->code;
            NNext(tree->code)->next = gencode(2,tree->Efalse," : ");
            break;
        case IF_ELSE_STMT:
            
            strcpy(tree->Etrue,newLabel());
            strcpy(tree->Efalse, newLabel());
            tree->code = gencode(1,"");
            TACGen(tree->child[0]);
            TACGen(tree->child[1]);
            TACGen(tree->child[2]);
            NNext(tree->code)->next = tree->child[0]->code;
            NNext(tree->code)->next = gencode(5,"IF ",tree->child[0]->alias," > 0 "," GOTO ", tree->Etrue);
            NNext(tree->code)->next = gencode(2,"GOTO ",tree->Efalse);
            NNext(tree->code)->next = gencode(2,tree->Etrue," : ");
            NNext(tree->code)->next = tree->child[1]->code;
            strcpy(code, newLabel());
            NNext(tree->code)->next = gencode(2,"GOTO ",code);
            NNext(tree->code)->next = gencode(2,tree->Efalse," : ");
            NNext(tree->code)->next = tree->child[2]->code;
            NNext(tree->code)->next = gencode(2,code, " : ");
            break;
        case WHILE_STMT:
            TACGen(tree->child[0]);
            TACGen(tree->child[1]);
            tree->code = gencode(1,"");
            strcpy(code, newLabel());       //ѭ����
            strcpy(tree->Etrue, newLabel());
            strcpy(tree->Efalse, newLabel());
            NNext(tree->code)->next = gencode(2,code," : ");
            NNext(tree->code)->next = tree->child[0]->code;
            NNext(tree->code)->next = gencode(5,"IF ",tree->child[0]->alias," > 0 ","GOTO ", tree->Etrue);
            NNext(tree->code)->next = gencode(2,"GOTO ", tree->Efalse);
            NNext(tree->code)->next = gencode(2,tree->Etrue," : ");
            NNext(tree->code)->next = tree->child[1]->code;
            NNext(tree->code)->next = gencode(2,"GOTO ",code);
            NNext(tree->code)->next = gencode(2,tree->Efalse," : ");
            break;
    }

}
















