//
// Created by xkvasn14 on 02.12.20.
//

#include "Connector.h"

void print_variable_declaration(tLinkedList *leftside, tPassedSide *rightside)
{
    tInstructionOperand *left;
    tInstructionOperand *right;

    left = CreateOperand(leftside->first->Content,"",rightside->first->data_type,Frame_LF);
    if(rightside->first->is_variable)
        right = CreateOperand(rightside->first->value,"",rightside->first->data_type,Frame_LF);
    else
        right = CreateOperand("",rightside->first->value,rightside->first->data_type,Frame_LF);

    Instruction1(I_DEFVAR,*left);
    Instruction2(I_MOVE,*left,*right);
}

void print_variable_assigment(tLinkedList *leftside,tExpressionList *rightside)
{
    char* str;
    int intVal = 0;
    float floatVal = 0.0;
    int lenL = StrLLLen(leftside);
    for(int i =0 ; i < lenL ; i++)
    {
        tListItem *LeftItem = StrLLLocateNthElem(leftside,i);
        tExpressionNode *RightItem = ExprLLGetNthNode(rightside,i);
        switch(RightItem->data_type)
        {
            case IntType:
                intVal = Calc_Int_Expression(RightItem);
                str = malloc(sizeof(intVal));
                sprintf(str,"%d",intVal);
                break;
            case Float64Type:
                floatVal = Calc_Int_Expression(RightItem);
                str = malloc(sizeof(floatVal));
                sprintf(str,"%f",floatVal);
                break;
            case StringType:
                break;
            case UnderscoreType:
                break;
            default:
                break;
        }

        tInstructionOperand *leftOp, *rightOp;
        leftOp = CreateOperand(LeftItem->Content,"",Unknown_type,Frame_LF);
        rightOp = CreateOperand("",str,RightItem->data_type,Frame_NaN);
        Instruction2(I_MOVE,*leftOp,*rightOp);

    }
}

int Calc_Int_Expression(tExpressionNode *Rules)
{
    // Dont forget to add funcs like CREATEFRAME, ADD, SUB, MUL atd... They have to be printed also
    tExpressionRule *rule;
    int tmp = 0;
    int expLength = ExprLLRuleRuleLen(Rules);
    rule = ExprLLGetNthRuleRule(Rules,0);
    tmp = atoi(rule->leftOperand->text->str);

    for(int i = 0; i < expLength; i++)
    {
        rule = ExprLLGetNthRuleRule(Rules,i);
        switch(rule->operator->text->str[0])
        {
            case '+':
                tmp = tmp + atoi(rule->rightOperand->text->str);
                break;
            case '-':
                tmp = tmp - atoi(rule->rightOperand->text->str);
                break;
            case '*':
                tmp = tmp * atoi(rule->rightOperand->text->str);
                break;
            case '/':
                tmp = tmp / atoi(rule->rightOperand->text->str);
                break;
            default:
                tmp = tmp;
                break;
        }
    }
    return tmp;
}

float Calc_Float_Expression(tExpressionNode *Rules)
{
    // Dont forget to add funcs like CREATEFRAME, ADD, SUB, MUL atd... They have to be printed also
    tExpressionRule *rule;
    float tmp = 0.0;
    int expLength = ExprLLRuleRuleLen(Rules);
    rule = ExprLLGetNthRuleRule(Rules,0);
    tmp = atof(rule->leftOperand->text->str);

    for(int i = 0; i < expLength; i++)
    {
        rule = ExprLLGetNthRuleRule(Rules,i);
        switch(rule->operator->text->str[0])
        {
            case '+':
                tmp = tmp + atof(rule->rightOperand->text->str);
                break;
            case '-':
                tmp = tmp - atof(rule->rightOperand->text->str);
                break;
            case '*':
                tmp = tmp * atof(rule->rightOperand->text->str);
                break;
            case '/':
                tmp = tmp / atof(rule->rightOperand->text->str);
                break;
            default:
                tmp = tmp;
                break;
        }
    }
    return tmp;
}


void print(tPassedSide *Frases)
{
    int numberOfFrases = PassedLLLen(Frases);

    for (int i = 0; i < numberOfFrases; ++i)
    {
        tPassedNode *text = PassedLLGetNode(Frases,i);
        int textLength = strlen(text->value);
        char* finalString = "";
        char* newstring = "";
        for(int k = 0; k < textLength; k++)
        {
            newstring = "";
            int number = text->value[k];
            if(number <= 32 && number >= 10)
            {
                sprintf(newstring,"%d",number);
                finalString = strcat(finalString,"\\0");
                finalString = strcat(finalString,newstring);
            }
            else if(number >= 0 && number <= 9)
            {
                sprintf(newstring,"%d",number);
                finalString = strcat(finalString,"\\00");
                finalString = strcat(finalString,newstring);
            }
            else if(number == 35)
            {
                finalString = strcat(finalString,"\\035");
            }
            else if(number == 92)
            {
                finalString = strcat(finalString,"\\092");
            }
            else
            {
                finalString = strcat(finalString,&text->value[k]);
            }
        }
        tInstructionOperand *op = CreateOperand("",finalString,StringType,Frame_NaN);
        Instruction1(I_WRITE,*op);
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
