#include <stdio.h>
#include "scanner.h"
#include "stack.h"
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



 typedef enum{   
    expPLUSepx,//E -> E + E;
    expMINUSepx,//E -> E - E;
    expMULepx,//E -> E * E;
    expDIVepx, //E -> E / E;
    expOPepx, //E -> E o E;
    expBrackets, //E -> (E);
    expIdentity //E -> i;


}ruleType;

typedef struct 
{
    tToken* leftOperand;
    tToken* rightOperand;
    tToken* operator;
    tToken* placeHolder;
    ruleType typeOfRule;
   struct expressionRule* next;
}expressionRule;

typedef struct tExpressionNode {
    tVarDataType data_type;
    expressionRule *first;
    struct tExpressionNode *next_node;
} tExpressionNode;

typedef struct tExpressionList {
    tExpressionNode *first;
} tExpressionList;

tToken* findTerminalToken(ptrStack* topOfStack);
void pushOpenTokenToStack(ptrStack* topOfStack, tToken* exprOpenToken);
expressionRule applyrule(ptrStack *stack, expressionRule rule);
expressionRule extractexpression(ptrStack *stack);
void printRule(expressionRule rule);
void printStack(ptrStack* topStack);
tExpressionList fillExpList(ptrStack *stack, tExpressionList *list);
//void fillList(ptrStack* stack, tExpressionList *L);
//void Insert(tExpressionList *L, tToken token);
