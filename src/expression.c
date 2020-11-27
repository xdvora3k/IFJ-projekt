#include "expression.h"

//#include <atoi.h>

int counter = 0;

int precedentTable[8][8] = //> za | < pred
        { //asociativita -- Leva -> zasobnikovy operand > vstupni operand


//  | +- |  */ |   \   | o |   ( |   ) |   i |   $ |    ->Vstupni token
                {'>', '<', '<', '>', '<', '>', '<', '>'}, // +-
                {'>', '>', '>', '>', '<', '>', '<', '>'}, // */
                {'>', '<', '>', '>', '<', '>', '<', '>'}, /* \ */
                {'<', '<', '<', 'E', '<', '>', '<', '>'}, // o (operators) (=) <> <= < > >= == !=    -> vrchol zasobniku
                {'<', '<', '<', '<', '<', '=', '<', 'E'}, // (
                {'>', '>', '>', '>', 'E', '>', 'E', '>'}, // )
                {'>', '>', '>', '>', 'E', '>', 'E', '>'}, // i
                {'<', '<', '<', '<', '<', 'E', '<', 'E'} // $
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

void precedencSA(string *input) {
    tLinkedList *tokens = get_tokens(input);

    ptrStack topOfStack;
    StackInit(&topOfStack);

    string dollar;
    init_string(&dollar);
    add_to_string(&dollar, '$');

    string exprOpenText;
    init_string(&exprOpenText);
    add_to_string(&exprOpenText, '<');

    tToken endToken;
    _save_to_token(&endToken, &dollar, tEOF, 0);

    tToken exprOpenToken;
    _save_to_token(&exprOpenToken, &exprOpenText, tExprOpen, 0);

    StackPush(&topOfStack, &endToken);

    //tToken* topToken = (tToken*)(topOfStack.top_stack->value);
    //printf("%s", topToken->text->str);
    //int actualToken = get_token(token);
    //tExpressionRule expression;
    //tExpressionList L;
    //tLinkedList precedenceList;
    tExpressionList *list = malloc(sizeof(tListItem));
    list->first = NULL;
    tExpressionRule rule;
    int index = 0;
    tToken *topToken;
    tToken *inputToken = (tToken *) (StrLLLocateNthElem(tokens, index)->Content);
    do {
        // printf("%d %s %d\n", index, inputToken->text->str, tokens->first.);
        topToken = findTerminalToken(&topOfStack);

        int firstIndex = getTokenTableIndex(topToken->type);
        printf("topToken  %d\n", topToken->type);
        printf("inputToken  %d\n", inputToken->type);
        int secondIndex = getTokenTableIndex(inputToken->type);

        //printf("result = %c", precedentTable[firstIndex][secondIndex]);
        //printf("indexes: %d %d\n", firstIndex, secondIndex);
        // printStack(&topOfStack);

        switch (precedentTable[firstIndex][secondIndex]) {
            case '>': //zamen <y

                rule = extractexpression(&topOfStack);
                rule = applyrule(&topOfStack, rule);
                printRule(rule);
                printStack(&topOfStack);
                // tToken* token = (tToken*)(topOfStack.top_stack->value);
                break;
                /*expression = applyrule(topOfStack ,extractexpression(topOfStack));
                if (expression.placeHolder->length != 0)
                {
                    StrLLInsert(list, &expression);
                }*/
            case '<':
                //zamen a za a<
                pushOpenTokenToStack(&topOfStack, &exprOpenToken);
                StackPush(&topOfStack, inputToken);
                //  printStack(&topOfStack);
                index++;
                tListItem *listItem = StrLLLocateNthElem(tokens, index);

                //  printf("listItem %p\n", listItem);
                //take next, if not available, return $
                if (listItem == NULL) {
                    inputToken = &endToken;
                } else {
                    inputToken = (tToken *) (listItem->Content);
                }

                break;
                /*case '=':
            StackPush(topOfStack, secondIndex);    //actualToken
            secondIndex = get_tokenEXP(token, startIndex);
            //actualToken = get_token(token);
    */
            case '=':
                StackPush(&topOfStack, inputToken);
                index++;
                printf("%d index =\n", index);
                tListItem *lItem = StrLLLocateNthElem(tokens, index);
                printf("%d index2 =\n", index);
                if (lItem == NULL) {
                    inputToken = &endToken;
                } else {

                    inputToken = (tToken *) (lItem->Content);
                    printf("iTem %s\n", inputToken->text->str);
                }

                //actualToken = get_token(token);

            case 'E':
                // return SYN_ERROR;
                break;
        }

        // printf("input: %d", inputToken->type);
        //printf("top: %d", topToken->type);

    } while (inputToken->type != tEOF || topToken->type != tEOF); // || topToken-> type != tComma
    printf("inputtoken type %d  toptokentype %d\n", inputToken->type, topToken->type);
    //fill List
    counter = 0;    // a+8 , o*89,
}

void pushOpenTokenToStack(ptrStack *topOfStack, tToken *exprOpenToken) {
    tToken *topToken = (tToken *) (topOfStack->top_stack->value);

    if (topToken->type == tId || topToken->type == tExprPlaceholder || topToken->type == tInteger ||
        topToken->type == tFloat) {
        StackPop(topOfStack);
        StackPush(topOfStack, exprOpenToken);
        StackPush(topOfStack, topToken);
    } else {
        StackPush(topOfStack, exprOpenToken);
    }
}

tToken *findTerminalToken(ptrStack *topOfStack) {
    tToken *topToken = (tToken *) (topOfStack->top_stack->value);
    tToken *nextToken;

    if (topOfStack->top_stack->next_value == NULL) {
        return topToken;
    } else {
        nextToken = (tToken *) (topOfStack->top_stack->next_value->value);
    }

    //Hack: if there is "<i" on top of stack, we want to process it, otherwise
    //we expect something like $i and in such case we consider i to be a node, not terminal
    //We try to avoid necesity to replace "i" with "E" and make things more complicated later
    if (topToken->type == tId || topToken->type == tExprPlaceholder || topToken->type == tInteger ||
        topToken->type == tFloat) {
        if (nextToken->type == tExprOpen) {
            return topToken;
        } else {
            return nextToken;
        }
    }
    return topToken;
}

tExpressionRule extractexpression(ptrStack *stack) {
    tExpressionRule exp;
    exp.leftOperand = NULL;
    exp.operator = NULL;
    exp.rightOperand = NULL;
    exp.placeHolder = NULL;
    printStack(stack);
    tToken *topToken = ((tToken *) stack->top_stack->value);
    printf("TOPTOKEN value %d\n", topToken->type);
    if (topToken->type != tExprOpen) {
        exp.rightOperand = topToken;
        StackPop(stack);
        topToken = ((tToken *) stack->top_stack->value);
    }

    if (topToken->type != tExprOpen) {
        exp.operator = topToken;
        StackPop(stack);
        topToken = ((tToken *) stack->top_stack->value);
    }

    if (topToken->type != tExprOpen) {
        exp.leftOperand = topToken;
        StackPop(stack);
        topToken = ((tToken *) stack->top_stack->value);
    }
    if (exp.operator != NULL) {
        printf("%s left %s %s right %s\n", topToken->text->str, exp.leftOperand->text->str, exp.operator->text->str,
               exp.rightOperand->text->str);
        printf("typ: %d\n", topToken->type);
    }
    printStack(stack);
    if (topToken->type != tExprOpen) {
        printf("EXIT\n");
        exit(SYN_ERROR);
    }
    StackPop(stack); //removing '<' from stack

    return exp;
}

tExpressionRule applyrule(ptrStack *stack, tExpressionRule rule) {
    if (rule.operator == NULL) {
        if (rule.rightOperand == NULL) {   //|| rule.rightOperand->type == tPlus
            exit(SYN_ERROR);
        }
        rule.typeOfRule = expIdentity;
        StackPush(stack, rule.rightOperand); //na vstupu je i
    } else {
        if (rule.rightOperand == NULL || rule.leftOperand == NULL) {
            exit(SYN_ERROR);
        }
        if (rule.rightOperand->type == tClosingSimpleBrace && rule.leftOperand->type == tOpeningSimpleBrace) {
            rule.typeOfRule = expBrackets;
            StackPush(stack, rule.operator); //nahrada (i) za i
        } else {
            if (rule.operator->type == tPlus) {
                rule.typeOfRule = expPLUSepx;


                printf("rule E+E");
                printStack(stack);
            }
            if (rule.operator->type == tMinus) {
                rule.typeOfRule = expMINUSepx;

                printf("rule E-E");

                printStack(stack);
            }
            if (rule.operator->type == tDivide) {
                rule.typeOfRule = expDIVepx;

                printf("rule E/E");

                printStack(stack);

            }
            if (rule.operator->type == tMultiply) {
                rule.typeOfRule = expMULepx;

                printf("rule E*E");
                printStack(stack);

            }
            if (rule.operator->type == tBiggerOrEqual || rule.operator->type == tBiggerThan ||
                rule.operator->type == tSmallerOrEqual || rule.operator->type == tSmallerThan ||
                rule.operator->type == tEqual || rule.operator->type == tNotEqual) {
                rule.typeOfRule = expOPepx;

                printf("rule E op E");
                printStack(stack);

            }
            printf("Counter %d\n", counter);
            counter++;
            string placeholderText;
            init_string(&placeholderText);
            add_to_string(&placeholderText, '{');
            add_to_string(&placeholderText, counter + '0');
            add_to_string(&placeholderText, '}');

            tToken *placeholder = malloc(sizeof(tToken));
            _save_to_token(placeholder, &placeholderText, tExprPlaceholder, 0);

            rule.placeHolder = placeholder;
            StackPush(stack, rule.placeHolder);
            printf("\n FINAL%s %s %s\n", rule.leftOperand->text->str, rule.operator->text->str,
                   rule.rightOperand->text->str);
        }
    }
    return rule;
}

void printRule(tExpressionRule rule) {
    printf("Rule: ");
    if (rule.leftOperand) {
        printf("%s", rule.leftOperand->text->str);
    }
    if (rule.operator) {
        printf("%s", rule.operator->text->str);
    }
    if (rule.rightOperand) {
        printf("%s", rule.rightOperand->text->str);
    }
    if (rule.placeHolder) {
        printf(" Place: %s", rule.placeHolder->text->str);
    }
    printf("\n");
}

void printStack(ptrStack *topStack) {
    tStack *actual = topStack->top_stack;
    printf("Stack: ");
    do {
        tToken *token = ((tToken *) (actual->value));
        printf("%s : ", token->text->str);
        actual = actual->next_value;
    } while (actual != NULL);
    printf("\n");
}

void ListInit(tExpressionList *L) {
    L->first = NULL;
}


// return expList;
//}

/*

void Insert(tExpressionList *L, tToken token){
    tExpressionRule *new_node = malloc(sizeof(tExpressionRule));
    tToken *new_content = malloc(sizeof(tToken));

}*/
