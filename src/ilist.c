/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 *         xkvasn14, Jaroslav Kvasnicka
 */

#include "ilist.h"

void StrLLInit(tLinkedList *L){
    L->first = NULL;
}

void StrLLInsert(tLinkedList *L, char *K){
    tListItem *new_item = malloc(sizeof(tListItem));
    int str_len = strlen(K);
    char *content = malloc(str_len + 1);
    strncpy(content, K, str_len);
    content[str_len] = '\0';
    new_item->Content = content;
    new_item->nextItem = NULL;

    if (!L->first){
        L->first = new_item;
        return;
    }

    tListItem *curr = L->first;
    while (curr->nextItem){
        curr = curr->nextItem;
    }
    curr->nextItem = new_item;
}

int StrLLStringAlreadyOccupied(tLinkedList *L, char *S){
    tListItem *node = L->first;
    while (node){
        if (!strcmp((char*) node->Content, S)){
            return TRUE;
        }
        node = node->nextItem;
    }
    return FALSE;
}

void StrLLDeleteLast(tLinkedList *L){
    if (!L->first){
        return;
    }

    tListItem **to_delete = &L->first;
    while ((*to_delete)->nextItem){
        to_delete = &(*to_delete)->nextItem;
    }

    free((*to_delete)->Content);
    free(*to_delete);
    *to_delete = NULL;
}

void StrLLDispose(tLinkedList *L){
    while (L->first){
        StrLLDeleteLast(L);
    }
}

tListItem* StrLLLocateNthElem(tLinkedList *L, int index){
    tListItem *item = L->first;
    for (int i = 0; i < index; i++){
        item = item->nextItem;
    }
    return item;
}

int StrLLLen(tLinkedList *L) {
    if (!L->first) {
        return 0;
    }
    int i = 0;

    tListItem *item = L->first;
    while (item) {
        i++;
        item = item->nextItem;
    }
    return i;
}

// Symtable List
//---------------------------------------------------------
void TableLLInit(tLinkedList *L){
    L->first = NULL;
}

void TableLLDeleteFirst(tLinkedList *L){
    if (!L->first){
        return;
    }
    tListItem *to_delete = L->first;
    L->first = to_delete->nextItem;
    SymTableDispose(to_delete->Content);
    free(to_delete);
}

void TableLLInsertFirstSeenVariable(tLinkedList *L, tLinkedList *variables, tExpressionList *expr_list){
    tDataVariable *var = SymTableInsertVariable(L->first->Content, variables->first->Content)->Content;
    var->dataType = expr_list->first->data_type;
}

void TableLLInsertFirst(tLinkedList *L, tSymtable *local_var_table){
    tListItem *new_node = malloc(sizeof(tListItem));
    new_node->Content = malloc(sizeof(tSymtable));
    if (!L->first){
        new_node->nextItem = NULL;
    }
    else {
        new_node->nextItem = L->first;
    }
    new_node->Content = local_var_table;
    L->first = new_node;
}

tListItem* TableLLLocateNthElem(tLinkedList *L, int index){
    tListItem *item = L->first;
    for (int i = 0; i < index; i++){
        item = item->nextItem;
    }
    return item;
}

int TableLLLen(tLinkedList *L){
    if (!L->first){
        return 0;
    }
    int i = 0;
    tListItem *item = L->first;
    while (item){
        i++;
        item = item->nextItem;
    }
    return i;
}

tDataVariable* TableLLGetSingleVariable(tLinkedList *func_variable_list, char* var){
    if (!func_variable_list || !func_variable_list->first){
        return NULL;
    }
    for (int i = 0; i < TableLLLen(func_variable_list); i++){
        tSymtable *curr_table = (tSymtable*) TableLLLocateNthElem(func_variable_list, i)->Content;
        if (!SymTableSearch(curr_table, var)){
            continue;
        }
        tDataVariable *var_node = (tDataVariable*) SymTableSearch(curr_table, var)->Content;
        if (var_node){
            return var_node;
        }
    }
    return NULL;
}

int TableLLFindAllVariables(tLinkedList *func_variable_list, tLinkedList *variables){
    if (!func_variable_list || !func_variable_list->first){
        return 100;
    }
    if (!variables || !variables->first){
        return 1000;
    }
    int not_found = 0;
    for (int i = 0; i < StrLLLen(variables); i++){
        char *var_name = StrLLLocateNthElem(variables, i)->Content;
        if (!TableLLGetSingleVariable(func_variable_list, var_name) && strcmp(var_name, "_")){
            not_found++;
        }
    }

    return not_found;
}

int TableLLGetNumOfNests(tLinkedList *func_variable_list, char* var){
    if (!func_variable_list || !func_variable_list->first){
        return -1;
    }
    int table_len = TableLLLen(func_variable_list);
    for (int i = 0; i < table_len; i++){
        tSymtable *curr_table = (tSymtable*) TableLLLocateNthElem(func_variable_list, i)->Content;
        if (SymTableSearch(curr_table, var)){
            return table_len - i;
        }
    }
    return -1;
}

// Passed side list
//---------------------------------------------------------

void PassedLLInit(tPassedSide *L){
    L->first = NULL;
}

void PassedLLInsert(tPassedSide *L, char* value, int is_variable, tVarDataType data_type){
    tPassedNode *new_node = malloc(sizeof(tPassedNode));
    new_node->is_variable = is_variable;
    int str_len = strlen(value);
    new_node->value = malloc(str_len + 1);
    strncpy(new_node->value, value, str_len);
    new_node->value[str_len] = '\0';
    new_node->data_type = data_type;
    new_node->nextItem = NULL;

    if (!L->first){
        L->first = new_node;
        return;
    }

    tPassedNode *item = L->first;
    while (item->nextItem){
        item = item->nextItem;
    }
    item->nextItem = new_node;
}

void PassedLLDeleteLast(tPassedSide *L){
    if (!L->first){
        return;
    }

    tPassedNode **to_delete = &L->first;
    while ((*to_delete)->nextItem){
        to_delete = &(*to_delete)->nextItem;
    }

    free(*to_delete);
    *to_delete = NULL;
}

void PassedLLDispose(tPassedSide *L){
    while (L->first){
        PassedLLDeleteLast(L);
    }
}

// Expression List
//---------------------------------------------------------

void ExprLLInit(tExpressionList *L){
    L->first = NULL;
}

void ExprLLCreateNextNode(tExpressionList *L, tVarDataType data_type){
    tExpressionNode *last_node = L->first;
    if (last_node) {
        while (last_node->next_node) {
            last_node = last_node->next_node;
        }
    }

    tExpressionNode *new_node = malloc(sizeof(tExpressionNode));
    new_node->first = NULL;
    new_node->data_type = data_type;
    new_node->next_node = NULL;

    if (!L->first){
        L->first = new_node;
        return;
    }
    last_node->next_node = new_node;
}

tToken* _insert_token_to_node(tToken *token){
    tToken *ret = malloc(sizeof(tToken));
    ret->endIndex = token->endIndex;
    ret->type = token->type;
    ret->text = malloc(sizeof(string));
    ret->text->allocSize = token->text->allocSize;
    ret->text->length = token->text->length;
    int str_len = strlen(token->text->str);
    ret->text->str = malloc(str_len + 1);
    strncpy(ret->text->str, token->text->str, str_len);
    ret->text->str[str_len] = '\0';
    return ret;
}

void ExprLLInsertExprToLastNode(tExpressionList *L, tToken *leftOperand, tToken* rightOperand, tToken* operator, tToken* placeHolder, ruleType typeOfRule){
    tExpressionNode *last_node = L->first;
    if (last_node) {
        while (last_node->next_node) {
            last_node = last_node->next_node;
        }
    }
    tExpressionRule *last_rule = last_node->first;
    if (last_rule) {
        while (last_rule->next) {
            last_rule = last_rule->next;
        }
    }

    tExpressionRule *new_rule = malloc(sizeof(tExpressionRule));
    new_rule->leftOperand = _insert_token_to_node(leftOperand);
    new_rule->rightOperand = _insert_token_to_node(rightOperand);
    new_rule->operator = _insert_token_to_node(operator);
    new_rule->placeHolder = _insert_token_to_node(placeHolder);
    new_rule->typeOfRule = typeOfRule;
    new_rule->next = NULL;
    if (!last_node->first){
        last_node->first = new_rule;
        return;
    }
    last_rule->next = new_rule;
}

int ExprLLNodeLen(tExpressionList *L){
    tExpressionNode *node = L->first;
    int i = 0;
    while (node){
        node = node->next_node;
        i++;
    }
    return i;
}

int ExprLLRuleLen(tExpressionList *L, int index){
    tExpressionNode *node = L->first;
    while (index){
        node = node->next_node;
        if (!node){
            return -1;
        }
        index--;
    }
    tExpressionRule *rule = node->first;
    int i = 0;
    while (rule){
        rule = rule->next;
        i++;
    }

    return i;
}

tExpressionNode* ExprLLGetNthNode(tExpressionList *L, int index){
    tExpressionNode *node = L->first;
    while (index){
        node = node->next_node;
        if (!node){
            return NULL;
        }
        index--;
    }
    return node;
}

tExpressionRule* ExprLLGetNthRule(tExpressionList *L, int node_index, int rule_index){
    tExpressionNode *node = ExprLLGetNthNode(L, node_index);
    if (!node){
        return NULL;
    }
    tExpressionRule *rule = node->first;
    while (rule_index){
        rule = rule->next;
        if (!rule){
            return NULL;
        }
        rule_index--;
    }
    return rule;
}

// Instruction List
//---------------------------------------------------------
void InstrLLInit(tLinkedList *L){
    L->first = NULL;
}

void InstrLLDisposeAll(tLinkedList *L){
    while (L->first){
        tListItem *to_delete = L->first;
        L->first = L->first->nextItem;
        free(to_delete);
    }
    L->first = NULL;
}

void InstrLLInsertFirst(tLinkedList *L, tInstr* Instruction){
    tListItem *new_node = (tListItem*) malloc(sizeof(tListItem));
    new_node->nextItem = L->first;
    new_node->Content = Instruction;
    L->first = new_node;
}

void InstrLLDeleteFirst(tLinkedList *L){
    if (!L->first){
        return;
    }
    tListItem *to_delete = L->first;
    L->first = L->first->nextItem;
    free(to_delete);
}


void CreateInstruction(INSTRUCTION InstrType, char *addr1, char *addr2, char *addr3)
{
    tInstr Instruction;
    Instruction.instType = InstrType;
    int str_len1 = strlen(addr1);
    int str_len2 = strlen(addr2);
    int str_len3 = strlen(addr3);
    Instruction.addr1 = malloc(str_len1 + 1);
    strncpy(Instruction.addr1, addr1, str_len1);
    Instruction.addr1[str_len1] = '\0';
    Instruction.addr2 = malloc(str_len2 + 1);
    strncpy(Instruction.addr2, addr2, str_len2);
    Instruction.addr2[str_len2] = '\0';
    Instruction.addr3 = malloc(str_len3 + 1);
    strncpy(Instruction.addr3, addr3, str_len3);
    Instruction.addr3[str_len3] = '\0';
    InstructionPrint(Instruction);
    free(Instruction.addr1);
    free(Instruction.addr2);
    free(Instruction.addr3);
}

tInstructionOperand CreateOperand(char* name, char* value, tVarDataType type, FRAME f)
{
    tInstructionOperand o;
    int str_len_name = strlen(name);
    int str_len_value = strlen(value);
    o.name = malloc(str_len_name + 1);
    strncpy(o.name, name, str_len_name);
    o.name[str_len_name] = '\0';
    o.value = malloc(str_len_value + 1);
    strncpy(o.name, value, str_len_value);
    o.name[str_len_value] = '\0';
    o.type = type;
    o.frame = f;
    return o;
}
void Instruction0(INSTRUCTION InstrType)
{
    CreateInstruction(InstrType,"","","");
}

char* _process_operand(tInstructionOperand *op){
    char* tmp;
    if(op->frame == Frame_NaN)
    {
        // func, konst, spec, labels
        if(op->type == Unknown_type)
        {
            // func or main, specs or labels
            tmp = op->name;
        }
        else
        {
            // konst
            switch(op->type) {
                case IntType:
                    tmp = "int@";
                    break;
                case Float64Type:
                    tmp = "float@";
                    break;
                case StringType:
                    tmp = "string@";
                    break;
                case UnderscoreType:
                    tmp = "nil@";
                    break;
                default:
                    tmp = "";
            }
            tmp = strcat(tmp,op->value);
        }
    }
    else // if (op->frame != Frame_NaN)
    {
        //variables
        if(op->frame == Frame_GF)
            tmp = "GF@";
        else if(op->frame == Frame_LF)
            tmp = "LF@";
        else if(op->frame == Frame_TF)
            tmp = "TF@";
        tmp = strcat(tmp,op->name);
    }
    return tmp;
}

void Instruction1(INSTRUCTION InstrType, tInstructionOperand op)
{
    char *tmp = _process_operand(&op);
    CreateInstruction(InstrType,tmp,"","");
}

void Instruction2(INSTRUCTION InstrType, tInstructionOperand op, tInstructionOperand op2)
{
    char* tmp = _process_operand(&op);
    char* tmp2 = _process_operand(&op2);
    CreateInstruction(InstrType,tmp,tmp2,"");
}

void Instruction3(INSTRUCTION InstrType, tInstructionOperand op, tInstructionOperand op2, tInstructionOperand op3)
{
    char *tmp = _process_operand(&op);
    char *tmp2 = _process_operand(&op2);
    char *tmp3 = _process_operand(&op3);
    CreateInstruction(InstrType,tmp,tmp2,tmp3);
}

char* _process_frame(tInstructionOperand *o){
    char *frame;
    switch (o->frame){
        case Frame_GF:
            frame = "GF";
            break;
        case Frame_LF:
            frame = "LF";
            break;
        case Frame_TF:
            frame = "TF";
            break;
        case Frame_NaN:
            frame = "";
            break;
    }
    return frame;
}

void inputi(tInstructionOperand o)
{
    printf("READ %s@%s int\n", _process_frame(&o), o.name);
}

void inputs(tInstructionOperand o)
{
    printf("READ %s@%s int\n", _process_frame(&o), o.name);
}

void inputf(tInstructionOperand o)
{
    printf("READ %s@%s int\n", _process_frame(&o), o.name);
}

void Instr_I_MOVE(tInstr i){            printf("MOVE %s %s\n",i.addr1,i.addr2);}
void Instr_I_CREATEFRAME(){             printf("CREATEFRAME\n");}
void Instr_I_PUSHFRAME(){               printf("PUSHFRAME\n");}
void Instr_I_POPFRAME(){                printf("POPFRAME\n");}
void Instr_I_DEFVAR(tInstr i){          printf("DEFVAR %s\n",i.addr1);}
void Instr_I_CALL(tInstr i){            printf("LABEL %s\n",i.addr1);}
void Instr_I_RETURN(){                  printf("RETURN\n");}
void Instr_I_PUSHS(tInstr i){           printf("PUSHS %s\n",i.addr1);}
void Instr_I_POPS(tInstr i){            printf("POPS %s\n",i.addr1);}
void Instr_I_CLEARS(){                  printf("CLEARS\n");}
void Instr_I_ADD(tInstr i){             printf("ADD %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_SUB(tInstr i){             printf("SUB %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_MUL(tInstr i){             printf("MUL %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_DIV(tInstr i){             printf("DIV %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_IDIV(tInstr i){            printf("IDIV %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_ADDS(){                    printf("ADDS\n");}
void Instr_I_SUBS(){                    printf("SUBS\n");}
void Instr_I_MULS(){                    printf("MULS\n");}
void Instr_I_DIVS(){                    printf("DIVS\n");}
void Instr_I_IDIVS(){                   printf("IDIVS\n");}
void Instr_I_LT(tInstr i){              printf("LT %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_GT(tInstr i){              printf("GT %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_EQ(tInstr i){              printf("EQ %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_LTS(){                     printf("LTS\n");}
void Instr_I_GTS(){                     printf("GTS\n");}
void Instr_I_EQS(){                     printf("EQS\n");}
void Instr_I_AND(tInstr i){             printf("AND %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_OR(tInstr i){              printf("OR %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_NOT(tInstr i){             printf("NOT %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_ANDS(){                    printf("ANDS\n");}
void Instr_I_ORS(){                     printf("ORS\n");}
void Instr_I_NOTS(){                    printf("NOTS\n");}
void Instr_I_FLOAT2INT(tInstr i){       printf("FLOAT2INT %s %s\n",i.addr1,i.addr2);}
void Instr_I_INT2FLOAT(tInstr i){       printf("INT2FLOAT %s %s\n",i.addr1,i.addr2);}
void Instr_I_INT2CHAR(tInstr i){        printf("INT2CHAR %s %s\n",i.addr1,i.addr2);}
void Instr_I_STRI2INT(tInstr i){        printf("STRI2INT %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_INT2FLOATS(){              printf("INT2FLOATS\n");}
void Instr_I_FLOAT2INTS(){              printf("FLOAT2INTS\n");}
void Instr_I_INT2CHARS(){               printf("INT2CHARS\n");}
void Instr_I_STRI2INTS(){               printf("STRI2INTS\n");}
void Instr_I_READ(tInstr i){            printf("READ %s %s\n",i.addr1,i.addr2);}
void Instr_I_WRITE(tInstr i){           printf("WRITE %s",i.addr1);}
void Instr_I_CONCAT(tInstr i){          printf("CONCAT %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_STRLEN(tInstr i){          printf("STRLEN %s %s\n",i.addr1,i.addr2);}
void Instr_I_GETCHAR(tInstr i){         printf("GETCHAR %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_SETCHAR(tInstr i){         printf("SETCHAR %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_TYPE(tInstr i){            printf("TYPE %s %s\n",i.addr1,i.addr2);}
void Instr_I_LABEL(tInstr i){           printf("LABEL %s\n",i.addr1);}
void Instr_I_JUMP(tInstr i){            printf("JUMP %s\n",i.addr1);}
void Instr_I_JUMPIFEQ(tInstr i){        printf("JUMPIFEQ %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_JUMPIFNEQ(tInstr i){       printf("JUMPIFNEQ %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_JUMPIFEQS(tInstr i){       printf("JUMPIFEQS %s\n",i.addr1);}
void Instr_I_JUMPIFNEQS(tInstr i){      printf("JUMPIFNEQS %s\n",i.addr1);}
void Instr_I_EXIT(tInstr i){            printf("EXIT %s\n",i.addr1);}
void Instr_I_BREAK(){                   printf("BREAK\n");}
void Instr_I_DPRINT(tInstr i){          printf("DPRINT %s\n",i.addr1);}

void InstructionPrint(tInstr i)
{
    switch (i.instType) {
        case I_MOVE:
            Instr_I_MOVE(i);
            break;
        case I_CREATEFRAME:
            Instr_I_CREATEFRAME();
            break;
        case I_PUSHFRAME:
            Instr_I_PUSHFRAME();
            break;
        case I_POPFRAME:
            Instr_I_POPFRAME();
            break;
        case I_DEFVAR:
            Instr_I_DEFVAR(i);
            break;
        case I_CALL:
            Instr_I_CALL(i);
            break;
        case I_RETURN:
            Instr_I_RETURN();
            break;
        case I_PUSHS:
            Instr_I_PUSHS(i);
            break;
        case I_POPS:
            Instr_I_POPS(i);
            break;
        case I_CLEARS:
            Instr_I_CLEARS();
            break;
        case I_ADD:
            Instr_I_ADD(i);
            break;
        case I_SUB:
            Instr_I_SUB(i);
            break;
        case I_MUL:
            Instr_I_MUL(i);
            break;
        case I_DIV:
            Instr_I_DIV(i);
            break;
        case I_IDIV:
            Instr_I_IDIV(i);
            break;
        case I_ADDS:
            Instr_I_ADDS();
            break;
        case I_SUBS:
            Instr_I_SUBS();
            break;
        case I_MULS:
            Instr_I_MULS();
            break;
        case I_DIVS:
            Instr_I_DIVS();
            break;
        case I_IDIVS:
            Instr_I_IDIVS();
            break;
        case I_LT:
            Instr_I_LT(i);
            break;
        case I_GT:
            Instr_I_GT(i);
            break;
        case I_EQ:
            Instr_I_EQ(i);
            break;
        case I_LTS:
            Instr_I_LTS();
            break;
        case I_GTS:
            Instr_I_GTS();
            break;
        case I_EQS:
            Instr_I_EQS();
            break;
        case I_AND:
            Instr_I_AND(i);
            break;
        case I_OR:
            Instr_I_OR(i);
            break;
        case I_NOT:
            Instr_I_NOT(i);
            break;
        case I_ANDS:
            Instr_I_ANDS();
            break;
        case I_ORS:
            Instr_I_ORS();
            break;
        case I_NOTS:
            Instr_I_NOTS();
            break;
        case I_FLOAT2INT:
            Instr_I_FLOAT2INT(i);
            break;
        case I_INT2FLOAT:
            Instr_I_INT2FLOAT(i);
            break;
        case I_INT2CHAR:
            Instr_I_INT2CHAR(i);
            break;
        case I_STRI2INT:
            Instr_I_STRI2INT(i);
            break;
        case I_INT2FLOATS:
            Instr_I_INT2FLOATS();
            break;
        case I_FLOAT2INTS:
            Instr_I_FLOAT2INTS();
            break;
        case I_INT2CHARS:
            Instr_I_INT2CHARS();
            break;
        case I_STRI2INTS:
            Instr_I_STRI2INTS();
            break;
        case I_READ:
            Instr_I_READ(i);
            break;
        case I_WRITE:
            Instr_I_WRITE(i);
            break;
        case I_CONCAT:
            Instr_I_CONCAT(i);
            break;
        case I_STRLEN:
            Instr_I_STRLEN(i);
            break;
        case I_GETCHAR:
            Instr_I_GETCHAR(i);
            break;
        case I_SETCHAR:
            Instr_I_SETCHAR(i);
            break;
        case I_TYPE:
            Instr_I_TYPE(i);
            break;
        case I_LABEL:
            Instr_I_LABEL(i);
            break;
        case I_JUMP:
            Instr_I_JUMP(i);
            break;
        case I_JUMPIFEQ:
            Instr_I_JUMPIFEQ(i);
            break;
        case I_JUMPIFNEQ:
            Instr_I_JUMPIFNEQ(i);
            break;
        case I_JUMPIFEQS:
            Instr_I_JUMPIFEQS(i);
            break;
        case I_JUMPIFNEQS:
            Instr_I_JUMPIFNEQS(i);
            break;
        case I_EXIT:
            Instr_I_EXIT(i);
            break;
        case I_BREAK:
            Instr_I_BREAK();
            break;
        case I_DPRINT:
            Instr_I_DPRINT(i);
            break;
        default:
            break;
    }
}

void Print_BuiltIn_Functions()
{
    printf(".IFJcode20\n\n");
    //DEF 2x tmp
    printf("JUMP $$main\n\n");

    //printf inputi
    printf("LABEL $inputi\n"
           "PUSHFRAME\n"
           "DEFVAR LF@reval\n"
           "READ LF@retval int\n"
           "POPFRAME\n"
           "RETURN\n");

    //printf inputf
    printf("LABEL $inputf\n"
           "PUSHFRAME\n"
           "DEFVAR LF@retval"
           "READ LF@retval float\n"
           "POPFRAME\n"
           "RETURN\n");

    //printf inputs
    printf("LABEL $inputs\n"
           "PUSHFRAME\n"
           "DEFVAR LF@retval\n"
           "READ LF@retval string\n"
           "POPFRAME\n"
           "RETURN\n");

    //printf int2float
    printf("LABEL $int2float\n"
           "PUSHFRAME\n"
           "DEFVAR LF@retval\n"
           "INT2FLOAT LF@retval LF@i\n"
           "POPFRAME\n"
           "RETURN\n");

    //printf float2int
    printf("LABEL $float2int\n"
           "PUSHFRAME\n"
           "DEFVAR LF@retval\n"
           "FLOAT2INT LF@retval LF@f\n"
           "POPFRAME\n"
           "RETURN\n");

    //Print len
    printf("LABEL $len\n"
           "PUSHFRAME\n"
           "DEFVAR LF@retval\n"
           "STRLEN LF@retval LF@s\n"
           "POPFRAME \n"
           "RETURN\n");

    //print substr
    printf("LABEL $substr\n"
           "PUSHFRAME\n"
           "DEFVAR LF@retval\n"
           "MOVE LF@retval string@\n"
           "DEFVAR LF@length\n"
           "CREATEFRAME\n"
           "DEFVAR TF%%0\n"
           "MOVE TF@%%0 LF%%0\n"
           "CALL $len\n"
           "MOVE LF@length TF@%%retval\n"
           "DEFVAR LF@ret\n"
           "LT LF@ret LF@length int@0\n"
           "JUMPIFEQ $substr$return LF@ret bool@true\n"
           "EQ LF@ret LF@length int@0\n"
           "JUMPIFEQ $substr$return LF@ret bool@true\n"
           "LT LF@ret LF@%%1 int@0\n"
           "JUMPIFEQ $substr$return LF@ret bool@true\n"
           "EQ LF@ret LF@%%1 int@0\n"
           "JUMPIFEQ $substr$return LF@ret bool@true\n"
           "GT LF@ret LF@%%1 LF@length\n"
           "JUMPIFEQ $substr$return LF@ret bool@true\n"
           "EQ LF@ret LF@%%2 int@0\n"
           "JUMPIFEQ $substr$return LF@ret bool@true\n"
           "DEFVAR LF@max\n"
           "MOVE LF@max LF@length\n"
           "SUB LF@max LF@max LF@%%1\n"
           "ADD LF@max LF@max int@1\n"
           "DEFVAR LF@edit\n"
           "LT LF@edit LF@%%2 int@0\n"
           "JUMPIFEQ $substr$edit LF@edit bool@true\n"
           "GT LF@edit LF@%%2 LF@max\n"
           "JUMPIFEQ $substr$edit LF@edit bool@true\n"
           "JUMP $substr$process\n"
           "LABEL $substr$edit\n"
           "MOVE LF@%%2 LF@max\n"
           "LABEL $substr$process\n"
           "DEFVAR LF@index\n"
           "MOVE LF@index LF@%%1\n"
           "SUB LF@index LF@index int@1\n"
           "DEFVAR LF@char\n"
           "DEFVAR LF@processloop\n"
           "LABEL $substr$loop\n"
           "GETCHAR LF@char LF@%%0 LF@index\n"
           "CONCAT LF@retval LF@retval LF@char\n"
           "ADD LF@index LF@index int@1\n"
           "SUB LF@%%2 LF@%%2 int@1\n"
           "GT LF@processloop LF@%%2 int@0\n"
           "JUMPIFEQ $substr$loop LF@processloop bool@true\n"
           "LABEL $substr$return\n"
           "POPFRAME\n"
           "RETURN\n");

    //printf chr
    printf("LABEL $chr\n"
           "PUSHFRAME\n"
           "DEFVAR LF@retval\n"
           "MOVE LF@retval string@\n"
           "DEFVAR LF@cond\n"
           "LT LF@cond LF@%%0 int@0\n"
           "JUMPIFEQ $chr$return LF@cond bool@true\n"
           "GT LF@cond LF@%%0 int@255\n"
           "JUMPIFEQ $chr$return LF@cond bool@true\n"
           "INT2CHAR LF@retval LF@%%0\n"
           "LABEL $chr$return\n"
           "POPFRAME\n"
           "RETURN\n");

    //printf ord
    printf("LABEL $ord\n"
           "PUSHFRAME\n"
           "DEFVAR LF@retval\n"
           "MOVE LF@retval int@0\n"
           "DEFVAR LF@cond\n"
           "LT LF@cond LF@%%1 int@1\n"
           "JUMPIFEQ $ord$return LF@cond bool@true\n"
           "DEFVAR LF@length\n"
           "CREATEFRAME\n"
           "DEFVAR TF@%%0\n"
           "MOVE TF@%%0 LF@%%0\n"
           "CALL $len\n"
           "MOVE LF@length TF@retval\n"
           "GT LF@cond LF@%%1 LF@length\n"
           "JUMPIFEQ $ord$return LF@cond bool@true\n"
           "SUB LF@%%1 LF@%%1 int@1\n"
           "STRI2INT LF@retval LF@%%0 LF@%%1\n"
           "LABEL $ord$return\n"
           "POPFRAME\n"
           "RETURN\n");

    //printf begin of main
    printf("LABEL $$main\n");
}
