//
// Created by xkvasn14 on 02.12.20.
//

#ifndef IFJ_CONNECTOR_H
#define IFJ_CONNECTOR_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ilist.h"
#include "parser.h"
#include "expression.h"

void print_variable_declaration_Expression(tLinkedList *leftside, tExpressionList *rightside);
void print_variable_assigment_Expression(tLinkedList *leftside,tExpressionList *rightside);
char* Calc_Int_Expression(tExpressionNode *Rules);
char* Calc_Float_Expression(tExpressionNode *Rules);
char* Calc_String_Expression(tExpressionNode *Rules);
void print_print_Expression(tPassedSide *Frases);
void print_int2float_Expression(tLinkedList *leftside, tPassedSide * rightside);
void print_float2int_Expression(tLinkedList *leftside, tPassedSide * rightside);
#endif //IFJ_CONNECTOR_H
