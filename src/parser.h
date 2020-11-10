/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 */

#include "symtable.h"
#include "ilist.h"
#include "str.h"
#include "scanner.h"
#include "error_codes.h"
#include "string.h"
#include "error_codes.h"
#include <stdbool.h>

#ifndef IFJ_PROJEKT_PARSER_H
#define IFJ_PROJEKT_PARSER_H

int end_of_line();
int program();
int parse(tBSTNodePtr *RootPtr, tListOfInstr *Instr);

#endif
