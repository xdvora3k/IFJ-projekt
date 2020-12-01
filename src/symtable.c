/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 */

#include "symtable.h"

void BSTInit(tBSTNodePtr *RootPtr){
    *RootPtr = NULL;
}

tBSTNodePtr BSTSearch(tBSTNodePtr RootPtr, char *K){
    if (!RootPtr){
        return NULL;
    }
    if (strcmp(K,(RootPtr)->Key) == 0){
        return RootPtr;
    }
    else if (strcmp(K, (RootPtr)->Key) > 0){
        return BSTSearch(RootPtr->RPtr, K);
    }
    else {
        return BSTSearch(RootPtr->LPtr, K);
    }
}

tBSTNodePtr BSTCreateNode(char* K, void* Data){
    tBSTNodePtr new_node = (tBSTNodePtr) malloc(sizeof(struct tBSTNode));
    new_node->Content = Data;
    new_node->LPtr = NULL;
    new_node->RPtr = NULL;
    char *key = malloc(sizeof(K));
    strcpy(key, K);
    new_node->Key = key;
    return new_node;
}

tBSTNodePtr BSTInsert(tBSTNodePtr* RootPtr, char* K, void* Data){
    if (!(*RootPtr)){
        *RootPtr = BSTCreateNode(K, Data);
        return *RootPtr;
    }
    // If already exists, Semantic error
    if (strcmp(K,(*RootPtr)->Key) == 0){  //K == (*RootPtr)->Key
        exit(SEM_ERROR);
    }
    else if (strcmp(K, (*RootPtr)->Key) > 0){ //K > (*RootPtr)->Key
        if ((*RootPtr)->RPtr){
            BSTInsert(&(*RootPtr)->RPtr, K, Data);
        }
        else {
            (*RootPtr)->RPtr = BSTCreateNode(K, Data);
            return (*RootPtr)->RPtr;
        }
    }
    else {
        if ((*RootPtr)->LPtr){
            BSTInsert(&(*RootPtr)->LPtr, K, Data);
        }
        else {
            (*RootPtr)->LPtr = BSTCreateNode(K, Data);
            return (*RootPtr)->LPtr;
        }
    }
    return NULL;
}

void ReplaceByRightmost(tBSTNodePtr PtrReplaced, tBSTNodePtr *RootPtr){
    if (!*RootPtr){
        return;
    }

    if (!(*RootPtr)->LPtr && !(*RootPtr)->RPtr){
        (*RootPtr)->RPtr = NULL;
        PtrReplaced->Content = (*RootPtr)->Content;
        PtrReplaced->Key = (*RootPtr)->Key;
        *RootPtr = NULL;

        free(*RootPtr);
    }
    else if (!(*RootPtr)->RPtr){
        tBSTNodePtr to_delete = *RootPtr;
        PtrReplaced->Content = (*RootPtr)->Content;
        PtrReplaced->Key = (*RootPtr)->Key;
        PtrReplaced->LPtr = (*RootPtr)->LPtr;

        free(&to_delete->Content);
        free(to_delete);
    }
    else {
        ReplaceByRightmost(PtrReplaced, &(*RootPtr)->RPtr);
    }
}

void BSTDelete(tBSTNodePtr *RootPtr, char *K){
    if (!*RootPtr){
        return;
    }

    if (strcmp(K, (*RootPtr)->Key) > 0){
        BSTDelete(&(*RootPtr)->RPtr, K);
    }
    else if (strcmp(K, (*RootPtr)->Key) < 0){
        BSTDelete(&(*RootPtr)->LPtr, K);
    }
    else {
        if (!(*RootPtr)->LPtr && !(*RootPtr)->RPtr){
            //free(&(*RootPtr)->Content);
            free(*RootPtr);
            *RootPtr = NULL;
            return;
        }
        else if (!(*RootPtr)->LPtr){
            tBSTNodePtr right = (*RootPtr)->RPtr;
            //free(&(*RootPtr)->Content);
            free(*RootPtr);
            *RootPtr = right;
            return;
        }
        else if (!(*RootPtr)->RPtr){
            tBSTNodePtr left = (*RootPtr)->LPtr;
            //free(&(*RootPtr)->Content);
            free(*RootPtr);
            *RootPtr = left;
            return;
        }
        else {
            ReplaceByRightmost(*RootPtr, &(*RootPtr)->LPtr);
        }
    }
}

void BSTDispose(tBSTNodePtr *RootPtr){
    if (!*RootPtr){
        return;
    }

    if ((*RootPtr)->RPtr){
        BSTDispose(&(*RootPtr)->RPtr);
    }
    if ((*RootPtr)->LPtr){
        BSTDispose(&(*RootPtr)->LPtr);
    }

    free((*RootPtr)->Content);
    free(*RootPtr);
    *RootPtr = NULL;
}

/*
 * ------------------------------------------------------------------------------------
 */

void SymTableInit(tSymtable* SymTable){
    BSTInit(&(SymTable->root));
}

tBSTNodePtr SymTableInsertFunction(tSymtable* SymTable, char *key){
    tDataFunction* funcPtr = (tDataFunction*) malloc(sizeof(struct tDataFunction));
    string params;
    init_string(&params);

    funcPtr->params = params;
    funcPtr->declared = FALSE;
    funcPtr->defined = FALSE;

    return BSTInsert(&(SymTable->root), key, funcPtr);
}

tBSTNodePtr SymTableInsertVariable(tSymtable* SymTable, char *key){
    tDataVariable* varPtr = (tDataVariable*) malloc(sizeof(struct tDataVariable));
    varPtr->dataType = -1;
    return BSTInsert(&(SymTable->root), key, varPtr);
}

tBSTNodePtr SymTableSearch(tSymtable* SymTable, char* key){
    return BSTSearch(SymTable->root, key);
}

void SymTableDelete(tSymtable* SymTable, char *key){
    BSTDelete(&SymTable->root, key);
}

void SymTableDispose(tSymtable* Symtable){
    BSTDispose(&(Symtable->root));
}

// Built-in functions
void InsertBuiltInFuncs(tSymtable* SymTable){
    tDataFunction* func;

    // function INPUTI()(int, int)
    //---------------------------------
    tBSTNodePtr new_node_inputi = SymTableInsertFunction(SymTable, "inputi");
    func = (tDataFunction*)(new_node_inputi->Content);
    func->defined = TRUE;
    func->declared = TRUE;
    init_string(&(func->returnType));
    adds_to_string(&(func->returnType),"ii");
    //---------------------------------

    // function INPUTS()(string, int)
    //---------------------------------
    tBSTNodePtr new_node_inputs = SymTableInsertFunction(SymTable, "inputs");
    func = (tDataFunction*)(new_node_inputs->Content);
    func->declared = TRUE;
    func->defined = TRUE;
    init_string(&(func->returnType));
    adds_to_string(&(func->returnType),"si");
    //---------------------------------

    // function INPUTF()(float64, int)
    //---------------------------------
    tBSTNodePtr new_node_inputf = SymTableInsertFunction(SymTable, "inputf");
    func = (tDataFunction*)(new_node_inputf->Content);
    func->defined = TRUE;
    func->declared = TRUE;
    init_string(&(func->returnType));
    adds_to_string(&(func->returnType),"fi");
    //---------------------------------

    // function PRINT(...,...,...)
    //---------------------------------
    tBSTNodePtr new_node_print = SymTableInsertFunction(SymTable, "print");
    func = (tDataFunction*)(new_node_print->Content);
    func->defined = TRUE;
    func->declared = TRUE;
    //---------------------------------

    // function INT2FLOAT(i int)(float64)
    //---------------------------------
    tBSTNodePtr new_node_int2float = SymTableInsertFunction(SymTable, "int2float");
    func = (tDataFunction*)(new_node_int2float->Content);
    func->defined = TRUE;
    func->declared = TRUE;
    func->list_initialized = TRUE;

    StrLLInit(&(func->paramNames));
    StrLLInsert(&(func->paramNames), "i");

    init_string(&(func->params));
    add_to_string(&(func->params),'i');

    init_string(&(func->returnType));
    add_to_string(&(func->returnType),'f');
    //---------------------------------

    // function FLOAT2INT(f float64)(int)
    //---------------------------------
    tBSTNodePtr new_node_float2int = SymTableInsertFunction(SymTable, "float2int");
    func = (tDataFunction*)(new_node_float2int->Content);
    func->defined = TRUE;
    func->declared = TRUE;
    func->list_initialized = TRUE;

    StrLLInit(&(func->paramNames));
    StrLLInsert(&(func->paramNames), "f");

    init_string(&(func->params));
    add_to_string(&(func->params),'f');

    init_string(&(func->returnType));
    add_to_string(&(func->returnType),'i');
    //---------------------------------

    // function LEN(s string)(int)
    //---------------------------------
    tBSTNodePtr new_node_len = SymTableInsertFunction(SymTable, "len");
    func = (tDataFunction*)(new_node_len->Content);
    func->declared = TRUE;
    func->defined = TRUE;
    func->list_initialized = TRUE;

    StrLLInit(&(func->paramNames));
    StrLLInsert(&(func->paramNames),"s");

    init_string(&(func->params));
    add_to_string(&(func->params),'s');

    init_string(&(func->returnType));
    add_to_string(&(func->returnType),'i');
    //---------------------------------

    // function SUBSTR(s string, i int, n int)(string, int)
    //---------------------------------
    tBSTNodePtr new_node_substr = SymTableInsertFunction(SymTable, "substr");
    func = (tDataFunction*)(new_node_substr->Content);
    func->defined = TRUE;
    func->declared = TRUE;
    func->list_initialized = TRUE;

    StrLLInit(&(func->paramNames));
    StrLLInsert(&(func->paramNames),"s");
    StrLLInsert(&(func->paramNames),"i");
    StrLLInsert(&(func->paramNames),"n");

    init_string(&(func->params));
    adds_to_string(&(func->params),"sii");

    init_string(&(func->returnType));
    adds_to_string(&(func->returnType),"si");
    //---------------------------------

    // function ORD(s string, i int)(int, int)
    //---------------------------------
    tBSTNodePtr new_node_ord = SymTableInsertFunction(SymTable, "ord");
    func = (tDataFunction*)(new_node_ord->Content);
    func->declared = TRUE;
    func->defined = TRUE;
    func->list_initialized = TRUE;

    StrLLInit(&(func->paramNames));
    StrLLInsert(&(func->paramNames),"s");
    StrLLInsert(&(func->paramNames),"i");

    init_string(&(func->params));
    adds_to_string(&(func->params),"si");

    init_string(&(func->returnType));
    adds_to_string(&(func->returnType),"ii");
    //---------------------------------

    // function CHR(i int)(string, int)
    //---------------------------------
    tBSTNodePtr new_node_chr = SymTableInsertFunction(SymTable, "chr");
    func = (tDataFunction*)(new_node_chr->Content);
    func->declared = TRUE;
    func->defined = TRUE;
    func->list_initialized = TRUE;

    StrLLInit(&(func->paramNames));
    StrLLInsert(&(func->paramNames),"i");

    init_string(&(func->params));
    add_to_string(&(func->params),'i');

    init_string(&(func->returnType));
    adds_to_string(&(func->returnType),"si");
    //---------------------------------
}