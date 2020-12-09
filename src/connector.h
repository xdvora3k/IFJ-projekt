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
void define_built_in_variables(tFinalList* FinalList);

void print_variable_declaration_Expression(tLinkedList *leftside, tExpressionList *rightside, tLinkedList *func_variable_list);
void print_return_assignment(tExpressionList *rightside, char* funcName, tLinkedList *func_variable_list);
void print_variable_assigment_Expression(tLinkedList *leftside,tExpressionList *rightside, tLinkedList *func_variable_list);
char* Calc_Int_Expression(tExpressionNode *Rules,tLinkedList *func_variable_list);
char* Calc_Float_Expression(tExpressionNode *Rules,tLinkedList *func_variable_list);
char* Calc_String_Expression(tExpressionNode *Rules,tLinkedList *func_variable_list);
void print_print_Expression(tPassedSide *Frases, tLinkedList* func_variable_list);
void print_int2float_Expression(tLinkedList *leftside, tPassedSide * rightside,tLinkedList* func_variable_list, char* funcName);
void print_float2int_Expression(tLinkedList *leftside, tPassedSide * rightside,tLinkedList* func_variable_list, char* funcName);
void print_function_begin(char* funcName);
void print_function_end();
void print_function_assigment(tLinkedList *leftside, char* funcName,tPassedSide* variables,tPassedSide *params, tLinkedList* func_variable_list, int is_return);
void print_if_begin(tExpressionNode* expList, tLinkedList *func_variable_list);
void print_else_begin(tLinkedList *func_variable_list);
void print_if_end(tLinkedList *func_variable_list);
void print_for_begin(tExpressionNode* expList, tLinkedList *leftsideAssigment, tExpressionList *rightsideAssigment ,tLinkedList* func_variable_list);
void print_for_end(tLinkedList* func_variable_list);
#endif //IFJ_CONNECTOR_H
