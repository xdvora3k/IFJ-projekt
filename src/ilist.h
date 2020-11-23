/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 */

#ifndef IFJ_PROJEKT_ILIST_H
#define IFJ_PROJEKT_ILIST_H

#include <stdio.h>
#include <stdlib.h>
#include "str.h"
#include <string.h>
#include "symtable.h"

// TODO: Fix Instr
typedef struct {
    int instType;
    void *addr1;
    void *addr2;
    void *addr3;
} tInstr;

typedef struct listItem{
    void* Content;
    struct listItem *nextItem;
} tListItem;

typedef struct {
    struct listItem *first;
} tLinkedList;

typedef enum {
    IntType,
    Float64Type,
    StringType,

    UnderscoreType = -1
} tVarDataType;

typedef struct tDataVariable {
    tVarDataType dataType;
} tDataVariable;

typedef struct tDataFunction {
    string returnType;
    bool declared;
    bool defined;
    string params;
    bool list_initialized;
    tLinkedList paramNames;
} tDataFunction;

void InstrLLInit(tLinkedList *L);
void InstrLLDisposeAll(tLinkedList *L);
void InstrLLInsertFirst(tLinkedList *L, tInstr *Instruction);
void InstrLLDeleteFirst(tLinkedList *L);

void StrLLInit(tLinkedList *L);
void StrLLInsert(tLinkedList *L, string *K);
int StrLLStringAlreadyOccupied(tLinkedList *L, char *S);
tListItem* StrLLLocateNthElem(tLinkedList *L, int index);
int StrLLLen(tLinkedList *L);
void StrLLDispose(tLinkedList *L);

void TableLLInit(tLinkedList *L);
void TableLLInsertFirst(tLinkedList *L, tSymtable *local_var_table);
void TableLLDeleteFirst(tLinkedList *L);
tListItem* TableLLLocateNthElem(tLinkedList *L, int index);
int TableLLLen(tLinkedList *L);
int TableLLFindAllVariables(tLinkedList *func_variable_list, tLinkedList *variables);
int TableLLGetNumOfNests(tLinkedList *func_variable_list, char* var);
tDataVariable* TableLLGetSingleVariable(tLinkedList *func_variable_list, char* var);

#endif
