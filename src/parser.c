/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 */

#include "parser.h"

tSymtable *GlobalRoot;
tLinkedList *GlobalInstr;
tState token;
string attr;


void free_and_exit(int error_code){
    free(attr.str);
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

void parse_func_header(){
    token = get_adjusted_token(&attr);
    check_lex_error();
    if (token != tId){
        free_and_exit(SYN_ERROR);
    }
    if (SymTableSearch(GlobalRoot, attr.str)){
        free_and_exit(SEM_ERROR);
    }
    tBSTNodePtr new_node = SymTableInsertFunction(GlobalRoot, &attr);
    ((tDataFunction *) new_node->Content)->declared = TRUE;
    ((tDataFunction *) new_node->Content)->defined = FALSE;

    // Proceeding with parsing params
    _parse_params(new_node);

    // Proceeding with parsing return types
    _parse_return_types(new_node);
}

void func_declaration(){
    parse_func_header();
    // token IS tOpeningCurlyBrace
    int skip_func = 1;
    while (skip_func){
        get_token_with_handle_EOL(&attr);
        if (token == tOpeningCurlyBrace){
            skip_func++;
        }
        if (token == tClosingCurlyBrace){
            skip_func--;
        }
    }
    get_token_with_handle_EOL(&attr);
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
        func_declaration();
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
