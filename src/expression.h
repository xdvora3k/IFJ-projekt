#include <stdio.h>
#include "scanner.h"

int getTokenTableIndex(tState state);
int instructionSwap (INSTRUCTION instr);

typedef enum 
{
    //  | +- |  */ | o |   ( |   ) |   i |   $ |
    plusMinusIndex, //0
    mulDivIndex,    //1
    operatorsIndex,
    leftBracketIndex,
    rightBrackedIndex,
    identificatorIndex,
    DollarIndex
}   IndexOfPrecedentTable;

/* typedef enum{   
    E -> E + E;
    E -> E - E;
    E -> E * E;
    E -> E / E;
    E -> E o E;
    E -> (E);
    E -> i;


}rules;*/