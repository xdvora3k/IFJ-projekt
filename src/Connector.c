//
// Created by xkvasn14 on 02.12.20.
//

#include "Connector.h"

extern FRAME currentFrame;

void print_variable_declaration_Expression(tLinkedList *leftside, tExpressionList *rightside)
{
    printf("PUSHFRAME\n");fflush(stdout);currentFrame = Frame_LF;
    printf("CREATEFRAME\n");fflush(stdout);currentFrame = Frame_TF;

    tInstructionOperand *left = CreateOperand("","",Unknown_type,Frame_NaN);
    tInstructionOperand *right = CreateOperand("","",Unknown_type,Frame_NaN);

    char* printfS = malloc(10);

    if(rightside->first->data_type == IntType)
    {
        printfS = Calc_int_Expression(rightside->first);
        left = ChangeOperand(left,leftside->first->Content,"",IntType,Frame_LF);
        right = ChangeOperand(right,printfS,"",IntType,currentFrame);
    }
    else if(rightside->first->data_type == Float64Type)
    {
        printfS = Calc_float_Expression(rightside->first);
        left = ChangeOperand(left,leftside->first->Content,"",Float64Type,Frame_LF);
        right = ChangeOperand(right,printfS,"",Float64Type,currentFrame);
    }
    else if(rightside->first->data_type == StringType)
    {
        printfS = Calc_string_Expression(rightside->first);
        left = ChangeOperand(left,leftside->first->Content,"",StringType,Frame_LF);
        right = ChangeOperand(right,printfS,"",StringType,currentFrame);
    }
    else if(rightside->first->data_type == UnderscoreType)
    {}
    else if(rightside->first->data_type == Unknown_type)
    {}

    Instruction1(I_DEFVAR,*left);
    Instruction2(I_MOVE,*left,*right);
    printf("POPFRAME");fflush(stdout);currentFrame = Frame_LF;
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

        switch(RightItem->data_type)
        {
            case IntType:
                printfS = Calc_int_Expression(RightItem);
                opVar = ChangeOperand(opVar,LeftItem->Content,"",IntType,Frame_LF);
                opVal = ChangeOperand(opVal,printfS,"",IntType,currentFrame);
                break;
            case Float64Type:
                printfS = Calc_int_Expression(RightItem);
                opVar = ChangeOperand(opVar,LeftItem->Content,"",Float64Type,Frame_LF);
                opVal = ChangeOperand(opVal,printfS,"",Float64Type,currentFrame);
                break;
            case StringType:
                printfS = Calc_string_Expression(RightItem);
                opVar = ChangeOperand(opVar,LeftItem->Content,"",StringType,Frame_LF);
                opVal = ChangeOperand(opVal,printfS,"",StringType,currentFrame);
                break;
            case UnderscoreType:
                break;
            default:
                break;
        }
        Instruction2(I_MOVE,*opVar,*opVal);
        printf("POPFRAME");fflush(stdout);currentFrame = Frame_LF;

    }
}

char* Calc_int_Expression(tExpressionNode *Rules)
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
        {opL = ChangeOperand(opL,rule->leftOperand->text->str,"",IntType,Frame_LF);}
        else
        {opL = ChangeOperand(opL,"",rule->leftOperand->text->str,IntType,Frame_LF);}
        if (rule->rightOperand->type == tId)
        {opR = ChangeOperand(opR,rule->rightOperand->text->str,"",IntType,Frame_LF);}
        else
        {opR = ChangeOperand(opR,"",rule->rightOperand->text->str,IntType,Frame_LF);}

        printf("DEFVAR %s\n",rule->placeHolder->text->str);
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

char* Calc_float_Expression(tExpressionNode *Rules)
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

        printf("DEFVAR %s",rule->placeHolder->text->str);
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

char* Calc_string_Expression(tExpressionNode *Rules)
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

        printf("DEFVAR %s",rule->placeHolder->text->str);
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

void print_int2float_Expression(tLinkedList *leftside, tPassedSide *rightside)
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

void print_float2int_Expression(tLinkedList *leftside, tPassedSide *rightside)
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

void print_if_begin_Expression(tExpressionList *rightside)
{
    rightside = rightside;


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
