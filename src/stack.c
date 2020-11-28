//
// Created by xkvasn14 on 16.11.20.
//

#include "stack.h"
//#include "instr.h"

void StackInit(ptrStack * s)
{
    s->top_stack = NULL;
    s->top_stack->value = NULL;
    s->top_stack->next_value = NULL;
}

void StackPush  (ptrStack* s, void* value)
{
    tStack* new_item;
    new_item = malloc(sizeof(struct stack_struct));
    if (new_item == NULL)
    {
        // chyba alokace 99
        return;
    }
    new_item->value = value;
    new_item->next_value = s->top_stack;
    s->top_stack = new_item;
}

void StackPop   (ptrStack* s)
{
    tStack* tmp;
    tmp = s->top_stack;
    s->top_stack = s->top_stack->next_value;
    free(tmp);
}

bool StackIsEmpty (ptrStack* s)
{
    return (s->top_stack == NULL);
}

void StackDispose (ptrStack* s)
{
    while(s->top_stack)
    {
        tStack* tmp;
        tmp = s->top_stack;
        s->top_stack = s->top_stack->next_value;
        free(tmp);
    }
}

ptrStack* StackTop(ptrStack* s){
    return (s->top_stack);
}