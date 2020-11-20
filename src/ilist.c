/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 */

#include "ilist.h"





/*#define TYPE_INT    "int@"
#define TYPE_FLOAT  "float@"
#define TYPE_BOOL   "bool@"
#define TYPE_STRING "string@"
#define TYPE_NIL    "nil@"
#define FRAMEGF     "GF@"
#define FRAMELF     "LF@"
#define FRAMETF     "TF@"
#define LABEL_S     "$"
#define MAIN        "$$main"*/


void CreateInstruction (tLinkedList *L, int InstrType, void *addr1, void *addr2, void *addr3)
{
    tInstr  Instruciton;
    Instruciton.instType = InstrType;
    Instruciton.addr1 = addr1;
    Instruciton.addr2 = addr2;
    Instruciton.addr3 = addr3;
    InstrLLInsertFirst(L,&Instruciton);
}

tInstructionOperand CreateOperand (char* value,tVarDataType type,FRAME frame,bool isItLabel, bool isItVar)
{
    tInstructionOperand o;
    adds_to_string(&(o.value),value);
    o.type = type;
    o.frame = frame;
    o.isLabel = isItLabel;
    o.isVariable = isItVar;
    return o;
}

void InstructionWithNoOperand(tLinkedList *L, tVarDataType InstrType)
{
    CreateInstruction(L,InstrType,NULL,NULL,NULL);
}

void InstructionWith1operand(tLinkedList *L, tVarDataType InstrType, tInstructionOperand op1)
{
    string operand;
    init_string(&operand);

    if(op1.type == Unknown_type)
    {
        /*if(op1.isVariable == true)
        {
            clear_str(&operand);
            adds_to_string(&operand,"GF@tmp");
        }*/
        if(op1.frame == Frame_GF)
        {
            clear_str(&operand);
            adds_to_string(&operand,"GF@");
            adds_to_string(&operand,op1.value.str);
        }
        else if(op1.frame == Frame_LF)
        {
            clear_str(&operand);
            adds_to_string(&operand,"LF@");
            adds_to_string(&operand,op1.value.str);
        }
        else if(op1.frame == Frame_TF)
        {
            clear_str(&operand);
            adds_to_string(&operand,"TF@");
            adds_to_string(&operand,op1.value.str);
        }
    }
    else if(op1.type == IntType)
    {
        clear_str(&operand);
        adds_to_string(&operand,"int@");
        adds_to_string(&operand,op1.value.str);
    }
    else if(op1.type == Float64Type)
    {
        clear_str(&operand);
        adds_to_string(&operand,"float@");
        adds_to_string(&operand,op1.value.str);
    }
    else if(op1.type == StringType)
    {
        clear_str(&operand);
        adds_to_string(&operand,"string@");
        adds_to_string(&operand,op1.value.str);
    }
    //else if(op1.type == tNil)
    else if(op1.type == -1)
    {
        clear_str(&operand);
        adds_to_string(&operand,"nil@");
        adds_to_string(&operand,op1.value.str);
    }
    if (op1.isLabel == true)
    {
        clear_str(&operand);
        adds_to_string(&operand,"$");
        if((strcmp(op1.value.str,"main")) == 0)
        {
            adds_to_string(&operand,"$main");
        }
        else
        {adds_to_string(&operand,op1.value.str);}
    }

    CreateInstruction(L,InstrType,operand.str,NULL,NULL);
}

void InstructionWith2operand(tLinkedList *L, tVarDataType InstrType, tInstructionOperand op1, tInstructionOperand op2)
{
    string operand1;
    string operand2;
    init_string(&operand1);
    init_string(&operand2);
    ///Element num 1
    if(op1.type == Unknown_type)
    {
        /*if(op1.isVariable == true)
        {
            clear_str(&operand1);
            adds_to_string(&operand1,"GF@tmp");
        }*/
        if(op1.frame == Frame_GF)
        {
            clear_str(&operand1);
            adds_to_string(&operand1,"GF@");
            adds_to_string(&operand1,op1.value.str);
        }
        else if(op1.frame == Frame_LF)
        {
            clear_str(&operand1);
            adds_to_string(&operand1,"LF@");
            adds_to_string(&operand1,op1.value.str);
        }
        else if(op1.frame == Frame_TF)
        {
            clear_str(&operand1);
            adds_to_string(&operand1,"TF@");
            adds_to_string(&operand1,op1.value.str);
        }
    }
    else if(op1.type == IntType)
    {
        clear_str(&operand1);
        adds_to_string(&operand1,"int@");
        adds_to_string(&operand1,op1.value.str);
    }
    else if(op1.type == Float64Type)
    {
        clear_str(&operand1);
        adds_to_string(&operand1,"float@");
        adds_to_string(&operand1,op1.value.str);
    }
    else if(op1.type == StringType)
    {
        clear_str(&operand1);
        adds_to_string(&operand1,"string@");
        adds_to_string(&operand1,op1.value.str);
    }
    //else if(op1.type == tNil)
    else if(op1.type == -1)
    {
        clear_str(&operand1);
        adds_to_string(&operand1,"nil@");
        adds_to_string(&operand1,op1.value.str);
    }
    if (op1.isLabel == true)
    {
        clear_str(&operand1);
        adds_to_string(&operand1,"$");
        if((strcmp(op1.value.str,"main")) == 0)
        {
            adds_to_string(&operand1,"$main");
        }
        else
        {adds_to_string(&operand1,op1.value.str);}
    }

    ///Element num 2
    if(op2.type == Unknown_type)
    {
        /*if(op2.isVariable == true)
        {
            clear_str(&operan2);
            adds_to_string(&operan2,"GF@tmp");
        }*/
        if(op2.frame == Frame_GF)
        {
            clear_str(&operand2);
            adds_to_string(&operand2,"GF@");
            adds_to_string(&operand2,op2.value.str);
        }
        else if(op2.frame == Frame_LF)
        {
            clear_str(&operand2);
            adds_to_string(&operand2,"LF@");
            adds_to_string(&operand2,op2.value.str);
        }
        else if(op2.frame == Frame_TF)
        {
            clear_str(&operand2);
            adds_to_string(&operand2,"TF@");
            adds_to_string(&operand2,op2.value.str);
        }
    }
    else if(op2.type == IntType)
    {
        clear_str(&operand2);
        adds_to_string(&operand2,"int@");
        adds_to_string(&operand2,op2.value.str);
    }
    else if(op2.type == Float64Type)
    {
        clear_str(&operand2);
        adds_to_string(&operand2,"float@");
        adds_to_string(&operand2,op2.value.str);
    }
    else if(op2.type == StringType)
    {
        clear_str(&operand2);
        adds_to_string(&operand2,"string@");
        adds_to_string(&operand2,op2.value.str);
    }
    //else if(op2.type == tNil)
    else if(op2.type == -1)
    {
        clear_str(&operand2);
        adds_to_string(&operand2,"nil@");
        adds_to_string(&operand2,op2.value.str);
    }
    if (op2.isLabel == true)
    {
        clear_str(&operand2);
        adds_to_string(&operand2,"$");
        if((strcmp(op2.value.str,"main")) == 0)
        {
            adds_to_string(&operand2,"$main");
        }
        else
        {adds_to_string(&operand2,op2.value.str);}
    }



    CreateInstruction(L,InstrType,operand1.str,operand2.str,NULL);
}

void InstructionWith3operand(tLinkedList *L, tVarDataType InstrType, tInstructionOperand op1, tInstructionOperand op2, tInstructionOperand op3)
{
    string operand1;
    string operand2;
    string operand3;
    init_string(&operand1);
    init_string(&operand2);
    init_string(&operand3);

    ///Element num 1
    if(op1.type == Unknown_type)
    {
        /*if(op1.isVariable == true)
        {
            clear_str(&operand1);
            adds_to_string(&operand1,"GF@tmp");
        }*/
        if(op1.frame == Frame_GF)
        {
            clear_str(&operand1);
            adds_to_string(&operand1,"GF@");
            adds_to_string(&operand1,op1.value.str);
        }
        else if(op1.frame == Frame_LF)
        {
            clear_str(&operand1);
            adds_to_string(&operand1,"LF@");
            adds_to_string(&operand1,op1.value.str);
        }
        else if(op1.frame == Frame_TF)
        {
            clear_str(&operand1);
            adds_to_string(&operand1,"TF@");
            adds_to_string(&operand1,op1.value.str);
        }
    }
    else if(op1.type == IntType)
    {
        clear_str(&operand1);
        adds_to_string(&operand1,"int@");
        adds_to_string(&operand1,op1.value.str);
    }
    else if(op1.type == Float64Type)
    {
        clear_str(&operand1);
        adds_to_string(&operand1,"float@");
        adds_to_string(&operand1,op1.value.str);
    }
    else if(op1.type == StringType)
    {
        clear_str(&operand1);
        adds_to_string(&operand1,"string@");
        adds_to_string(&operand1,op1.value.str);
    }
        //else if(op1.type == tNil)
    else if(op1.type == -1)
    {
        clear_str(&operand1);
        adds_to_string(&operand1,"nil@");
        adds_to_string(&operand1,op1.value.str);
    }
    if (op1.isLabel == true)
    {
        clear_str(&operand1);
        adds_to_string(&operand1,"$");
        if((strcmp(op1.value.str,"main")) == 0)
        {
            adds_to_string(&operand1,"$main");
        }
        else
        {adds_to_string(&operand1,op1.value.str);}
    }

    ///Element num 2
    if(op2.type == Unknown_type)
    {
        /*if(op2.isVariable == true)
        {
            clear_str(&operand2);
            adds_to_string(&operand2,"GF@tmp");
        }*/
        if(op2.frame == Frame_GF)
        {
            clear_str(&operand2);
            adds_to_string(&operand2,"GF@");
            adds_to_string(&operand2,op2.value.str);
        }
        else if(op2.frame == Frame_LF)
        {
            clear_str(&operand2);
            adds_to_string(&operand2,"LF@");
            adds_to_string(&operand2,op2.value.str);
        }
        else if(op2.frame == Frame_TF)
        {
            clear_str(&operand2);
            adds_to_string(&operand2,"TF@");
            adds_to_string(&operand2,op2.value.str);
        }
    }
    else if(op2.type == IntType)
    {
        clear_str(&operand2);
        adds_to_string(&operand2,"int@");
        adds_to_string(&operand2,op2.value.str);
    }
    else if(op2.type == Float64Type)
    {
        clear_str(&operand2);
        adds_to_string(&operand2,"float@");
        adds_to_string(&operand2,op2.value.str);
    }
    else if(op2.type == StringType)
    {
        clear_str(&operand2);
        adds_to_string(&operand2,"string@");
        adds_to_string(&operand2,op2.value.str);
    }
        //else if(op2.type == tNil)
    else if(op2.type == -1)
    {
        clear_str(&operand2);
        adds_to_string(&operand2,"nil@");
        adds_to_string(&operand2,op2.value.str);
    }
    if (op2.isLabel == true)
    {
        clear_str(&operand2);
        adds_to_string(&operand2,"$");
        if((strcmp(op2.value.str,"main")) == 0)
        {
            adds_to_string(&operand2,"$main");
        }
        else
        {adds_to_string(&operand2,op2.value.str);}
    }

    ///Element num 3
    if(op3.type == Unknown_type)
    {
        /*if(op3.isVariable == true)
        {
            clear_str(&operand3);
            adds_to_string(&operand3,"GF@tmp");
        }*/
        if(op3.frame == Frame_GF)
        {
            clear_str(&operand3);
            adds_to_string(&operand3,"GF@");
            adds_to_string(&operand3,op3.value.str);
        }
        else if(op3.frame == Frame_LF)
        {
            clear_str(&operand3);
            adds_to_string(&operand3,"LF@");
            adds_to_string(&operand3,op3.value.str);
        }
        else if(op3.frame == Frame_TF)
        {
            clear_str(&operand3);
            adds_to_string(&operand3,"TF@");
            adds_to_string(&operand3,op3.value.str);
        }
    }
    else if(op3.type == IntType)
    {
        clear_str(&operand3);
        adds_to_string(&operand3,"int@");
        adds_to_string(&operand3,op3.value.str);
    }
    else if(op3.type == Float64Type)
    {
        clear_str(&operand3);
        adds_to_string(&operand3,"float@");
        adds_to_string(&operand3,op3.value.str);
    }
    else if(op3.type == StringType)
    {
        clear_str(&operand3);
        adds_to_string(&operand3,"string@");
        adds_to_string(&operand3,op3.value.str);
    }
        //else if(op3.type == tNil)
    else if(op3.type == -1)
    {
        clear_str(&operand3);
        adds_to_string(&operand3,"nil@");
        adds_to_string(&operand3,op3.value.str);
    }
    if (op3.isLabel == true)
    {
        clear_str(&operand3);
        adds_to_string(&operand3,"$");
        if((strcmp(op3.value.str,"main")) == 0)
        {
            adds_to_string(&operand3,"$main");
        }
        else
        {adds_to_string(&operand3,op3.value.str);}
    }

    CreateInstruction(L,InstrType,operand1.str,operand2.str,operand3.str);
}





void StrLLInit(tLinkedList *L){
    L->first = NULL;
}

void StrLLInsert(tLinkedList *L, string *K){
    tListItem *new_node = (tListItem*) malloc(sizeof(tListItem));
    new_node->Content = K;
    if (!L->first){
        L->first = new_node;
        return;
    }
    tListItem *curr = L->first;
    while (curr->nextItem){
        curr = curr->nextItem;
    }
    curr->nextItem = new_node;
}

int StrLLStringAlreadyOccupied(tLinkedList *L, char *S){
    tListItem *node = L->first;
    while (node){
        if (!strcmp(((string*) node->Content)->str, S)){
            return TRUE;
        }
        node = node->nextItem;
    }
    return FALSE;
}

void StrLLDeleteLast(tLinkedList *L){
    tListItem **to_delete = &L->first;
    if (!to_delete){
        return;
    }
    while ((*to_delete)->nextItem){
        to_delete = &(*to_delete)->nextItem;
    }
    (*to_delete)->nextItem = NULL;
    free(*to_delete);
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

int StrLLLen(tLinkedList *L){
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

// Symtable List
//---------------------------------------------------------
void TableLLInit(tLinkedList *L){
    L->first = NULL;
}

void TableLLDeleteFirst(tLinkedList *L){
    if (!L->first){
        return;
    }
    SymTableDispose(L->first->Content);
    L->first = L->first->nextItem;
}

void TableLLInsertFirst(tLinkedList *L, tSymtable *local_var_table){
    tListItem *new_node = malloc(sizeof(tListItem));
    new_node->Content = local_var_table;
    if (!L->first){
        L->first = new_node;
        new_node->nextItem = NULL;
    }
    new_node->nextItem = L->first;
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

tSymtable* TableLLGetLastElem(tLinkedList *L){
    tListItem *node = L->first;
    while (node->nextItem){
        node = node->nextItem;
    }
    return (tSymtable*) node->Content;
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