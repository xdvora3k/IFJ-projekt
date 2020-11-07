/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 */

#include "parser.h"

tSymtable *GlobalRoot;
tLinkedList *GlobalInstr;
tState token;
string attr;
string function_name;


void free_and_exit(int error_code){
    free(attr.str);
    free(function_name.str);
    SymTableDispose(GlobalRoot);
    InstrLLDisposeAll(GlobalInstr);
    exit(error_code);
}

void check_lex_error(){
    if (token == tLEX_ERROR){
        free_and_exit(LEX_ERROR);
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
void get_token_with_handle_EOL(string *string){
    token = get_adjusted_token(string);
    if (token == EOL){
        get_token_with_handle_EOL(string);
    }
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
            free_and_exit(INTERNAL_ERROR);
    }
    if (add_to_string(&Content->params, datatype)){
        free_and_exit(INTERNAL_ERROR);
    }
}

// Private-like helper function
// Parse `i int` `x float64`
void _parse_param(tBSTNodePtr node){
    token = get_adjusted_token(&attr);
    if (token == tClosingSimpleBrace){
        return;
    }
    if (token != tId){
        free_and_exit(SYN_ERROR);
    }

    tDataFunction* Content = (tDataFunction*) node->Content;
    if (!Content->list_initialized){
        Content->list_initialized = TRUE;
        init_string(&Content->returnType);
        init_string(&Content->params);
        StrLLInit(&Content->paramNames);
    }
    if (StrLLStringAlreadyOccupied(&Content->paramNames, attr.str)){
        free_and_exit(SEM_ERROR);
    }
    StrLLInsert(&Content->paramNames, &attr);
    token = get_adjusted_token(&attr);

    if (!is_datatype(&attr)){
        free_and_exit(SEM_ERROR);
    }
    _insert_datatype(Content);

    token = get_adjusted_token(&attr);
    if (token != tClosingSimpleBrace && token != tComma){
        free_and_exit(SYN_ERROR);
    }
}

// Private-like helper function
// Parse `(i int)(char, int)`
// Token current state ~= tId
void _parse_params(tBSTNodePtr node){
    // Expecting `(`
    token = get_adjusted_token(&attr);
    if (token != tOpeningSimpleBrace){
        free_and_exit(SYN_ERROR);
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
            free_and_exit(INTERNAL_ERROR);
    }
    if (add_to_string(&((tDataFunction*) node->Content)->returnType, datatype)){
        free_and_exit(INTERNAL_ERROR);
    }
}

void _parse_return_type(tBSTNodePtr node){
    token = get_adjusted_token(&attr);
    if (token == tClosingSimpleBrace){
        return;
    }
    if (!is_datatype(&attr)){
        free_and_exit(SYN_ERROR);
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
    if (token == tOpeningCurlyBrace){
        return;
    }
    if (token != tOpeningSimpleBrace){
        free_and_exit(SEM_ERROR);
    }
    while (token != tOpeningCurlyBrace){
        _parse_return_type(node);
    }
}

char* parse_func_header(){
    token = get_adjusted_token(&attr);
    check_lex_error();
    char* func_name = attr.str;
    if (token != tId){
        free_and_exit(SYN_ERROR);
    }
    if (SymTableSearch(GlobalRoot, func_name)){
        free_and_exit(SEM_ERROR);
    }
    tBSTNodePtr new_node = SymTableInsertFunction(GlobalRoot, &attr);
    ((tDataFunction *) new_node->Content)->declared = TRUE;
    ((tDataFunction *) new_node->Content)->defined = FALSE;

    // Proceeding with parsing params
    _parse_params(new_node);

    // Proceeding with parsing return types
    _parse_return_types(new_node);
    return func_name;
}

int can_be_variable(tState token){
    switch (token){
        case sInt:
        case sFloat64:
        case sString:
            return TRUE;
        default:
            return FALSE;
    }
}

/*
 * Check if variable exists (declaration) in table or the opposite (assignment)
 * FALSE: declaration
 * TRUE: assignment
 */
void check_variables_in_table(tSymtable *table, tLinkedList *variable_list, int invert){
    if (invert){
        // Check that every variable name was declared at some point
        for (int i = 0; i < StrLLLen(variable_list); i++){
            char* var_name = ((string*) StrLLLocateNthElem(variable_list, i)->Content)->str;
            if (!strcmp(var_name, "_")){
                continue;
            }
            if (!SymTableSearch(table, var_name)){
                free_and_exit(SEM_ERROR);
            }
        }
    }
    else {
        // Check that no variable name was declared at some point
        for (int i = 0; i < StrLLLen(variable_list); i++){
            char* var_name = ((string*) StrLLLocateNthElem(variable_list, i)->Content)->str;
            if (SymTableSearch(table, var_name)){
                free_and_exit(SEM_ERROR);
            }
        }
    }
}

void _process_var_declaration(tLinkedList *variable_list, tSymtable *local_table){
    check_variables_in_table(local_table, variable_list, FALSE);
    if (StrLLLen(variable_list) > 1){
        free_and_exit(SEM_ERROR);
    }
    token = get_adjusted_token(&attr);
    if (token == tId){
        tBSTNodePtr func_node = SymTableSearch(GlobalRoot, attr.str);
        if (!func_node){
            free_and_exit(SEM_ERROR);
        }
        // TODO: Assign values to variables from function call
    }

    if (!can_be_variable(token)){
        free_and_exit(SYN_ERROR);
    }
    // TODO: Assign first variable value

}

void _process_function_call(){
    // TODO
}

void _process_one_term(tLinkedList *list){
    // TODO
}

tLinkedList _assignment_process_right_part(){
    tLinkedList right_part;
    StrLLInit(&right_part);
    do {
        token = get_adjusted_token(&attr);
        if (token == tId || token == tBuiltIn){
            _process_function_call();
            token = get_adjusted_token(&attr);
            if (token != EOL){
                free_and_exit(SYN_ERROR);
            }
            break;
        }
        else {
            _process_one_term(&right_part);
        }
    } while(token == tComma);

    return right_part;
}

void _process_var_assignment(tLinkedList *variable_list, tSymtable *local_table){
    check_variables_in_table(local_table, variable_list, TRUE);
    tLinkedList right_part = _assignment_process_right_part();
    // TODO
}

// tId tComma/sDeclare/sAssign ...
void _process_tId_token(tSymtable *local_table){
    tLinkedList stored_variables;  // in case there is more than one
    StrLLInit(&stored_variables);
    StrLLInsert(&stored_variables, &attr);
    token = get_adjusted_token(&attr);  // token resulting in tComma triggers while loop

    while (token == tComma){
        token = get_adjusted_token(&attr);
        if (token != tId){
            free_and_exit(SYN_ERROR);
        }
        // Check if there are unique variable names
        if (StrLLStringAlreadyOccupied(&stored_variables, attr.str)){
            if (!strcmp(attr.str, "_")){
                free_and_exit(SEM_ERROR);
            }
        }
        StrLLInsert(&stored_variables, &attr);
        token = get_adjusted_token(&attr);
    }

    switch (token){
        case sDeclare:
            _process_var_declaration(&stored_variables, local_table);
            break;
        case sAssign:
            _process_var_assignment(&stored_variables, local_table);
            break;
        default:
            free_and_exit(SYN_ERROR);
    }
}

void _process_one_line(tSymtable *table){
    get_token_with_handle_EOL(&attr);
    switch (token){
        case tId:
            _process_tId_token(table);
            break;
        default:
            free_and_exit(INTERNAL_ERROR);
    }
}

// token is tOpeningCurlyBrace
void _parse_func_body(tSymtable *table){
    while (token != tClosingCurlyBrace) {  // end of function
        _process_one_line(table);
    }
}

void _insert_right_datatype(tBSTNodePtr var_node, char key){
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
            free_and_exit(INTERNAL_ERROR);
    }
}

void func_definition(char* func_name){
    tBSTNodePtr func_node = SymTableSearch(GlobalRoot, func_name);
    tSymtable local_variables;
    SymTableInit(&local_variables);

    int num_of_params = ((tDataFunction*) func_node->Content)->params.length;
    for (int i = 0; i < num_of_params; i++){
        tListItem *var_item = StrLLLocateNthElem(&((tDataFunction*) func_node->Content)->paramNames, i);
        // Insert variable to local symtable
        tBSTNodePtr var_node = SymTableInsertVariable(&local_variables, ((string*) var_item->Content));
        _insert_right_datatype(var_node, ((tDataFunction*) func_node->Content)->params.str[i]);
    }

    // Proceeding with parsing function body
    _parse_func_body(&local_variables);



}

void parse_func(){
    char* func_name = parse_func_header();
    // token IS tOpeningCurlyBrace
    func_definition(func_name);
}

void program(){
    get_token_with_handle_EOL(&attr);
    if (token != tKeyword) {
        free_and_exit(SYN_ERROR);
    }

    if (strcmp(attr.str, "package")){
        free_and_exit(SYN_ERROR);
    }
    token = get_token(&attr);
    if (strcmp(attr.str, "main")){
        free_and_exit(SYN_ERROR);
    }

    // Proceeding with function declaration
    get_token_with_handle_EOL(&attr);
    /*
     *  After `package main`, expecting at least `main` function
     */
    if (token != sFunc){
        free_and_exit(SEM_ERROR);
    }

    while (token != tEOF){
        parse_func();
    }

    // Find main function
    if (!SymTableSearch(GlobalRoot, "main")){
        free_and_exit(SEM_ERROR);
    }

}

// Init all strings
// Return 0 if everything goes right
int init_strings(){
    int result = 0;
    result += init_string(&attr);
    result += init_string(&function_name);

    return result;
}

void parse(tSymtable *RootPtr, tLinkedList *Instr){
    GlobalRoot = RootPtr;
    GlobalInstr = Instr;

    // Init all global strings, return INTERNAL_ERROR when fails
    if (init_strings()){
        free_and_exit(INTERNAL_ERROR);
    }
    program();
}
