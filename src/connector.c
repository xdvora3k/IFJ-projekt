/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 *         xkvasn14, Jaroslav Kvasnicka
 */

#include "connector.h"

extern tFinalList *final_variables;
extern string FUNC_NAME;
extern tSymtable *GlobalFuncRoot;
extern tLinkedList *instructions;


int str_cut(char *str, int begin, int len)
{
    int l = strlen(str);

    if (len < 0) len = l - begin;
    if (begin + len > l) len = l - begin;
    memmove(str + begin, str + begin + len, l - len + 1);

    return len;
}


void VarLLInit(tFinalList *L) {
    L->first = NULL;
}

tFinalVariable *_search_for_variable(tFinalList *L, char *key) {
    if (!L || !L->first) {
        return NULL;
    }
    if (!strcmp(L->first->key, key)){
        return L->first;
    }

    tFinalVariable *curr = L->first;
    while (curr && strcmp(curr->key, key)) {
        curr = curr->next;
    }
    if (!curr) {
        return NULL;
    }
    return curr;
}

char *VarLLInsert(tFinalList *L, char *name, char *func_name, tLinkedList *func_variable_list) {
    if (!func_name) {
        func_name = FUNC_NAME.str;
    }

    tFinalVariable *variable = malloc(sizeof(tFinalVariable));
    int malloc_size = strlen(name) + strlen(func_name) + 10;
    variable->key = malloc(malloc_size);

    int nests;
    if(func_variable_list)
        nests = TableLLGetNumOfNests(func_variable_list, name);
    else
        nests = 1;
    if ((!strcmp(name, "-if") || !strcmp(name, "-else") || !strcmp(name, "-ifend") || !strcmp(name, "-forbegin") ||
         !strcmp(name, "-forend")) && nests == -1) {
         nests = TableLLLen(func_variable_list) - 1;
    }


    snprintf(variable->key, malloc_size, "%s_%s_%d", name, func_name, nests);
    tFinalVariable *last_found = _search_for_variable(L, name);
    if (!last_found) {
        variable->count = 0;
    } else {
        variable->count = last_found->count + 1;
    }
    variable->real_variable_name = malloc(malloc_size + 10);
    snprintf(variable->real_variable_name, malloc_size + 10, "%s_%d", variable->key, variable->count);

    if (!L->first) {
        variable->next = NULL;
    } else {
        variable->next = L->first;
    }
    L->first = variable;
    return variable->real_variable_name;
}

char *VarLLGetRealName(tFinalList *L, char *name, char *func_name, tLinkedList *func_variable_list) {
    if (!func_name) {
        func_name = FUNC_NAME.str;
    }

    int nests;
    char *key = malloc(strlen(name) + strlen(func_name) + 10);
    if (func_variable_list) {
        nests = TableLLGetNumOfNests(func_variable_list, name);
    } else {
        nests = 1;
    }

    if ((!strcmp(name, "-if") || !strcmp(name, "-else") || !strcmp(name, "-ifend") || !strcmp(name, "-forbegin") ||
         !strcmp(name, "-forend")) && nests == -1) {
        nests = TableLLLen(func_variable_list) - 1;
    }
    snprintf(key, strlen(name) + strlen(func_name) + 10, "%s_%s_%d", name, func_name, nests);
    tFinalVariable *var = _search_for_variable(L, key);
    if (!var) {
        return NULL;
    }
    return var->real_variable_name;
}

char *VarLLGetReturnRealName(char *func_name, int index) {
    if (!func_name) {
        func_name = FUNC_NAME.str;
    }
    tBSTNodePtr func_node = SymTableSearch(GlobalFuncRoot, func_name);
    char *retval = malloc(strlen(func_name) + 20);
    switch (((tDataFunction *) func_node->Content)->returnType.str[index]) {
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
            break;
    }
    return retval;
}

void preprint_variables_in_function(tBSTNodePtr node){
    tDataFunction *func_node = node->Content;
    for (int i = 0; i < StrLLLen(&func_node->paramNames); i++){
        char *var_name = StrLLLocateNthElem(&func_node->paramNames, i)->Content;
        char *var_real_name = VarLLInsert(final_variables, var_name, node->Key, NULL);
        char *instr = "DEFVAR GF@";
        int size = strlen(var_real_name) + strlen(instr) + 1;
        char *final = malloc(size);
        snprintf(final, size, "%s%s", instr, var_real_name);
        // TODO
    }

    for (int i = 0; i < func_node->returnType.length; i++){
        char *var_real_name = VarLLGetReturnRealName(node->Key, i);
        char *instr = "DEFVAR GF@";
        int size = strlen(var_real_name) + strlen(instr) + 1;
        char *final = malloc(size);
        snprintf(final, size, "%s%s", instr, var_real_name);
        // TODO
    }
}

void preprint_variables_in_functions(tBSTNodePtr node){
    if (!node){
        return;
    }
    if (node->LPtr){
        preprint_variables_in_functions(node->LPtr);
    }
    if (node->RPtr){
        preprint_variables_in_functions(node->RPtr);
    }
    preprint_variables_in_function(node);
}

void define_built_in_variables(){
    preprint_variables_in_functions(GlobalFuncRoot->root);
    fflush(stdout);
    char* s = "CREATEFRAME";
    // TODO
}

void print_variable_declaration_Expression(tLinkedList *leftside, tExpressionList *rightside,
                                           tLinkedList *func_variable_list) {
    char *a = "PUSHFRAME";
    char *b = "CREATEFRAME";
    // TODO
    tInstructionOperand *left = CreateOperand("", "", Unknown_type, Frame_NaN);
    tInstructionOperand *right = CreateOperand("", "", Unknown_type, Frame_NaN);
    char *printfS = malloc(10);
    if (rightside->first->data_type == IntType) {
        // NELZE NAJÍT OPERAND N V TABULCE
        printfS = Calc_Int_Expression(rightside->first, func_variable_list);
        left = ChangeOperand(left, VarLLInsert(final_variables, leftside->first->Content, NULL, func_variable_list), "",
                             IntType, Frame_GF);
        right = ChangeOperand(right, printfS, "", IntType, Frame_TF);
    } else if (rightside->first->data_type == Float64Type) {
        printfS = Calc_Float_Expression(rightside->first, func_variable_list);
        left = ChangeOperand(left, VarLLInsert(final_variables, leftside->first->Content, NULL, func_variable_list), "",
                             Float64Type, Frame_GF);
        right = ChangeOperand(right, printfS, "", Float64Type, Frame_TF);
    } else if (rightside->first->data_type == StringType) {
        printfS = Calc_String_Expression(rightside->first, func_variable_list);
        left = ChangeOperand(left, VarLLInsert(final_variables, leftside->first->Content, NULL, func_variable_list), "",
                             StringType, Frame_GF);
        right = ChangeOperand(right, printfS, "", StringType, Frame_TF);
    } else if (rightside->first->data_type == UnderscoreType) {
        left = ChangeOperand(left, VarLLInsert(final_variables, leftside->first->Content, NULL, func_variable_list), "",
                             UnderscoreType, Frame_GF);
        right = ChangeOperand(right, "", "nil", UnderscoreType, Frame_NaN);
    } else if (rightside->first->data_type == Unknown_type) {}

    Instruction1(I_DEFVAR, *left);
    Instruction2(I_MOVE, *left, *right);
}

void print_return_assignment(tExpressionList *rightside, char *funcName, tLinkedList *func_variable_list) {
    int numberOReturns = ExprLLNodeLen(rightside);
    char *printfS = malloc(10);


    for (int i = 0; i < numberOReturns; i++) {
        char *returnVars = VarLLGetReturnRealName(funcName, i);


        tExpressionNode *RightItem = ExprLLGetNthNode(rightside, i);
        tInstructionOperand *opVar = CreateOperand("", "", Unknown_type, Frame_NaN);
        tInstructionOperand *opVal = CreateOperand("", "", Unknown_type, Frame_NaN);


        switch (RightItem->data_type) {
            case IntType:
                printfS = Calc_Int_Expression(RightItem, func_variable_list);
                opVar = ChangeOperand(opVar, returnVars, "", IntType, Frame_GF);
                opVal = ChangeOperand(opVal, printfS, "", IntType, Frame_TF);
                break;
            case Float64Type:
                printfS = Calc_Float_Expression(RightItem, func_variable_list);
                opVar = ChangeOperand(opVar, returnVars, "", Float64Type, Frame_GF);
                opVal = ChangeOperand(opVal, printfS, "", Float64Type, Frame_TF);
                break;
            case StringType:
                printfS = Calc_String_Expression(RightItem, func_variable_list);
                opVar = ChangeOperand(opVar, returnVars, "", StringType, Frame_GF);
                opVal = ChangeOperand(opVal, printfS, "", StringType, Frame_TF);
                break;
            case UnderscoreType:
                break;
            default:
                break;
        }
        Instruction2(I_MOVE, *opVar, *opVal);
    }
}

void print_variable_assigment_Expression(tLinkedList *leftside, tExpressionList *rightside,
                                         tLinkedList *func_variable_list) {
    char *a = "PUSHFRAME";
    char *b = "CREATEFRAME";
    // TODO

    int lenL = StrLLLen(leftside);
    char *printfS = malloc(10);
    for (int i = 0; i < lenL; i++) {
        tListItem *LeftItem = StrLLLocateNthElem(leftside, i);
        tExpressionNode *RightItem = ExprLLGetNthNode(rightside, i);
        tInstructionOperand *opVar = CreateOperand("", "", Unknown_type, Frame_NaN);
        tInstructionOperand *opVal = CreateOperand("", "", Unknown_type, Frame_NaN);


        switch (RightItem->data_type) {
            case IntType:

                printfS = Calc_Int_Expression(RightItem, func_variable_list);
                opVar = ChangeOperand(opVar,
                                      VarLLGetRealName(final_variables, LeftItem->Content, NULL, func_variable_list),
                                      "", IntType, Frame_GF);
                opVal = ChangeOperand(opVal, printfS, "", IntType, Frame_TF);
                break;
            case Float64Type:
                printfS = Calc_Float_Expression(RightItem, func_variable_list);
                opVar = ChangeOperand(opVar,
                                      VarLLGetRealName(final_variables, LeftItem->Content, NULL, func_variable_list),
                                      "", Float64Type, Frame_GF);
                opVal = ChangeOperand(opVal, printfS, "", Float64Type, Frame_TF);
                break;
            case StringType:
                printfS = Calc_String_Expression(RightItem, func_variable_list);
                opVar = ChangeOperand(opVar,
                                      VarLLGetRealName(final_variables, LeftItem->Content, NULL, func_variable_list),
                                      "", StringType, Frame_GF);
                opVal = ChangeOperand(opVal, printfS, "", StringType, Frame_TF);
                break;
            case UnderscoreType:
                break;
            default:
                break;
        }
        Instruction2(I_MOVE, *opVar, *opVal);
        char *c = "POPFRAME"
        // TODO
    }


}

char *Calc_Int_Expression(tExpressionNode *Rules, tLinkedList *func_variable_list) {
    tExpressionRule *rule;
    int expLength = ExprLLRuleRuleLen(Rules);
    rule = ExprLLGetNthRuleRule(Rules, 0);
    string ifStatement;
    init_string(&ifStatement);

    //1 + 2 = tmp
    tInstructionOperand *opL = CreateOperand("", "", Unknown_type, Frame_NaN); // 1
    tInstructionOperand *opR = CreateOperand("", "", Unknown_type, Frame_NaN); // 2
    tInstructionOperand *opV = CreateOperand("", "", Unknown_type, Frame_NaN); // tmp

    for (int i = 0; i < expLength; i++) {
        rule = ExprLLGetNthRuleRule(Rules, i);
        string ruleLeftStr;
        init_string(&ruleLeftStr);
        string ruleRightStr;
        init_string(&ruleRightStr);
        //printf("%p\n",(void*)rule->leftOperand);
        //printf("* %s %p %p *\n",rule->rightOperand->text->str,(void*)rule->leftOperand,(void*)rule->operator);
        if (!(void *) rule->leftOperand) {
            tInstructionOperand *rescueOp = CreateOperand("", "", Unknown_type, Frame_NaN);
            if (rule->rightOperand->type == tId) {
                rescueOp = ChangeOperand(rescueOp,
                                         VarLLGetRealName(final_variables, rule->rightOperand->text->str, NULL,
                                                          func_variable_list), "", IntType, Frame_GF);
            } else {
                rescueOp = ChangeOperand(rescueOp, "", rule->rightOperand->text->str, IntType, Frame_NaN);
            }
            opV = ChangeOperand(opV, "-tmp1", "", IntType, Frame_TF);
            Instruction1(I_DEFVAR, *opV);
            Instruction2(I_MOVE, *opV, *rescueOp);

        } else {
            if (rule->leftOperand->type == tId) {
                adds_to_string(&ruleLeftStr, rule->leftOperand->text->str);
                clear_str(rule->leftOperand->text);
                adds_to_string(rule->leftOperand->text,
                               VarLLGetRealName(final_variables, ruleLeftStr.str, NULL, func_variable_list));
                opL = ChangeOperand(opL, rule->leftOperand->text->str, "", IntType, Frame_GF);
            } else {
                fflush(stdout);
                opL = ChangeOperand(opL, "", rule->leftOperand->text->str, IntType, Frame_NaN);
            }
            if (rule->rightOperand->type == tId) {
                adds_to_string(&ruleRightStr, rule->rightOperand->text->str);

                clear_str(rule->rightOperand->text);
                adds_to_string(rule->rightOperand->text,
                               VarLLGetRealName(final_variables, ruleRightStr.str, NULL, func_variable_list));
                opR = ChangeOperand(opR, rule->rightOperand->text->str, "", IntType, Frame_GF);
            } else {
                opR = ChangeOperand(opR, "", rule->rightOperand->text->str, IntType, Frame_NaN);
            }

            opV = ChangeOperand(opV, rule->placeHolder->text->str, "", IntType, Frame_TF);
            Instruction1(I_DEFVAR, *opV);

            switch (rule->operator->text->str[0]) {
                case '+':
                    Instruction3(I_ADD, *opV, *opL, *opR);
                    break;
                case '-':
                    Instruction3(I_SUB, *opV, *opL, *opR);
                    break;
                case '*':
                    Instruction3(I_MUL, *opV, *opL, *opR);
                    break;
                case '/':
                    Instruction3(I_DIV, *opV, *opL, *opR);
                    break;
                case '%':
                    Instruction3(I_IDIV, *opV, *opL, *opR);
                    break;
                case '>':
                case '<':
                case '=':
                case '!':
                    clear_str(&ifStatement);
                    if (rule->leftOperand->type == tId) {
                        adds_to_string(&ifStatement, "GF@");
                        adds_to_string(&ifStatement, rule->leftOperand->text->str);
                    } else {
                        adds_to_string(&ifStatement, "int@");
                        adds_to_string(&ifStatement, rule->leftOperand->text->str);
                    }
                    adds_to_string(&ifStatement, rule->operator->text->str);
                    if (rule->rightOperand->type == tId) {
                        adds_to_string(&ifStatement, "GF@");
                        adds_to_string(&ifStatement, rule->rightOperand->text->str);
                    } else {
                        adds_to_string(&ifStatement, "int@");
                        adds_to_string(&ifStatement, rule->rightOperand->text->str);
                    }
                    return ifStatement.str;

                    break;
                default:
                    break;
            }
        }
    }

    return opV->name;
}

char *Calc_Float_Expression(tExpressionNode *Rules, tLinkedList *func_variable_list) {
    tExpressionRule *rule;
    int expLength = ExprLLRuleRuleLen(Rules);
    rule = ExprLLGetNthRuleRule(Rules, 0);
    string ifStatement;
    init_string(&ifStatement);

    //1 + 2 = tmp
    tInstructionOperand *opL = CreateOperand("", "", Unknown_type, Frame_NaN); // 1
    tInstructionOperand *opR = CreateOperand("", "", Unknown_type, Frame_NaN); // 2
    tInstructionOperand *opV = CreateOperand("", "", Unknown_type, Frame_NaN); // tmp

    for (int i = 0; i < expLength; i++) {
        rule = ExprLLGetNthRuleRule(Rules, i);
        string ruleLeftStr;
        init_string(&ruleLeftStr);
        string ruleRightStr;
        init_string(&ruleRightStr);
        if (!(void *) rule->leftOperand) {
            tInstructionOperand *rescueOp = CreateOperand("", "", Unknown_type, Frame_NaN);
            if (rule->rightOperand->type == tId) {rescueOp = ChangeOperand(rescueOp,VarLLGetRealName(final_variables, rule->rightOperand->text->str, NULL,func_variable_list), "", IntType, Frame_GF);
            } else {
                string mytmp; init_string(&mytmp);
                adds_to_string(&mytmp,"0x");
                adds_to_string(&mytmp,rule->rightOperand->text->str);
                adds_to_string(&mytmp,"p+0");
                rescueOp = ChangeOperand(rescueOp, "", mytmp.str, Float64Type, Frame_NaN);
            }
            opV = ChangeOperand(opV, "-tmp1", "", Float64Type, Frame_TF);
            Instruction1(I_DEFVAR, *opV);
            Instruction2(I_MOVE, *opV, *rescueOp);

        } else {
            fflush(stdout);
            if (rule->leftOperand->type == tId) {
                adds_to_string(&ruleLeftStr, rule->leftOperand->text->str);
                clear_str(rule->leftOperand->text);adds_to_string(rule->leftOperand->text,VarLLGetRealName(final_variables, ruleLeftStr.str, NULL, func_variable_list));
                opL = ChangeOperand(opL, rule->leftOperand->text->str, "", Float64Type, Frame_GF);
            } else {

                string mytmp; init_string(&mytmp);
                adds_to_string(&mytmp,"0x");
                adds_to_string(&mytmp,rule->leftOperand->text->str);
                adds_to_string(&mytmp,"p+0");
                opL = ChangeOperand(opL, "",mytmp.str, Float64Type, Frame_NaN);
            }
            if (rule->rightOperand->type == tId) {
                adds_to_string(&ruleRightStr, rule->rightOperand->text->str);
                clear_str(rule->rightOperand->text);
                adds_to_string(rule->rightOperand->text,
                               VarLLGetRealName(final_variables, ruleRightStr.str, NULL, func_variable_list));
                opR = ChangeOperand(opR, rule->rightOperand->text->str, "", Float64Type, Frame_GF);
            } else {
                string mytmp; init_string(&mytmp);
                adds_to_string(&mytmp,"0x");
                adds_to_string(&mytmp,rule->rightOperand->text->str);
                adds_to_string(&mytmp,"p+0");
                opR = ChangeOperand(opR, "", mytmp.str, Float64Type, Frame_NaN);
            }

            opV = ChangeOperand(opV, rule->placeHolder->text->str, "", Float64Type, Frame_TF);
            Instruction1(I_DEFVAR, *opV);

        switch (rule->operator->text->str[0]) {
            case '+':
                Instruction3(I_ADD, *opV, *opL, *opR);
                break;
            case '-':
                Instruction3(I_SUB, *opV, *opL, *opR);
                break;
            case '*':
                Instruction3(I_MUL, *opV, *opL, *opR);
                break;
            case '/':
                Instruction3(I_DIV, *opV, *opL, *opR);
                break;
            case '%':
                Instruction3(I_IDIV, *opV, *opL, *opR);
                break;
            case '>':
            case '<':
            case '=':
            case '!':
                clear_str(&ifStatement);
                if (rule->leftOperand->type == tId) {
                    adds_to_string(&ifStatement, "GF@");
                    adds_to_string(&ifStatement, rule->leftOperand->text->str);
                } else {
                    adds_to_string(&ifStatement, "float@");
                    adds_to_string(&ifStatement, rule->leftOperand->text->str);
                }
                adds_to_string(&ifStatement, rule->operator->text->str);
                if (rule->rightOperand->type == tId) {
                    adds_to_string(&ifStatement, "GF@");
                    adds_to_string(&ifStatement, rule->rightOperand->text->str);
                } else {
                    adds_to_string(&ifStatement, "float@");
                    adds_to_string(&ifStatement, rule->rightOperand->text->str);
                }
                return ifStatement.str;

                break;
            default:
                break;
        }}
    }
    return opV->name;
}

char *Calc_String_Expression(tExpressionNode *Rules, tLinkedList *func_variable_list) {
    tExpressionRule *rule;
    int expLength = ExprLLRuleRuleLen(Rules);
    rule = ExprLLGetNthRuleRule(Rules, 0);
    string ifStatement;
    init_string(&ifStatement);

    //1 + 2 = tmp
    tInstructionOperand *opL = CreateOperand("", "", Unknown_type, Frame_NaN); // 1
    tInstructionOperand *opR = CreateOperand("", "", Unknown_type, Frame_NaN); // 2
    tInstructionOperand *opV = CreateOperand("", "", Unknown_type, Frame_NaN); // tmp

    for (int i = 0; i < expLength; i++) {
        rule = ExprLLGetNthRuleRule(Rules, i);
        string ruleLeftStr;
        init_string(&ruleLeftStr);
        string ruleRightStr;
        init_string(&ruleRightStr);

       if (!(void *) rule->leftOperand) {
            tInstructionOperand *rescueOp = CreateOperand("", "", Unknown_type, Frame_NaN);
            if (rule->rightOperand->type == tId) {
                rescueOp = ChangeOperand(rescueOp,
                                         VarLLGetRealName(final_variables, rule->rightOperand->text->str, NULL,
                                                          func_variable_list), "", IntType, Frame_GF);
            } else {
                rescueOp = ChangeOperand(rescueOp, "", rule->rightOperand->text->str, StringType, Frame_NaN);
            }
            opV = ChangeOperand(opV, "-tmp1", "", StringType, Frame_TF);
            Instruction1(I_DEFVAR, *opV);
            Instruction2(I_MOVE, *opV, *rescueOp);

        } else {
            fflush(stdout);
            if (rule->leftOperand->type == tId) {
                adds_to_string(&ruleLeftStr, rule->leftOperand->text->str);
                clear_str(rule->leftOperand->text);
                adds_to_string(rule->leftOperand->text,
                               VarLLGetRealName(final_variables, ruleLeftStr.str, NULL, func_variable_list));
                opL = ChangeOperand(opL, rule->leftOperand->text->str, "", StringType, Frame_GF);
            } else {
                fflush(stdout);
                opL = ChangeOperand(opL, "", rule->leftOperand->text->str, StringType, Frame_NaN);
            }
            if (rule->rightOperand->type == tId) {
                adds_to_string(&ruleRightStr, rule->rightOperand->text->str);
                clear_str(rule->rightOperand->text);
                adds_to_string(rule->rightOperand->text,
                               VarLLGetRealName(final_variables, ruleRightStr.str, NULL, func_variable_list));
                opR = ChangeOperand(opR, rule->rightOperand->text->str, "", StringType, Frame_GF);
            } else {
                opR = ChangeOperand(opR, "", rule->rightOperand->text->str, StringType, Frame_NaN);
            }

            opV = ChangeOperand(opV, rule->placeHolder->text->str, "", StringType, Frame_TF);
            Instruction1(I_DEFVAR, *opV);

        switch (rule->operator->text->str[0]) {
            case '+':
                Instruction3(I_CONCAT, *opV, *opL, *opR);
                break;
            case '>':
            case '<':
            case '=':
            case '!':
                clear_str(&ifStatement);
                if (rule->leftOperand->type == tId) {
                    adds_to_string(&ifStatement, "GF@");
                    adds_to_string(&ifStatement, rule->leftOperand->text->str);
                } else {
                    adds_to_string(&ifStatement, "string@");
                    adds_to_string(&ifStatement, rule->leftOperand->text->str);
                }
                adds_to_string(&ifStatement, rule->operator->text->str);
                if (rule->rightOperand->type == tId) {
                    adds_to_string(&ifStatement, "GF@");
                    adds_to_string(&ifStatement, rule->rightOperand->text->str);
                } else {
                    adds_to_string(&ifStatement, "string@");
                    adds_to_string(&ifStatement, rule->rightOperand->text->str);
                }
                return ifStatement.str;

                break;
            default:
                break;
        }
    }}
    return opV->name;
}

void print_print_Expression(tPassedSide *Frases, tLinkedList *func_variable_list) {

    int numberOfFrases = PassedLLLen(Frases);
    for (int i = 0; i < numberOfFrases; i++) {
        tPassedNode *text = PassedLLGetNode(Frases, i);
        int textLength = strlen(text->value);
        string finalString;
        init_string(&finalString);
        char *newstring = malloc(10);
        int number = 0;

        for (int k = 0; k < textLength; k++) {
            number = text->value[k];

            if (number == 92) {
                k++;
                number = text->value[k];

                switch (text->value[k]) {
                    case 34:
                        adds_to_string(&finalString, "\\034");
                        break;
                    case 39:
                        adds_to_string(&finalString, "\\039");
                        break;
                    case 92:
                        adds_to_string(&finalString, "\\092");
                        break;
                    case 97:
                        adds_to_string(&finalString, "\\007");
                        break;
                    case 98:
                        adds_to_string(&finalString, "\\008");
                        break;
                    case 102:
                        adds_to_string(&finalString, "\\012");
                        break;
                    case 110:
                        adds_to_string(&finalString, "\\010");
                        break;
                    case 114:
                        adds_to_string(&finalString, "\\013");
                        break;
                    case 116:
                        adds_to_string(&finalString, "\\009");
                        break;
                    case 118:
                        adds_to_string(&finalString, "\\011");
                        break;
                    default:
                        break;
                }
            } else {
                if (number <= 32 && number >= 10) {
                    sprintf(newstring, "%d", number);
                    fflush(stdout);
                    adds_to_string(&finalString, "\\0");
                    adds_to_string(&finalString, newstring);
                } else if (number >= 0 && number <= 9) {
                    sprintf(newstring, "%d", number);
                    fflush(stdout);
                    adds_to_string(&finalString, "\\00");
                    adds_to_string(&finalString, newstring);
                } else if (number == 35) {
                    adds_to_string(&finalString, "\\035");
                } else {
                    add_to_string(&finalString, text->value[k]);
                }
            }
        }
        free(newstring);
        tInstructionOperand *op;
        if (text->is_variable) {
            switch (text->data_type) {
                case IntType:
                    op = CreateOperand(VarLLGetRealName(final_variables, finalString.str, NULL, func_variable_list), "",
                                       IntType, Frame_GF);
                    break;
                case Float64Type:
                    op = CreateOperand(VarLLGetRealName(final_variables, finalString.str, NULL, func_variable_list), "",
                                       Float64Type, Frame_GF);
                    break;
                case StringType:
                    op = CreateOperand(VarLLGetRealName(final_variables, finalString.str, NULL, func_variable_list), "",
                                       StringType, Frame_GF);
                    break;
                case UnderscoreType:
                    op = CreateOperand(VarLLGetRealName(final_variables, finalString.str, NULL, func_variable_list), "",
                                       UnderscoreType, Frame_GF);
                    break;
                default:
                    break;
            }
        } else {
            op = CreateOperand("", finalString.str, StringType, Frame_NaN);
        }

        clear_str(&finalString);
        Instruction1(I_WRITE, *op);

    }
}

void print_int2float_Expression(tLinkedList *leftside, tPassedSide *rightside, tLinkedList *func_variable_list, char* funcName) {
    char *a = "PUSHFRAME";
    char *b = "CREATEFRAME";
    // TODO

    tInstructionOperand *i = CreateOperand("-i", "", IntType, Frame_GF);
    Instruction1(I_DEFVAR, *i);
    tInstructionOperand *intvalue;
    if (rightside->first->is_variable)
        intvalue = CreateOperand(VarLLGetRealName(final_variables, rightside->first->value, NULL, func_variable_list),
                                 "", IntType, Frame_GF);
    else
        intvalue = CreateOperand("", rightside->first->value, IntType, Frame_NaN);

    Instruction2(I_MOVE, *i, *intvalue);
    char *c = "CALL $int2float";
    // TODO

    tInstructionOperand *floatvalue = CreateOperand(
            VarLLGetRealName(final_variables, leftside->first->Content, NULL, func_variable_list), "", Float64Type,
            Frame_GF);
    char * retval1 = VarLLGetReturnRealName(funcName,0);
    tInstructionOperand *retval = CreateOperand(retval1, "", Float64Type, Frame_GF);
    if (retval)
        Instruction2(I_MOVE, *floatvalue, *retval);

    char *d = "POPFRAME";
    // TODO
}

void print_float2int_Expression(tLinkedList *leftside, tPassedSide *rightside, tLinkedList *func_variable_list, char* funcName) {
    char *a = "PUSHFRAME";
    char *b = "CREATEFRAME";
    // TODO

    tInstructionOperand *f = CreateOperand("-f", "", Float64Type, Frame_GF);
    Instruction1(I_DEFVAR, *f);
    tInstructionOperand *floatvalue;
    if (rightside->first->is_variable)
        floatvalue = CreateOperand(VarLLGetRealName(final_variables, rightside->first->value, NULL, func_variable_list),
                                   "", Float64Type, Frame_GF);
    else
        floatvalue = CreateOperand("", rightside->first->value, Float64Type, Frame_NaN);

    Instruction2(I_MOVE, *f, *floatvalue);
    char *c = "CALL $float2int";
    // TODO

    tInstructionOperand *intvalue = CreateOperand(
            VarLLGetRealName(final_variables, leftside->first->Content, NULL, func_variable_list), "", IntType,
            Frame_GF);
    char* retval1 = VarLLGetReturnRealName(funcName,0);
    tInstructionOperand *retval = CreateOperand(retval1, "", IntType, Frame_GF);
    if (retval)
        Instruction2(I_MOVE, *intvalue, *retval);

    char *d = "POPFRAME";
    // TODO
}

void print_function_begin(char *funcName) {
    char *a = "#";
    int sizeb = strlen(funcName) + strlen("LABEL ") + 1;
    char *b = malloc(sizeb);
    snprintf(b, sizeb, "LABEL %s", funcName);
    char *c = "PUSHFRAME";
    char *d = "CREATEFRAME";
    // TODO
}

void print_function_end() {
    char *a = "POPFRAME";
    char *b = "RETURN";
    char *c = "#";
    // TODO
}

void print_function_assigment(tLinkedList *leftside, char *funcName, tPassedSide *variables, tPassedSide *params,
                              tLinkedList *func_variable_list, int is_return) {
    /*
    int numberLside = StrLLLen(leftside);
    for(int i = 0; i < numberLside; i++)
    {
        tListItem *LeftItem = StrLLLocateNthElem(leftside,i);
        LeftItem->Content = VarLLGetRealName(final_variables,LeftItem->Content,NULL, func_variable_list);
    }

    int numberParams = PassedLLLen(params);
    for(int i = 0; i < numberParams; i++)if(inputVariableNode->is_variable)
    {
        tPassedNode *ParamsNode = PassedLLGetNode(params,i);
        ParamsNode->value = VarLLGetRealName(final_variables,ParamsNode->value,NULL, func_variable_list);
    }*/
    //  ZKONTROLOVAT JESTLI MÁM DOBŘE VSTUPY A VÝSTUPY včetně built-in funkcí
    char *ret1;
    char *ret2;
    if (!leftside && is_return) {
        ret1 = VarLLGetReturnRealName(funcName, 0);
        ret2 = VarLLGetReturnRealName(funcName, 1);
    } else {
        ret1 = VarLLGetRealName(final_variables, leftside->first->Content, NULL, func_variable_list);
        if (leftside->first->nextItem) {
            ret2 = VarLLGetRealName(final_variables, leftside->first->nextItem->Content, NULL, func_variable_list);
        }
    }
    if (strcmp(funcName, "inputi") == 0) {
        tInstructionOperand *opI = CreateOperand(ret1, "", IntType, Frame_GF);
        tInstructionOperand *opII = CreateOperand(ret2, "", IntType, Frame_GF);
        char* retval1 = VarLLGetReturnRealName(funcName,0);
        tInstructionOperand *retI = CreateOperand(retval1, "", IntType, Frame_GF);
        tInstructionOperand *retII = CreateOperand("", "0", IntType, Frame_NaN);
        char *inputi = "CALL $inputi";
        // TODO

        if (ret1)
            Instruction2(I_MOVE, *opI, *retI);
        if (ret2)
            Instruction2(I_MOVE, *opII, *retII);
    } else if (strcmp(funcName, "inputf") == 0) {
        tInstructionOperand *opI = CreateOperand(ret1, "", Float64Type, Frame_GF);
        tInstructionOperand *opII = CreateOperand(ret2, "", Float64Type, Frame_GF);
        char* retval = VarLLGetReturnRealName(funcName,0);
        tInstructionOperand *retI = CreateOperand(retval, "", IntType, Frame_GF);
        tInstructionOperand *retII = CreateOperand("", "0", IntType, Frame_NaN);
        char *inputf = "CALL $inputf";
        // TODO
        if (ret1)
            Instruction2(I_MOVE, *opI, *retI);
        if (ret2)
            Instruction2(I_MOVE, *opII, *retII);
    } else if (strcmp(funcName, "inputs") == 0) {
        //printf("*\n");
        tInstructionOperand *opI = CreateOperand(ret1, "", StringType, Frame_GF);
        tInstructionOperand *opII = CreateOperand(ret2, "", StringType, Frame_GF);
        char* retval1 = VarLLGetReturnRealName(funcName,0);
        tInstructionOperand *retI = CreateOperand(retval1, "", IntType, Frame_GF);
        tInstructionOperand *retII = CreateOperand("", "0", IntType, Frame_NaN);
        char *inputs = "CALL $inputs";
        // TODO
        if (ret1)
            Instruction2(I_MOVE, *opI, *retI);
        if (ret2)
            Instruction2(I_MOVE, *opII, *retII);
    } else if (strcmp(funcName, "int2float") == 0) {
        print_int2float_Expression(leftside, params, func_variable_list,funcName);
    } else if (strcmp(funcName, "float2int") == 0) {
        print_float2int_Expression(leftside, params, func_variable_list,funcName);
    } else if (strcmp(funcName, "len") == 0) {
        tInstructionOperand *opP = CreateOperand("", "", Unknown_type, Frame_NaN);
        if (params->first->is_variable) {
            opP = ChangeOperand(opP, VarLLGetRealName(final_variables, params->first->value, NULL, func_variable_list),
                                "", StringType, Frame_GF);
        } else {
            opP = ChangeOperand(opP, "", params->first->value, StringType, Frame_NaN);
        }
        char *var_real_name = VarLLGetRealName(final_variables, "s", "len", NULL);
        int size = strlen(var_real_name) + strlen("DEFVAR GF@") + 1;
        char *f = malloc(size);
        snprintf(f, size, "DEFVAR GF@%s", var_real_name);
        // TODO
        tInstructionOperand *opS = CreateOperand("s", "", StringType, Frame_GF);
        Instruction2(I_MOVE, *opS, *opP);
        tInstructionOperand *opI = CreateOperand(ret1, "", Float64Type, Frame_GF);
        char* retval1 = VarLLGetReturnRealName(funcName,0);
        tInstructionOperand *retI = CreateOperand(retval1, "", IntType, Frame_GF);
        char *g = "CALL $len"
        // TODO
        if (ret1)
            Instruction2(I_MOVE, *opI, *retI);

    } else if (strcmp(funcName, "ord") == 0) {

        tInstructionOperand *opI = CreateOperand(ret1, "", StringType, Frame_GF);
        tInstructionOperand *opII = CreateOperand(ret2, "", StringType, Frame_GF);
        char* retval1 = VarLLGetReturnRealName(funcName,0);
        tInstructionOperand *retI = CreateOperand(retval1, "", IntType, Frame_GF);
        tInstructionOperand *retII = CreateOperand("", "0", IntType, Frame_NaN);
        char* ord_s = VarLLGetRealName(final_variables,"s",funcName,NULL);
        tInstructionOperand *substrI = CreateOperand(ord_s,"", StringType, Frame_GF);
        char* ord_i = VarLLGetRealName(final_variables,"i",funcName,NULL);
        tInstructionOperand *substrII = CreateOperand(ord_i, "", IntType, Frame_GF);
        tInstructionOperand *opParamI = CreateOperand("", "", IntType, Frame_GF);
        tInstructionOperand *opParamII = CreateOperand("", "", IntType, Frame_GF);


        if (params->first->is_variable) {
            opParamI = ChangeOperand(opParamI,
                                     VarLLGetRealName(final_variables, params->first->value, NULL, func_variable_list),
                                     "", StringType, Frame_GF);
        } else {
            opParamI = ChangeOperand(opParamI, "", params->first->value, StringType, Frame_NaN);
        }

        if (params->first->nextItem->is_variable) {
            opParamII = ChangeOperand(opParamII, VarLLGetRealName(final_variables, params->first->nextItem->value, NULL,
                                                                  func_variable_list), "", IntType, Frame_GF);
        } else {
            opParamII = ChangeOperand(opParamII, "", params->first->nextItem->value, IntType, Frame_NaN);
        }
        Instruction2(I_MOVE, *substrI, *opParamI);
        Instruction2(I_MOVE, *substrII, *opParamII);

        char *h = "CALL $ord"
        // TODO
        if (ret1)
            Instruction2(I_MOVE, *opI, *retI);
        if (ret2)
            Instruction2(I_MOVE, *opII, *retII);
    } else if (strcmp(funcName, "chr") == 0) {

        tInstructionOperand *opI = CreateOperand(ret1, "", StringType, Frame_GF);
        tInstructionOperand *opII = CreateOperand(ret2, "", StringType, Frame_GF);

        char* retval1 = VarLLGetReturnRealName(funcName,0);
        tInstructionOperand *retI = CreateOperand(retval1, "", IntType, Frame_GF);
        tInstructionOperand *retII = CreateOperand("", "0", IntType, Frame_NaN);
        char* chr_i = VarLLGetRealName(final_variables,"i",funcName,NULL);
        tInstructionOperand *opChr = CreateOperand(chr_i, "", IntType, Frame_GF);
        tInstructionOperand *opParamI;
        if(params->first->is_variable)
            opParamI = CreateOperand(VarLLGetRealName(final_variables, params->first->value, NULL, func_variable_list), "", IntType,Frame_GF);
        else
            opParamI = CreateOperand( "",params->first->value,IntType,Frame_NaN);


        char *j = funcName;
        char *k = chr_i;
        // TODO
        Instruction2(I_MOVE, *opChr, *opParamI);

        char *l = "CALL $chr";
        // TODO
        if (ret1)
            Instruction2(I_MOVE, *opI, *retI);
        if (ret2)
            Instruction2(I_MOVE, *opII, *retII);
    } else if (strcmp(funcName, "substr") == 0) {
        tInstructionOperand *opI = CreateOperand(ret1, "", StringType, Frame_GF);
        tInstructionOperand *opII = CreateOperand(ret2, "", StringType, Frame_GF);
        char* retval1 = VarLLGetReturnRealName(funcName,0);
        tInstructionOperand *retI = CreateOperand(retval1, "", IntType, Frame_GF);
        tInstructionOperand *retII = CreateOperand("", "0", IntType, Frame_NaN);
        tInstructionOperand *opParamI;
        if(params->first->is_variable)
            opParamI = CreateOperand(VarLLGetRealName(final_variables, params->first->value, NULL, func_variable_list), "", StringType,Frame_GF);
        else
            opParamI = CreateOperand("",params->first->value,StringType,Frame_NaN);
        tInstructionOperand *opParamII;
        if(params->first->nextItem->is_variable)
            opParamII = CreateOperand(VarLLGetRealName(final_variables, params->first->nextItem->value, NULL, func_variable_list), "",IntType, Frame_GF);
        else
            opParamII = CreateOperand("",params->first->nextItem->value,IntType, Frame_NaN);
        tInstructionOperand *opParamIII;
        if(params->first->nextItem->nextItem->is_variable)
            opParamIII = CreateOperand(VarLLGetRealName(final_variables, params->first->nextItem->nextItem->value, NULL, func_variable_list),"", IntType, Frame_GF);
        else
            opParamIII = CreateOperand("",params->first->nextItem->nextItem->value,IntType, Frame_NaN);
        char* substr_s = VarLLGetRealName(final_variables,"s",funcName,NULL);
        char* substr_i = VarLLGetRealName(final_variables,"i",funcName,NULL);
        char* substr_n = VarLLGetRealName(final_variables,"n",funcName,NULL);
        tInstructionOperand *substrI = CreateOperand(substr_s, "", StringType, Frame_GF);
        tInstructionOperand *substrII = CreateOperand(substr_i, "", IntType, Frame_GF);
        tInstructionOperand *substrIII = CreateOperand(substr_n, "", IntType, Frame_GF);
        Instruction2(I_MOVE, *substrI, *opParamI);
        Instruction2(I_MOVE, *substrII, *opParamII);
        Instruction2(I_MOVE, *substrIII, *opParamIII);

        char *m = "CALL $substr"
        // TODO
        if (ret1)
            Instruction2(I_MOVE, *opI, *retI);
        if (ret2)
            Instruction2(I_MOVE, *opII, *retII);
    } else {//MY FUNCTIONS

        int numberReturnVariables = StrLLLen(leftside);
        int numberInputVariables = PassedLLLen(params);

        tInstructionOperand *varI = CreateOperand("", "", Unknown_type, Frame_NaN);
        tInstructionOperand *parI = CreateOperand("", "", Unknown_type, Frame_NaN);
        tPassedNode *InputVariable;
        tPassedNode *InputParameter;

        for (int i = 0; i < numberInputVariables; i++) {
            InputVariable = PassedLLGetNode(variables, i);
            InputParameter = PassedLLGetNode(params, i);


            varI = ChangeOperand(varI, VarLLGetRealName(final_variables, InputVariable->value, funcName, NULL), "",
                                 InputVariable->data_type, Frame_GF);

            if (InputParameter->is_variable) {

                parI = ChangeOperand(parI, VarLLGetRealName(final_variables, InputParameter->value, NULL, NULL), "",
                                     InputParameter->data_type, Frame_GF);
            } else {

                parI = ChangeOperand(parI, "", InputParameter->value, InputParameter->data_type, Frame_NaN);

            }

            Instruction2(I_MOVE, *varI, *parI);


        }
        int sizen = strlen(funcName) + strlen("CALL $") + 1;
        char *n = malloc(sizen);
        snprintf(n, sizen, "CALL $%s", funcName);
        // TODO

        for (int i = 0; i < numberReturnVariables; i++) {
            tListItem *LeftItem = StrLLLocateNthElem(leftside, i);
            char *ReturnVariable = VarLLGetReturnRealName(funcName, i);

            tInstructionOperand *opI = CreateOperand(
                    VarLLGetRealName(final_variables, LeftItem->Content, NULL, func_variable_list), "", Unknown_type,
                    Frame_GF);
            tInstructionOperand *retI = CreateOperand(ReturnVariable, "", Unknown_type, Frame_GF);
            if (ret1)
                Instruction2(I_MOVE, *opI, *retI);
        }
    }
}

void print_if_begin(tExpressionNode *expList, tLinkedList *func_variable_list) {

    char *a = "#";
    // TODO
    string ifExpression;
    init_string(&ifExpression);

    switch (expList->data_type) {
        case IntType:
            adds_to_string(&ifExpression, Calc_Int_Expression(expList, func_variable_list));
            break;
        case Float64Type:
            adds_to_string(&ifExpression, Calc_Float_Expression(expList, func_variable_list));
            break;
        case StringType:
            adds_to_string(&ifExpression, Calc_String_Expression(expList, func_variable_list));
            break;
        case UnderscoreType:
            break;
        case Unknown_type:
            break;
        default:
            break;
    }

    string leftStr;
    init_string(&leftStr);
    string rightStr;
    init_string(&rightStr);
    string middleOperand;
    init_string(&middleOperand);
    int switcher = 0; //0-left 1-right

    for (int i = 0; i < ifExpression.length; i++) {
        switch (switcher) {
            case 0:
                switch (ifExpression.str[i]) {
                    case '<':
                        if (ifExpression.str[i + 1] == '=') {
                            adds_to_string(&middleOperand, "<=");
                            i++;
                            switcher++;
                        } else {
                            add_to_string(&middleOperand, '<');
                            switcher++;
                        }
                        break;
                    case '>':
                        if (ifExpression.str[i + 1] == '=') {
                            adds_to_string(&middleOperand, ">=");
                            i++;
                            switcher++;
                        } else {
                            add_to_string(&middleOperand, '>');
                            switcher++;
                        }
                        break;
                    case '=':
                        adds_to_string(&middleOperand, "==");
                        i++;
                        switcher++;
                        break;
                    case '!':
                        adds_to_string(&middleOperand, "!=");
                        i++;
                        switcher++;
                        break;
                    default:
                        add_to_string(&leftStr, ifExpression.str[i]);
                        break;
                }
                break;
            case 1:
                add_to_string(&rightStr, ifExpression.str[i]);
                break;
            default:
                break;
        }
    }


    tInstructionOperand *boolTmp = CreateOperand("boolTmp", "", Unknown_type, Frame_TF);
    tInstructionOperand *boolTmp2 = CreateOperand("boolTmp2", "", Unknown_type, Frame_TF);
    tInstructionOperand *leftOp = CreateOperand(leftStr.str, "", Unknown_type, Frame_NaN);
    tInstructionOperand *rightOp = CreateOperand(rightStr.str, "", Unknown_type, Frame_NaN);


    string specIf;
    init_string(&specIf);
    string specElse;
    init_string(&specElse);
    string specEndIf;
    init_string(&specEndIf);
    adds_to_string(&specIf, VarLLInsert(final_variables, "-if", NULL, func_variable_list));
    adds_to_string(&specElse, VarLLInsert(final_variables, "-else", NULL, func_variable_list));
    adds_to_string(&specEndIf, VarLLInsert(final_variables, "-ifend", NULL, func_variable_list));
    tInstructionOperand *elseBegin = CreateOperand(specElse.str, "", Unknown_type, Frame_NaN);
    tInstructionOperand *boolFalse = CreateOperand("bool@false", "", Unknown_type, Frame_NaN);
    //tInstructionOperand *boolTrue = CreateOperand("bool@true","",Unknown_type,Frame_NaN);

    int sizes = strlen(specIf.str) + strlen("LABEL ") + 1;
    char *s = malloc(sizes);
    snprintf(s, sizes, "LABEL %s", specIf.str);
    char *b = "PUSHFRAME"
    char *c = "CREATEFRAME"
    char *d = "DEFVAR TF@boolTmp";
    char *e = "DEFVAR TF@boolTmp2";
    // TODO

    switch (middleOperand.str[0]) {
        case '<':
            if (middleOperand.str[1] == '=') {
                //LT||EQ
                Instruction3(I_LT, *boolTmp, *leftOp, *rightOp);
                Instruction3(I_EQ, *boolTmp2, *leftOp, *rightOp);
                Instruction3(I_OR, *boolTmp, *boolTmp, *boolTmp2);
            } else {
                //LT
                Instruction3(I_LT, *boolTmp, *leftOp, *rightOp);
            }
            break;
        case '>':
            if (middleOperand.str[1] == '=') {
                //GT||EQ
                Instruction3(I_GT, *boolTmp, *leftOp, *rightOp);
                Instruction3(I_EQ, *boolTmp2, *leftOp, *rightOp);
                Instruction3(I_OR, *boolTmp, *boolTmp, *boolTmp2);
            } else {
                //GT
                Instruction3(I_GT, *boolTmp, *leftOp, *rightOp);
            }
            break;
        case '=':
            //EQ
            Instruction3(I_EQ, *boolTmp, *leftOp, *rightOp);
            break;
        case '!':
            //NEQ
            Instruction3(I_EQ, *boolTmp, *leftOp, *rightOp);
            Instruction2(I_NOT, *boolTmp, *boolTmp);
            break;
        default:
            break;
    }

    Instruction3(I_JUMPIFEQ, *elseBegin, *boolTmp, *boolFalse);
}

void print_else_begin(tLinkedList *func_variable_list) {
    string specIfEnd;
    init_string(&specIfEnd);
    string specElse;
    init_string(&specElse);

    adds_to_string(&specIfEnd, VarLLGetRealName(final_variables, "-ifend", NULL, func_variable_list));
    adds_to_string(&specElse, VarLLGetRealName(final_variables, "-else", NULL, func_variable_list));
    int sizea = strlen(specIfEnd.str) + strlen("JUMP ") + 1;
    char *a = malloc(sizes);
    snprintf(a, sizea, "JUMP %s", specIfEnd.str);
    int sizeb = strlen(specIfEnd.str) + strlen("LABEL ") + 1;
    char *b = malloc(sizes);
    snprintf(b, sizeb, "LABEL %s", specIfEnd.str);
    // TODO
}

void print_if_end(tLinkedList *func_variable_list) {
    string specIfEnd;
    init_string(&specIfEnd);
    adds_to_string(&specIfEnd, VarLLGetRealName(final_variables, "-ifend", NULL, func_variable_list));
    int sizea = strlen(specIfEnd.str) + strlen("LABEL ") + 1;
    char *a = malloc(sizea);
    snprintf(a, sizea, "LABEL %s", specIfEnd.str);
    char *b = "POPFRAME";
    char *c = "#";
    // TODO
}

void print_for_begin(tExpressionNode *expList, tLinkedList *leftsideAssigment, tExpressionList *rightsideAssigment,
                     tLinkedList *func_variable_list) {
    char *a = "#";
    // TODO
    //---------------------IF EXPRESSION-------------------------//
    string ifExpression;
    init_string(&ifExpression);
    switch (expList->data_type) {
        case IntType:
            adds_to_string(&ifExpression, Calc_Int_Expression(expList, func_variable_list));
            break;
        case Float64Type:
            adds_to_string(&ifExpression, Calc_Float_Expression(expList, func_variable_list));
            break;
        case StringType:
            adds_to_string(&ifExpression, Calc_String_Expression(expList, func_variable_list));
            break;
        case UnderscoreType:
            break;
        case Unknown_type:
            break;
        default:
            break;
    }

    string leftStr;
    init_string(&leftStr);
    string rightStr;
    init_string(&rightStr);
    string middleOperand;
    init_string(&middleOperand);
    int switcher = 0; //0-left 1-right

    for (int i = 0; i < ifExpression.length; i++) {
        switch (switcher) {
            case 0:
                switch (ifExpression.str[i]) {
                    case '<':
                        if (ifExpression.str[i + 1] == '=') {
                            adds_to_string(&middleOperand, "<=");
                            i++;
                            switcher++;
                        } else {
                            add_to_string(&middleOperand, '<');
                            switcher++;
                        }
                        break;
                    case '>':
                        if (ifExpression.str[i + 1] == '=') {
                            adds_to_string(&middleOperand, ">=");
                            i++;
                            switcher++;
                        } else {
                            add_to_string(&middleOperand, '>');
                            switcher++;
                        }
                        break;
                    case '=':
                        adds_to_string(&middleOperand, "==");
                        i++;
                        switcher++;
                        break;
                    case '!':
                        adds_to_string(&middleOperand, "!=");
                        i++;
                        switcher++;
                        break;
                    default:
                        add_to_string(&leftStr, ifExpression.str[i]);
                        break;
                }
                break;
            case 1:
                add_to_string(&rightStr, ifExpression.str[i]);
                break;
            default:
                break;
        }
    }


    tInstructionOperand *boolTmp = CreateOperand("boolTmp", "", Unknown_type, Frame_TF);
    tInstructionOperand *boolTmp2 = CreateOperand("boolTmp2", "", Unknown_type, Frame_TF);



    tInstructionOperand *leftOp = CreateOperand(leftStr.str, "", Unknown_type, Frame_NaN);
    tInstructionOperand *rightOp = CreateOperand(rightStr.str, "", Unknown_type, Frame_NaN);

    string specBeginFor;
    init_string(&specBeginFor);
    string specEndFor;
    init_string(&specEndFor);
    adds_to_string(&specBeginFor, VarLLInsert(final_variables, "-forbegin", NULL, func_variable_list));
    adds_to_string(&specEndFor, VarLLInsert(final_variables, "-forend", NULL, func_variable_list));
    tInstructionOperand *forEndOp = CreateOperand(specEndFor.str, "", Unknown_type, Frame_NaN);

    tInstructionOperand *boolFalse = CreateOperand("bool@false", "", Unknown_type, Frame_NaN);
    //tInstructionOperand *boolTrue = CreateOperand("bool@true","",Unknown_type,Frame_NaN);

    char *a = "PUSHFRAME";
    char *b = "CREATEFRAME";
    char *c = "DEFVAR TF@boolTmp";
    char *d = "DEFVAR TF@boolTmp2";
    int sizee = strlen(specBeginFor.str) + strlen("LABEL ") + 1;
    char *e = malloc(sizee);
    snprintf(e, sizee, "LABEL %s", specBeginFor.str);
    // TODO

    switch (middleOperand.str[0]) {
        case '<':
            if (middleOperand.str[1] == '=') {
                //LT||EQ
                Instruction3(I_LT, *boolTmp, *leftOp, *rightOp);
                Instruction3(I_EQ, *boolTmp2, *leftOp, *rightOp);
                Instruction3(I_OR, *boolTmp, *boolTmp, *boolTmp2);
            } else {
                //LT
                Instruction3(I_LT, *boolTmp, *leftOp, *rightOp);
            }
            break;
        case '>':
            if (middleOperand.str[1] == '=') {
                //GT||EQ
                Instruction3(I_GT, *boolTmp, *leftOp, *rightOp);
                Instruction3(I_EQ, *boolTmp2, *leftOp, *rightOp);
                Instruction3(I_OR, *boolTmp, *boolTmp, *boolTmp2);
            } else {
                //GT

                Instruction3(I_GT, *boolTmp, *leftOp, *rightOp);
            }
            break;
        case '=':
            //EQ
            Instruction3(I_EQ, *boolTmp, *leftOp, *rightOp);
            break;
        case '!':
            //NEQ
            Instruction3(I_EQ, *boolTmp, *leftOp, *rightOp);
            Instruction2(I_NOT, *boolTmp, *boolTmp);
            break;
        default:
            break;
    }

    Instruction3(I_JUMPIFEQ, *forEndOp, *boolTmp, *boolFalse);

    //---------------------AFTER FOR CONDITION-------------------//

    print_variable_assigment_Expression(leftsideAssigment, rightsideAssigment, func_variable_list);
}

void print_for_end(tLinkedList *func_variable_list) {
    string specForEnd;
    init_string(&specForEnd);
    string specForBegin;
    init_string(&specForBegin);
    adds_to_string(&specForBegin, VarLLGetRealName(final_variables, "-forbegin", NULL, func_variable_list));
    adds_to_string(&specForEnd, VarLLGetRealName(final_variables, "-forend", NULL, func_variable_list));
    int jump_size = strlen(specForBegin.str) + strlen("JUMP ") + 1;
    char* jump = malloc(jump_size);
    snprintf(jump, jump_size, "JUMP %s", specForBegin.str);
    int label_size = strlen(specForEnd.str) + strlen("LABEL ") + 1;
    char* label = malloc(label_size);
    snprintf(label, label_size, "LABEL %s", specForEnd.str);
    char *a ="POPFRAME";
    char *b = "#";
    // TODO
}
