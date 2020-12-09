/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 */


#include "scanner.h"
#include "str.h"
#include "error_codes.h"

FILE *source;

void set_source_file(FILE *file) {
    source = file;
}

int get_token(string *attr, int is_expression){
    tState state = start;
    char c;
    clear_str(attr);

    while (TRUE) {
        c = getc(source);
        switch (state) {
            case start:
                if (is_whitespace(c)) {
                    continue;
                }
                else if (is_EOL(c)) {
                    return EOL;
                }
                else if (is_alpha(c)) {
                    add_to_string(attr, c);
                    state = unknown_identifier;
                }
                else if (c == '_') {
                    add_to_string(attr, c);
                    state = tId;
                }
                else if (is_num(c)) {
                    add_to_string(attr, c);
                    state = tInteger;
                }
                else if (c == '"') {
                    if (is_expression){
                        add_to_string(attr, c);
                    }
                    state = string_start;
                }
                else if (c == '+') {
                    add_to_string(attr, c);
                    return tPlus;
                }
                else if (c == '-') {
                    add_to_string(attr, c);
                    return tMinus;
                }
                else if (c == '*') {
                    add_to_string(attr, c);
                    return tMultiply;
                }
                else if (c == '/') {
                    add_to_string(attr, c);
                    state = tDivide;
                }
                else if (c == ':') {
                    add_to_string(attr, c);
                    state = tDeclaration;
                }
                else if (c == '!') {
                    add_to_string(attr, c);
                    state = tNotEqual;
                }
                else if (c == '=') {
                    add_to_string(attr, c);
                    state = tAssignment;
                }
                else if (c == '<') {
                    add_to_string(attr, c);
                    state = tSmallerThan;
                }
                else if (c == '>') {
                    add_to_string(attr, c);
                    state = tBiggerThan;
                }
                else if (c == ';') {
                    add_to_string(attr, c);
                    return tSemicolon;
                }
                else if (c == ',') {
                    add_to_string(attr, c);
                    return tComma;
                }
                else if (c == '(') {
                    add_to_string(attr, c);
                    return tOpeningSimpleBrace;
                }
                else if (c == ')') {
                    add_to_string(attr, c);
                    return tClosingSimpleBrace;
                }
                else if (c == '{') {
                    add_to_string(attr, c);
                    return tOpeningCurlyBrace;
                }
                else if (c == '}') {
                    add_to_string(attr, c);
                    return tClosingCurlyBrace;
                }
                else if (c == EOF) {
                    return tEOF;
                }
                break;
            case tBiggerThan:
                if (c == '=') {
                    add_to_string(attr, c);
                    return tBiggerOrEqual;
                }
                else if (!comparison_assumption(c)) {
                    return tLEX_ERROR;
                }
                ungetc(c, source);
                return tBiggerThan;
            case tSmallerThan:
                if (c == '=') {
                    add_to_string(attr, c);
                    return tSmallerOrEqual;
                }
                else if (!comparison_assumption(c)) {
                    return tLEX_ERROR;
                }
                ungetc(c, source);
                return tSmallerThan;
            case tAssignment:
                if (c == '=') {
                    add_to_string(attr, c);
                    state = tEqual;
                }
                else {
                    return tAssignment;
                }
                break;
            case tNotEqual:
                if (c == '=') {
                    add_to_string(attr, c);
                    return tNotEqual;
                }
                return tLEX_ERROR;
            case tDeclaration:
                if (c == '=') {
                    add_to_string(attr, c);
                    return tDeclaration;
                }
                return tLEX_ERROR;
            case string_start:
                if (c == '"') {
                    if (is_expression){
                        add_to_string(attr, c);
                    }
                    return tString;
                }
                else if (c == '\\') {
                    add_to_string(attr, c);
                    state = string_special;
                }
                else {
                    add_to_string(attr, c);
                }
                break;
            case string_special:
                if (c == 'x') {
                    add_to_string(attr, c);
                    state = string_hex1;
                }
                else if (c == 'n' ||
                         c == 't' ||
                         c == '"' ||
                         c == '\\') {
                    add_to_string(attr, c);
                    state = string_start;
                }
                else {
                    return tLEX_ERROR;
                }
                break;
            case string_hex1:
                if (is_hex_char(c)) {
                    add_to_string(attr, c);
                    state = string_hex2;
                }
                else {
                    return tLEX_ERROR;
                }
                break;
            case string_hex2:
                if (is_hex_char(c)) {
                    add_to_string(attr, c);
                    state = string_start;
                }
                else {
                    return tLEX_ERROR;
                }
                break;
            case tInteger:
                if (is_num(c)) {
                    add_to_string(attr, c);
                }
                else if (c == '.') {
                    add_to_string(attr, c);
                    state = float_point;
                }
                else if (c == 'e' || c == 'E') {
                    add_to_string(attr, c);
                    state = float_exponent;
                }
                else if (is_alpha(c)) {
                    return tLEX_ERROR;
                }
                else {
                    ungetc(c, source);
                    return tInteger;
                }
                break;
            case float_point:
                // 25. 22222.
                if (is_num(c)) {
                    add_to_string(attr, c);
                    state = float_point_number;
                }
                else {
                    ungetc(c, source);
                    return tLEX_ERROR;
                }
                break;
            case float_point_number:
                // 25.2 222.2
                if (is_num(c)) {
                    add_to_string(attr, c);
                }
                else if (c == 'e' || c == 'E') {
                    add_to_string(attr, c);
                    state = float_exponent;
                }
                else {
                    ungetc(c, source);
                    return tFloat;
                }
                break;
            case float_exponent:
                // 25.2E 25E
                if (is_num(c)) {
                    add_to_string(attr, c);
                    state = float_exponent_number;
                }
                else if (c == '+' || c == '-') {
                    add_to_string(attr, c);
                    state = float_singed_exponent;
                }
                else {
                    ungetc(c, source);
                    return tLEX_ERROR;
                }
                break;
            case float_exponent_number:
                // 25E2 25.2E2
                if (is_num(c)) {
                    add_to_string(attr, c);
                }
                else if (!number_ending(c)) {
                    return tLEX_ERROR;
                }
                else {
                    ungetc(c, source);
                    return tFloat;
                }
                break;
            case float_singed_exponent:
                if (is_num(c)) {
                    add_to_string(attr, c);
                    state = tFloat;
                }
                else {
                    ungetc(c, source);
                    return tLEX_ERROR;
                }
                break;
            case tFloat:
                if (is_num(c)) {
                    add_to_string(attr, c);
                }
                else {
                    ungetc(c, source);
                    return tFloat;
                }
                break;
            case unknown_identifier:
                if (c == '_') {
                    add_to_string(attr, c);
                    state = tId;
                }
                else if (is_alphanumeric(c)) {
                    add_to_string(attr, c);
                }
                else {
                    ungetc(c, source);
                    if (is_keyword(attr)) {
                        return tKeyword;
                    }
                    else if (is_built_in_func(attr)) {
                        return tBuiltIn;
                    }
                    return tId;
                }
                break;
            case tId:
                if (is_alphanumeric(c) || c == '_') {
                    add_to_string(attr, c);
                }
                else{
                    ungetc(c, source);
                    return tId;
                }
                break;
            case tDivide:
                if (c == '*') {
                    state = block_comment;
                }
                else if (c == '/') {
                    state = line_comment;
                }
                else {
                    ungetc(c, source);
                    return tDivide;
                }
                break;
            case line_comment:
                if (c == EOF) {
                    state = start;
                    clear_str(attr);
                }
                else if (c == '\n'){
                    return EOL;
                }
                break;
            case block_comment:
                add_to_string(attr, c);
                if (c == '*') {
                    state = block_comment_ending;
                }
                break;
            case block_comment_ending:
                add_to_string(attr, c);
                if (c == '/') {
                    clear_str(attr);
                    state = start;
                }
                else {
                    state = block_comment;
                }
                break;
            case tEqual:
                if (!comparison_assumption(c)) {
                    return tLEX_ERROR;
                }
                ungetc(c, source);
                return tEqual;
            default:
                continue;
        }
    }
}

void reset_file_pointer(){
    fseek(source, 0, SEEK_SET);
}
