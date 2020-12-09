/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 */

#include "parser.h"

tSymtable *GlobalFuncRoot;
tLinkedList *GlobalInstr;
tState token;
string attr;
tFinalList *final_variables;


int for_header_state;
int func_has_return;
string FUNC_NAME;

void free_and_exit(int error_code, tLinkedList *func_variable_list, tLinkedList* left_variables){
    if (func_variable_list){
        if (func_variable_list->first) {
            tListItem *node = func_variable_list->first;
            while (node) {
                SymTableDispose((tSymtable *) node->Content);
                node = node->nextItem;
            }
        }
    }
    if (left_variables){
        if (left_variables->first){
            StrLLDispose(left_variables);
        }
        free(left_variables);
    }
    free(attr.str);
    SymTableDispose(GlobalFuncRoot);
    InstrLLDisposeAll(GlobalInstr);
    exit(error_code);

}

void check_lex_error(){
    if (token == tLEX_ERROR){
        free_and_exit(LEX_ERROR, NULL, NULL);
    }
}

// see get_adjusted_token()
tState translate_scanner_states(string *attr){
    char* compare = attr->str;
    if (!strcmp(compare, "func")){
        return sFunc;
    }
    else if (!strcmp(compare, "for")){
        return sFor;
    }
    else if (!strcmp(compare, "if")){
        return sIf;
    }
    else if (!strcmp(compare, "else")){
        return sElse;
    }
    else if (!strcmp(compare, "return")){
        return sReturn;
    }
    else if (!strcmp(compare, ":=")){
        return sDeclare;
    }
    else if (!strcmp(compare, "=")){
        return sAssign;
    }
    else if (!strcmp(compare, "string")){
        return sString;
    }
    else if (!strcmp(compare, "int")){
        return sInt;
    }
    else if (!strcmp(compare, "float64")){
        return sFloat64;
    }
    return token;
}

// Try to adjust token type for parser
tState get_adjusted_token(string *string){
    token = get_token(string);
    check_lex_error();
    return translate_scanner_states(string);
}

// Get token while skipping EOL
tState get_token_with_handle_EOL(string *string){
    token = get_adjusted_token(string);
    if (token == EOL){
        token = get_token_with_handle_EOL(string);
    }
    return token;
}

void _insert_datatype(tDataFunction* Content){
    char datatype;
    switch (token){
        case sInt:
            datatype = 'i';
            break;
        case sFloat64:
            datatype = 'f';
            break;
        case sString:
            datatype = 's';
            break;
        default:
            free_and_exit(INTERNAL_ERROR, NULL, NULL);
    }
    if (add_to_string(&Content->params, datatype)){
        free_and_exit(INTERNAL_ERROR, NULL, NULL);
    }
}

void _parse_param(tBSTNodePtr node){
    token = get_token_with_handle_EOL(&attr);
    if (token == tClosingSimpleBrace){
        return;
    }
    if (token != tId){
        free_and_exit(SYN_ERROR, NULL, NULL);
    }

    tDataFunction* Content = (tDataFunction*) node->Content;
    if (!Content->list_initialized){
        Content->list_initialized = TRUE;
        init_string(&Content->returnType);
        init_string(&Content->params);
        StrLLInit(&Content->paramNames);
    }
    if (StrLLStringAlreadyOccupied(&Content->paramNames, attr.str)){
        free_and_exit(SEM_ERROR, NULL, NULL);
    }
    StrLLInsert(&Content->paramNames, attr.str);
    token = get_adjusted_token(&attr);

    if (!is_datatype(&attr)){
        free_and_exit(SEM_DATATYPE_ERROR, NULL, NULL);
    }
    _insert_datatype(Content);

    token = get_adjusted_token(&attr);
    if (token != tClosingSimpleBrace && token != tComma){
        free_and_exit(SYN_ERROR, NULL, NULL);
    }
}

// Private-like helper function
// Parse `(i int)(char, int)`
// Token current state ~= tId
void _parse_params(tBSTNodePtr node){
    token = get_adjusted_token(&attr);
    // Expecting tOpeningSimpleBrace
    if (token != tOpeningSimpleBrace){
        free_and_exit(SYN_ERROR, NULL, NULL);
    }
    while (token != tClosingSimpleBrace){
        _parse_param(node);
    }
}

void _insert_return_type(tBSTNodePtr node){
    char datatype;
    switch (token){
        case sInt:
            datatype = 'i';
            break;
        case sFloat64:
            datatype = 'f';
            break;
        case sString:
            datatype = 's';
            break;
        default:
            free_and_exit(SYN_ERROR, NULL, NULL);
    }
    if (add_to_string(&((tDataFunction*) node->Content)->returnType, datatype)){
        free_and_exit(INTERNAL_ERROR, NULL, NULL);
    }
}

void _parse_return_type(tBSTNodePtr node){
    token = get_token_with_handle_EOL(&attr);
    if (token == tClosingSimpleBrace){
        return;
    }
    if (!is_datatype(&attr)){
        free_and_exit(SYN_ERROR, NULL, NULL);
    }

    tDataFunction *Content = ((tDataFunction*) node->Content);
    if (!Content->list_initialized){
        init_string(&Content->returnType);
        Content->list_initialized = TRUE;
    }

    _insert_return_type(node);
    token = get_adjusted_token(&attr);
}

void _parse_return_types(tBSTNodePtr node){
    token = get_adjusted_token(&attr);
    if (token != tOpeningSimpleBrace && token != tOpeningCurlyBrace){
        free_and_exit(SYN_ERROR, NULL, NULL);
    }
    if (token == tOpeningCurlyBrace){
        return;
    }

    while (token != tClosingSimpleBrace){
        _parse_return_type(node);
    }
}

void parse_func_header() {
    token = get_adjusted_token(&attr);
    clear_str(&FUNC_NAME);
    adds_to_string(&FUNC_NAME, attr.str);
    if (token != tId) {
        free_and_exit(SYN_ERROR, NULL, NULL);
    }
    if (SymTableSearch(GlobalFuncRoot, FUNC_NAME.str)) {
        free_and_exit(SEM_ERROR, NULL, NULL);
    }
    tBSTNodePtr new_node = SymTableInsertFunction(GlobalFuncRoot, FUNC_NAME.str);
    ((tDataFunction *) new_node->Content)->declared = TRUE;
    ((tDataFunction *) new_node->Content)->defined = FALSE;
    // Proceeding with parsing params

    _parse_params(new_node);

    // Proceeding with parsing return types

    _parse_return_types(new_node);
}

int _get_expression_to_string(string* string, char *first, int is_condition, tLinkedList *func_variable_list){
    int nums_of_brackets = 0;
    int expecting_something = FALSE;
    int comparison_used = FALSE;
    tVarDataType data_type = -1;
    tDataVariable *var;
    if (!strcmp(attr.str, ",")){
        token = get_adjusted_token(&attr);
    }
    if (first){
        adds_to_string(string, first);
    }
    while (TRUE){
        switch (token){
            case sDeclare:
            case sAssign:
                token = get_adjusted_token(&attr);
                continue;
            case tId:
                var = TableLLGetSingleVariable(func_variable_list, attr.str);
                if (!var){
                    return SEM_ERROR;
                }
                if (data_type != -1){
                    if (data_type != var->dataType){
                        return SEM_GET_DATATYPE_ERROR;
                    }
                }
                else {
                    data_type = var->dataType;
                }
                expecting_something = FALSE;
                break;
            case tFloat:
                if (data_type != -1){
                    if (data_type != Float64Type){
                        return SEM_GET_DATATYPE_ERROR;
                    }
                }
                else {
                    data_type = Float64Type;
                }
                expecting_something = FALSE;
                break;
            case tInteger:
                if (data_type != -1){
                    if (data_type != IntType){
                        return SEM_GET_DATATYPE_ERROR;
                    }
                }
                else {
                    data_type = IntType;
                }
                expecting_something = FALSE;
                break;
            case tString:
                if (data_type != -1){
                    if (data_type != StringType){
                        return SEM_GET_DATATYPE_ERROR;
                    }
                }
                else {
                    data_type = StringType;
                }
                expecting_something = FALSE;
                break;
            case tOpeningSimpleBrace:
                nums_of_brackets++;
                break;
            case tClosingSimpleBrace:
                nums_of_brackets--;
                if (nums_of_brackets < 0){
                    return SEM_GET_DATATYPE_ERROR;
                }
                break;
            case tSemicolon:
                if (!expecting_something && !nums_of_brackets && (for_header_state == 1 || (for_header_state == 2 && comparison_used))){
                    return 0;
                }
                return SEM_GET_DATATYPE_ERROR;
            case EOL:
                if (!expecting_something && !nums_of_brackets && string->str[0] != '\0'){
                    return 0;
                }
                break;
            case tSmallerThan:
            case tSmallerOrEqual:
            case tBiggerThan:
            case tBiggerOrEqual:
            case tEqual:
            case tNotEqual:
                if (comparison_used){
                    return SEM_OTHER_ERROR;
                }
                if (is_condition){
                    comparison_used = TRUE;
                    expecting_something = TRUE;
                    break;
                }
                return SEM_GET_DATATYPE_ERROR;
            case tPlus:
            case tMinus:
            case tMultiply:
            case tDivide:
                expecting_something = TRUE;
                break;
            case tComma:
                if (!expecting_something && !nums_of_brackets && string->str[0] != '\0'){
                    return 0;
                }
                return SEM_ERROR;
            case tOpeningCurlyBrace:
                if (!expecting_something && !nums_of_brackets && (for_header_state == 3 || comparison_used)){
                    return 0;
                }
                return SEM_GET_DATATYPE_ERROR;
            default:
                return SEM_ERROR;
        }
        adds_to_string(string, attr.str);
        token = get_adjusted_token(&attr);
    }
}

tExpressionList* get_expressions(tLinkedList *func_variable_list, char* first, int is_condition){
    tExpressionList *expr_list = malloc(sizeof(tExpressionList));
    ExprLLInit(expr_list);
    int error;
    string expr;
    init_string(&expr);
    do {
        error = _get_expression_to_string(&expr, first, is_condition, func_variable_list);
        if (error){
            free_and_exit(error, func_variable_list, NULL);
        }
        first = NULL;
        expr_list = precedencSA(&expr, expr_list, func_variable_list);
        clear_str(&expr);
    } while (token == tComma && !is_condition);

    return expr_list;
}

int insert_value_to_right_side(tLinkedList *func_variable_list, tPassedSide *right_side, char type_from_func){
    tVarDataType data_type;
    switch (type_from_func){
        case 'i':
            data_type = IntType;
            break;
        case 'f':
            data_type = Float64Type;
            break;
        case 's':
            data_type = StringType;
            break;
        case 'a': // auto - get from variable
            if (!TableLLGetSingleVariable(func_variable_list, attr.str)){
                return 1;
            }
            data_type = TableLLGetSingleVariable(func_variable_list, attr.str)->dataType;
            break;
        default:
            return 1;
    }
    switch (token){
        case tString:
            if (data_type != StringType){
                return 1;
            }
            PassedLLInsert(right_side, attr.str, FALSE, data_type);
            break;
        case tInteger:
            if (data_type != IntType){
                return 1;
            }
            PassedLLInsert(right_side, attr.str, FALSE, data_type);
            break;
        case tFloat:
            if (data_type != Float64Type){
                return 1;
            }
            PassedLLInsert(right_side, attr.str, FALSE, data_type);
            break;
        case tId:
            PassedLLInsert(right_side, attr.str, TRUE, data_type);
            break;
        default:
            return 1;
    }
    return 0;
}

int _check_correct_params_and_return_types(tLinkedList *func_variable_list, tLinkedList *left_variables, tBSTNodePtr func_node){
    for (int i = 0; i < StrLLLen(left_variables); i++){
        char *variable = (char*) StrLLLocateNthElem(left_variables, i)->Content;
        if (!strcmp(variable, "_")){
            continue;
        }
        tVarDataType var_datatype = TableLLGetSingleVariable(func_variable_list, variable)->dataType;
        char func_ret_datatype = ((tDataFunction*) func_node->Content)->returnType.str[i];
        switch (func_ret_datatype) {
            case 'f':
                if (var_datatype != Float64Type) {
                    return 1;
                }
                break;
            case 'i':
                if (var_datatype != IntType) {
                    return 1;
                }
                break;
            case 's':
                if (var_datatype != StringType) {
                    return 1;
                }
                break;
            default:
                return 1;
        }
    }
    return 0;
}

void _check_correct_left_and_right_types(tLinkedList *func_variable_list, tLinkedList *left_variables, tExpressionList *right_side){
    if (StrLLLen(left_variables) != ExprLLNodeLen(right_side)){
        free_and_exit(SEM_ERROR, func_variable_list, left_variables);
    }
    for (int i = 0; i < StrLLLen(left_variables); i++){
        char* left_var = (char*) StrLLLocateNthElem(left_variables, i)->Content;
        if (!strcmp(left_var, "_")){
            continue;
        }
        tVarDataType left_data_type = TableLLGetSingleVariable(func_variable_list, left_var)->dataType;
        tVarDataType right_data_type = ExprLLGetNthNode(right_side, i)->data_type;
        if (left_data_type == right_data_type){
            continue;
        }
        free_and_exit(SEM_DATATYPE_ERROR, func_variable_list, left_variables);
    }
}

void _process_var_declaration(tLinkedList *func_variable_list, tLinkedList *left_variables){
    if (StrLLLen(left_variables) != 1) {
        free_and_exit(SEM_ERROR, func_variable_list, left_variables);
    }
    // Declaration => only 1 variable in list
    if (!strcmp((char*) left_variables->first->Content, "_")){
        // Declaring `_` as variable
        free_and_exit(SEM_OTHER_ERROR, func_variable_list, left_variables);
    }
    // Check if variable does not exist in current table
    if (SymTableSearch((tSymtable*) func_variable_list->first->Content, (char*) left_variables->first->Content)){
        free_and_exit(SEM_ERROR, func_variable_list, left_variables);
    }

    tExpressionList *right_side = get_expressions(func_variable_list, NULL, FALSE);
    if (ExprLLNodeLen(right_side) != 1){
        free_and_exit(SEM_ERROR, func_variable_list, left_variables);
    }

    TableLLInsertFirstSeenVariable(func_variable_list, left_variables, right_side);
    print_variable_declaration_Expression(left_variables, right_side, func_variable_list);
    if (for_header_state == 1){
        return;
    }
    if (token != EOL){
        free_and_exit(SYN_ERROR, func_variable_list, left_variables);
    }
}

int _parse_func_right_side(tLinkedList *func_variable_list, tPassedSide *right_side, tBSTNodePtr func_node) {
    int func_param_len = ((tDataFunction *) func_node->Content)->params.length;
    int error;
    if (!strcmp(func_node->Key, "print")) {
        token = get_token_with_handle_EOL(&attr);
        if (token != tOpeningSimpleBrace){
            return 1;
        }
        // process variable number of params for print
        char data_type;
        while (TRUE) {
            token = get_token_with_handle_EOL(&attr);
            switch (token) {
                case tString:
                    data_type = 's';
                    break;
                case tInteger:
                    data_type = 'i';
                    break;
                case tFloat:
                    data_type = 'f';
                    break;
                case tId:
                    data_type = 'a';
                    break;
                default:
                    free_and_exit(SYN_ERROR, func_variable_list, NULL);
            }
            error = insert_value_to_right_side(func_variable_list, right_side, data_type);
            if (error) {
                return 1;
            }
            token = get_adjusted_token(&attr);
            switch (token){
                case tComma:
                    break;
                case tClosingSimpleBrace:
                    return 0;
                default:
                    return 1;
            }
        }
    }
    else {
        if (token != tOpeningSimpleBrace){
            return 1;
        }

        for (int i = 0; i < func_param_len; i++) {
            // get value
            token = get_token_with_handle_EOL(&attr);
            error = insert_value_to_right_side(func_variable_list, right_side,
                                               ((tDataFunction *) func_node->Content)->params.str[i]);
            if (error) {
                return 1;
            }
            // expecting `)` or `,`
            token = get_adjusted_token(&attr);
            if (token != tClosingSimpleBrace && token != tComma) {
                return 1;
            }
        }
        if (!func_param_len) {
            token = get_adjusted_token(&attr);
            if (token != tClosingSimpleBrace) {
                return 1;
            }
        }
        return 0;
    }
}

int _get_function_param_names(tPassedSide *right_param_names, char* func_name){
    tDataFunction *func_node = SymTableSearch(GlobalFuncRoot, func_name)->Content;
    int param_len = StrLLLen(&func_node->paramNames);
    tVarDataType data_type;
    for (int i = 0; i < param_len; i++){
        char* var_name =  StrLLLocateNthElem(&func_node->paramNames, i)->Content;
        switch (func_node->params.str[i]){
            case 'i':
                data_type = IntType;
                break;
            case 'f':
                data_type = Float64Type;
                break;
            case 's':
                data_type = StringType;
                break;
            default:
                return -1;
        }
        PassedLLInsert(right_param_names, var_name, TRUE, data_type);
    }
    return 0;
}

// Can be called in both cases - assigning, just call (check return types)
void _process_function_call(tLinkedList *func_variable_list, tLinkedList *left_variables, char* called_func_name, int is_return){
    if (!SymTableSearch(GlobalFuncRoot, called_func_name)){
        free_and_exit(SEM_ERROR, func_variable_list, left_variables);
    }
    if (left_variables) {
        int var_len = StrLLLen(left_variables);
        tBSTNodePtr func_node = SymTableSearch(GlobalFuncRoot, called_func_name);
        int func_ret_len = ((tDataFunction *) func_node->Content)->returnType.length;
        // Check correct number of params
        if (var_len != func_ret_len) {
            free_and_exit(SEM_PARAM_ERROR, func_variable_list, left_variables);
        }
        // Check correct type of params and func return types
        if (_check_correct_params_and_return_types(func_variable_list, left_variables, func_node)){
            free_and_exit(SEM_PARAM_ERROR, func_variable_list, left_variables);
        }

        // Check correct type of passed params
        tPassedSide *right_side = malloc(sizeof(tPassedSide));
        PassedLLInit(right_side);
        if (_parse_func_right_side(func_variable_list, right_side, func_node)){
            free_and_exit(SEM_ERROR, func_variable_list, left_variables);
        }

        tPassedSide *right_param_names = malloc(sizeof(tPassedSide));
        PassedLLInit(right_param_names);
        if (_get_function_param_names(right_param_names, called_func_name)){
            free_and_exit(INTERNAL_ERROR, func_variable_list, left_variables);
        }

        token = get_adjusted_token(&attr);
        if (token != EOL){
            free_and_exit(SYN_ERROR, func_variable_list, left_variables);
        }
        print_function_assigment(left_variables, called_func_name, right_param_names, right_side, func_variable_list, is_return);
    }
    else {
        tBSTNodePtr func_node = SymTableSearch(GlobalFuncRoot, called_func_name);
        int func_ret_len = ((tDataFunction *) func_node->Content)->returnType.length;
        if (func_ret_len){
            free_and_exit(SEM_PARAM_ERROR, func_variable_list, left_variables);
        }
        tPassedSide *right_side = malloc(sizeof(tPassedSide));
        PassedLLInit(right_side);
        if (_parse_func_right_side(func_variable_list, right_side, func_node)){
            free_and_exit(SEM_ERROR, func_variable_list, left_variables);
        }

        tPassedSide *right_param_names = malloc(sizeof(tPassedSide));
        PassedLLInit(right_param_names);
        if (_get_function_param_names(right_param_names, called_func_name)){
            free_and_exit(INTERNAL_ERROR, func_variable_list, left_variables);
        }

        token = get_adjusted_token(&attr);
        if (token != EOL){
            free_and_exit(SYN_ERROR, func_variable_list, left_variables);
        }
        if (!strcmp(called_func_name, "print")){
            print_print_Expression(right_side, func_variable_list);
        }
        else {
            print_function_assigment(NULL, called_func_name, right_param_names, right_side, func_variable_list, is_return);
        }
    }
}

void _process_var_assignment(tLinkedList *func_variable_list, tLinkedList *left_variables){
    if (TableLLFindAllVariables(func_variable_list, left_variables)){
        free_and_exit(SEM_ERROR, func_variable_list, left_variables);
    }

    token = get_adjusted_token(&attr);
    string name;
    init_string(&name);
    adds_to_string(&name, attr.str);
    if (token == tId || token == tBuiltIn){
        if (!TableLLGetSingleVariable(func_variable_list, name.str)) {
            token = get_adjusted_token(&attr);
            if (token == tOpeningSimpleBrace) {
                if (SymTableSearch(GlobalFuncRoot, name.str)) {
                    _process_function_call(func_variable_list, left_variables, name.str, FALSE);
                    return;
                }
                free_and_exit(SYN_ERROR, func_variable_list, left_variables);
            }
        }
    }
    tExpressionList *right_side = get_expressions(func_variable_list, NULL, FALSE);
    _check_correct_left_and_right_types(func_variable_list, left_variables, right_side);
    if (for_header_state == 1 && token == tSemicolon){
        return;
    }
    if (for_header_state == 3 && token == tOpeningCurlyBrace){
        return;
    }
    if (token != EOL){
        free_and_exit(SYN_ERROR, func_variable_list, left_variables);
    }
    print_variable_assigment_Expression(left_variables, right_side, func_variable_list);

}

void get_left_side_of_assignment(tLinkedList *stored_variables, char *first){
    // Add passed first variable
    StrLLInsert(stored_variables, first);

    while (token == tComma){
        token = get_adjusted_token(&attr);
        if (token != tId){
            free_and_exit(SYN_ERROR, NULL, stored_variables);
        }
        // Check if there are unique variable names
        if (StrLLStringAlreadyOccupied(stored_variables, attr.str)){
            if (strcmp(attr.str, "_")){
                StrLLDispose(stored_variables);
                free_and_exit(SEM_ERROR, NULL, stored_variables);
            }
        }
        StrLLInsert(stored_variables, attr.str);
        token = get_adjusted_token(&attr);
    }
}

// tId tComma/sDeclare/sAssign ...
void _process_tId_token(tLinkedList *func_variable_list, char *name, int forbid_declare, int forbid_assign){
    tLinkedList *left_variables = malloc(sizeof(tLinkedList));
    StrLLInit(left_variables);
    get_left_side_of_assignment(left_variables, name);

    switch (token){
        case sDeclare:
            if (forbid_declare){
                free_and_exit(SYN_ERROR, func_variable_list, left_variables);
            }
            _process_var_declaration(func_variable_list, left_variables);
            break;
        case sAssign:
            if (forbid_assign){
                free_and_exit(SYN_ERROR, func_variable_list, left_variables);
            }
            _process_var_assignment(func_variable_list, left_variables);
            break;
        default:
            free_and_exit(SYN_ERROR, func_variable_list, left_variables);
    }
    StrLLDispose(left_variables);
}

void _process_if_call(tLinkedList *func_variable_list){
    token = get_adjusted_token(&attr);
    tExpressionList *condition = get_expressions(func_variable_list, NULL, TRUE);

    print_if_begin(condition->first, func_variable_list);

    // token ~= tOpeningCurlyBrace
    tSymtable *if_layer = malloc(sizeof(tSymtable));
    SymTableInit(if_layer);
    TableLLInsertFirst(func_variable_list, if_layer);
    while (token != tClosingCurlyBrace){
        if (token == tEOF){
            free_and_exit(SEM_OTHER_ERROR, func_variable_list, NULL);
        }
        _process_one_line_and_follow(func_variable_list);
    }
    print_else_begin(func_variable_list);
    // delete if layer
    TableLLDeleteFirst(func_variable_list);
    free(if_layer);
    token = get_adjusted_token(&attr);
    // else is required after if
    if (strcmp(attr.str, "else")){
        free_and_exit(SYN_ERROR, func_variable_list, NULL);
    }
    token = get_adjusted_token(&attr);
    if (token != tOpeningCurlyBrace){
        free_and_exit(SYN_ERROR, func_variable_list, NULL);
    }
    token = get_adjusted_token(&attr);
    if (token != EOL){
        free_and_exit(SYN_ERROR, func_variable_list, NULL);
    }
    // insert else layer
    tSymtable *else_layer = malloc(sizeof(tSymtable));
    SymTableInit(else_layer);
    TableLLInsertFirst(func_variable_list, else_layer);
    while (token != tClosingCurlyBrace){
        if (token == tEOF){
            free_and_exit(SEM_OTHER_ERROR, func_variable_list, NULL);
        }
        _process_one_line_and_follow(func_variable_list);
    }
    token = get_adjusted_token(&attr);
    if (token != EOL){
        free_and_exit(SYN_ERROR, func_variable_list, NULL);
    }
    print_if_end(func_variable_list);
    TableLLDeleteFirst(func_variable_list);
    free(else_layer);
}

void _process_for_cycle(tLinkedList *func_variable_list){
    tSymtable *for_layer = malloc(sizeof(tSymtable));
    SymTableInit(for_layer);
    TableLLInsertFirst(func_variable_list, for_layer);

    token = get_adjusted_token(&attr);
    string name;
    init_string(&name);
    adds_to_string(&name, attr.str);
    // sets global variable to indicate that we are in for cycle
    for_header_state = 1;
    if (token == tId){
        token = get_adjusted_token(&attr);
        // first part of for cycle is taken care of
        _process_tId_token(func_variable_list, name.str, FALSE, TRUE); // assigning at this part is forbidden
    }
    if (token != tSemicolon){
        free_and_exit(SEM_ERROR, func_variable_list, NULL);
    }

    for_header_state = 2;
    token = get_adjusted_token(&attr);
    tExpressionList *condition = get_expressions(func_variable_list, NULL, TRUE);
    if (!condition){
        free_and_exit(SEM_ERROR, func_variable_list, NULL);
    }

    for_header_state = 3;
    // Condition is processed, check tOpeningCurlyBrace
    token = get_adjusted_token(&attr);

    tLinkedList *left_for_assignment = malloc(sizeof(tLinkedList));
    StrLLInit(left_for_assignment);
    tExpressionList* right_for_assignment = malloc(sizeof(tExpressionList));
    ExprLLInit(right_for_assignment);
    if (token != tOpeningCurlyBrace){
        if (token != tId) {
            free_and_exit(SEM_OTHER_ERROR, func_variable_list, NULL);
        }
        // process variable assignment
        clear_str(&name);
        adds_to_string(&name, attr.str);
        get_left_side_of_assignment(left_for_assignment, name.str);
        token = get_adjusted_token(&attr);
        switch (token){
            case tDeclaration:
                free_and_exit(SEM_ERROR, func_variable_list, left_for_assignment);
                break;
            case tAssignment:
                right_for_assignment = get_expressions(func_variable_list, NULL, FALSE);
                break;
            default:
                free_and_exit(SYN_ERROR, func_variable_list, left_for_assignment);
        }
    }
    if (token != tOpeningCurlyBrace){
        free_and_exit(SYN_ERROR, func_variable_list, NULL);
    }
    token = get_adjusted_token(&attr);
    if (token != EOL){
        free_and_exit(SYN_ERROR, func_variable_list, NULL);
    }

    print_for_begin(condition->first, left_for_assignment, right_for_assignment, func_variable_list);

    tSymtable *for_cycle = malloc(sizeof(tSymtable));
    SymTableInit(for_cycle);
    TableLLInsertFirst(func_variable_list, for_cycle);
    for_header_state = 0;
    // proceeding with for cycle body
    while (token != tClosingCurlyBrace){
        if (token == tEOF){
            free_and_exit(SEM_OTHER_ERROR, func_variable_list, NULL);
        }
        _process_one_line_and_follow(func_variable_list);
    }
    token = get_adjusted_token(&attr);
    if (token != EOL){
        free_and_exit(SYN_ERROR, func_variable_list, NULL);
    }

    print_for_end(func_variable_list);
    // Delete 2 tables (for cycle, for header)
    TableLLDeleteFirst(func_variable_list);
    TableLLDeleteFirst(func_variable_list);
}

void _process_return(tLinkedList *func_variable_list){
    string* return_type = &((tDataFunction*) SymTableSearch(GlobalFuncRoot, FUNC_NAME.str)->Content)->returnType;
    token = get_adjusted_token(&attr);
    string name;
    init_string(&name);
    adds_to_string(&name, attr.str);
    token = get_adjusted_token(&attr);
    if (token == tId){
        if (token == tOpeningSimpleBrace) {
            if (SymTableSearch(GlobalFuncRoot, name.str)){
                // Compare return types
                string* ret_func_type = &((tDataFunction*) SymTableSearch(GlobalFuncRoot, name.str)->Content)->returnType;
                if (strcmp(return_type->str, ret_func_type->str)){
                    free_and_exit(SEM_PARAM_ERROR, func_variable_list, NULL);
                }
                _process_function_call(func_variable_list, NULL, name.str, TRUE);
            }
        }
    }

    tExpressionList *right_side = get_expressions(func_variable_list, name.str, FALSE);
    print_return_assignment(right_side, FUNC_NAME.str, func_variable_list);
}

void _process_one_line_and_follow(tLinkedList *func_variable_list){
    token = get_token_with_handle_EOL(&attr);
    string name;
    init_string(&name);
    adds_to_string(&name, attr.str);
    printf("-%s\n",name.str);
    switch (token){
        case tId:
            token = get_adjusted_token(&attr);
            // First check if variable exists and after that if function exists
            if (TableLLGetSingleVariable(func_variable_list, name.str)){
                _process_tId_token(func_variable_list, name.str, FALSE, FALSE);
                break;
            }
            if (SymTableSearch(GlobalFuncRoot, name.str)){
                if (token != tOpeningSimpleBrace){
                    free_and_exit(SYN_ERROR, func_variable_list, NULL);
                }
                // Process function call
                _process_function_call(func_variable_list, NULL, name.str, FALSE);
                free(name.str);
                break;
            }
            // Passing table with local vars and first variable
            // token is `:=`, `=`, `,`
            _process_tId_token(func_variable_list, name.str, FALSE, FALSE);
            free(name.str);
            break;
        case tBuiltIn:
            free(name.str);
            if (strcmp(attr.str, "print")){
                free_and_exit(SEM_ERROR, func_variable_list, NULL);
            }
            // Only function without return types is print
            // Others are handled in assignment
            _process_function_call(func_variable_list, NULL, "print", FALSE);
            break;
        case sFor:
            free(name.str);
            _process_for_cycle(func_variable_list);
            break;
        case sIf:
            free(name.str);
            _process_if_call(func_variable_list);
            break;
        case sReturn:
            func_has_return = FALSE;
            free(name.str);
            _process_return(func_variable_list);
            break;
        case tClosingCurlyBrace:
            free(name.str);
            break;
        default:
            free(name.str);
            free_and_exit(SYN_ERROR, func_variable_list, NULL);
    }
}

void _parse_func_body(tLinkedList *func_variable_list){
    while (token != tClosingCurlyBrace){
        _process_one_line_and_follow(func_variable_list);
    }
    TableLLDeleteFirst(func_variable_list);

    // check if return was detected
    if (func_has_return){
        free_and_exit(SEM_OTHER_ERROR, func_variable_list, NULL);
    }
}

void _insert_var_datatype(tBSTNodePtr var_node, char key){
    tVarDataType *data_type = &((tDataVariable*) var_node->Content)->dataType;
    switch (key){
        case 'i':
            *data_type = IntType;
            break;
        case 'f':
            *data_type = Float64Type;
            break;
        case 's':
            *data_type = StringType;
            break;
        default:
            free_and_exit(INTERNAL_ERROR, NULL, NULL);
    }
}

void skip_func_declaration(){
    while (token != tOpeningCurlyBrace){
        token = get_adjusted_token(&attr);
    }
}

void _print_function_defvar_param(char* var_name, char* func_name, tLinkedList *func_variable_list){
    char* var_real_name = VarLLInsert(final_variables, var_name, func_name, func_variable_list);
    printf("DEFVAR LF@%s\n", var_real_name);
}

void func_definition(){
    tBSTNodePtr func_node = SymTableSearch(GlobalFuncRoot, FUNC_NAME.str);
    tSymtable local_variables;
    SymTableInit(&local_variables);
    tLinkedList func_variable_list;
    if (((tDataFunction*) SymTableSearch(GlobalFuncRoot, FUNC_NAME.str)->Content)->returnType.length){
        func_has_return = TRUE;
    }

    // Create Linked List with Symtable `layers`
    // List->first points to the deepest layer
    TableLLInit(&func_variable_list);
    TableLLInsertFirst(&func_variable_list, &local_variables);

    skip_func_declaration(); // skip header line, token ~= tOpeningCurlyBrace
    ((tDataFunction*) func_node->Content)->defined = TRUE;

    // Insert header variables to local symtable
    int num_of_params = ((tDataFunction*) func_node->Content)->params.length;
    for (int i = 0; i < num_of_params; i++){
        tListItem *var_item = StrLLLocateNthElem(&((tDataFunction*) func_node->Content)->paramNames, i);
        tBSTNodePtr var_node = SymTableInsertVariable(&local_variables, (char*) var_item->Content);
        _insert_var_datatype(var_node, ((tDataFunction*) func_node->Content)->params.str[i]);
        _print_function_defvar_param(var_node->Key, FUNC_NAME.str, &func_variable_list);
    }

    int num_of_return_types = ((tDataFunction*) func_node->Content)->returnType.length;
    for (int i = 0; i < num_of_return_types; i++){
        switch (((tDataFunction*) func_node->Content)->returnType.str[i]){
            case 'i':
                printf("DEFVAR LF@-retvalInt_%s_%d\n", FUNC_NAME.str, i);
                break;
            case 's':
                printf("DEFVAR LF@-retvalString_%s_%d\n", FUNC_NAME.str, i);
                break;
            case 'f':
                printf("DEFVAR LF@-retvalFloat_%s_%d\n", FUNC_NAME.str, i);
                break;
            default:
                free_and_exit(INTERNAL_ERROR, &func_variable_list, NULL);
        }
    }

    // Proceeding with parsing function body
    _parse_func_body(&func_variable_list);
}

tVarDataType _get_datatype_from_char(char c){
    switch (c){
        case 'i':
            return IntType;
        case 'f':
            return Float64Type;
        case 's':
            return StringType;
        default:
            return Unknown_type;
    }
}

void parse_func(){
    switch (token){
        case sFunc:
            token = get_adjusted_token(&attr);
            if (token != tId) {
                free_and_exit(SYN_ERROR, NULL, NULL);
            }
            clear_str(&FUNC_NAME);
            adds_to_string(&FUNC_NAME, attr.str);
            print_function_begin(FUNC_NAME.str);
            func_definition();
            print_function_end();
            return;
        default:
            free_and_exit(SYN_ERROR, NULL, NULL);
    }
}

void skip_to_next_function(){
    int curly_braces = 0;
    if (token != tOpeningCurlyBrace){
        token = get_adjusted_token(&attr);
    }
    if (token == tOpeningCurlyBrace){
        curly_braces++;
    }

    while (token != sFunc && token != tEOF){
        token = get_token_with_handle_EOL(&attr);
        if (token == tOpeningCurlyBrace){
            curly_braces++;
        }
        if (token == tClosingCurlyBrace){
            curly_braces--;
        }
    }
    if (curly_braces){
        free_and_exit(SEM_ERROR, NULL, NULL);
    }
}

void first_pass(){
    skip_to_next_function();
    while (token != tEOF){
        parse_func_header();
        skip_to_next_function();
    }
    if (!SymTableSearch(GlobalFuncRoot, "main")){
        free_and_exit(SEM_ERROR, NULL, NULL);
    }
    reset_file_pointer();
}

void check_package_main(){
    token = get_token_with_handle_EOL(&attr);
    if (strcmp(attr.str, "package")){
        free_and_exit(SYN_ERROR, NULL, NULL);
    }
    token = get_token(&attr);
    if (strcmp(attr.str, "main")){
        free_and_exit(SYN_ERROR, NULL, NULL);
    }
}

void program(){
    check_package_main();
    first_pass(); // declare functions
    check_package_main();
    // Proceeding with function def
    final_variables = malloc(sizeof(tFinalList));
    VarLLInit(final_variables);

    token = get_token_with_handle_EOL(&attr);
    while (token != tEOF){
        parse_func();
        token = get_token_with_handle_EOL(&attr);
    }
}

void parse(tSymtable *RootPtr, tLinkedList *Instr){
    GlobalFuncRoot = RootPtr;
    GlobalInstr = Instr;
    if (init_string(&attr)){
        free_and_exit(INTERNAL_ERROR, NULL, NULL);
    }
    if (init_string(&FUNC_NAME)){
        free_and_exit(INTERNAL_ERROR, NULL, NULL);
    }

    program();
}
