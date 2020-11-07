#ifndef IFJ_PROJEKT_STR_H
#define IFJ_PROJEKT_STR_H

#include <stdio.h>
#include <stdbool.h>

typedef struct string{
    char* str;
    int length;
    int allocSize;
} string;

#define BASE_STRING_LENGTH 10
#define TRUE 1
#define FALSE 0

int init_string(string *string);
int add_to_string(string *string, char c);
int adds_to_string(string *string, char* S);

int is_num(char c);
int is_whitespace(char c);
int is_EOL(char c);
int is_alpha(char c);
int is_alphanumeric(char c);
int is_hex_char(char c);

int is_built_in_func(string *string);
int is_keyword(string *string);
int is_datatype(string *string);
int is_reserved(string *string);

void clear_str(string *string);

int comparison_assumption(char c);
int number_ending(char c);

#endif
