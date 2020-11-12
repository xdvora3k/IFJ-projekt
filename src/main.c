/*
 * Author: xdvora3k, Jakub Dvorak
 */

#include <stdio.h>
#include "scanner.h"
#include "parser.h"

#define FILE_ERROR 5

int main(int argc, char *argv[]){
    if (argc == 1){
        printf("File is missing.\n");
        return FILE_ERROR;
    }

    FILE *f;
    if (!(f = fopen(argv[1], "r"))){
        printf("File could not be opened.\n");
        return FILE_ERROR;
    }
    set_source_file(f);

    tSymtable SymTable;
    tLinkedList InstrList;
    SymTableInit(&SymTable);
    InstrLLInit(&InstrList);

    parse(&SymTable, &InstrList);

    // TODO: add other functions
}
