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
    Frame_TF,
    Frame_NaN
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
    I_DIVS,
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
    UnderscoreType = -1// nilType
} tVarDataType;

typedef struct tDataVariable {
    tVarDataType dataType;
} tDataVariable;


typedef struct operands{
    char* name;
    char* value;
    tVarDataType type;
    FRAME frame;
}tInstructionOperand;




typedef struct {
    INSTRUCTION instType;
    char *addr1;
    char *addr2;
    char *addr3;
} tInstr;

typedef struct listItem{
    void* Content;
    struct listItem *nextItem;
} tListItem;

typedef struct {
    tListItem *first;
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
void StrLLInsert(tLinkedList *L, char *K);
int StrLLStringAlreadyOccupied(tLinkedList *L, char *S);
tListItem* StrLLLocateNthElem(tLinkedList *L, int index);
int StrLLLen(tLinkedList *L);
void StrLLDeleteLast(tLinkedList *L);
void StrLLDispose(tLinkedList *L);

void TableLLInit(tLinkedList *L);
void TableLLInsertFirst(tLinkedList *L, tSymtable *local_var_table);
void TableLLDeleteFirst(tLinkedList *L);
tListItem* TableLLLocateNthElem(tLinkedList *L, int index);
int TableLLLen(tLinkedList *L);
int TableLLFindAllVariables(tLinkedList *func_variable_list, tLinkedList *variables);
int TableLLGetNumOfNests(tLinkedList *func_variable_list, char* var);
tDataVariable* TableLLGetSingleVariable(tLinkedList *func_variable_list, char* var);


void CreateInstruction (INSTRUCTION InstrType, void *addr1, void *addr2, void *addr3);
tInstructionOperand CreateOperand (char* name,char* value, tVarDataType type,FRAME f);
void Instruction0(INSTRUCTION InstrType);
void Instruction1(INSTRUCTION InstrType, tInstructionOperand op);
void Instruction2(INSTRUCTION InstrType, tInstructionOperand op, tInstructionOperand op2);
void Instruction3(INSTRUCTION InstrType, tInstructionOperand op, tInstructionOperand op2, tInstructionOperand op3);
void len(char* s, tInstructionOperand *out);
void InstructionPrint(tInstr i);



void Instr_I_MOVE(tInstr i);
void Instr_I_CREATEFRAME();
void Instr_I_PUSHFRAME();
void Instr_I_POPFRAME();
void Instr_I_DEFVAR(tInstr i);
void Instr_I_CALL(tInstr i);
void Instr_I_RETURN();
void Instr_I_PUSHS(tInstr i);
void Instr_I_POPS(tInstr i);
void Instr_I_CLEARS();
void Instr_I_ADD(tInstr i);
void Instr_I_SUB(tInstr i);
void Instr_I_MUL(tInstr i);
void Instr_I_DIV(tInstr i);
void Instr_I_IDIV(tInstr i);
void Instr_I_ADDS();
void Instr_I_SUBS();
void Instr_I_MULS();
void Instr_I_DIVS();
void Instr_I_IDIVS();
void Instr_I_LT(tInstr i);
void Instr_I_GT(tInstr i);
void Instr_I_EQ(tInstr i);
void Instr_I_LTS();
void Instr_I_GTS();
void Instr_I_EQS();
void Instr_I_AND(tInstr i);
void Instr_I_OR(tInstr i);
void Instr_I_NOT(tInstr i);
void Instr_I_ANDS();
void Instr_I_ORS();
void Instr_I_NOTS();
void Instr_I_FLOAT2INT(tInstr i);
void Instr_I_INT2FLOAT(tInstr i);
void Instr_I_INT2CHAR(tInstr i);
void Instr_I_STRI2INT(tInstr i);
void Instr_I_INT2FLOATS();
void Instr_I_FLOAT2INTS();
void Instr_I_INT2CHARS();
void Instr_I_STRI2INTS();
void Instr_I_READ(tInstr i);
void Instr_I_WRITE(tInstr i);
void Instr_I_CONCAT(tInstr i);
void Instr_I_STRLEN(tInstr i);
void Instr_I_GETCHAR(tInstr i);
void Instr_I_SETCHAR(tInstr i);
void Instr_I_TYPE(tInstr i);
void Instr_I_LABEL(tInstr i);
void Instr_I_JUMP(tInstr i);
void Instr_I_JUMPIFEQ(tInstr i);
void Instr_I_JUMPIFNEQ(tInstr i);
void Instr_I_JUMPIFEQS(tInstr i);
void Instr_I_JUMPIFNEQS(tInstr i);
void Instr_I_EXIT(tInstr i);
void Instr_I_BREAK();
void Instr_I_DPRINT(tInstr i);
#endif