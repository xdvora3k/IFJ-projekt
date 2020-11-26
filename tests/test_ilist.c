/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 */

#include "../src/ilist.h"

int main(){
    tLinkedList *list = malloc(sizeof(tListItem));
    StrLLInit(list);
    StrLLInsert(list, "a");
    printf("2-- %s\n", (char*) list->first->Content);
    StrLLInsert(list, "aaaaaaaaaaaaaaaaaa");
    printf("3-- %s\n", (char*) list->first->nextItem->Content);
    StrLLDeleteLast(list);
    printf("4-- %s\n", (char*) list->first->Content);
    fflush(stdout);
    StrLLDeleteLast(list);
    printf("5-- %p\n", (void*) list->first);

    StrLLInsert(list, "bbbb");
    StrLLInsert(list, "dddddddddddddddddd");
    StrLLDispose(list);
    printf("6-- %p\n", (void*) list->first);
}
