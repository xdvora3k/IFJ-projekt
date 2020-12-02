#include "scanner.h"
#include "error_codes.h"
#include "ilist.h"
#include <stdio.h>


typedef struct{
    string text;
    tState type;
    int endIndex;
} tToken;

tLinkedList* get_tokens(string *s);
void get_tokenExp(tToken* token, string *input, int startIndex);