/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 *         xkvasn14, Jaroslav Kvasnicka
 */

#include "ilist.h"
extern tLinkedList *instructions;

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
    if (!L || !L->first) {
        return 0;
    }
    int i = 0;

    tListItem *item = L->first;
    while (item) {
        fflush(stdout);
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
    int str_len;
    if (value) {
        str_len = strlen(value);
    }
    else {
        str_len = 0;
    }
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

int PassedLLLen(tPassedSide *L){
    tPassedNode *node = L->first;
    int i = 0;
    while (node){
        node = node->nextItem;
        i++;
    }
    return i;
}

tPassedNode* PassedLLGetNode(tPassedSide *L, int index){
    tPassedNode *node = L->first;
    while (index){
        node = node->nextItem;
        if (!node){
            return NULL;
        }
        index--;
    }
    return node;
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
    ret->dataType = token->dataType;
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
    if(leftOperand != NULL){
        new_rule->leftOperand = _insert_token_to_node(leftOperand);
    }
    if(rightOperand != NULL){
        new_rule->rightOperand = _insert_token_to_node(rightOperand);
    }
    if(operator != NULL){
        new_rule->operator = _insert_token_to_node(operator);
    }
    if(placeHolder != NULL){
        new_rule->placeHolder = _insert_token_to_node(placeHolder);
    }
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

int ExprLLRuleRuleLen(tExpressionNode *L){
    tExpressionRule *node = L->first;
    int i = 0;
    while (node){
        node = node->next;
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

tExpressionRule* ExprLLGetNthRuleRule(tExpressionNode *L,int rule_index){
    tExpressionNode *node = L;
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

tInstructionOperand* CreateOperand(char* name, char* value, tVarDataType type, FRAME f)
{
    if(!name)
    {
        return NULL;
    }
    tInstructionOperand *o = malloc(sizeof(tInstructionOperand));
    int str_len_name = strlen(name);
    int str_len_value = strlen(value);
    o->name = malloc(str_len_name + 1);
    strncpy(o->name, name, str_len_name);
    o->name[str_len_name] = '\0';
    o->value = malloc(str_len_value + 1);
    strncpy(o->value, value, str_len_value);
    o->value[str_len_value] = '\0';
    o->type = type;
    o->frame = f;
    return o;
}

tInstructionOperand* ChangeOperand (tInstructionOperand *op, char* name,char* value, tVarDataType type,FRAME f)
{
    //printf("%s , %s\n",name,value);
    int str_len_name = strlen(name);
    int str_len_value = strlen(value);
    op->name = realloc(op->name,str_len_name + 1);
    op->value = realloc(op->value,str_len_value + 1);
    strncpy(op->name,name,str_len_name);
    strncpy(op->value,value,str_len_value);
    op->name[str_len_name] = '\0';
    op->value[str_len_value] = '\0';
    op->type = type;
    op->frame = f;
    return op;
}

void Instruction0(INSTRUCTION InstrType)
{
    CreateInstruction(InstrType,"","","");
}

char* _process_operand(tInstructionOperand *op){

    string tmp;
    init_string(&tmp);

    if(op->frame == Frame_NaN)
    {
        // func, konst, spec, labels
        if(op->type == Unknown_type)
        {
            // func or main, specs or labels
            adds_to_string(&tmp,op->name);
        }
        else
        {
            // konst
            switch(op->type) {
                case IntType:
                    adds_to_string(&tmp,"int@");
                    break;
                case Float64Type:
                    adds_to_string(&tmp,"float@");
                    break;
                case StringType:
                    adds_to_string(&tmp,"string@");
                    break;
                case UnderscoreType:
                    adds_to_string(&tmp,"nil@");
                    break;
                default:
                    break;
            }
            adds_to_string(&tmp,op->value);
        }
    }
    else // if (op->frame != Frame_NaN)
    {
        //variables
        if(op->frame == Frame_GF)
        adds_to_string(&tmp,"GF@");
        else if(op->frame == Frame_LF)
            adds_to_string(&tmp,"LF@");
        else if(op->frame == Frame_TF)
            adds_to_string(&tmp,"TF@");

        adds_to_string(&tmp,op->name);
    }
    return tmp.str;

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
    int size = strlen(_process_frame(&o)) + strlen(o.name) + strlen("READ  int") + 1;
    char *a = malloc(size);
    snprintf(a, size, "READ %s@%s int", _process_frame(&o), o.name);
    // TODO
}

void inputs(tInstructionOperand o)
{
    int size = strlen(_process_frame(&o)) + strlen(o.name) + strlen("READ  string") + 1;
    char *a = malloc(size);
    snprintf(a, size, "READ %s@%s string", _process_frame(&o), o.name);
    // TODO
}

void inputf(tInstructionOperand o)
{
    int size = strlen(_process_frame(&o)) + strlen(o.name) + strlen("READ  float") + 1;
    char *a = malloc(size);
    snprintf(a, size, "READ %s@%s float", _process_frame(&o), o.name);
    // TODO
}

void Instr_I_MOVE(tInstr i){
    int size = strlen(i.addr1) + strlen(i.addr2) + strlen("MOVE  ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "MOVE %s %s", i.addr1, i.addr2);
    // TODO
}
void Instr_I_CREATEFRAME(){             char *a = "CREATEFRAME";}
void Instr_I_PUSHFRAME(){               char *a = "PUSHFRAME";}
void Instr_I_POPFRAME(){                char *a = "POPFRAME";}
void Instr_I_DEFVAR(tInstr i){
    int size = strlen(i.addr1) + strlen("DEFVAR ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "DEFVAR %s", i.addr1);
    // TODO
}
void Instr_I_CALL(tInstr i){
    int size = strlen(i.addr1) + strlen("LABEL ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "LABEL %s", i.addr1);
    // TODO
}
void Instr_I_RETURN(){                  char *a = "RETURN";}
void Instr_I_PUSHS(tInstr i){
    int size = strlen(i.addr1) + strlen("PUSHS ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "PUSHS %s", i.addr1);
    // TODO
}
void Instr_I_POPS(tInstr i){
    int size = strlen(i.addr1) + strlen("POPS ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "POPS %s", i.addr1);
    // TODO
}
void Instr_I_CLEARS(){                  printf("CLEARS\n");fflush(stdout);}
void Instr_I_ADD(tInstr i){
    int size = strlen(i.addr1) + strlen(i.addr2) + strlen(i.addr3) + strlen("ADD   ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "ADD %s %s %s", i.addr1, i.addr2, i.addr3);
    // TODO
}
void Instr_I_SUB(tInstr i) {
    int size = strlen(i.addr1) + strlen(i.addr2) + strlen(i.addr3) + strlen("SUB   ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "SUB %s %s %s", i.addr1, i.addr2, i.addr3);
    // TODO
}
void Instr_I_MUL(tInstr i){
    int size = strlen(i.addr1) + strlen(i.addr2) + strlen(i.addr3) + strlen("MUL   ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "MUL %s %s %s", i.addr1, i.addr2, i.addr3);
    // TODO
}
void Instr_I_DIV(tInstr i){
    int size = strlen(i.addr1) + strlen(i.addr2) + strlen(i.addr3) + strlen("DIV   ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "DIV %s %s %s", i.addr1, i.addr2, i.addr3);
    // TODO
}
void Instr_I_IDIV(tInstr i) {
    int size = strlen(i.addr1) + strlen(i.addr2) + strlen(i.addr3) + strlen("IDIV   ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "IDIV %s %s %s", i.addr1, i.addr2, i.addr3);
    // TODO
}
void Instr_I_ADDS(){                    char *a = "ADDS";}
void Instr_I_SUBS(){                    char *a = "SUBS";}
void Instr_I_MULS(){                    char *a = "MULS";}
void Instr_I_DIVS(){                    char *a = "DIVS";}
void Instr_I_IDIVS(){                   char *a = "IDIVS";}
void Instr_I_LT(tInstr i){
    int size = strlen(i.addr1) + strlen(i.addr2) + strlen(i.addr3) + strlen("LT   ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "LT %s %s %s", i.addr1, i.addr2, i.addr3);
    // TODO
}
void Instr_I_GT(tInstr i) {
    int size = strlen(i.addr1) + strlen(i.addr2) + strlen(i.addr3) + strlen("GT   ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "GT %s %s %s", i.addr1, i.addr2, i.addr3);
    // TODO
}
void Instr_I_EQ(tInstr i) {
    int size = strlen(i.addr1) + strlen(i.addr2) + strlen(i.addr3) + strlen("EQ   ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "EQ %s %s %s", i.addr1, i.addr2, i.addr3);
    // TODO
}
void Instr_I_LTS(){                     char *a = "LTS";}
void Instr_I_GTS(){                     char *a = "GTS";}
void Instr_I_EQS(){                     char *a = "EQS";}
void Instr_I_AND(tInstr i) {
    int size = strlen(i.addr1) + strlen(i.addr2) + strlen(i.addr3) + strlen("AND   ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "AND %s %s %s", i.addr1, i.addr2, i.addr3);
    // TODO
}
void Instr_I_OR(tInstr i) {
    int size = strlen(i.addr1) + strlen(i.addr2) + strlen(i.addr3) + strlen("OR   ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "OR %s %s %s", i.addr1, i.addr2, i.addr3);
    // TODO
}
void Instr_I_NOT(tInstr i) {
    int size = strlen(i.addr1) + strlen(i.addr2) + strlen(i.addr3) + strlen("NOT   ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "NOT %s %s %s", i.addr1, i.addr2, i.addr3);
    // TODO
}
void Instr_I_ANDS(){                    char *a = "ANDS";}
void Instr_I_ORS(){                     char *a = "ORS";}
void Instr_I_NOTS(){                    char *a = "NOTS";}
void Instr_I_FLOAT2INT(tInstr i) {
    int size = strlen(i.addr1) + strlen(i.addr2) + strlen("FLOAT2INT   ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "FLOAT2INT %s %s", i.addr1, i.addr2);
    // TODO
}
void Instr_I_INT2FLOAT(tInstr i) {
    int size = strlen(i.addr1) + strlen(i.addr2) + strlen("INT2FLOAT   ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "INT2FLOAT %s %s", i.addr1, i.addr2);
    // TODO
}
void Instr_I_INT2CHAR(tInstr i) {
    int size = strlen(i.addr1) + strlen(i.addr2) + strlen("INT2CHAR   ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "INT2CHAR %s %s", i.addr1, i.addr2);
    // TODO
}
void Instr_I_STRI2INT(tInstr i) {
    int size = strlen(i.addr1) + strlen(i.addr2) + strlen(i.addr3) + strlen("STRI2INT   ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "STRI2INT %s %s %s", i.addr1, i.addr2, i.addr3);
    // TODO
}
void Instr_I_INT2FLOATS(){              char *a = "INT2FLOATS";}
void Instr_I_FLOAT2INTS(){              char *a = "FLOAT2INTS";}
void Instr_I_INT2CHARS(){               char *a = "INT2CHARS";}
void Instr_I_STRI2INTS(){               char *a = "STRI2INTS";}
void Instr_I_READ(tInstr i){
    int size = strlen(i.addr1) + strlen(i.addr2) + strlen("READ   ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "READ %s %s", i.addr1, i.addr2);
    // TODO
}
void Instr_I_WRITE(tInstr i) {
    int size = strlen(i.addr1) + strlen("WRITE  ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "WRITE %s", i.addr1);
    // TODO
}
void Instr_I_CONCAT(tInstr i) {
    int size = strlen(i.addr1) + strlen(i.addr2) + strlen(i.addr3) + strlen("CONCAT   ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "CONCAT %s %s %s", i.addr1, i.addr2, i.addr3);
    // TODO
}
void Instr_I_STRLEN(tInstr i){
    int size = strlen(i.addr1) + strlen(i.addr2) + strlen("STRLEN   ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "STRLEN %s %s", i.addr1, i.addr2);
    // TODO
}
void Instr_I_GETCHAR(tInstr i) {
    int size = strlen(i.addr1) + strlen(i.addr2) + strlen(i.addr3) + strlen("GETCHAR   ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "GETCHAR %s %s %s", i.addr1, i.addr2, i.addr3);
    // TODO
}
void Instr_I_SETCHAR(tInstr i) {
    int size = strlen(i.addr1) + strlen(i.addr2) + strlen(i.addr3) + strlen("SETCHAR   ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "SETCHAR %s %s %s", i.addr1, i.addr2, i.addr3);
    // TODO
}
void Instr_I_TYPE(tInstr i) {
    int size = strlen(i.addr1) + strlen(i.addr2) + strlen("TYPE   ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "TYPE %s %s", i.addr1, i.addr2);
    // TODO
}
void Instr_I_LABEL(tInstr i) {
    int size = strlen(i.addr1) + strlen("LABEL  ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "LABEL %s", i.addr1);
    // TODO
}
void Instr_I_JUMP(tInstr i) {
    int size = strlen(i.addr1) + strlen("JUMP  ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "JUMP %s", i.addr1);
    // TODO
}
void Instr_I_JUMPIFEQ(tInstr i) {
    int size = strlen(i.addr1) + strlen(i.addr2) + strlen(i.addr3) + strlen("JUMPIFEQ   ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "JUMPIFEQ %s %s %s", i.addr1, i.addr2, i.addr3);
    // TODO
}
void Instr_I_JUMPIFNEQ(tInstr i) {
    int size = strlen(i.addr1) + strlen(i.addr2) + strlen(i.addr3) + strlen("JUMPIFNEQ   ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "JUMPIFNEQ %s %s %s", i.addr1, i.addr2, i.addr3);
    // TODO
}
void Instr_I_JUMPIFEQS(tInstr i) {
    int size = strlen(i.addr1) + strlen("JUMPIFEQS  ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "JUMPIFEQS %s", i.addr1);
    // TODO
}
void Instr_I_JUMPIFNEQS(tInstr i) {
    int size = strlen(i.addr1) + strlen("JUMPIFNEQS  ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "JUMPIFNEQS %s", i.addr1);
    // TODO
}
void Instr_I_EXIT(tInstr i) {
    int size = strlen(i.addr1) + strlen("EXIT  ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "EXIT %s", i.addr1);
    // TODO
}
void Instr_I_BREAK(){                   char *a = "BREAK";}
void Instr_I_DPRINT(tInstr i) {
    int size = strlen(i.addr1) + strlen("DPRINT  ") + 1;
    char *a = malloc(size);
    snprintf(a, size, "DPRINT %s", i.addr1);
    // TODO
}

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

void Print_BuiltIn_Functions(tLinkedList *instructions)
{
    StrLLInsert(instructions, "IFJcode20");
    StrLLInsert(instructions, "JUMP $$main");
    StrLLInsert(instructions, "LABEL $inputi");

    StrLLInsert(instructions, "PUSHFRAME");
    StrLLInsert(instructions, "READ GF@-retvalInt_inputi_0 int");
    StrLLInsert(instructions, "POPFRAME");
    StrLLInsert(instructions, "RETURN");
    StrLLInsert(instructions, "LABEL $inputi");
    StrLLInsert(instructions, "LABEL $inputi");
    StrLLInsert(instructions, "LABEL $inputi");

    //printf inputi
    fflush(stdout);

    StrLLInsert(instructions, "LABEL $inputf");
    StrLLInsert(instructions, "PUSHFRAME");
    StrLLInsert(instructions, "READ GF@-retvalFloat_inputf_0 float");
    StrLLInsert(instructions, "POPFRAME");
    StrLLInsert(instructions, "RETURN");
    
    //printf inputf


    StrLLInsert(instructions, "LABEL $inputs");
    StrLLInsert(instructions, "PUSHFRAME");
    StrLLInsert(instructions, "READ GF@-retvalString_inputs_0 string");
    StrLLInsert(instructions, "POPFRAME");
    StrLLInsert(instructions, "RETURN");


    //printf inputs


    StrLLInsert(instructions, "LABEL $int2float");
    StrLLInsert(instructions, "PUSHFRAME");
    StrLLInsert(instructions, "INT2FLOAT GF@-retvalFloat_int2float_0 GF@i_int2float_1_0");
    StrLLInsert(instructions, "POPFRAME");
    StrLLInsert(instructions, "RETURN");

    //printf int2float
  

    //printf float2int

    StrLLInsert(instructions, "LABEL $float2int");
    StrLLInsert(instructions, "PUSHFRAME");
    StrLLInsert(instructions, "FLOAT2INT GF@-retvalInt_float2int_0 GF@f_float2int_1_0");
    StrLLInsert(instructions, "POPFRAME");
    StrLLInsert(instructions, "RETURN");



    //Print len
    StrLLInsert(instructions, "LABEL $len");
    StrLLInsert(instructions, "PUSHFRAME");
    StrLLInsert(instructions, "STRLEN GF@-retvalInt_len_0 GF@s_len_1_0");
    StrLLInsert(instructions, "POPFRAME");
    StrLLInsert(instructions, "RETURN");



    //print substr

    StrLLInsert(instructions, "LABEL $substr");
    StrLLInsert(instructions, "PUSHFRAME");
    StrLLInsert(instructions, "MOVE GF@-retvalString_substr_0 string@");
    StrLLInsert(instructions, "CREATEFRAME");
    StrLLInsert(instructions, "DEFVAR TF@s");

    StrLLInsert(instructions, "MOVE TF@s GF@substr_s");
    StrLLInsert(instructions, "CALL $len");
    StrLLInsert(instructions, "MOVE GF@substr_length GF@-retvalInt_len_0");
    StrLLInsert(instructions, "LT GF@ret_substr GF@substr_length int@0");
    StrLLInsert(instructions, "JUMPIFEQ $substr$return GF@ret_substr bool@true");


    StrLLInsert(instructions, "EQ GF@ret_substr GF@substr_length int@0");
    StrLLInsert(instructions, "JUMPIFEQ $substr$return GF@ret_substr bool@true");
    StrLLInsert(instructions, "LT GF@ret_substr GF@substr_i int@0");
    StrLLInsert(instructions, "JUMPIFEQ $substr$return GF@ret_substr bool@true");
    StrLLInsert(instructions, "EQ GF@ret_substr GF@substr_i int@0");


    StrLLInsert(instructions, "JUMPIFEQ $substr$return GF@ret_substr bool@true");
    StrLLInsert(instructions, "GT GF@ret_substr GF@substr_i GF@substr_length");
    StrLLInsert(instructions, "JUMPIFEQ $substr$return GF@ret_substr bool@true");
    StrLLInsert(instructions, "EQ GF@ret_substr GF@substr_n int@0");
    StrLLInsert(instructions, "JUMPIFEQ $substr$return GF@ret_substr bool@true");


    StrLLInsert(instructions, "MOVE GF@max_substr GF@substr_length");
    StrLLInsert(instructions, "SUB GF@max_substr GF@max_substr GF@substr_i");
    StrLLInsert(instructions, "ADD GF@max_substr GF@max_substr int@1");
    StrLLInsert(instructions, "LT GF@edit GF@substr_n int@0");
    StrLLInsert(instructions, "JUMPIFEQ $substr$edit GF@edit bool@true");


    StrLLInsert(instructions, "GT GF@edit GF@substr_n GF@max_substr");
    StrLLInsert(instructions, "JUMPIFEQ $substr$edit GF@edit bool@true");
    StrLLInsert(instructions, "JUMP $substr$process");
    StrLLInsert(instructions, "LABEL $substr$edit");
    StrLLInsert(instructions, "MOVE GF@substr_n GF@max_substr");


    StrLLInsert(instructions, "LABEL $substr$process");
    StrLLInsert(instructions, "MOVE GF@index_substr GF@substr_i");
    StrLLInsert(instructions, "SUB GF@index_substr GF@index_substr int@1");
    StrLLInsert(instructions, "DEFVAR GF@char");
    StrLLInsert(instructions, "DEFVAR GF@processloop");


    StrLLInsert(instructions, "LABEL $substr$loop");
    StrLLInsert(instructions, "GETCHAR GF@char GF@substr_s GF@index_substr");
    StrLLInsert(instructions, "CONCAT GF@-retvalString_substr_0 GF@-retvalString_substr_0 GF@char");
    StrLLInsert(instructions, "ADD GF@index_substr GF@index_substr int@1");
    StrLLInsert(instructions, "SUB GF@substr_n GF@substr_n int@1");


    StrLLInsert(instructions, "GT GF@processloop GF@substr_n int@0");
    StrLLInsert(instructions, "JUMPIFEQ $substr$loop GF@processloop bool@true");
    StrLLInsert(instructions, "LABEL $substr$return");
    StrLLInsert(instructions, "POPFRAME");
    StrLLInsert(instructions, "RETURN");

    

    //printf chr

    StrLLInsert(instructions, "LABEL $chr");
    StrLLInsert(instructions, "PUSHFRAME");
    StrLLInsert(instructions, "MOVE GF@-retvalString_chr_0 string@");
    StrLLInsert(instructions, "LT GF@-retvalInt_chr_1 GF@i_chr_1_0 int@0");
    StrLLInsert(instructions, "JUMPIFEQ $chr$return GF@-retvalInt_chr_1 bool@true");


    StrLLInsert(instructions, "GT GF@cond GF@i_chr_1_0 int@255");
    StrLLInsert(instructions, "JUMPIFEQ $chr$return GF@-retvalInt_chr_1 bool@true");
    StrLLInsert(instructions, "INT2CHAR GF@-retvalString_chr_0 GF@i_chr_1_0");
    StrLLInsert(instructions, "LABEL $chr$return");
    StrLLInsert(instructions, "POPFRAME");
    StrLLInsert(instructions, "RETURN");

 

    //printf ord


    StrLLInsert(instructions, "LABEL $ord");
    StrLLInsert(instructions, "PUSHFRAME");
    StrLLInsert(instructions, "MOVE GF@-retvalInt_ord_0 int@0");
    StrLLInsert(instructions, "LT GF@-retvalInt_ord_1 GF@s_ord_1_0 int@0");
    StrLLInsert(instructions, "JUMPIFEQ $ord$return GF@-retvalInt_ord_1 bool@true");
    StrLLInsert(instructions, "CREATEFRAME");


    StrLLInsert(instructions, "MOVE GF@s_len_1_0 GF@s_ord_1_0");
    StrLLInsert(instructions, "CALL $len");
    StrLLInsert(instructions, "MOVE GF@length_ord GF@-retvalInt_len_0");
    StrLLInsert(instructions, "GT GF@-retvalInt_ord_1 GF@s_len_1_0 GF@length_ord");
    StrLLInsert(instructions, "JUMPIFEQ $ord$return GF@-retvalInt_ord_1 bool@true");
    StrLLInsert(instructions, "SUB GF@i_ord_1_0 GF@s_len_1_0 int@1");


    StrLLInsert(instructions, "STRI2INT GF@-retvalInt_ord_0 GF@s_ord_1_0 GF@s_len_1_0");
    StrLLInsert(instructions, "LABEL $ord$return");
    StrLLInsert(instructions, "POPFRAME");
    StrLLInsert(instructions, "RETURN");


}

void print_Mainframe_begin()
{
    //printf begin of main
    printf("LABEL $$main\n\n");
    fflush(stdout);
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");

    fflush(stdout);

}

void print_Mainframe_end()
{
    printf("POPFRAME\n");fflush(stdout);
}
