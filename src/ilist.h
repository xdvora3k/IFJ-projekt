/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 */

#ifndef IFJ_PROJEKT_ILIST_H
#define IFJ_PROJEKT_ILIST_H

#include <stdio.h>
#include <stdlib.h>
#include "str.h"

// TODO: Define types of instructions
// Example:
#define I_FUNC 0
#define I_PLUS 1
#define I_MINUS 2
#define I_TIMES 3
#define I_DIVIDE 4
#define I_EQUAL 5
#define I_ANOTHER 6
// Once this is fixed, the test script needs to be fixed as well.

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

void InstrLLInit(tLinkedList *L);
void InstrLLDisposeAll(tLinkedList *L);
void InstrLLInsertFirst(tLinkedList *L, tInstr Instruction);
void InstrLLCopyFirst(tLinkedList *L, tInstr *Instruction);
void InstrLLDeleteFirst(tLinkedList *L);

void StrLLInit(tLinkedList *L);
void StrLLInsert(tLinkedList *L, string K);
int StrLLStringAlreadyOccupied(tLinkedList *L, char *S);

#endif
