#include <stdio.h>
#include "scanner.h"
#include "ilist.h"

int getTokenTableIndex(tState state);
//int instructionSwap (INSTRUCTION instr);

typedef enum 
{
    //  | +- |  */ | o |   ( |   ) |   i |   $ |
    plusMinusIndex, //0
    mulDivIndex,    //1
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
     //E -> (E);
      //E -> i;


}rules;

typedef struct tExpression {
    char* left_value;
    int is_left_variable;
    char* right_value;
    int is_right_variable;
    tState sign;
} tExpression;

typedef struct tExpressionNode {
    tVarDataType data_type;
    tExpression *first;
} tExpressionNode;

typedef struct tExpressionList {
    tExpressionNode *first;
} tExpressionList;
tExpression generateInstruction(string *exp);
bool chceckBracket(string *toCheck);