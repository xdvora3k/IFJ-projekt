/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 */

#ifndef IFJ_PROJEKT_SCANNER_H
#define IFJ_PROJEKT_SCANNER_H

#include <stdio.h>
#include "str.h"
#include "error_codes.h"

void set_source_file(FILE *file);
int get_token(string *attr);
void reset_file_pointer();

typedef enum{
    start,

    unknown_identifier,
    tId,
    tKeyword,
    tBuiltIn, //4

    tInteger,

    float_point,
    float_point_number,
    float_exponent,
    float_exponent_number, //9
    float_singed_exponent,
    tFloat,

    string_start,
    string_special,
    string_hex1, //14
    string_hex2,
    tString,

    tPlus,
    tMinus,
    tMultiply, //19
    tDivide,

    tDeclaration,
    tAssignment,

    tEqual,
    tNotEqual, //24
    tBiggerThan,
    tSmallerThan,
    tBiggerOrEqual,
    tSmallerOrEqual,

    tComma, //29
    tSemicolon,
    tOpeningSimpleBrace,
    tClosingSimpleBrace,
    tOpeningCurlyBrace,
    tClosingCurlyBrace, //34

    line_comment,
    line_or_block_comment,
    block_comment,
    block_comment_ending,

    end_of_line, //39
    end_of_file,

    EOL,
    tEOF,
    tLEX_ERROR,

    // ------------------------- For parser
    sFunc, //44

    sFor,

    sIf,
    sElse,

    sReturn,

    sDeclare, //49
    sAssign,

    sEnd,

    // Datatypes
    sString,
    sInt,
    sFloat64,

    //Precedence table states
    tExprOpen, //55
    tExprClose,
    tExprEq,
    tExprPlaceholder

}tState;

#endif
