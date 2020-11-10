/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 */

#include "ilist.h"

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

void InstrLLInsertFirst(tLinkedList *L, tInstr Instruction){
    tListItem *new_node = (tListItem*) malloc(sizeof(tListItem));
    new_node->nextItem = L->first;
    ((tInstr*) new_node->Content) = Instruction;
    L->first = new_node;
}

void StrLLInsertFirst(tLinkedList *L, string K){
    tListItem *new_node = (tListItem*) malloc(sizeof(tListItem));
    ((string*) new_node->Content) = K;
    new_node->nextItem = L->first;
    L->first = new_node;
}

void InstrLLCopyFirst(tLinkedList *L, tInstr *Instruction){
    if (!L->first){
        return;
    }
    *Instruction = ((tInstr*) L->first->Content);
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
        if (((string*) node->Content)->str == S){
            return TRUE;
        }
        node = node->nextItem;
    }
    return FALSE;
}
