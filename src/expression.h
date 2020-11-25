#include <stdio.h>


int getTokenIndex(tState state);

typedef enum 
{
    plusMinusIndex,
    mulDivIndex,
    leftBracketIndex,
    rightBrackedIndex,
    identificatorIndex,
    DollarIndex
}   IndexOfPrecedentTable;