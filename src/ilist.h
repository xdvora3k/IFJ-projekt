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
#include "scanner.h"



typedef enum{
    Frame_GF,
    Frame_LF,
    Frame_TF
}FRAME;

typedef enum{
    I_MOVE,
    I_CREATEFRAME,
    I_PUSHFRAME,
    I_POPFRAME,
    I_DEFVAR,
    I_CALL,
    I_RETURN,

    I_PUSHS,
    I_POPS,
    I_CLEARS,

    I_ADD,
    I_SUB,
    I_MUL,
    I_DIV,
    I_IDIV,
    I_ADDS,
    I_SUBS,
    I_MULS,
    I_DISV,
    I_IDIVS,
    I_LT,
    I_GT,
    I_EQ,
    I_LTS,
    I_GTS,
    I_EQS,
    I_AND,
    I_OR,
    I_NOT,
    I_ANDS,
    I_ORS,
    I_NOTS,
    I_FLOAT2INT,
    I_INT2FLOAT,
    I_INT2CHAR,
    I_STRI2INT,
    I_INT2FLOATS,
    I_FLOAT2INTS,
    I_INT2CHARS,
    I_STRI2INTS,

    I_READ,
    I_WRITE,

    I_CONCAT,
    I_STRLEN,
    I_GETCHAR,
    I_SETCHAR,

    I_TYPE,

    I_LABEL,
    I_JUMP,
    I_JUMPIFEQ,
    I_JUMPIFNEQ,
    I_JUMPIFEQS,
    I_JUMPIFNEQS,
    I_EXIT,

    I_BREAK,
    I_DPRINT
}INSTRUCTION;


typedef enum {
    IntType,
    Float64Type,
    StringType,
    Unknown_type,
    UnderscoreType = -1
} tVarDataType;

typedef struct tDataVariable {
    tVarDataType dataType;
} tDataVariable;


typedef struct operands{
    string value;
    tVarDataType type;
    FRAME frame;
    bool isVariable;
    bool isLabel;  // Label == Navesti
}tInstructionOperand;




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

void TableLLInit(tLinkedList *L);
void TableLLInsertFirst(tLinkedList *L, tSymtable *local_var_table);
tListItem* TableLLLocateNthElem(tLinkedList *L, int index);
int TableLLLen(tLinkedList *L);
tSymtable* TableLLGetLastElem(tLinkedList *L);

#endif