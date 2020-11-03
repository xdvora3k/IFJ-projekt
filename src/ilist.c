/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 */

#include "ilist.h"

void LLInit(tListOfInstr *L){
    L->active = NULL;
    L->first = NULL;
    L->last = NULL;
}

void LLDisposeAll(tListOfInstr *L){
    while (L->first){
        tListItem *to_delete = L->first;
        L->first = L->first->nextItem;
        free(to_delete);
    }
    L->active = NULL;
    L->first = NULL;
    L->last = NULL;
}

void LLInsertFirst(tListOfInstr *L, tInstr Instruction){
    tListItem *new_node = (tListItem*) malloc(sizeof(tListItem));
    new_node->nextItem = L->first;
    new_node->Instruction = Instruction;
    if (!L->first || !L->first->nextItem){
        L->last = new_node;
    }
    L->first = new_node;
}

void LLSetActiveToFirst(tListOfInstr *L){
    L->active = L->first;
}

void LLSetActiveToLast(tListOfInstr *L){
    L->active = L->last;
}

void LLCopyFirst(tListOfInstr *L, tInstr *Instruction){
    if (!L->first){
        return;
    }
    *Instruction = L->first->Instruction;
}

void LLDeleteFirst(tListOfInstr *L){
    if (!L->first){
        return;
    }
    tListItem *to_delete = L->first;
    L->first = L->first->nextItem;
    if (L->active == to_delete){
        L->active = NULL;
    }
    free(to_delete);
}

void LLPostDelete(tListOfInstr *L){
    if (!L->active || !L->active->nextItem){
        return;
    }
    if (L->last == L->active->nextItem){
        L->last = L->active;
    }
    tListItem *to_delete = L->active->nextItem;
    L->active->nextItem = L->active->nextItem->nextItem;
    free(&to_delete->Instruction);
    free(to_delete);
}

void LLPostInsert(tListOfInstr *L, tInstr Instruction){
    if (!L->active){
        return;
    }
    tListItem *new_node = (tListItem*) malloc(sizeof(tListItem));
    new_node->Instruction = Instruction;
    tListItem *to_move = L->active->nextItem;
    new_node->nextItem = to_move;
    if (!L->active->nextItem){
        L->last = new_node;
    }
    L->active->nextItem = new_node;
}

void LLInsertLast(tListOfInstr *L, tInstr Instruction){
    tListItem *new_node = (tListItem*) malloc(sizeof(tListItem));
    new_node->Instruction = Instruction;
    if (!L->last){
        L->first = new_node;
        L->last = new_node;
        return;
    }
    L->last->nextItem = new_node;
    L->last = new_node;
}

void LLCopyActive(tListOfInstr *L, tInstr *Instruction){
    if (!L->active){
        return;
    }
    *Instruction = L->active->Instruction;
}

void LLActualize(tListOfInstr *L, tInstr Instruction){
    if (!L->active){
        return;
    }
    L->active->Instruction = Instruction;
}

void LLActiveNext(tListOfInstr *L){
    if (!L->active){
        return;
    }
    L->active = L->active->nextItem;
}
