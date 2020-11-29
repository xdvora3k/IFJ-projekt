#include "scanner.h"
#include "error_codes.h"
#include "ilist.h"
#include <stdio.h>


typedef struct{
    string text;
    tState type;
    int endIndex;
} tToken;

tLinkedList get_tokens(string *s);
tToken get_tokenExp(string *input, int startIndex);