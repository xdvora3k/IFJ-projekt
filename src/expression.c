#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "expression.h"
#include "ilist.h"
#include "stack.h"

int counter = 0;


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

int getTokenTableIndex(int token)
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
/*
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

} */ 

void precedencSA(string* token){
    ptrStack* topOfStack;
    StackInit(topOfStack);
    StackTop(topOfStack);
    topOfStack->top_stack->value = DollarIndex;
    int actualToken = get_token(token);
    expressionRule expression;
    tExpressionList L;
    tLinkedList linkedList;
    tLinkedList *list = malloc(sizeof(tListItem));
    StrLLInit(list);
    StrLLInsert(list, "a");
    
    do{
        int firstIndex = getTokenTableIndex(topOfStack->top_stack->value);
        int secondIndex = getTokenTableIndex(token);
        switch (precedentTable[firstIndex][secondIndex])
        {
        case '>': //zamen <y          
           expression = applyrule(topOfStack ,extractexpression(topOfStack));
           if (expression.placeHolder->length != 0)
           {
               StrLLInsert(list, &expression);
           }
        case '<':
        //zamen a za a<
        StackPush(topOfStack, '<');
        StackPush(topOfStack, actualToken);
        actualToken = get_token(token);
        break;
        case '=':
        StackPush(topOfStack, secondIndex);    //actualToken
        secondIndex = get_token(token);
        //actualToken = get_token(token);

        }
        
    }while(actualToken == DollarIndex && topOfStack == DollarIndex);
    
}

expressionRule extractexpression(ptrStack *stack)
{
    ptrStack *stackTop;
    stackTop = StackTop(stack);
    expressionRule exp; 
    if (stackTop != '<'){ 
        exp.rightOperand = stackTop;
        StackPop(stackTop);
        stackTop = StackTop(stack);

    } 
     
    if (stackTop != '<'){ 
        exp.operator = stackTop;
        StackPop(stackTop);
        stackTop = StackTop(stack);
    } 
    
    if (stackTop != '<'){ 
        exp.leftOperand = stackTop;
        StackPop(stackTop);
        stackTop = StackTop(stack);
    } 

    if(stackTop != '<'){
        SYN_ERROR;
    }
    StackPop(stackTop); //removing '<' from stack
    return exp;
}

expressionRule applyrule(ptrStack *stack, expressionRule rule){
    if(rule.operator->length == 0)
    { 
        if(rule.rightOperand->length == 0){
        SYN_ERROR;
        }
        StackPush(stack, rule.rightOperand);
        
    }
    else
    {
        if(rule.rightOperand->length == 0 || rule.leftOperand->length == 0){
        SYN_ERROR;
        }
        if(rule.rightOperand->str == ')' && rule.leftOperand->str == '('){ 
            StackPush(stack, rule.operator); //nahrada (i) za i

        }
        else{
        rule.placeHolder = '{' + counter + '}';
        StackPush(stack, rule.placeHolder);
        }
    }
    return rule;
}
