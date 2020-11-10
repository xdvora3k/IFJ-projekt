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

void InstrLLInit(tLinkedList *L);
void InstrLLDisposeAll(tLinkedList *L);
void InstrLLInsertFirst(tLinkedList *L, tInstr *Instruction);
void InstrLLDeleteFirst(tLinkedList *L);

void StrLLInit(tLinkedList *L);
void StrLLInsert(tLinkedList *L, string *K);
int StrLLStringAlreadyOccupied(tLinkedList *L, char *S);

#endif
