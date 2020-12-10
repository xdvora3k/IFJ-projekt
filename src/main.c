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
    tLinkedList instructions_list;
    StrLLInit(&instructions_list);
    InsertBuiltInFuncs(&SymTable);
    Print_BuiltIn_Functions(&instructions_list);

    parse(&SymTable, &InstrList, &FinalList, &instructions_list);
}
