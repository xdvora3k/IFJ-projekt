/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 */

#include "ilist.h"

#define TYPE_INT    "int@"
#define TYPE_FLOAT  "float@"
#define TYPE_BOOL   "bool@"
#define TYPE_STRING "string@"
#define TYPE_NIL    "nil@"
#define FRAMEGF     "GF@"
#define FRAMELF     "LF@"
#define FRAMETF     "TF@"
#define LABEL_S     "$"
#define MAIN        "$$main"


void CreateInstruction (tLinkedList *L, int InstrType, void *addr1, void *addr2, void *addr3)
{
    tInstr  Instruciton;
    Instruciton.instType = InstrType;
    Instruciton.addr1 = addr1;
    Instruciton.addr2 = addr2;
    Instruciton.addr3 = addr3;
    InstrLLInsertFirst(L,&Instruciton);
}

tInstructionOperand CreateOperand (string value,int type,FRAME frame,bool isItLabel, bool isItVar)
{
    tInstructionOperand o;
    add_to_string(o.value,value);
    o.type = type;
    o.frame = frame;
    o.isLabel = isItLabel;
    o.isVariable = isItVar;
    return o;
}

void InstructionWithNoOperand(tLinkedList *L, int InstrType)
{
    CreateInstruction(L,InstrType,NULL,NULL,NULL);
}
/*
void InstructionWithOneOperand(tLinkedList *L, int InstrType)
{
    CreateInstruction(L,InstrType);
}

void InstructionWithTwoOperand(tLinkedList *L, int InstrType)
{
    CreateInstruction(L,InstrType);
}

void InstructionWithThreeOperand(tLinkedList *L, int InstrType)
{
    CreateInstruction(L,InstrType);
}
 ????
*/












void InstrLLInit(tLinkedList *L){
    L->first = NULL;
}

void StrLLInit(tLinkedList *L){
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

void InstrLLDeleteFirst(tLinkedList *L){
    if (!L->first){
        return;
    }
    tListItem *to_delete = L->first;
    L->first = L->first->nextItem;
    free(to_delete);
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
    int i = 0;
    tListItem *item = L->first;
    while (item){
        i++;
        item = item->nextItem;
    }
    return i;
}
