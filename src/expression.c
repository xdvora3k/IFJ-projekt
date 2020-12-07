#include "expression.h"

//#include <atoi.h>

int counter = 0;

int precedentTable[8][8] = //> za | < pred
    {
        //asociativita -- Leva -> zasobnikovy operand > vstupni operand

        //  | +- |  */ |   \   | o |   ( |   ) |   i |   $ |    ->Vstupni token
        {'>', '<', '<', '>', '<', '>', '<', '>'}, // +-
        {'>', '>', '>', '>', '<', '>', '<', '>'}, // */
        {'>', '<', '>', '>', '<', '>', '<', '>'}, /* \ */
        {'<', '<', '<', 'E', '<', '>', '<', '>'}, // o (operators) (=) <> <= < > >= == !=    -> vrchol zasobniku
        {'<', '<', '<', '<', '<', '=', '<', 'E'}, // (
        {'>', '>', '>', '>', 'E', '>', 'E', '>'}, // )
        {'>', '>', '>', '>', 'E', '>', 'E', '>'}, // i
        {'<', '<', '<', '<', '<', 'E', '<', 'E'}  // $
                                                  // nepovolene kombinace i( a )i )(, ($ a $), i(, ii a )i, $), ($ a $$
                                                  //'E' -> error/nepovolena kombinace
};

int getTokenTableIndex(tState type)
{
    switch (type)
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

void precedencSA(string *input, tExpressionList *expL, tLinkedList *linkedL)
{
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
    _save_to_token(&endToken, &dollar, tEOF, 0, linkedL);

    tToken exprOpenToken;
    _save_to_token(&exprOpenToken, &exprOpenText, tExprOpen, 0, linkedL);

    StackPush(&topOfStack, &endToken);

    expL = malloc(sizeof(tExpressionList));
    ExprLLInit(expL);

    tExpressionList justTestingList;
    ExprLLInit(&justTestingList);
    printf("INIT\n");
    tExpressionRule rule;
    int index = 0;
    tToken *topToken;
    tToken *inputToken = (tToken *)(StrLLLocateNthElem(tokens, index)->Content);
    do
    {
        // printf("%d %s %d\n", index, inputToken->text->str, tokens->first.);
        topToken = findTerminalToken(&topOfStack);

        int firstIndex = getTokenTableIndex(topToken->type);
        printf("topToken  %d %d\n", topToken->type, topToken->dataType);
        printf("inputToken  %d %d\n", inputToken->type, inputToken->dataType);
        int secondIndex = getTokenTableIndex(inputToken->type);

        printf("result = %c", precedentTable[firstIndex][secondIndex]);
        //printf("indexes: %d %d\n", firstIndex, secondIndex);
        // printStack(&topOfStack);
        printf("\n********first index %d and second index %d ++++\n", firstIndex, secondIndex);
        if (inputToken->type != tEOF || topToken->type != tEOF)
        {
            switch (precedentTable[firstIndex][secondIndex])
            {
            case '>': //zamen <y
                printf(">>>>>>>>>>\n");
                rule = extractexpression(&topOfStack);
                rule = applyrule(&topOfStack, rule, linkedL);
                if (rule.operator!= NULL)
                {
                    InsertList(rule, expL, linkedL);
                    printList(expL, rule);
                }

                printRule(rule);
                printStack(&topOfStack);
                break;

            case '<':
                //zamen a za a<
                printf("<<<<<<<<<<<<<<<<\n");
                pushOpenTokenToStack(&topOfStack, &exprOpenToken);
                StackPush(&topOfStack, inputToken);
                //  printStack(&topOfStack);
                index++;
                tListItem *listItem = StrLLLocateNthElem(tokens, index);

                //take next, if not available, return $
                if (listItem == NULL)
                {
                    inputToken = &endToken;
                }
                else
                {
                    inputToken = (tToken *)(listItem->Content);
                }

                break;

            case '=':
                printf("=========\n");
                StackPush(&topOfStack, inputToken);
                index++;
                printf("%d index =\n", index);
                tListItem *lItem = StrLLLocateNthElem(tokens, index);
                printf("%d index2 =\n", index);
                if (lItem == NULL)
                {
                    inputToken = &endToken;
                }
                else
                {

                    inputToken = (tToken *)(lItem->Content);
                    printf("iTem %s\n", inputToken->text->str);
                }

                break;
            case 'E':
                exit(SYN_ERROR);
                break;
            }
        }

        // printf("input: %d", inputToken->type);
        printf("top: %d", topToken->type);

    } while (inputToken->type != tEOF || topToken->type != tEOF); // || topToken-> type != tComma
    printf("inputtoken type %d  toptokentype %d\n", inputToken->type, topToken->type);
    //fill List //vraci se v rule
    // tToken *left = rule.leftOperand;
    //tToken *right = rule.rightOperand;
    // CreateEXPList(L, left, right, counter);
    //ExprLLCreateNextNode(L, L->first->data_type);
    //ExprLLInsertExprToLastNode(L, rule.leftOperand, rule.rightOperand, rule.operator, rule.placeHolder, rule.typeOfRule);
    printf("\n !!!!!!rule l: %s %s r:%s\n", rule.leftOperand->text->str, rule.operator->text->str, rule.rightOperand->text->str);
    /*  tExpressionList justTestingList;
   ExprLLInit(&justTestingList);
   justTestingList = InsertList(rule);*/

    // justTestingList.first->data_type = IntType;
    tLinkedList list;
    StrLLInit(&list);
    //printf("%d type\n", justTestingList.first->data_type);
    printf("\n");
    //printf("\n Insert list %s %s %s\n", justTestingList.first->first->leftOperand->text->str, justTestingList.first->first->operator->text->str, justTestingList.first->first->rightOperand->text->str);
    counter = 0;
}

void pushOpenTokenToStack(ptrStack *topOfStack, tToken *exprOpenToken)
{
    tToken *topToken = (tToken *)(topOfStack->top_stack->value);

    if (topToken->type == tId || topToken->type == tExprPlaceholder || topToken->type == tInteger ||
        topToken->type == tFloat || topToken->type == tString)
    {
        StackPop(topOfStack);
        StackPush(topOfStack, exprOpenToken);
        StackPush(topOfStack, topToken);
    }
    else
    {
        StackPush(topOfStack, exprOpenToken);
    }
}

tToken *findTerminalToken(ptrStack *topOfStack)
{
    tToken *topToken = (tToken *)(topOfStack->top_stack->value);
    tToken *nextToken;

    if (topOfStack->top_stack->next_value == NULL)
    {
        return topToken;
    }
    else
    {
        nextToken = (tToken *)(topOfStack->top_stack->next_value->value);
    }

    //Hack: if there is "<i" on top of stack, we want to process it, otherwise
    //we expect something like $i and in such case we consider i to be a node, not terminal
    //We try to avoid necesity to replace "i" with "E" and make things more complicated later
    if (topToken->type == tId || topToken->type == tExprPlaceholder || topToken->type == tInteger ||
        topToken->type == tFloat || topToken->type == tString)
    {
        if (nextToken->type == tExprOpen)
        {
            return topToken;
        }
        else
        {
            return nextToken;
        }
    }
    return topToken;
}

tExpressionRule extractexpression(ptrStack *stack)
{
    tExpressionRule exp;
    exp.leftOperand = NULL;
    exp.operator= NULL;
    exp.rightOperand = NULL;
    exp.placeHolder = NULL;
    printStack(stack);
    tToken *topToken = ((tToken *)stack->top_stack->value);
    printf("TOPTOKEN value %d\n", topToken->type);
    if (topToken->type != tExprOpen)
    {
        exp.rightOperand = topToken;
        StackPop(stack);
        topToken = ((tToken *)stack->top_stack->value);
        printf("type %d\n", exp.rightOperand->dataType);
    }
    printf("operator\n");
    printf("top %d\n", topToken->type);
    if (topToken->type != tExprOpen)
    {
        exp.operator= topToken;
        StackPop(stack);
        topToken = ((tToken *)stack->top_stack->value);
        if ((exp.operator->type == tDivide) && (strcmp(exp.rightOperand->text->str, "0") == 0)) //divided by zero
        {
            printf("operator %s and %d\n", exp.operator->text->str, exp.operator->type);
            printf("FALSE\n");
            exit(ZERO_DIVISION);
        }
    }
    printf("operator2\n");
    if (topToken->type != tExprOpen)
    {
        exp.leftOperand = topToken;
        printf("stack data %d\n", ((tToken *)stack->top_stack->value)->dataType);
        StackPop(stack);
        topToken = ((tToken *)stack->top_stack->value);
        printf("type L %d %d\n", exp.leftOperand->dataType, ((tToken *)stack->top_stack->value)->dataType);
    }
    printf("operator3\n");
    if (exp.operator!= NULL)
    {
        printf("%s left %s %s right %s\n", topToken->text->str, exp.leftOperand->text->str, exp.operator->text->str,
               exp.rightOperand->text->str);
        printf("typ: %d\n", topToken->type);
        // InsertList(rule, L);
    }
    printf("operator4\n");
    printStack(stack);
    if (topToken->type != tExprOpen)
    {
        printf("EXIT\n");
        exit(SYN_ERROR);
    }
    StackPop(stack); //removing '<' from stack

    return exp;
}

tVarDataType determinePlaceholderDatatype(tExpressionRule rule)
{
    tVarDataType result;
    switch (rule.typeOfRule)
    {
    case expIdentity:
        result = rule.rightOperand->dataType;
    case expBrackets:
        result = rule.operator->dataType;
    case expPLUSepx:
    case expMINUSepx:
    case expMULepx:
    case expDIVepx:
    case expOPepx:
        printf("exp datatype %d %d and %d %d\n", rule.leftOperand->dataType, rule.leftOperand->type, rule.rightOperand->dataType, rule.rightOperand->type);
        if (rule.leftOperand->dataType != rule.rightOperand->dataType)
        {
            printf("ERROr\n");
            exit(SEM_GET_DATATYPE_ERROR);
        }
        result = rule.rightOperand->dataType;
    }
    return result;
}

tExpressionRule applyrule(ptrStack *stack, tExpressionRule rule, tLinkedList *L)
{
    if (rule.operator== NULL)
    {
        if (rule.rightOperand == NULL)
        {
            exit(SYN_ERROR);
        }
        rule.typeOfRule = expIdentity;
        StackPush(stack, rule.rightOperand); //na vstupu je i
    }
    else
    {
        if (rule.rightOperand == NULL || rule.leftOperand == NULL)
        {
            exit(SYN_ERROR);
        }
        if (rule.rightOperand->type == tClosingSimpleBrace && rule.leftOperand->type == tOpeningSimpleBrace)
        {
            rule.typeOfRule = expBrackets;
            StackPush(stack, rule.operator); //nahrada (i) za i
        }
        else
        {
            if (rule.operator->type == tPlus)
            {
                rule.typeOfRule = expPLUSepx;

                printf("rule E+E");
                printStack(stack);
            }
            if (rule.operator->type == tMinus)
            {
                rule.typeOfRule = expMINUSepx;

                printf("rule E-E");

                printStack(stack);
            }
            if (rule.operator->type == tDivide)
            {
                rule.typeOfRule = expDIVepx;

                printf("rule E/E");

                printStack(stack);
            }
            if (rule.operator->type == tMultiply)
            {
                rule.typeOfRule = expMULepx;

                printf("rule E*E");
                printStack(stack);
            }
            if (rule.operator->type == tBiggerOrEqual || rule.operator->type == tBiggerThan ||
                rule.
                operator->type == tSmallerOrEqual || rule.
                operator->type == tSmallerThan ||
                rule.
                operator->type == tEqual || rule.
                operator->type == tNotEqual)
            {
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
            _save_to_token(placeholder, &placeholderText, tExprPlaceholder, 0, L);
            placeholder->dataType = determinePlaceholderDatatype(rule);
            rule.placeHolder = placeholder;
            StackPush(stack, rule.placeHolder);
            printf("\n FINAL%s %s %s palceHolder %s\n", rule.leftOperand->text->str, rule.operator->text->str,
                   rule.rightOperand->text->str, placeholder->text->str);
        }
    }
    return rule;
}
void printList(tExpressionList *List, tExpressionRule rule)
{
    printf("-------------------List: ");
    tExpressionNode *last_node = List->first;
    if (last_node)
    {
        while (last_node->next_node)
        {
            last_node = last_node->next_node;
        }
    }
    if (rule.leftOperand)
    {
        /*  if(List->first->next_node){
            printf("%s", List->first->next_node->first->leftOperand->text->str);
        } else
        {*/
        printf("%s", last_node->first->leftOperand->text->str);
    }
    if (rule.operator)
    {

        printf("%s", last_node->first->operator->text->str);
    }
    if (rule.rightOperand)
    {

        printf("%s", last_node->first->rightOperand->text->str);
    }
    if (rule.placeHolder)
    {

        printf(" ListPlace: %s", last_node->first->placeHolder->text->str);
    }
    printf("*************************\n");
}

void printRule(tExpressionRule rule)
{
    printf("Rule: ");
    if (rule.leftOperand)
    {
        printf("%s", rule.leftOperand->text->str);
    }
    if (rule.operator)
    {
        printf("%s", rule.operator->text->str);
    }
    if (rule.rightOperand)
    {
        printf("%s", rule.rightOperand->text->str);
    }
    if (rule.placeHolder)
    {
        printf(" Place: %s", rule.placeHolder->text->str);
    }
    printf("\n");
}

void printStack(ptrStack *topStack)
{
    tStack *actual = topStack->top_stack;
    printf("Stack: ");
    do
    {
        tToken *token = ((tToken *)(actual->value));
        printf("%s : ", token->text->str);
        actual = actual->next_value;
    } while (actual != NULL);
    printf("\n");
}

// return expList;
//}

void InsertList(tExpressionRule rule, tExpressionList *expL, tLinkedList *linkedL)
{
    printf("insert\n");

    //switch
    //case: tInt    rule.leftOperand->type == tInteger
    //ExprLLCreateNextNode(L, IntType);

    //case tId
    //TableLLGetSingleVariable(tLinkedList *func_variable_list, char* var)
    //esgFault protoze left je placeholder

    /*    if(rule.leftOperand->type == tExprPlaceholder){ //kdyz budou oba 58?
        rule.leftOperand->type = rule.rightOperand->type;
    }*/
    if (rule.typeOfRule == expBrackets)
    {
        ExprLLCreateNextNode(expL, rule.operator->dataType);
    }
    else
    {
        ExprLLCreateNextNode(expL, rule.rightOperand->dataType);
    }
    ExprLLInsertExprToLastNode(expL, rule.leftOperand, rule.rightOperand, rule.operator, rule.placeHolder, rule.typeOfRule);
    printf("++++insert LIST %s +++\n", expL->first->first->rightOperand->text->str);

    //ExprLLInsertExprToLastNode(L, rule.leftOperand, rule.rightOperand, rule.operator, rule.placeHolder, rule.typeOfRule);
}