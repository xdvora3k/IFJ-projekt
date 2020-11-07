/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 */

#ifndef IFJ_PROJEKT_SYMTABLE_H
#define IFJ_PROJEKT_SYMTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include "str.h"
#include "ilist.h"
#include "error_codes.h"

#define TRUE 1
#define FALSE 0

typedef enum {
    IntType,
    Float64Type,
    StringType
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

typedef struct tBSTNode {
    char* Key;
    void* Content;
    struct tBSTNode *LPtr;
    struct tBSTNode *RPtr;
} *tBSTNodePtr;

typedef struct tSymtable {
    tBSTNodePtr root;
} tSymtable;

void BSTInit(tBSTNodePtr *RootPtr);
tBSTNodePtr BSTSearch(tBSTNodePtr RootPtr, char *K);
tBSTNodePtr BSTCreateNode(char* K, void* Data);
tBSTNodePtr BSTInsert(tBSTNodePtr* RootPtr, char* K, void* Data);
void BSTDelete(tBSTNodePtr *RootPtr, char* K);
void BSTDispose(tBSTNodePtr *RootPtr);

void SymTableInit(tSymtable* SymTable);
tBSTNodePtr SymTableInsertFunction(tSymtable* SymTable, string *key);
tBSTNodePtr SymTableInsertVariable(tSymtable* SymTable, string *key);
tBSTNodePtr SymTableSearch(tSymtable* SymTable, char *key);
void SymTableDelete(tSymtable* SymTable, string *key);
void SymTableDispose(tSymtable* Symtable);
void InsertBuiltInFuncs(tSymtable* SymTable);

#endif
