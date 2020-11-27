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
        return rightBracketIndex;
    
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
    case tFloat:
        return identificatorIndex;
    default:
        //return DollarIndex;
        break;
    }
    return -1;
}

/*int instructionSwap (INSTRUCTION instr){
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
        
    case tAssignment:
        return 
    case tNotEqual:
        return ;
      case tEqual:
      
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
}*/
/* if 5 -> chyaba 2*/

bool chceckBracket(string *toCheck){
    bool isBracketInExp = false, isLeftBracketToo = false; 
    while(get_token(toCheck) != EOL){   //tOpeningSimpleBrace
    if(get_token(toCheck) == tOpeningSimpleBrace){
        isBracketInExp = true;
        //return isBracketInExp;
    if(get_token(toCheck) == tClosingSimpleBrace){
        isLeftBracketToo = true;     
    }
        //chceckBracket(toCheck);
    }
        chceckBracket(toCheck);
    }
   /* if(isBracketInExp == true && isLeftBracketToo == false){
        //error - syntax?
    } else*/ if(isBracketInExp == true && isLeftBracketToo == true)
    {
        return true;
    }
    return false;
    
    /*isBracketInExp = false;
    return isBracketInExp;*/
    //tClosingB...
}

tExpression generateInstruction(string *exp){
    bool result;
    
   result = chceckBracket(exp);
   if(result == true){
       //prvni vyraz v zavorkach
   } else{
       
      //a+b -> lftVar 1, rghVar 1
      //a+5 -> lftVar 1, rghVar 0
      //5+5 -> 0 0 
      //5+b 0 1 
      //a + (b + z) -> (1 1) -> 1 + (1)
   }

}

tExpression switchStruct(string *exp, tExpressionNode node){ //change this name
    tExpression expStruct;
    switch (get_token(exp))
       {
       case tMultiply:
            expStruct.sign = tMultiply;
        case tDivide:
            expStruct.sign = tDivide;
        case tMinus:
            expStruct.sign = tMinus;
        case tPlus:
            expStruct.sign = tPlus;
        case tSmallerThan:
            expStruct.sign = tSmallerThan;
        case tBiggerThan:
            expStruct.sign = tBiggerThan;
        case tBiggerOrEqual:
           expStruct.sign = tBiggerOrEqual;

        case tInteger: 
        node.data_type = IntType;
        case tFloat:
        node.data_type = Float64Type;
        default:
            break;


       }
       return expStruct;

}  

void rulesFunction(){
    //stacInit
    //getToken -> stack
}
