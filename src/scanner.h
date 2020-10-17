#ifndef IFJ_PROJEKT_SCANNER_H
#define IFJ_PROJEKT_SCANNER_H

#include <stdio.h>
#include "str.h"

void set_source_file(FILE *file);
int get_token(string *attr);

typedef enum{
    start,

    unknown_identifier,
    tId,
    tKeyword,
    tBuiltIn,

    tInteger,

    double_point,
    double_point_number,
    double_exponent,
    double_exponent_number,
    double_singed_exponent,
    tDouble,

    string_start,
    string_special,
    string_hex1,
    string_hex2,
    tString,

    tPlus,
    tMinus,
    tMultiply,
    tDivide,

    tDeclaration,
    tAssignment,

    tEqual,
    tNotEqual,
    tBiggerThan,
    tSmallerThan,
    tBiggerOrEqual,
    tSmallerOrEqual,

    tComma,
    tSemicolon,
    tOpeningSimpleBrace,
    tClosingSimpleBrace,
    tOpeningCurlyBrace,
    tClosingCurlyBrace,

    line_comment,
    line_or_block_comment,
    block_comment,
    block_comment_ending,

    end_of_line,
    end_of_file,

    EOL,

    LEX_ERROR = 1000

}tState;

#endif
