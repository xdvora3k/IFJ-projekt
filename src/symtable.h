/*
 * IFJ project 2020
 * Authors: xdvora3k, Jakub Dvorak
 *          xkuzel08, Marie Kuzelova
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include "str.h"
#include "ilist.h"
#include "error_codes.h"

#ifndef IFJ_PROJEKT_SYMTABLE_H
#define IFJ_PROJEKT_SYMTABLE_H

#define TRUE 1
#define FALSE 0

typedef struct tBSTNode {
    char* Key;
    void* Content;
    struct tBSTNode *LPtr;
    struct tBSTNode *RPtr;
} *tBSTNodePtr;

typedef struct {
    tBSTNodePtr root;
} tSymtable;

void BSTInit(tBSTNodePtr *RootPtr);
tBSTNodePtr BSTSearch(tBSTNodePtr RootPtr, char *K);
tBSTNodePtr BSTCreateNode(char* K, void* Data);
tBSTNodePtr BSTInsert(tBSTNodePtr* RootPtr, char* K, void* Data);
void BSTDelete(tBSTNodePtr *RootPtr, char* K);
void BSTDispose(tBSTNodePtr *RootPtr);

void SymTableInit(tSymtable* SymTable);
tBSTNodePtr SymTableInsertFunction(tSymtable* SymTable, char *key);
tBSTNodePtr SymTableInsertVariable(tSymtable* SymTable, char *key);
tBSTNodePtr SymTableSearch(tSymtable* SymTable, char *key);
void SymTableDelete(tSymtable* SymTable, char *key);
void SymTableDispose(tSymtable* Symtable);
void InsertBuiltInFuncs(tSymtable* SymTable);

#endif
