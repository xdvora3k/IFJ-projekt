#ifndef IFJ_PROJEKT_EXPRESSION_PARSER_H
#define IFJ_PROJEKT_EXPRESSION_PARSER_H

#include <stdio.h>
#include "ilist.h"


tLinkedList* get_tokens(string *s);
void get_tokenExp(tToken* token, string *input, int startIndex);
void _save_to_token(tToken *token, string *string, tState type, int end_index, tLinkedList *list);
tVarDataType getDataType(tToken *token, tLinkedList *linkedL);
#endif
