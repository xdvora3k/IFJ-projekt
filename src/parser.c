/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 */

#include "parser.h"

tSymtable *GlobalRoot;
tLinkedList *GlobalInstr;
tState token;
string *attr;


void check_lex_error(){
    if (token == LEX_ERROR){ // TODO: FIX, won't work
        exit(LEX_ERROR);
    }
}

// see get_adjusted_token()
int translate_scanner_states(string *attr){
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
// Returns LEX_ERROR if get_token() fails
int get_adjusted_token(){
    int result = get_token(attr);
    if (result == EOF){
        return sEnd;
    }
    token = result;
    check_lex_error();
    return translate_scanner_states(attr);
}

// skip EOL
// return 0 if everything goes right
void handle_EOL(){
    token = get_adjusted_token();
    if (token == tKeyword){
        return;
    }
    handle_EOL();
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
            exit(INTERNAL_ERROR);
    }
    if (add_to_string(&Content->params, datatype)){
        exit(INTERNAL_ERROR);
    }
}

// Private-like helper function
// Parse `i int` `x float64`
void _parse_param(tBSTNodePtr node){
    token = get_adjusted_token();
    if (token == tClosingSimpleBrace){
        return;
    }
    if (token != tId){
        exit(SYN_ERROR);
    }

    tDataFunction* Content = (tDataFunction*) node->Content;
    if (!Content->list_initialized){
        Content->list_initialized = TRUE;
        init_string(&Content->returnType);
        init_string(&Content->params);
        StrLLInit(&Content->paramNames);
    }
    if (StrLLStringAlreadyOccupied(&Content->paramNames, attr->str)){
        exit(SEM_ERROR);
    }
    StrLLInsert(&Content->paramNames, attr);
    token = get_adjusted_token();

    if (!is_datatype(attr)){
        exit(SEM_ERROR);
    }
    _insert_datatype(Content);

    token = get_adjusted_token();
    if (token != tClosingSimpleBrace && token != tComma){
        exit(SYN_ERROR);
    }
}

// Private-like helper function
// Parse `(i int)(char, int)`
// Token current state ~= tId
void _parse_params(tBSTNodePtr node){
    token = get_adjusted_token();

    // Expecting `(`
    token = get_adjusted_token();
    if (token != tOpeningSimpleBrace){
        exit(SYN_ERROR);
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
            exit(INTERNAL_ERROR);
    }
    if (add_to_string(&((tDataFunction*) node->Content)->returnType, datatype)){
        exit(INTERNAL_ERROR);
    }
}

void _parse_return_type(tBSTNodePtr node){
    token = get_adjusted_token();
    if (token == tClosingSimpleBrace){
        return;
    }
    if (!is_datatype(attr)){
        exit(SYN_ERROR);
    }

    tDataFunction *Content = ((tDataFunction*) node->Content);
    if (!Content->list_initialized){
        init_string(&Content->returnType);
        Content->list_initialized = TRUE;
    }

    _insert_return_type(node);
    token = get_adjusted_token();

}

void _parse_return_types(tBSTNodePtr node){
    token = get_adjusted_token();
    if (token == tOpeningCurlyBrace){
        return;
    }
    if (token != tOpeningSimpleBrace){
        exit(SEM_ERROR);
    }
    while (token != tClosingCurlyBrace){
        _parse_return_type(node);
    }
}

void parse_func_header(){
    token = get_adjusted_token();
    check_lex_error();
    if (token != tId){
        exit(SYN_ERROR);
    }

    if (SymTableSearch(GlobalRoot, attr)){
        exit(SEM_ERROR);
    }
    tBSTNodePtr new_node = SymTableInsertFunction(GlobalRoot, attr);
    ((tDataFunction *) new_node->Content)->declared = TRUE;
    ((tDataFunction *) new_node->Content)->defined = FALSE;

    // Proceeding with parsing params
    _parse_params(new_node);

    // Proceeding with parsing return types
    _parse_return_types(new_node);

}

void func_declaration(){
    switch (token){
        case sFunc:
            parse_func_header();
            break;
        default:
            exit(0); // TODO
    }
}

void program(){
    if (token == EOL){
        handle_EOL();
    }

    if (token != tKeyword) {
        exit(SYN_ERROR);
    }
    if (strcmp(attr->str, "package")){
        exit(SYN_ERROR);
    }
    token = get_token(attr);
    if (strcmp(attr->str, "main")){
        exit(SYN_ERROR);
    }
    printf("lolololloolo\n");
}

// Init all strings
// Return 0 if everything goes right
int init_strings(){
    int result = 0;
    result += init_string(attr);

    return result;
}

void parse(tSymtable *RootPtr, tLinkedList *Instr){
    GlobalRoot = RootPtr;
    GlobalInstr = Instr;

    // Init all global strings, return INTERNAL_ERROR when fails
    if (init_strings()){
        exit(INTERNAL_ERROR);
    }
    if ((token = get_token(attr)) == LEX_ERROR){
        clear_str(attr);
        exit(LEX_ERROR);
    }
    program();
}
