/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 */

#include "ilist.h"


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