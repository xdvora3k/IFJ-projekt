/*
 * Author: xdvora3k, Jakub Dvorak
 */

#include <stdio.h>
#include "scanner.h"
#include "ilist.h"
#include "symtable.h"
#include "parser.h"

FRAME currentFrame = Frame_GF;

int main() {
    set_source_file(stdin);

    tSymtable SymTable;
    tLinkedList InstrList;
    SymTableInit(&SymTable);
    InstrLLInit(&InstrList);

    InsertBuiltInFuncs(&SymTable);

    parse(&SymTable, &InstrList);
}
