/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 */

#ifndef IFJ_PROJEKT_ILIST_H
#define IFJ_PROJEKT_ILIST_H

#include <stdio.h>
#include <stdlib.h>

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
    tInstr Instruction;
    struct listItem *nextItem;
} tListItem;

typedef struct {
    struct listItem *first;
    struct listItem *last;
    struct listItem *active;
} tListOfInstr;

void LLInit(tListOfInstr *L);
void LLDisposeAll(tListOfInstr *L);
void LLInsertFirst(tListOfInstr *L, tInstr Instruction);
void LLSetActiveToFirst(tListOfInstr *L);
void LLSetActiveToLast(tListOfInstr *L);
void LLCopyFirst(tListOfInstr *L, tInstr *Instruction);
void LLDeleteFirst(tListOfInstr *L);
void LLPostDelete(tListOfInstr *L);
void LLPostInsert(tListOfInstr *L, tInstr Instruction);
void LLInsertLast(tListOfInstr *L, tInstr Instruction);
void LLCopyActive(tListOfInstr *L, tInstr *Instruction);
void LLActualize(tListOfInstr *L, tInstr Instruction);
void LLActiveNext(tListOfInstr *L);

#endif
