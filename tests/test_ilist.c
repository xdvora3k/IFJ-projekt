/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 */

#include "../src/ilist.h"
#include <string.h>
#include "../src/str.h"

int main(){
    tLinkedList List;
    StrLLInit(&List);
    int result;
    printf("Starting test.\n");
    string key;
    result = init_string(&key);
    result = adds_to_string(&key, "content");
    StrLLInsert(&List, &key);
    string *Content = ((string*) List.first->Content);
    printf("1.Expecting: %s, Actual: %s\n\n", "content", Content->str);
    if (strcmp(Content->str, "content")){
        return 1;
    }
    result = StrLLStringAlreadyOccupied(&List, key.str);
    if (!result){
        return 1;
    }
    result = add_to_string(&key, '1');
    StrLLInsert(&List, &key);
    Content = ((string*) List.first->nextItem->Content);
    printf("2.Expecting: %s, Actual: %s\n\n", "content1", Content->str);
    if (strcmp(Content->str, "content1")){
        return 1;
    }

    result = adds_to_string(&key, "234567890");
    StrLLInsert(&List, &key);
    Content = ((string*) List.first->nextItem->nextItem->Content);
    printf("3.Expecting: %s, Actual: %s\n\n", "content1234567890", Content->str);
    if (strcmp(Content->str, "content1234567890")){
        return 1;
    }
}
