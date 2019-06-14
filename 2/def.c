#include "def.h"

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
                    table[top].offset = table[top -1].offset;
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
            remove(level);
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
            if(tree->type == EXP) semanticExp(tree->child[0],level);                //�жϱ����Ƿ�δ�����ʹ��
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
            semanticAnalysisStatic(tree->child[1],level);
    }
    return;
}









