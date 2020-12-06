/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 *         xkvasn14, Jaroslav Kvasnicka
 */

#include "connector.h"

extern FRAME currentFrame;
extern tFinalList *final_variables;
extern string FUNC_NAME;
extern tSymtable *GlobalFuncRoot;

void VarLLInit(tFinalList *L){
    L->first = NULL;
}

tFinalVariable* _search_for_variable(tFinalList *L, char* key){
    if (!L || !L->first){
        return NULL;
    }
    tFinalVariable *curr = L->first;
    while (curr && strcmp(curr->key, key)){
        curr = curr->next;
    }
    if (!curr){
        return NULL;
    }
    return curr;
}

char* VarLLInsert(tFinalList *L, char* name, char* func_name, tLinkedList *func_variable_list){
    if (!func_name){
        func_name = FUNC_NAME.str;
    }
    tFinalVariable *variable = malloc(sizeof(tFinalVariable));
    int malloc_size = strlen(name) + strlen(func_name) + 10;
    variable->key = malloc(malloc_size);
    int nests = TableLLGetNumOfNests(func_variable_list, name);
    snprintf(variable->key, malloc_size, "%s_%s_%d", name, func_name, nests);
    tFinalVariable *last_found = _search_for_variable(L, name);
    if (!last_found){
        variable->count = 0;
    }
    else {
        variable->count = last_found->count + 1;
    }
    variable->real_variable_name = malloc(malloc_size + 10);
    snprintf(variable->real_variable_name, malloc_size + 10, "%s_%d", variable->key, variable->count);

    if (!L->first){
        variable->next = NULL;
    }
    else {
        variable->next = L->first;
    }
    L->first = variable;
    return variable->real_variable_name;
}

char* VarLLGetRealName(tFinalList *L, char* name, char* func_name, tLinkedList *func_variable_list){
    if (!func_name){
        func_name = FUNC_NAME.str;
    }
    char *key = malloc(strlen(name) + 10);
    int nests = TableLLGetNumOfNests(func_variable_list, name);
    snprintf(key, strlen(name) + 10, "%s%d", name, nests);
    tFinalVariable *var = _search_for_variable(L, key);
    if (!var){
        return NULL;
    }
    return var->real_variable_name;
}

char* VarLLGetReturnRealName(char* func_name, int index){
    if (!func_name){
        func_name = FUNC_NAME.str;
    }
    tDataFunction *func_node = (tDataFunction*) SymTableSearch(GlobalFuncRoot, func_name);
    char* retval = malloc(strlen(func_name) + 20);
    switch (func_node->returnType.str[index]){
        case 'i':
            snprintf(retval, strlen(func_name) + 20, "-retvalInt_%s_%d", func_name, index);
            break;
        case 's':
            snprintf(retval, strlen(func_name) + 20, "-retvalString_%s_%d", func_name, index);
            break;
        case 'f':
            snprintf(retval, strlen(func_name) + 20, "-retvalFloat_%s_%d", func_name, index);
            break;
        default:
            exit(INTERNAL_ERROR);
    }
    return retval;
}

void print_variable_declaration_Expression(tLinkedList *leftside, tExpressionList *rightside)
{
    printf("PUSHFRAME\n");fflush(stdout);currentFrame = Frame_LF;
    printf("CREATEFRAME\n");fflush(stdout);currentFrame = Frame_TF;

    tInstructionOperand *left = CreateOperand("","",Unknown_type,Frame_NaN);
    tInstructionOperand *right = CreateOperand("","",Unknown_type,Frame_NaN);

    char* printfS = malloc(10);

    int expressionNumber = ExprLLRuleRuleLen(rightside->first);
    tExpressionRule *rule = ExprLLGetNthRuleRule(rightside->first,0);
    if(rule->leftOperand->type == tId)
        rule->leftOperand->text->str = VarLLGetRealName(myVariables,rule->leftOperand->text->str,final_variables);
    for(int i = 0; i < expressionNumber; i++)
    {
        if(rule->rightOperand->type == tId)
            rule->rightOperand->text->str = VarLLGetRealName(myVariables,rule->rightOperand->text->str,final_variables);
    }

    if(rightside->first->data_type == IntType)
    {
        printfS = Calc_Int_Expression(rightside->first);
        left = ChangeOperand(left,VarLLInsert(myVariables,leftside->first->Content,final_variables),"",IntType,Frame_LF);
        right = ChangeOperand(right,printfS,"",IntType,currentFrame);
    }
    else if(rightside->first->data_type == Float64Type)
    {
        printfS = Calc_Float_Expression(rightside->first);
        left = ChangeOperand(left,VarLLInsert(myVariables,leftside->first->Content,final_variables),"",Float64Type,Frame_LF);
        right = ChangeOperand(right,printfS,"",Float64Type,currentFrame);
    }
    else if(rightside->first->data_type == StringType)
    {
        printfS = Calc_String_Expression(rightside->first);
        left = ChangeOperand(left,VarLLInsert(myVariables,leftside->first->Content,final_variables),"",StringType,Frame_LF);
        right = ChangeOperand(right,printfS,"",StringType,currentFrame);
    }
    else if(rightside->first->data_type == UnderscoreType)
    {
        left = ChangeOperand(left,"","nil",UnderscoreType,Frame_NaN);
        right = ChangeOperand(right,"","nil",UnderscoreType,Frame_NaN);
    }
    else if(rightside->first->data_type == Unknown_type)
    {}

    Instruction1(I_DEFVAR,*left);
    Instruction2(I_MOVE,*left,*right);
    printf("POPFRAME\n");fflush(stdout);currentFrame = Frame_LF;
}

void print_variable_assigment_Expression(tLinkedList *leftside,tExpressionList *rightside)
{
    printf("PUSHFRAME\n");fflush(stdout);currentFrame = Frame_LF;
    printf("CREATEFRAME\n");fflush(stdout);currentFrame = Frame_TF;

    int lenL = StrLLLen(leftside);
    char* printfS = malloc(10);
    for(int i =0 ; i < lenL ; i++)
    {
        tListItem *LeftItem = StrLLLocateNthElem(leftside,i);
        tExpressionNode *RightItem = ExprLLGetNthNode(rightside,i);
        tInstructionOperand *opVar = CreateOperand("","",Unknown_type,Frame_NaN);
        tInstructionOperand *opVal = CreateOperand("","",Unknown_type,Frame_NaN);

        int expresionLength = ExprLLRuleRuleLen(RightItem);

        tExpressionRule *rule = ExprLLGetNthRuleRule(RightItem,0);
        if(rule->leftOperand->type == tId)
            rule->leftOperand->text->str = VarLLGetRealName(myVariables,rule->leftOperand->text->str,final_variables);
        for(int j = 0; j < expresionLength; j++)
        {
            tExpressionRule *rule = ExprLLGetNthRuleRule(RightItem,j);
            if(rule->rightOperand->type == tId)
                rule->rightOperand->text->str = VarLLGetRealName(myVariables,rule->rightOperand->text->str,final_variables);
        }

        switch(RightItem->data_type)
        {
            case IntType:
                printfS = Calc_Int_Expression(RightItem);
                opVar = ChangeOperand(opVar,VarLLGetRealName(myVariables,LeftItem->Content,final_variables),"",IntType,Frame_LF);
                opVal = ChangeOperand(opVal,printfS,"",IntType,currentFrame);
                break;
            case Float64Type:
                printfS = Calc_Int_Expression(RightItem);
                opVar = ChangeOperand(opVar,VarLLGetRealName(myVariables,LeftItem->Content,final_variables),"",Float64Type,Frame_LF);
                opVal = ChangeOperand(opVal,printfS,"",Float64Type,currentFrame);
                break;
            case StringType:
                printfS = Calc_String_Expression(RightItem);
                opVar = ChangeOperand(opVar,VarLLGetRealName(myVariables,LeftItem->Content,final_variables),"",StringType,Frame_LF);
                opVal = ChangeOperand(opVal,printfS,"",StringType,currentFrame);
                break;
            case UnderscoreType:
                break;
            default:
                break;
        }
        Instruction2(I_MOVE,*opVar,*opVal);
        printf("POPFRAME\n");fflush(stdout);currentFrame = Frame_LF;

    }
}

char* Calc_Int_Expression(tExpressionNode *Rules)
{
    tExpressionRule *rule;
    int expLength = ExprLLRuleRuleLen(Rules);
    rule = ExprLLGetNthRuleRule(Rules,0);


    //1 + 2 = tmp
    tInstructionOperand *opL = CreateOperand("","",Unknown_type,Frame_NaN); // 1
    tInstructionOperand *opR = CreateOperand("","",Unknown_type,Frame_NaN); // 2
    tInstructionOperand *opV = CreateOperand("","",Unknown_type,Frame_NaN); // tmp

    for(int i = 0; i < expLength; i++)
    {
        rule = ExprLLGetNthRuleRule(Rules,i);
        if(rule->leftOperand->type == tId)
        {

            opL = ChangeOperand(opL,rule->leftOperand->text->str,"",IntType,Frame_LF);
        }
        else
        {opL = ChangeOperand(opL,"",rule->leftOperand->text->str,IntType,Frame_LF);}

        if (rule->rightOperand->type == tId)
        {
            opR = ChangeOperand(opR,rule->rightOperand->text->str,"",IntType,Frame_LF);
        }
        else
        {opR = ChangeOperand(opR,"",rule->rightOperand->text->str,IntType,Frame_LF);}

        printf("DEFVAR %s\n",rule->placeHolder->text->str);fflush(stdout);
        opV = ChangeOperand(opV,rule->placeHolder->text->str,"",IntType,currentFrame);

        switch(rule->operator->text->str[0])
        {
            case '+':
                Instruction3(I_ADD,*opV,*opL,*opR);
                break;
            case '-':
                Instruction3(I_SUB,*opV,*opL,*opR);
                break;
            case '*':
                Instruction3(I_MUL,*opV,*opL,*opR);
                break;
            case '/':
                Instruction3(I_DIV,*opV,*opL,*opR);
                break;
            case '%':
                Instruction3(I_IDIV,*opV,*opL,*opR);
                break;
            default:
                break;
        }
    }
    return opV->name;
}

char* Calc_Float_Expression(tExpressionNode *Rules)
{
    tExpressionRule *rule;
    int expLength = ExprLLRuleRuleLen(Rules);
    rule = ExprLLGetNthRuleRule(Rules,0);

    //1 + 2 = tmp
    tInstructionOperand *opL = CreateOperand("","",Unknown_type,Frame_NaN); // 1
    tInstructionOperand *opR = CreateOperand("","",Unknown_type,Frame_NaN); // 2
    tInstructionOperand *opV = CreateOperand("","",Unknown_type,Frame_NaN); // tmp

    for(int i = 0; i < expLength; i++)
    {
        rule = ExprLLGetNthRuleRule(Rules,i);
        if(rule->leftOperand->type == tId)
        {opL = ChangeOperand(opL,rule->leftOperand->text->str,"",Float64Type,Frame_LF);}
        else
        {opL = ChangeOperand(opL,"",rule->leftOperand->text->str,Float64Type,Frame_LF);}
        if (rule->rightOperand->type == tId)
        {opR = ChangeOperand(opR,rule->rightOperand->text->str,"",Float64Type,Frame_LF);}
        else
        {opR = ChangeOperand(opR,"",rule->rightOperand->text->str,Float64Type,Frame_LF);}

        printf("DEFVAR %s\n",rule->placeHolder->text->str);fflush(stdout);
        opV = ChangeOperand(opV,rule->placeHolder->text->str,"",Float64Type,currentFrame);

        switch(rule->operator->text->str[0])
        {
            case '+':
                Instruction3(I_ADD,*opV,*opL,*opR);
                break;
            case '-':
                Instruction3(I_SUB,*opV,*opL,*opR);
                break;
            case '*':
                Instruction3(I_MUL,*opV,*opL,*opR);
                break;
            case '/':
                Instruction3(I_DIV,*opV,*opL,*opR);
                break;
            case '%':
                Instruction3(I_IDIV,*opV,*opL,*opR);
                break;
            default:
                break;
        }
    }
    return opV->name;
}

char* Calc_String_Expression(tExpressionNode *Rules)
{
    tExpressionRule *rule;
    int expLength = ExprLLRuleRuleLen(Rules);
    rule = ExprLLGetNthRuleRule(Rules,0);

    //1 + 2 = tmp
    tInstructionOperand *opL = CreateOperand("","",Unknown_type,Frame_NaN); // 1
    tInstructionOperand *opR = CreateOperand("","",Unknown_type,Frame_NaN); // 2
    tInstructionOperand *opV = CreateOperand("","",Unknown_type,Frame_NaN); // tmp

    for(int i = 0; i < expLength; i++)
    {
        rule = ExprLLGetNthRuleRule(Rules,i);
        if(rule->leftOperand->type == tId)
        {opL = ChangeOperand(opL,rule->leftOperand->text->str,"",StringType,Frame_LF);}
        else
        {opL = ChangeOperand(opL,"",rule->leftOperand->text->str,StringType,Frame_LF);}
        if (rule->rightOperand->type == tId)
        {opR = ChangeOperand(opR,rule->rightOperand->text->str,"",StringType,Frame_LF);}
        else
        {opR = ChangeOperand(opR,"",rule->rightOperand->text->str,StringType,Frame_LF);}

        printf("DEFVAR %s\n",rule->placeHolder->text->str);fflush(stdout);
        opV = ChangeOperand(opV,rule->placeHolder->text->str,"",StringType,currentFrame);

        switch(rule->operator->text->str[0])
        {
            case '+':
                Instruction3(I_CONCAT,*opV,*opL,*opR);
                break;
            default:
                break;
        }
    }
    return opV->name;
}

void print_print_Expression(tPassedSide *Frases)
{
    int numberOfFrases = PassedLLLen(Frases);

    for (int i = 0; i < numberOfFrases; ++i)
    {
        tPassedNode *text = PassedLLGetNode(Frases,i);
        int textLength = strlen(text->value);
        string finalString;
        init_string(&finalString);
        char* newstring = malloc(10);
        int number = 0;

        for(int k = 0; k < textLength; k++)
        {
            number = text->value[k];

            if(number == 92)
            {
                k++;
                number = text->value[k];

                switch (text->value[k]){
                    case 34:
                        adds_to_string(&finalString,"\\034");
                        break;
                    case 39:
                        adds_to_string(&finalString,"\\039");
                        break;
                    case 92:
                        adds_to_string(&finalString,"\\092");
                        break;
                    case 97:
                        adds_to_string(&finalString,"\\097");
                        break;
                    case 98:
                        adds_to_string(&finalString,"\\098");
                        break;
                    case 102:
                        adds_to_string(&finalString,"\\102");
                        break;
                    case 110:
                        adds_to_string(&finalString,"\\110");
                        break;
                    case 114:
                        adds_to_string(&finalString,"\\114");
                        break;
                    case 116:
                        adds_to_string(&finalString,"\\116");
                        break;
                    case 118:
                        adds_to_string(&finalString,"\\118");
                        break;
                    default:
                        break;
                }
            }
            else
            {
                if(number <= 32 && number >= 10)
                {
                    sprintf(newstring,"%d",number);
                    fflush(stdout);
                    adds_to_string(&finalString,"\\0");
                    adds_to_string(&finalString,newstring);
                }
                else if(number >= 0 && number <= 9)
                {
                    sprintf(newstring,"%d",number);
                    fflush(stdout);
                    adds_to_string(&finalString,"\\00");
                    adds_to_string(&finalString,newstring);
                }
                else if(number == 35)
                {
                    adds_to_string(&finalString,"\\035");
                }
                else
                {
                    add_to_string(&finalString,text->value[k]);
                }
            }
        }
        free(newstring);
        tInstructionOperand *op;
        if(text->is_variable) {
            switch (text->data_type) {
                case IntType:
                    op = CreateOperand(finalString.str,"",IntType,currentFrame);
                    break;
                case Float64Type:
                    op = CreateOperand(finalString.str,"",Float64Type,currentFrame);
                    break;
                case StringType:
                    op = CreateOperand(finalString.str,"",StringType,currentFrame);
                    break;
                case UnderscoreType:
                    op = CreateOperand(finalString.str,"",UnderscoreType,currentFrame);
                    break;
                default:
                    break;
            }
        }
        else
        {
            op = CreateOperand("",finalString.str,StringType,Frame_NaN);
        }

        clear_str(&finalString);
        Instruction1(I_WRITE,*op);

    }
}

void print_int2float_Expression(tLinkedList *leftside, tPassedSide * rightside)
{
    printf("PUSHFRAME\n");fflush(stdout);currentFrame = Frame_LF;
    printf("CREATEFRAME\n");fflush(stdout);currentFrame = Frame_TF;


    tInstructionOperand *i = CreateOperand("i","",IntType,currentFrame);
    Instruction1(I_DEFVAR,*i);
    tInstructionOperand *intvalue;
    if(rightside->first->is_variable)
        intvalue = CreateOperand(rightside->first->value,"",IntType,Frame_LF);
    else
        intvalue = CreateOperand("",rightside->first->value,IntType,Frame_NaN);

    Instruction2(I_MOVE,*i,*intvalue);
    printf("CALL $int2float\n");fflush(stdout);

    tInstructionOperand *floatvalue = CreateOperand(leftside->first->Content,"",Float64Type,Frame_LF);
    tInstructionOperand *retval = CreateOperand("retval","",Float64Type,currentFrame);
    Instruction2(I_MOVE,*floatvalue,*retval);

    printf("POPFRAME\n");fflush(stdout);currentFrame = Frame_LF;

}

void print_float2int_Expression(tLinkedList *leftside, tPassedSide * rightside)
{
    printf("PUSHFRAME\n");fflush(stdout);currentFrame = Frame_LF;
    printf("CREATEFRAME\n");fflush(stdout);currentFrame = Frame_TF;


    tInstructionOperand *f = CreateOperand("f","",Float64Type,currentFrame);
    Instruction1(I_DEFVAR,*f);
    tInstructionOperand *floatvalue;
    if(rightside->first->is_variable)
        floatvalue = CreateOperand(rightside->first->value,"",Float64Type,Frame_LF);
    else
        floatvalue= CreateOperand("",rightside->first->value,Float64Type,Frame_NaN);

    Instruction2(I_MOVE,*f,*floatvalue);
    printf("CALL $float2int\n");fflush(stdout);

    tInstructionOperand *intvalue = CreateOperand(leftside->first->Content,"",IntType,Frame_LF);
    tInstructionOperand *retval = CreateOperand("retval","",IntType,currentFrame);
    Instruction2(I_MOVE,*intvalue,*retval);

    printf("POPFRAME\n");fflush(stdout);currentFrame = Frame_LF;

}

void print_function_begin(char* funcName, tPassedSide * retvariables, tPassedSide* variables)
{
    printf("LABEL %s\n",funcName);fflush(stdout);
    printf("PUSHFRAME\n");fflush(stdout);
    printf("CREATEFRAME\n");fflush(stdout);


    int number_input_vars = PassedLLLen(variables);

    for(int i = 0; i < number_input_vars; i++)
    {
        tPassedNode *inputVariableNode = PassedLLGetNode(variables,i);
        char* tmpS = malloc(10);

        tmpS = VarLLInsert(myVariables,inputVariableNode->value,final_variables);
        tInstructionOperand *tmpString = CreateOperand(tmpS,"",Unknown_type,Frame_LF);
        Instruction1(I_DEFVAR,*tmpString);

        if(inputVariableNode->is_variable)
            inputVariableNode->value = VarLLGetRealName(myVariables,inputVariableNode->value,final_variables);

        tInstructionOperand *moveString = CreateOperand(inputVariableNode->value,"",Unknown_type,Frame_LF);
        Instruction2(I_MOVE,*tmpString,*moveString);
    }

    int returnNumber = PassedLLLen(retvariables);

    for(int i = 0; i < returnNumber; i++)
    {
        tPassedNode *returnVariable = PassedLLGetNode(retvariables,i);
        char* tmpS = malloc(10);

        tmpS = VarLLInsert(myVariables,returnVariable->value,final_variables);
        tInstructionOperand *tmpString = CreateOperand(tmpS,"",Unknown_type,Frame_LF);
        Instruction1(I_DEFVAR,*tmpString);
    }

}

void print_function_end()
{
    printf("POPFRAME\n");fflush(stdout);
    printf("RETURN\n");fflush(stdout);
}

void print_function_assigment(tLinkedList *leftside, char* funcName,tPassedSide *retvariables,tPassedSide* variables,tPassedSide *params)
{
    int numberLside = StrLLLen(leftside);
    for(int i = 0; i < numberLside; i++)
    {
        tListItem *LeftItem = StrLLLocateNthElem(leftside,i);
        LeftItem->Content = VarLLGetRealName(myVariables,LeftItem->Content,final_variables);
    }

    int numberParams = PassedLLLen(params);
    for(int i = 0; i < numberParams; i++)
    {
        tPassedNode *ParamsNode = PassedLLGetNode(params,i);
        ParamsNode->value = VarLLGetRealName(myVariables,ParamsNode->value,final_variables);
    }

    //  ZKONTROLOVAT JESTLI MÁM DOBŘE VSTUPY A VÝSTUPY včetně built-in funkcí

    if (strcmp(funcName,"inputi") == 0)
    {
        tInstructionOperand *opI = CreateOperand(leftside->first->Content,"",IntType,Frame_LF);
        tInstructionOperand *opII = CreateOperand(leftside->first->Content,"",IntType,Frame_LF);
        tInstructionOperand *retI = CreateOperand("retval","",IntType,Frame_LF);
        tInstructionOperand *retII = CreateOperand("","0",IntType,Frame_NaN);
        printf("CALL $inputi\n");fflush(stdout);
        Instruction2(I_MOVE,*opI,*retI);
        Instruction2(I_MOVE,*opII,*retII);
    }
    else if(strcmp(funcName,"inputf") == 0)
    {
        tInstructionOperand *opI = CreateOperand(leftside->first->Content,"",Float64Type,Frame_LF);
        tInstructionOperand *opII = CreateOperand(leftside->first->Content,"",Float64Type,Frame_LF);
        tInstructionOperand *retI = CreateOperand("retval","",IntType,Frame_LF);
        tInstructionOperand *retII = CreateOperand("","0",IntType,Frame_NaN);
        printf("CALL $inputf\n");fflush(stdout);
        Instruction2(I_MOVE,*opI,*retI);
        Instruction2(I_MOVE,*opII,*retII);
    }
    else if(strcmp(funcName,"inputs") == 0)
    {
        tInstructionOperand *opI = CreateOperand(leftside->first->Content,"",StringType,Frame_LF);
        tInstructionOperand *opII = CreateOperand(leftside->first->Content,"",StringType,Frame_LF);
        tInstructionOperand *retI = CreateOperand("retval","",IntType,Frame_LF);
        tInstructionOperand *retII = CreateOperand("","0",IntType,Frame_NaN);
        printf("CALL $inputs\n");fflush(stdout);
        Instruction2(I_MOVE,*opI,*retI);
        Instruction2(I_MOVE,*opII,*retII);
    }
    else if(strcmp(funcName,"int2float") == 0)
    {
        print_int2float_Expression(leftside,params);
    }
    else if(strcmp(funcName,"float2int") == 0)
    {
        print_float2int_Expression(leftside,params);
    }
    else if(strcmp(funcName,"len") == 0)
    {
        tInstructionOperand *opP = CreateOperand(params->first->value,"",StringType,Frame_LF);
        printf("DEFVAR LF@s");
        tInstructionOperand *opS = CreateOperand("s","",StringType,Frame_LF);
        Instruction2(I_MOVE,*opS,*opP);
        tInstructionOperand *opI = CreateOperand(leftside->first->Content,"",Float64Type,Frame_LF);
        tInstructionOperand *retI = CreateOperand("retval","",IntType,Frame_LF);
        printf("CALL $len\n");fflush(stdout);
        Instruction2(I_MOVE,*opI,*retI);
    }
    else if(strcmp(funcName,"ord") == 0)
    {
        tInstructionOperand *opI = CreateOperand(leftside->first->Content,"",StringType,Frame_LF);
        tInstructionOperand *opII = CreateOperand(leftside->first->nextItem->Content,"",StringType,Frame_LF);
        tInstructionOperand *retI = CreateOperand("retval","",IntType,Frame_LF);
        tInstructionOperand *retII = CreateOperand("","0",IntType,Frame_NaN);
        tInstructionOperand *opParamI = CreateOperand(params->first->value,"",IntType,Frame_LF);
        tInstructionOperand *opParamII = CreateOperand(params->first->nextItem->value,"",IntType,Frame_LF);
        tInstructionOperand *substrI = CreateOperand("ord_s","",StringType,Frame_LF);
        tInstructionOperand *substrII = CreateOperand("ord_i","",IntType,Frame_LF);
        Instruction2(I_MOVE,*substrI,*opParamI);
        Instruction2(I_MOVE,*substrII,*opParamII);

        printf("CALL $ord\n");fflush(stdout);
        Instruction2(I_MOVE,*opI,*retI);
        Instruction2(I_MOVE,*opII,*retII);
    }
    else if(strcmp(funcName,"chr") == 0)
    {
        tInstructionOperand *opI = CreateOperand(leftside->first->Content,"",StringType,Frame_LF);
        tInstructionOperand *opII = CreateOperand(leftside->first->Content,"",StringType,Frame_LF);
        tInstructionOperand *retI = CreateOperand("retval","",IntType,Frame_LF);
        tInstructionOperand *retII = CreateOperand("","0",IntType,Frame_NaN);
        tInstructionOperand *opChr = CreateOperand("LF@chr_int","",IntType,Frame_LF);
        tInstructionOperand *opParamI = CreateOperand(params->first->value,"",IntType,Frame_LF);
        Instruction2(I_MOVE,*opChr,*opParamI);

        printf("CALL $chr\n");fflush(stdout);
        Instruction2(I_MOVE,*opI,*retI);
        Instruction2(I_MOVE,*opII,*retII);
    }
    else if(strcmp(funcName,"substr") == 0)
    {
        tInstructionOperand *opI = CreateOperand(leftside->first->Content,"",StringType,Frame_LF);
        tInstructionOperand *opII = CreateOperand(leftside->first->nextItem->Content,"",StringType,Frame_LF);
        tInstructionOperand *retI = CreateOperand("retval","",IntType,Frame_LF);
        tInstructionOperand *retII = CreateOperand("","0",IntType,Frame_NaN);
        tInstructionOperand *opParamI = CreateOperand(params->first->value,"",IntType,Frame_LF);
        tInstructionOperand *opParamII = CreateOperand(params->first->nextItem->value,"",IntType,Frame_LF);
        tInstructionOperand *opParamIII = CreateOperand(params->first->nextItem->nextItem->value,"",IntType,Frame_LF);
        tInstructionOperand *substrI = CreateOperand("substr_s","",StringType,Frame_LF);
        tInstructionOperand *substrII = CreateOperand("substr_i","",IntType,Frame_LF);
        tInstructionOperand *substrIII = CreateOperand("substr_n","",IntType,Frame_LF);
        Instruction2(I_MOVE,*substrI,*opParamI);
        Instruction2(I_MOVE,*substrII,*opParamII);
        Instruction2(I_MOVE,*substrIII,*opParamIII);

        printf("CALL $substr\n");fflush(stdout);
        Instruction2(I_MOVE,*opI,*retI);
        Instruction2(I_MOVE,*opII,*retII);
    }
    else
    {
        int numberReturnVariables = StrLLLen(leftside);
        int numberInputVariables = PassedLLLen(params);

        for(int i = 0; i < numberInputVariables; i++)
        {
            tPassedNode *InputVariable = PassedLLGetNode(variables,i);
            tPassedNode *InputParameter = PassedLLGetNode(params,i);

            tInstructionOperand *varI = CreateOperand(InputVariable->value,"",InputVariable->data_type,Frame_LF);
            tInstructionOperand *parI = CreateOperand(InputParameter->value,"",InputParameter->data_type,Frame_LF);
            Instruction2(I_MOVE,*varI,*parI);
        }


        printf("CALL $%s\n",funcName);fflush(stdout);

        for(int i = 0; i < numberReturnVariables; i++)
        {
            tListItem *LeftItem = StrLLLocateNthElem(leftside,i);
            tPassedNode *ReturnVariable = PassedLLGetNode(retvariables,i);

            tInstructionOperand *opI = CreateOperand(LeftItem->Content,"",Unknown_type,Frame_LF);
            tInstructionOperand *retI = CreateOperand(ReturnVariable->value,"",ReturnVariable->data_type,Frame_LF);
            Instruction2(I_MOVE,*opI,*retI);
        }


        tInstructionOperand *opParamI = CreateOperand(params->first->value,"",IntType,Frame_LF);
        tInstructionOperand *opParamII = CreateOperand(params->first->nextItem->value,"",IntType,Frame_LF);
        tInstructionOperand *opParamIII = CreateOperand(params->first->nextItem->nextItem->value,"",IntType,Frame_LF);
        tInstructionOperand *substrI = CreateOperand("substr_s","",StringType,Frame_LF);
        tInstructionOperand *substrII = CreateOperand("substr_i","",IntType,Frame_LF);
        tInstructionOperand *substrIII = CreateOperand("substr_n","",IntType,Frame_LF);
        Instruction2(I_MOVE,*substrI,*opParamI);
        Instruction2(I_MOVE,*substrII,*opParamII);
        Instruction2(I_MOVE,*substrIII,*opParamIII);


    }


}

/*
// For cycle:
print_for_start(); // Start for cycle
// declaration or assignment - one time
print_for_condition(); // talk with Marie
print_for_assignment(tLinkedList *left_variables, tExpressionList *right_side); // repeating every loop
print_end_for(); // End for cycle

// If call:
print_if_start(); // probably with passed condition @cc Marie
print_if_end();
print_else_start();
print_else_end();

print_variable_declaration(tLinkedList *left_variables, tPassedSide *right_side);
print_variable_assignment(tLinkedList *left_variables, tExpressionList *right_side);
print_function_call(tLinkedList *left_variables, char* func_name, tPassedSide *right_side); // if non-return func -> left_variables = NULL
print_return(tExpressionList *right_side);
*/
