/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 *         xkvasn14, Jaroslav Kvasnicka
 */

#ifndef IFJ_PROJEKT_ILIST_H
#define IFJ_PROJEKT_ILIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "str.h"
#include "scanner.h"
#include "symtable.h"
#include "expression.h"

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

typedef struct tPassedNode {
    char* value;
    int is_variable;
    tVarDataType data_type;
    struct tPassedNode *nextItem;
} tPassedNode;

typedef struct tPassedSide {
    tPassedNode *first;
} tPassedSide;

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

typedef struct tExpressionRule{
    tToken* leftOperand;
    tToken* rightOperand;
    tToken* operator;
    tToken* placeHolder;
    ruleType typeOfRule;
    struct tExpressionRule *next;
} tExpressionRule;

typedef struct tExpressionNode {
    tVarDataType data_type;
    tExpressionRule *first;
    struct tExpressionNode *next_node;
} tExpressionNode;

typedef struct tExpressionList {
    tExpressionNode *first;
} tExpressionList;

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
void TableLLInsertFirstSeenVariable(tLinkedList *L, tLinkedList *variables, tExpressionList *expr_list);
tListItem* TableLLLocateNthElem(tLinkedList *L, int index);
int TableLLLen(tLinkedList *L);
int TableLLFindAllVariables(tLinkedList *func_variable_list, tLinkedList *variables);
int TableLLGetNumOfNests(tLinkedList *func_variable_list, char* var);
tDataVariable* TableLLGetSingleVariable(tLinkedList *func_variable_list, char* var);

void CreateInstruction (INSTRUCTION InstrType, char *addr1, char *addr2, char *addr3);
tInstructionOperand CreateOperand (char* name,char* value, tVarDataType type,FRAME f);
void Instruction0(INSTRUCTION InstrType);
void Instruction1(INSTRUCTION InstrType, tInstructionOperand op);
void Instruction2(INSTRUCTION InstrType, tInstructionOperand op, tInstructionOperand op2);
void Instruction3(INSTRUCTION InstrType, tInstructionOperand op, tInstructionOperand op2, tInstructionOperand op3);
void InstructionPrint(tInstr i);
void Print_BuiltIn_Functions();

void PassedLLInit(tPassedSide *L);
void PassedLLInsert(tPassedSide *L, char* value, int is_variable, tVarDataType data_type);
void PassedLLDeleteLast(tPassedSide *L);
void PassedLLDispose(tPassedSide *L);

void ExprLLInit(tExpressionList *L);
void ExprLLCreateNextNode(tExpressionList *L, tVarDataType data_type);
void ExprLLInsertExprToLastNode(tExpressionList *L, tToken *leftOperand, tToken *rightOperand, tToken *operator, tToken *placeHolder, ruleType typeOfRule);
int ExprLLNodeLen(tExpressionList *L);
int ExprLLRuleLen(tExpressionList *L, int index);
tExpressionNode* ExprLLGetNthNode(tExpressionList *L, int index);
tExpressionRule* ExprLLGetNthRule(tExpressionList *L, int node_index, int rule_index);

#endif
