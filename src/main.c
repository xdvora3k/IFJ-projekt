/*
 * Author: xdvora3k, Jakub Dvorak
 */

#include <stdio.h>
#include "scanner.h"
#include "symtable.h"

#define FILE_ERROR 5

int main(){
    set_source_file(stdin);

    tSymtable SymTable;
    tLinkedList InstrList;
    SymTableInit(&SymTable);
    InstrLLInit(&InstrList);

    //parse(&SymTable, &InstrList);

    // TODO: print final code
}
