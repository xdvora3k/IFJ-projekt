#ifndef IFJ_PROJEKT_EXPRESSION_H
#define IFJ_PROJEKT_EXPRESSION_H
#include <stdio.h>
#include "stack.h"
#include "ilist.h"
#include "scanner.h"
#include "expressionParser.h"

int getTokenTableIndex(tState type);
void precedencSA(string* input);
//int instructionSwap (INSTRUCTION instr);

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

tToken* findTerminalToken(ptrStack* topOfStack);
void pushOpenTokenToStack(ptrStack* topOfStack, tToken* exprOpenToken);
tExpressionRule applyrule(ptrStack *stack, tExpressionRule rule);
tExpressionRule extractexpression(ptrStack *stack);
void printRule(tExpressionRule rule);
void printStack(ptrStack* topStack);
//void fillList(ptrStack* stack, tExpressionList *L);
//void Insert(tExpressionList *L, tToken token);

#endif
