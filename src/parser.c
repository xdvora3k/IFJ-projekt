/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 */

#include "parser.h"

tSymtable *GlobalRoot;
tListOfInstr *GlobalInstr;
tState token;
string attr;


void check_lex_error(){
    if (token == LEX_ERROR){
        exit(1);
    }
}

void check_syn_error(int error){
    if (error == SYN_ERROR){
        exit(2);
    }
}

void check_sem_error(int error){
    if (error == SEM_ERROR){
        exit(3);
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
    token = get_token(&attr);
    check_lex_error();
    else if (token == EOL){
        return sEnd;
    }
    return translate_scanner_states(&attr);
}

// skip EOL
// return 0 if everything goes right
int end_of_line(){
    token = get_adjusted_token();
    check_lex_error();
    else if (token == tKeyword){
        return 0;
    }
    else if (token == EOF){
        return SYN_ERROR;
    }
    return end_of_line();
}

// Private-like helper function
// Parse `i int` `x float64`
int _parse_param(tBSTNodePtr node){
    token = get_adjusted_token();
    check_lex_error();
    if (token != tId){
        return SYN_ERROR;
    }

    tDataFunction* Content = (tDataFunction*) node->Content;
    if (!Content->list_initialized){
        Content->list_initialized = TRUE;
        StrLLInit(Content->paramNames);
    }
    if (StrLLStringAlreadyOccupied(Content->paramNames, &attr.str)){
        return SEM_ERROR;
    }
    StrLLInsertFirst(Content->paramNames, &attr.str);
    token = get_adjusted_token();


}

// Private-like helper function
// Parse `(i int)(char, int)`
// Token current state ~= tId
int _parse_params(tBSTNodePtr node){
    token = get_adjusted_token();

    // Expecting `(`
    token = get_adjusted_token();
    check_lex_error();
    if (token != tOpeningSimpleBrace){
        return SYN_ERROR;
    }
    int error_state;
    while (token != tClosingSimpleBrace){
        error_state = _parse_param(node);
        check_lex_error();
        else if (error_state == SYN_ERROR){
            return SYN_ERROR;
        }
        else if (error_state == SEM_ERROR){
            return SEM_ERROR;
        }
    }
    return NO_ERROR;
}

int _parse_return_type(){

}

int _parse_return_types(){

}

int parse_func_header(){
    if (get_adjusted_token() == LEX_ERROR){
        return LEX_ERROR;
    }
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
    result = _parse_params(new_node);
    // TODO

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
