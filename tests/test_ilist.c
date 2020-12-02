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


    tExpressionList L;
    ExprLLInit(&L);
    ExprLLCreateNextNode(&L, IntType);
    tToken token;
    string hello;
    init_string(&hello);
    adds_to_string(&hello, "hello");
    token.text = &hello;
    token.type = tString;
    token.endIndex = hello.length;
    ExprLLInsertExprToLastNode(&L, &token, &token, &token, &token, expIdentity);
    printf("%s\n", L.first->first->rightOperand->text->str);
    printf("%d\n", L.first->first->rightOperand->text->length);
    clear_str(&hello);
    adds_to_string(&hello, "noHello");
    token.endIndex = 50;
    printf("%s\n", L.first->first->rightOperand->text->str);
    printf("%d\n", L.first->first->rightOperand->text->length);
    ExprLLInsertExprToLastNode(&L, &token, &token, &token, &token, expIdentity);
    printf("%s\n", L.first->first->next->rightOperand->text->str);
    printf("%d\n", L.first->first->next->rightOperand->text->length);

}
