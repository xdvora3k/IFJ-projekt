//
// Created by xkvasn14 on 16.11.20.
//

#ifndef IFJ_PROJEKT_STACK_H
#define IFJ_PROJEKT_STACK_H

#include <stdio.h>

typedef struct stack_struct{
    struct stack_struct* next_value;
    void* value;
}tStack;

typedef struct {
    tStack* top_stack;
}ptrStack;

void StackInit  (ptrStack* s);
void StackPush  (ptrStack* s, void* value);
void StackPop   (ptrStack* s);
int StackIsEmpty (ptrStack* s);
void StackDispose (ptrStack* s);
tStack* StackTop(ptrStack* s);

#endif //IFJ_PROJEKT_STACK_H