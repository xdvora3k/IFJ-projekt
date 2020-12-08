/*
 * IFJ project 2020
 * Author: xkuzel08, Marie Kuzelova
 */

#include "../src/ilist.h"
#include <string.h>
#include <stdio.h>
#include "../src/str.h"
#include "../src/error_codes.h"
#include "../src/symtable.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#define SYMTABLE_TEST_ERROR -1

int main(){ 

tSymtable SymTable;
SymTableInit(&SymTable);

printf("\n*****TEST STARTED*****\n");

tDataVariable *datatypeTest;

tBSTNodePtr test;
BSTInit(&test);
printf("\n Init TEST\n");
printf("~~~~~~~~~~~~~~~~~~~~~~~~~\n");
printf("~~~~~~~~~~~~~~~~~~~~~~~~~\n");

if(test == NULL){
    printf("Testing tree successfully initialized. \n \n");
} else {
    printf("Testing tree initialization failed. \n");
    return SYMTABLE_TEST_ERROR;
}

/* ---------------- inserting keys --------------*/
test = SymTableInsertVariable(&SymTable, "d");

printf("Test 1: Insert first variable \n");
printf("Expected Left Node is NOT set: %s \n", ((void*)(&SymTable)->root->LPtr) == NULL ? "TRUE" : "FALSE");      //expect nil
printf("Expected Right Node is NOT set: %s \n", ((void*)(&SymTable)->root->RPtr) == NULL ? "TRUE" : "FALSE");
printf("Expected Key value: d, actual value: %s \n", (&SymTable)->root->Key);
datatypeTest = (&SymTable)->root->Content;
printf("Expected data type value: -1, actual value: %d \n", datatypeTest->dataType);

printf("\nTest 2: Insert second variable \n");
test = SymTableInsertVariable(&SymTable, "u");
printf("Expected Left Node is NOT set: %s \n", ((void*)(&SymTable)->root->LPtr) == NULL ? "TRUE" : "FALSE"); 
printf("Expected Right Node is set: %s \n", ((void*)(&SymTable)->root->RPtr) != NULL ? "TRUE" : "FALSE");
printf("Expected Key value: u, actual value: %s \n", (&SymTable)->root->RPtr->Key);
datatypeTest = (&SymTable)->root->Content;
printf("Expected data type value: -1, actual value: %d \n", datatypeTest->dataType);

printf("\nTest 3: Insert 3. variable \n");
test = SymTableInsertVariable(&SymTable, "b");
printf("Expected Left Node is set: %s \n", ((void*)(&SymTable)->root->LPtr) != NULL ? "TRUE" : "FALSE");
printf("Expected Right Node is set: %s \n", ((void*)(&SymTable)->root->RPtr) != NULL ? "TRUE" : "FALSE");
printf("Expected Key value: b, actual value: %s \n", (&SymTable)->root->LPtr->Key);
datatypeTest = (&SymTable)->root->Content;
printf("Expected data type value: -1, actual value: %d \n", datatypeTest->dataType);

printf("\nTest 4: Insert 4. variable \n");
test = SymTableInsertVariable(&SymTable, "a");
printf("Expected Left Node is set: %s \n", ((void*)(&SymTable)->root->LPtr->LPtr) != NULL ? "TRUE" : "FALSE");
printf("Expected Right Node is NOT set: %s \n", ((void*)(&SymTable)->root->RPtr->RPtr) == NULL ? "TRUE" : "FALSE");
printf("Expected Key value: a, actual value: %s \n", (&SymTable)->root->LPtr->LPtr->Key);
datatypeTest = (&SymTable)->root->Content;
printf("Expected data type value: -1, actual value: %d \n", datatypeTest->dataType);

printf("\nTest 5: Insert 5. variable \n");
test = SymTableInsertVariable(&SymTable, "k");
printf("Expected Left Node is set: %s \n", ((void*)(&SymTable)->root->RPtr->LPtr) != NULL ? "TRUE" : "FALSE");
printf("Expected Right Node is NOT set: %s \n", ((void*)(&SymTable)->root->RPtr->RPtr) == NULL ? "TRUE" : "FALSE");
printf("Expected Key value: k, actual value: %s \n", (&SymTable)->root->RPtr->LPtr->Key);
datatypeTest = (&SymTable)->root->Content;
printf("Expected data type value: -1, actual value: %d \n", datatypeTest->dataType);

printf("\nTest 6: Insert 6. variable \n");
test = SymTableInsertVariable(&SymTable, "z");
printf("Expected Left Node is set: %s \n", ((void*)(&SymTable)->root->LPtr->LPtr) != NULL ? "TRUE" : "FALSE");
printf("Expected Right Node is set: %s \n", ((void*)(&SymTable)->root->RPtr->RPtr) != NULL ? "TRUE" : "FALSE");
printf("Expected Key value: z, actual value: %s \n", (&SymTable)->root->RPtr->RPtr->Key);
datatypeTest = (&SymTable)->root->Content;
printf("Expected data type value: -1, actual value: %d \n", datatypeTest->dataType);


printf("\n\nFinal tree structure:\n\t (root: %s)\n", (&SymTable)->root->Key);

printf("\tlptr:  rptr:\n");
printf("\t %s \t%s\n", (&SymTable)->root->LPtr->Key, (&SymTable)->root->RPtr->Key);

printf("lptr:  rptr:\tlptr:  rptr:\n");
printf(" %s \t%s\t %s\t%s\n", (&SymTable)->root->LPtr->LPtr->Key, "null", (&SymTable)->root->RPtr->LPtr->Key, (&SymTable)->root->RPtr->RPtr->Key);

 
printf("~~~~~~~~~~~~~~~~~~~~~~~~~\n");

/*------Sem error - test ----*/
/*
printf(vkladani uz existujucuho klice);
result = init_string(&key);
result = adds_to_string(&key, "a");
test = SymTableInsertVariable(&SymTable, &key);
printf("Sem error\n");
printf("\nleft for k %p\n", (void*)(&SymTable)->root->LPtr);      
printf("right %p\n", (void*)(&SymTable)->root->RPtr);*/


/*------------------------------searching keys------------------------*/
printf("\nSearch in testing tree\n\n");
printf("~~~~~~~~~~~~~~~~~~~~~~~~~\n");
tBSTNodePtr emptyTree;
tSymtable freeSymtable;

SymTableInit(&freeSymtable);

emptyTree = SymTableSearch(&freeSymtable, "a");
if(emptyTree != NULL){
    printf("SYMTABLE_TEST_ERROR\n");
    return SYMTABLE_TEST_ERROR;
}

emptyTree = SymTableInsertVariable(&freeSymtable, "u");
printf("\n SearchTest: searching key that exists\n");
emptyTree = SymTableSearch(&freeSymtable, "u");
if(emptyTree != NULL){  
    printf("found: ''%s'' expect: ''u''\n", emptyTree->Key);

} else
{
    printf("nothing found\n");
    return SYMTABLE_TEST_ERROR;
}
printf("\n SearchTest: searching key that not exist\n");

emptyTree = SymTableSearch(&freeSymtable, "a");
if(emptyTree != NULL){
    printf("found: ''a''\n");
    return SYMTABLE_TEST_ERROR;

} else
{
    printf("nothing found \n");
}

printf("\n SearchTest: searching key that exists\n");
emptyTree = SymTableInsertVariable(&freeSymtable, "o");
emptyTree = SymTableSearch(&freeSymtable, "o");
if(emptyTree != NULL){
    printf("found: ''%s'', expect: ''o''\n", emptyTree->Key);
    
} else
{
    printf("nothing found\n");
    return SYMTABLE_TEST_ERROR;
}
printf("\n SearchTest: searching key that exists\n");
emptyTree = SymTableInsertVariable(&freeSymtable, "w");
emptyTree = SymTableSearch(&freeSymtable, "w");
if(emptyTree != NULL){
    printf("found: ''%s'' expect: ''w''\n", emptyTree->Key);
} else{
    printf(" ''%s'' not found \n", "w");
    return SYMTABLE_TEST_ERROR;
}
printf("\n SearchTest: searching key that exists\n");
emptyTree = SymTableInsertVariable(&freeSymtable, "c");
emptyTree = SymTableSearch(&freeSymtable, "c");
if(emptyTree != NULL){  
    printf("found: ''%s'', expect: ''c''\n", emptyTree->Key);

} else
{
    printf("ERROR\n");
    return SYMTABLE_TEST_ERROR;
}
printf("~~~~~~~~~~~~~~~~~~~~~~~~~\n");
emptyTree = SymTableInsertFunction(&freeSymtable, "function");
tDataFunction *functionTest = (&SymTable)->root->Content;
printf(" insertFunction declared: %s, defined: %s, params \n ", functionTest->declared ? "TRUE" : "FALSE", functionTest->defined ? "TRUE" : "FALSE");
printf("params %d\n", functionTest->params.length);


printf("pointer %p\n", (void *)(&freeSymtable)->root);
printf("~~~~~~~~~~~~~~~~~~~~~~~~~\n");
tSymtable testNull;
SymTableInit(&testNull);

if((&testNull)->root != NULL){
    return SYMTABLE_TEST_ERROR;
}
printf("\n Dispose tree\n");
printf("~~~~~~~~~~~~~~~~~~~~~~~~~\n");

printf("\nSymtable with 0 elements\n");
printf("Expected root pointer is NOT set: %s\n", ((void *)(&testNull)->root == NULL ? "TRUE" : "FALSE"));
SymTableDispose((&testNull)); 
printf("Expected root pointer is NOT set: %s (after dispose)\n", (void *)(&testNull)->root == NULL ? "TRUE" : "FALSE");


printf("\nSymtable with 1 element\n");
SymTableInsertVariable(&testNull, "e");
printf("Expected root pointer is set: %s (key: %s)\n", ((void *)(&testNull)->root != NULL ? "TRUE" : "FALSE"), (&testNull)->root->Key);
SymTableDispose(&testNull);
printf("Expected root pointer is NOT set: %s (after dispose)\n", (void *)(&testNull)->root == NULL ? "TRUE" : "FALSE");


printf("\nSymtable with more elements\n");
printf("Expected root pointer is set: %s (key: %s)\n", ((void *)(&freeSymtable)->root != NULL ? "TRUE" : "FALSE"), (&freeSymtable)->root->Key);
SymTableDispose((&freeSymtable));
printf("Expected root pointer is NOT set: %s (after dispose)\n", (void *)(&freeSymtable)->root  == NULL ? "TRUE" : "FALSE");
/*----ruseni celeho symtable***/
if((&freeSymtable)->root != NULL){
    printf("nonEmpty\n");
    return SYMTABLE_TEST_ERROR;
}

printf("~~~~~~~~~~~~~~~~~~~~~~~~~\n");
printf("\n Delete node\n");

SymTableDelete(&SymTable, "z");
printf("Key ''z'' is deleted: %s\n", (void *)(&SymTable)->root->RPtr->RPtr == NULL ? "TRUE" : "FALSE");

SymTableDelete(&SymTable, "a");
printf("Key ''a'' is deleted: %s\n", (void *)(&SymTable)->root->LPtr->LPtr == NULL ? "TRUE" : "FALSE");



printf("\n*****TEST ENDED SUCESSFULLY*****\n");
return 0;
}

