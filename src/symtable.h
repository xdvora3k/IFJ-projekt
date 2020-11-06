/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 */

#ifndef IFJ_PROJEKT_SYMTABLE_H
#define IFJ_PROJEKT_SYMTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define TRUE 1
#define FALSE 0

typedef struct {
    char* type;
    double position;
} tBSTContent;

typedef struct tBSTNode {
    char* Key;
    tBSTContent Content;
    struct tBSTNode *LPtr;
    struct tBSTNode *RPtr;
} *tBSTNodePtr;

void BSTInit(tBSTNodePtr *RootPtr);
int BSTSearch(tBSTNodePtr RootPtr, char* K, tBSTContent *Content);
tBSTNodePtr BSTCreateNode(char* K, char* type, double position);
void BSTInsert(tBSTNodePtr *RootPtr, char* K, char* type, double position);
void BSTDelete(tBSTNodePtr *RootPtr, char* K);
void BSTDispose(tBSTNodePtr *RootPtr);

#endif
