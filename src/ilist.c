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
    string new;
    init_string(&new);
    adds_to_string(&new, K->str);
    new_node->Content = &new;
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
    tListItem *to_delete = L->first;
    L->first = L->first->nextItem;
    SymTableDispose(to_delete->Content);
    free(to_delete);
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

int TableLLFindAllVariables(tLinkedList *func_variable_list, tLinkedList *variables){
    if (!func_variable_list || !func_variable_list->first){
        return 100;
    }
    if (!variables || !variables->first){
        return 1000;
    }
    int not_found = 0;
    for (int i = 0; i < StrLLLen(variables); i++){
        for (int j = 0; j < TableLLLen(func_variable_list); j++){
            tSymtable *curr_table = (tSymtable*) TableLLLocateNthElem(func_variable_list, j)->Content;
            tListItem *var_node = StrLLLocateNthElem(variables, i);
            if (SymTableSearch(curr_table, ((string*) var_node->Content)->str)){
                continue;
            }
        }
        not_found++;
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

tDataVariable* TableLLGetSingleVariable(tLinkedList *func_variable_list, char* var){
    if (!func_variable_list || !func_variable_list->first){
        return NULL;
    }
    for (int i = 0; i < TableLLLen(func_variable_list); i++){
        tSymtable *curr_table = (tSymtable*) TableLLLocateNthElem(func_variable_list, i)->Content;
        tDataVariable *var_node = (tDataVariable*) SymTableSearch(curr_table, var)->Content;
        if (var_node){
            return var_node;
        }
    }
    return NULL;
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
