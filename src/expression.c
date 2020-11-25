#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "expression.h"
#include "ilist.h"

int precedentTable[8][8] =      //> za | < pred
{   //asociativita -- Leva -> zasobnikovy operand > vstupni operand

    
//  | +- |  */ |   \   | o |   ( |   ) |   i |   $ |    ->Vstupni token
    { '>' , '<' , '<' , '>' , '<' , '>' , '<' , '>' }, // +-
	{ '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' }, // */
	{ '>' , '<' , '>' , '>' , '<' , '>' , '<' , '>' }, /* \ */
	{ '<' , '<' , '<' , 'E' , '<' , '>' , '<' , '>' }, // o (operators) (=) <> <= < > >= == !=    -> vrchol zasobniku
	{ '<' , '<' , '<' , '<' , '<' , '=' , '<' , 'E' }, // (
	{ '>' , '>' , '>' , '>' , 'E' , '>' , 'E' , '>' }, // )
	{ '>' , '>' , '>' , '>' , 'E' , '>' , 'E' , '>' }, // i 
	{ '<' , '<' , '<' , '<' , '<' , 'E' , '<' , 'E' } // $
   // nepovolene kombinace i( a )i )(, ($ a $), i(, ii a )i, $), ($ a $$ 
    //'E' -> error/nepovolena kombinace
}; 

int getTokenTableIndex(tState token)
{ 
    switch (token)
    {
    case tPlus:
    case tMinus:
        return plusMinusIndex;
    case tMultiply:
    case tDivide:
        return mulDivIndex;
    case tOpeningSimpleBrace:
        return leftBracketIndex;
    case tClosingSimpleBrace:
        return rightBrackedIndex;
    
    case tAssignment:
    case tEqual:
    case tNotEqual:
    case tBiggerThan:
    case tSmallerThan:
    case tBiggerOrEqual:
    case tSmallerOrEqual:
        return operatorsIndex;
       // break;
    case EOL:
        return DollarIndex;

    case tInteger:
    case tId:
    case tString:
    case tDouble:
        return identificatorIndex;
    default:
        //return DollarIndex;
        break;
    }
    return -1;
}

int instructionSwap (INSTRUCTION instr){
    switch (instr)
    {
    case tPlus:
        return I_ADD;
    case tMinus:
        return I_SUB;
    case tMultiply:
        return I_MUL;    
    case tDivide:
        return I_DIV;
        
    /*case tAssignment:
        return */
    /*case tNotEqual:
        return ;
      case tEqual:
      */
    case tBiggerThan:
        return I_GT;
    case tSmallerThan:
        return I_LT;
    case tBiggerOrEqual:
        return I_GTS;   
    case tSmallerOrEqual:
        return I_LTS;
    default:
             
    break;
    }
    return -1;
}

