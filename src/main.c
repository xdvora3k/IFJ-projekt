/*
 * Author: xdvora3k, Jakub Dvorak
 */

#include <stdio.h>
#include "scanner.h"
#include "ilist.h"
#include "symtable.h"
#include "parser.h"

int main() {
    set_source_file(stdin);

    tSymtable SymTable;
    tLinkedList InstrList;
    SymTableInit(&SymTable);
    InstrLLInit(&InstrList);
    tFinalList FinalList;
    VarLLInit(&FinalList);
    InsertBuiltInFuncs(&SymTable);
    define_built_in_variables(&FinalList);
    Print_BuiltIn_Functions();


    parse(&SymTable, &InstrList, &FinalList);
}
