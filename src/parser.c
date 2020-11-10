/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 */

#include "parser.h"

tSymtable *GlobalRoot;
tListOfInstr *GlobalInstr;
tState token;
string *attr;


void check_lex_error(){
    if (token == LEX_ERROR){ // TODO: FIX, won't work
        exit(LEX_ERROR);
    }
}

// see get_adjusted_token()
int translate_scanner_states(string *attr){
    switch (attr->str){
        case "func":
            return sFunc;
        case "for":
            return sFor;
        case "if":
            return sIf;
        case "else":
            return sElse;
        case "return":
            return sReturn;
        case ":=":
            return sDeclare;
        case "=":
            return sAssign;
        case "string":
            return sString;
        case "int":
            return sInt;
        case "float64":
            return sFloat64;
        default:
            return token;
    }
}

// Try to adjust token type for parser
// Returns LEX_ERROR if get_token() fails
int get_adjusted_token(){
    token = get_token(attr);
    check_lex_error();
    else if (token == EOL){
        return sEnd;
    }
    return translate_scanner_states(attr);
}

// skip EOL
// return 0 if everything goes right
void end_of_line(){
    token = get_adjusted_token();
    else if (token == tKeyword){
        return;
    }
    else if (token == EOF){
        exit(SYN_ERROR);
    }
    return end_of_line();
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
    if (StrLLStringAlreadyOccupied(&Content->paramNames, attr.str)){
        exit(SEM_ERROR);
    }
    StrLLInsert(&Content->paramNames, &attr);
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
    char return_type;
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
    }
    if (add_to_string(((tDataFunction*) node->Content)->returnType, datatype)){
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

    add_to_string((tDataFunction*) node->Content)->returnType, )
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

int parse_func_header(){
    token = get_adjusted_token();
    check_lex_error();
    if (token != tId){
        return SYN_ERROR;
    }

    if (SymTableSearch(*GlobalRoot->root, &attr)){
        return SEM_ERROR;
    }
    tBSTNodePtr new_node = SymTableInsertFunction(*GlobalRoot->root, &attr);
    ((tDataFunction *) new_node->Content)->declared = true;
    ((tDataFunction *) new_node->Content)->defined = false;

    // Proceeding with parsing params
    _parse_params(new_node);

    // Proceeding with parsing return types
    _parse_return_types(new_node);

}

int func_declaration(){
    int result;
    switch (token){
        case sFunc:
            result = parse_func_header();


    }
}

int program(){
    int result;
    switch (token){
        case EOL:
            result = end_of_line();
            if (result){
                return result;
            }
        case tKeyword:
            if (strcmp(attr.str, "package")){
                return SYN_ERROR;
            }
            token = get_token(&attr);
            if (strcmp(attr.str, "main")){
                return SYN_ERROR;
            }
            result = func_definition();


        default:
            return SYN_ERROR;
    }
}

// Init all strings
// Return 0 if everything goes right
int init_strings(){
    int result = 0;
    result += init_string(&attr);

    return result;
}

int parse(tSymtable *RootPtr, tListOfInstr *Instr){
    GlobalRoot = RootPtr;
    GlobalInstr = Instr;
    int result;

    // Init all global strings, return INTERNAL_ERROR when fails
    if (init_strings()){
        return INTERNAL_ERROR;
    }
    if ((token = get_token(&attr)) == LEX_ERROR){
        clear_str(&attr);
        return LEX_ERROR;
    }
    result = program();
    return result;
}
