/*
 * Author: xdvora3k, Jakub Dvorak
 */

#include <stdio.h>
#include "scanner.h"
#include "ilist.h"
#include "symtable.h"


#define FILE_ERROR 5
tLinkedList *L;


int main(){
    set_source_file(stdin);

    tSymtable SymTable;
    tLinkedList InstrList;
    SymTableInit(&SymTable);
    InstrLLInit(&InstrList);

