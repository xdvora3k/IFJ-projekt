#include <stdio.h>

#include <stdlib.h>

#include "expression.h"

#include "ilist.h"

//#include <atoi.h>

int counter = 0;

int precedentTable[8][8] = //> za | < pred
        { //asociativita -- Leva -> zasobnikovy operand > vstupni operand


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

int getTokenTableIndex(tState type) {
    switch (type) {
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
        case tEOF:
            return DollarIndex;

        case tInteger:
        case tId:
        case tString:
        case tFloat:
        case tExprPlaceholder:
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

void precedencSA(string * input) {
    printf("start pecedenc SA- %s\n", input);
    tLinkedList * tokens = get_tokens(input);

    ptrStack topOfStack;
    StackInit( & topOfStack);

    string dollar;
    init_string( & dollar);
    add_to_string( & dollar, '$');

    string exprOpenText;
    init_string( & exprOpenText);
    add_to_string( & exprOpenText, '<');

    tToken endToken;
    endToken.type = tEOF;
    endToken.text = dollar;

    tToken exprOpenToken;
    exprOpenToken.type = tExprOpen;
    exprOpenToken.text = exprOpenText;

    StackPush( & topOfStack, & endToken);

    //tToken* topToken = (tToken*)(topOfStack.top_stack->value);
    //printf("%s", topToken->text.str);
    //int actualToken = get_token(token);
    //expressionRule expression;
    //tExpressionList L;
    //tLinkedList precedenceList;
    tExpressionList * list = malloc(sizeof(tListItem));
    list -> first = NULL;
    expressionRule rule;
    int index = 0;
    tToken * topToken;
    tToken * inputToken = (tToken * )(StrLLLocateNthElem(tokens, index) -> Content);
    do {
        // printf("%d %s %d\n", index, inputToken->text.str, tokens->first.);
        topToken = findTerminalToken( & topOfStack);

        int firstIndex = getTokenTableIndex(topToken -> type);
        int secondIndex = getTokenTableIndex(inputToken -> type);

        //printf("result = %c", precedentTable[firstIndex][secondIndex]);
        //printf("indexes: %d %d\n", firstIndex, secondIndex);
        // printStack(&topOfStack);

        switch (precedentTable[firstIndex][secondIndex]) {
            case '>': //zamen <y
                
                rule = extractexpression( & topOfStack);
                rule = applyrule( & topOfStack, rule);
                printRule(rule);
                printStack( & topOfStack);
                // tToken* token = (tToken*)(topOfStack.top_stack->value);
                break;
                /*expression = applyrule(topOfStack ,extractexpression(topOfStack));
                if (expression.placeHolder->length != 0)
                {
                    StrLLInsert(list, &expression);
                }*/
            case '<':
                //zamen a za a<
                pushOpenTokenToStack( & topOfStack, & exprOpenToken);
                StackPush( & topOfStack, inputToken);
                //  printStack(&topOfStack);
                index++;
                tListItem * listItem = StrLLLocateNthElem(tokens, index);

                //  printf("listItem %p\n", listItem);
                //take next, if not available, return $
                if (listItem == NULL) {
                    inputToken = & endToken;
                } else {
                    inputToken = (tToken * )(listItem -> Content);
                }

                break;
                /*case '=':
            StackPush(topOfStack, secondIndex);    //actualToken
            secondIndex = get_tokenEXP(token, startIndex);
            //actualToken = get_token(token);
    */
        }

        // printf("input: %d", inputToken->type);
        //printf("top: %d", topToken->type);

    } while (inputToken -> type != tEOF || topToken -> type != tEOF);
    printf("inputtoken type %d  toptokentype %d\n", inputToken->type, topToken->type);

    fillExpList( & topOfStack, list);

}

void pushOpenTokenToStack(ptrStack * topOfStack, tToken * exprOpenToken) {
    tToken * topToken = (tToken * )(topOfStack -> top_stack -> value);

    if (topToken -> type == tId || topToken -> type == tExprPlaceholder) {
        StackPop(topOfStack);
        StackPush(topOfStack, exprOpenToken);
        StackPush(topOfStack, topToken);
    } else {
        StackPush(topOfStack, exprOpenToken);
    }
}

tToken * findTerminalToken(ptrStack * topOfStack) {
    tToken * topToken = (tToken * )(topOfStack -> top_stack -> value);
    tToken * nextToken;

    if (topOfStack -> top_stack -> next_value == NULL) {
        return topToken;
    } else {
        nextToken = (tToken * )(topOfStack -> top_stack -> next_value -> value);
    }

    //Hack: if there is "<i" on top of stack, we want to process it, otherwise
    //we expect something like $i and in such case we consider i to be a node, not terminal
    //We try to avoid necesity to replace "i" with "E" and make things more complicated later
    if (topToken -> type == tId || topToken -> type == tExprPlaceholder) {
        if (nextToken -> type == tExprOpen) {
            return topToken;
        } else {
            return nextToken;
        }
    }
    return topToken;
}

expressionRule extractexpression(ptrStack * stack) {
    expressionRule exp;
    exp.leftOperand = NULL;
    exp.operator = NULL;
    exp.rightOperand = NULL;
    exp.placeHolder = NULL;
    printStack(stack);
    tToken * topToken = ((tToken * ) stack -> top_stack -> value);
    printf("TOPTOKEN value %d\n", topToken->type);
    if (topToken -> type != tExprOpen) {
        exp.rightOperand = topToken;
        StackPop(stack);
        topToken = ((tToken * ) stack -> top_stack -> value);
    }

    if (topToken -> type != tExprOpen) {
        exp.operator = topToken;
        StackPop(stack);
        topToken = ((tToken * ) stack -> top_stack -> value);
    }

    if (topToken -> type != tExprOpen) {
        exp.leftOperand = topToken;
        StackPop(stack);
        topToken = ((tToken * ) stack -> top_stack -> value);
    }
    if(exp.operator != NULL){ 
    printf("%s left %s %s right %s\n", topToken->text.str, exp.leftOperand->text.str, exp.operator->text.str,exp.rightOperand->text.str);
    printf("typ: %d\n", topToken->type);
    }
    printStack(stack);
    if (topToken->type != tExprOpen)
    {
     printf("EXIT\n");
        exit(SYN_ERROR);
    }
    StackPop(stack); //removing '<' from stack

    return exp;
}

expressionRule applyrule(ptrStack * stack, expressionRule rule) {
    if (rule.operator == NULL) {
        if (rule.rightOperand == NULL) {
            exit(SYN_ERROR);
        }
        rule.typeOfRule = expIdentity;
        StackPush(stack, rule.rightOperand); //na vstupu je i
    } else {
        if (rule.rightOperand == NULL || rule.leftOperand == NULL) {
            exit(SYN_ERROR);
        }
        if (rule.rightOperand -> type == tClosingSimpleBrace && rule.leftOperand -> type == tOpeningSimpleBrace) {
            rule.typeOfRule = expBrackets;
            StackPush(stack, rule.operator); //nahrada (i) za i
        } else {
            if (rule.operator -> type == tPlus) {
                rule.typeOfRule = expPLUSepx;
                
               
                printf("rule E+E");
                printStack(stack);
            }
            if (rule.operator -> type == tMinus) {
                rule.typeOfRule = expMINUSepx;
                
                printf("rule E-E");
             
                printStack(stack);
            }
            if (rule.operator -> type == tDivide) {
                rule.typeOfRule = expDIVepx;
               
                printf("rule E/E");
              
                printStack(stack);

            }
            if (rule.operator -> type == tMultiply) {
                rule.typeOfRule = expMULepx;
                
                printf("rule E*E");
                printStack(stack);
               
            }
            printf("Counter %d\n", counter);
            counter++;
            string placeholderText;
            init_string( & placeholderText);
            add_to_string( & placeholderText, '{');
            add_to_string(&placeholderText, counter + '0');
            add_to_string( & placeholderText, '}');

            tToken * placeholder;
            placeholder = malloc(sizeof(tToken));
            placeholder -> text = placeholderText;
            placeholder -> type = tExprPlaceholder;

            rule.placeHolder = placeholder;
            StackPush(stack, rule.placeHolder);
        }
    }
    return rule;
}

void printRule(expressionRule rule) {
    printf("Rule: ");
    if (rule.leftOperand) {
        printf("%s", rule.leftOperand -> text.str);
    }
    if (rule.operator) {
        printf("%s", rule.operator -> text.str);
    }
    if (rule.rightOperand) {
        printf("%s", rule.rightOperand -> text.str);
    }
    if (rule.placeHolder) {
        printf(" Place: %s", rule.placeHolder -> text.str);
    }
    printf("\n");
}

void printStack(ptrStack * topStack) {
    tStack * actual = topStack -> top_stack;
    printf("Stack: ");
    do {
        tToken * token = ((tToken * )(actual -> value));
        printf("%s : ", token -> text.str);
        actual = actual -> next_value;
    } while (actual != NULL);
    printf("\n");
}
void ListInit(tExpressionList * L) {
    L -> first = NULL;
}
tExpressionList fillExpList(ptrStack * stack, tExpressionList * list) { //void fillExpList(ptrStack* stack, tExpressionList *L)
    /*tExpressionList expList;
    ListInit( & expList);

    printf("%p\n", (void * ) stack -> top_stack -> value);
    while (stack -> top_stack -> value) {
        printf("----------%s\n", ((tToken * ) stack -> top_stack -> value) -> text.str);
        printf("set\n");*/
        // if(expList.first == NULL){

        //tokens.text.str = ((tToken *)stack->top_stack->value)->text.str;
       /* printf("Ttoken %s\n", ((tToken * ) stack -> top_stack -> value) -> text.str);
        expList.first -> first -> rightOperand -> text = ((tToken * ) stack -> top_stack -> value) -> text;
        printf("get list: %p\n", (void * ) expList.first);
        expList.first -> first -> leftOperand -> type = ((tToken * ) stack -> top_stack -> value) -> type;
*/
        //printf("get list: %s\n", expList.first->leftOperand->text.str);

        /*string testing;
    init_string(&testing);
    add_to_string(&testing, 'b');
    expList.first->leftOperand->text.str = testing.str;*/
     /*   printf("set1.5\n");
        printStack(stack);

        StackPop(stack);
        printStack(stack);
*/
        /* StackPop(stack);
    expList.first->operator = stack->top_stack->value;
    StackPop(stack);
    expList.first->rightOperand = stack->top_stack->value;
    StackPop(stack);
    } else{
        printf("set2\n");
        newNode.first = expList.first;*/
   // }
   // printf("get here\n");
    //exit(2); //just for testing
}
// return expList;
//}

/*

void Insert(tExpressionList *L, tToken token){
    expressionRule *new_node = malloc(sizeof(expressionRule));
    tToken *new_content = malloc(sizeof(tToken));

}*/