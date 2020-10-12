/*
 * Author: xdvora3k, Jakub Dvorak
 */

#include <stdio.h>

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

    // TODO: add other functions
}