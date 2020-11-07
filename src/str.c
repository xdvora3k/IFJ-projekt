#include "str.h"
#include "error_codes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int init_string(string *string){
    string->str = (char*) malloc(BASE_STRING_LENGTH * sizeof(char));
    if (!string->str){
        return INTERNAL_ERROR;
    }
    string->str[0] = '\0';
    string->allocSize = BASE_STRING_LENGTH;
    string->length = 0;
    return NO_ERROR;
}

int add_to_string(string *string, char c){
    if (string->allocSize == string->length + 1){
        if (!(string->str = (char*) realloc(string->str, string->allocSize + BASE_STRING_LENGTH * sizeof(char)))){
            return INTERNAL_ERROR;
        }
        string->allocSize += BASE_STRING_LENGTH;
    }
    string->str[string->length] = c;
    string->length += 1;

    return NO_ERROR;
}

int adds_to_string(string *string, char* S){
    for (int i = 0; S[i] ; i++){
        if (add_to_string(string, S[i]) == NO_ERROR){
            continue;
        }
    }
    return INTERNAL_ERROR;
}

int is_whitespace(char c){
    if (c == ' ' || c == '\t'){
        return TRUE;
    }
    return FALSE;
}

int is_EOL(char c){
    if (c == '\n'){
        return TRUE;
    }
    return FALSE;
}

int is_num(char c){
    if (c < 58 && c > 47){
        return TRUE;
    }
    return FALSE;
}

int is_alpha(char c){
    if ((c < 91 && c > 64) || (c < 123 && c > 96)){
        return TRUE;
    }
    return FALSE;
}

int is_alphanumeric(char c){
    if (is_alpha(c) || is_num(c)){
        return TRUE;
    }
    return FALSE;
}

int is_hex_char(char c){
    if (is_num(c) || ((c < 71 && c > 64) || (c < 103 && c > 96))){
        return TRUE;
    }
    return FALSE;
}

int is_built_in_func(string *string){
    char *build_in_funcs[] = {"inputs\0", "inputi\0", "inputf\0",
                              "print\0", "int2float\0", "float2int\0",
                              "len\0", "substr\0", "ord\0", "chr\0"};
    int bi_size = sizeof(build_in_funcs) / sizeof(build_in_funcs[0]);
    for (int i = 0; i < bi_size; i++){
        if (!strcmp(string->str, build_in_funcs[i])){
            return TRUE;
        }
    }
    return FALSE;
}

int is_keyword(string *string){
    char *keywords[] = {"else\0", "float64\0", "for\0", "func\0", "if\0",
                        "int\0", "return\0", "string\0",
                        "main\0", "package\0"};
    int kw_size = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < kw_size; i++){
        if (!strcmp(string->str, keywords[i])){
            return TRUE;
        }
    }
    return FALSE;
}

int is_datatype(string *string){
    char *datatypes[] = {"int\0", "float64\0", "string\0"};
    int datasize = sizeof(datatypes) / sizeof(datatypes[0]);
    for (int i = 0; i < datasize; i++){
        if (!strcmp(string->str, datatypes[i])){
            return TRUE;
        }
    }
    return FALSE;
}

int is_reserved(string *string){
    if (is_keyword(string) || is_built_in_func(string)){
        return TRUE;
    }
    return FALSE;
}

void clear_str(string *string){
    for (int i = 0; i < string->length; i++){
        string->str[i] = '\0';
    }
    string->length = 0;
}

int comparison_assumption(char c){
    if (is_alphanumeric(c) || c == '"' || c == '(' || c == '!' || c == ' ' || c == '\n' || c == EOF){
        return TRUE;
    }
    return FALSE;
}

int number_ending(char c){
    if (c == ';' || c == ' ' || c == '\n' || c == EOF){
        return TRUE;
    }
    return FALSE;
}
