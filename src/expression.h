#ifndef IFJ_PROJEKT_EXPRESSION_H
#define IFJ_PROJEKT_EXPRESSION_H
#include <stdio.h>
#include "stack.h"
#include "ilist.h"
#include "scanner.h"
#include "expressionParser.h"




typedef enum 
{
    //  | +- |  */ | \ | o |   ( |   ) |   i |   $ |
    plusMinusIndex, //0
    mulDivIndex,    //1
    backslashIndex,
    operatorsIndex,
    leftBracketIndex,
    rightBracketIndex,
    identificatorIndex,
    DollarIndex
}   IndexOfPrecedentTable;

int getTokenTableIndex(tState type);
void precedencSA(string* input, tExpressionList *expL, tLinkedList *linkedL);
tToken* findTerminalToken(ptrStack* topOfStack);
void pushOpenTokenToStack(ptrStack* topOfStack, tToken* exprOpenToken);
tExpressionRule applyrule(ptrStack *stack, tExpressionRule rule);
tExpressionRule extractexpression(ptrStack *stack);
void printRule(tExpressionRule rule);
void printStack(ptrStack* topStack);
void printList(tExpressionList *List, tExpressionRule rule);
//void CreateEXPList(tExpressionList *L , /*tExpressionRule *rule*/ tToken *leftToken, tToken *rightToken, int counter);
void InserList(tExpressionRule rule, tExpressionList *expL, tLinkedList *linkedL);
#endif
