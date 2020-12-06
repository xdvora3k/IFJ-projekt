/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 *         xkvasn14, Jaroslav Kvasnicka
 */


#ifndef IFJ_CONNECTOR_H
#define IFJ_CONNECTOR_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ilist.h"
#include "parser.h"
#include "expression.h"

typedef struct FinalVariable{
    char* key;
    char* real_variable_name;
    int count;
    struct FinalVariable *next;
} tFinalVariable;

typedef struct tFinalList{
    tFinalVariable *first;
} tFinalList;

void VarLLInit(tFinalList *L);
char* VarLLInsert(tFinalList *L, char* name, char* func_name, tLinkedList *func_variable_list);

void print_variable_declaration_Expression(tLinkedList *leftside, tExpressionList *rightside);
void print_variable_assigment_Expression(tLinkedList *leftside,tExpressionList *rightside);
char* Calc_Int_Expression(tExpressionNode *Rules);
char* Calc_Float_Expression(tExpressionNode *Rules);
char* Calc_String_Expression(tExpressionNode *Rules);
void print_print_Expression(tPassedSide *Frases);
void print_int2float_Expression(tLinkedList *leftside, tPassedSide * rightside);
void print_float2int_Expression(tLinkedList *leftside, tPassedSide * rightside);
void print_function_begin(char* funcName, tPassedSide * retvariables, tPassedSide* variables);
void print_function_end();
void print_function_assigment(tLinkedList *leftside, char* funcName,tPassedSide *retvariables,tPassedSide* variables,tPassedSide *params);
#endif //IFJ_CONNECTOR_H
