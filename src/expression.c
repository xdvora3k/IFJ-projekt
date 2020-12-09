/*
 * IFJ project 2020
 * Author: xkuzel08, Marie Kuzelova
 *         
 */
#include "expression.h"

int counter = 0;
int is_start = TRUE;

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

tExpressionList* precedencSA(string *input, tExpressionList *expL, tLinkedList *linkedL)
{
    tLinkedList *tokens = get_tokens(input, linkedL);

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

    tExpressionRule rule;
    int index = 0;
    tToken *topToken;
    tToken *inputToken = (tToken *)(StrLLLocateNthElem(tokens, index)->Content);

    topToken = findTerminalToken(&topOfStack);
    do
    {
    
        int firstIndex = getTokenTableIndex(topToken->type);

        int secondIndex = getTokenTableIndex(inputToken->type);

        if (inputToken->type != tEOF || topToken->type != tEOF)
        {
            switch (precedentTable[firstIndex][secondIndex])
            {
                case '>': //zamen <y
                    rule = extractexpression(&topOfStack);
                    rule = applyrule(&topOfStack, rule, linkedL);
                    if(is_start){
                        CreateNode(rule, expL);
                        is_start = FALSE;
                    }
                    if (rule.operator!= NULL)   
                    {
                        InsertList(rule, expL);

                    }
                   if(input->length == 1 ){
                        InsertList(rule, expL);
                    }

                  //  printRule(rule);
                   // printStack(&topOfStack);
                  //  printf("r %s\n", expL->first->first->rightOperand->text->str);
                    break;

                case '<':
                    //zamen a za a<

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
                    StackPush(&topOfStack, inputToken);
                    index++;

                    tListItem *lItem = StrLLLocateNthElem(tokens, index);
                    if (lItem == NULL)
                    {
                        inputToken = &endToken;
                    }
                    else
                    {
                        inputToken = (tToken *)(lItem->Content);
                    }

                    break;
                case 'E':
                    exit(SYN_ERROR);
                    break;
            }
        }
        topToken = findTerminalToken(&topOfStack);

    } while (inputToken->type != tEOF || topToken->type != tEOF); // || topToken-> type != tComma

    counter = 0;
    is_start = TRUE;
    return expL;
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
    //printStack(stack);
    tToken *topToken = ((tToken *)stack->top_stack->value);
    if (topToken->type != tExprOpen)
    {
        exp.rightOperand = topToken;
        StackPop(stack);
        topToken = ((tToken *)stack->top_stack->value);
    }
    if (topToken->type != tExprOpen)
    {
        exp.operator= topToken;
        StackPop(stack);
        topToken = ((tToken *)stack->top_stack->value);
        if ((exp.operator->type == tDivide) && (strcmp(exp.rightOperand->text->str, "0") == 0)) //divided by zero
        {
            exit(ZERO_DIVISION);
        }
    }
    if (topToken->type != tExprOpen)
    {
        exp.leftOperand = topToken;
        StackPop(stack);
        topToken = ((tToken *)stack->top_stack->value);
    }
    if (exp.operator!= NULL && exp.leftOperand != NULL)
    {
        // printf("%s left %s %s right %s\n", topToken->text->str, exp.leftOperand->text->str, exp.operator->text->str,
        // exp.rightOperand->text->str);

    }
    if (topToken->type != tExprOpen)
    {
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
            break;
        case expBrackets:
            result = rule.operator->dataType;
            break;
        case expPLUSepx:
        case expMINUSepx:
        case expMULepx:
        case expDIVepx:
        case expOPepx:
            if (rule.leftOperand->dataType != rule.rightOperand->dataType)
            {
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
                // printf("rule E+E");
                // printStack(stack);
            }
            if (rule.operator->type == tMinus)
            {
                rule.typeOfRule = expMINUSepx;
                // printf("rule E-E");

                //  printStack(stack);
            }
            if (rule.operator->type == tDivide)
            {
                rule.typeOfRule = expDIVepx;
                //  printf("rule E/E");

                //  printStack(stack);
            }
            if (rule.operator->type == tMultiply)
            {
                rule.typeOfRule = expMULepx;
                //  printf("rule E*E");
                //  printStack(stack);
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
                //  printf("rule E op E");
                //  printStack(stack);
            }
            counter++;
            string placeholderText;
            init_string(&placeholderText);
            //add_to_string(&placeholderText, '{');
            adds_to_string(&placeholderText, "-tmp");
            add_to_string(&placeholderText, counter + '0');
            // add_to_string(&placeholderText, '}');

            tToken *placeholder = malloc(sizeof(tToken));
            _save_to_token(placeholder, &placeholderText, tExprPlaceholder, 0, L);
            placeholder->dataType = determinePlaceholderDatatype(rule);
            rule.placeHolder = placeholder;
            StackPush(stack, rule.placeHolder);
        }
    }
    return rule;
}
/*void printList(tExpressionList *List)
{
    printf("-------------------List: ");
    tExpressionNode *last_node = List->first;
    if (last_node)
    {
        while (!last_node)
        {
            if (last_node->first->leftOperand)
            {
              if(List->first->next_node){
            printf("%s", List->first->next_node->first->leftOperand->text->str);
            } else
            {
                printf("%s", last_node->first->leftOperand->text->str);
            }
            if (last_node->first->operator)
            {
                printf("%s", last_node->first->operator->text->str);
            }
            if (last_node->first->rightOperand)
            {
                printf("%s", last_node->first->rightOperand->text->str);
            }
            if (last_node->first->placeHolder)
            {
                printf(" ListPlace: %s", last_node->first->placeHolder->text->str);
            }
            last_node = last_node->first;
        }
    }
    printf("*************************\n");
}*/

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
void CreateNode(tExpressionRule rule, tExpressionList *expL){

    if (rule.typeOfRule == expBrackets)
    {
        
        ExprLLCreateNextNode(expL, rule.operator->dataType);
    }
    else
    {
        
        ExprLLCreateNextNode(expL, rule.rightOperand->dataType);
    }
   
}
void InsertList(tExpressionRule rule, tExpressionList *expL)
{
    ExprLLInsertExprToLastNode(expL, rule.leftOperand, rule.rightOperand, rule.operator, rule.placeHolder, rule.typeOfRule);
    //ExprLLInsertExprToLastNode(L, rule.leftOperand, rule.rightOperand, rule.operator, rule.placeHolder, rule.typeOfRule);
}