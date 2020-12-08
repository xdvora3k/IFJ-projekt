/*
 * IFJ project 2020
 * Author: xkuzel08, Marie Kuzelova
 */

#ifndef IFJ_PROJEKT_EXPRESSION_PARSER_H
#define IFJ_PROJEKT_EXPRESSION_PARSER_H

#include <stdio.h>
#include "ilist.h"


tLinkedList* get_tokens(string *s, tLinkedList *List);
void get_tokenExp(tToken* token, string *input, int startIndex, tLinkedList *L);
void _save_to_token(tToken *token, string *string, tState type, int end_index, tLinkedList *list);
tVarDataType getDataType(tToken *token, tLinkedList *linkedL);
#endif
