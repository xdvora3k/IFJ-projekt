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

    if (RootPtr->Key == K){
        return RootPtr;
    }
    else if (K > RootPtr->Key){
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
    new_node->Key = K;
    return new_node;
}

tBSTNodePtr BSTInsert(tBSTNodePtr* RootPtr, char* K, void* Data){
    if (!(*RootPtr)){
        *RootPtr = BSTCreateNode(K, Data);
        return *RootPtr;
    }

    // If already exists, Semantic error
    if (K == (*RootPtr)->Key){
        exit(SEM_ERROR);
    }
    else if (K > (*RootPtr)->Key){
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

    if (K > (*RootPtr)->Key){
        BSTDelete(&(*RootPtr)->RPtr, K);
    }
    else if (K < (*RootPtr)->Key){
        BSTDelete(&(*RootPtr)->LPtr, K);
    }
    else {
        if (!(*RootPtr)->LPtr && !(*RootPtr)->RPtr){
            free(&(*RootPtr)->Content);
            free(*RootPtr);
            *RootPtr = NULL;
            return;
        }
        else if (!(*RootPtr)->LPtr){
            tBSTNodePtr right = (*RootPtr)->RPtr;
            free(&(*RootPtr)->Content);
            free(*RootPtr);
            *RootPtr = right;
            return;
        }
        else if (!(*RootPtr)->RPtr){
            tBSTNodePtr left = (*RootPtr)->LPtr;
            free(&(*RootPtr)->Content);
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

    free(&(*RootPtr)->Content);
    free(*RootPtr);
    *RootPtr = NULL;
}

/*
 * ------------------------------------------------------------------------------------
 */

void SymTableInit(tSymtable* SymTable){
    BSTInit(&(SymTable->root));
}

tBSTNodePtr SymTableInsertFunction(tSymtable* SymTable, string *key){
    tDataFunction* funcPtr = (tDataFunction*) malloc(sizeof(struct tDataFunction));
    string params;
    init_string(&params);

    funcPtr->params = params;
    funcPtr->declared = FALSE;
    funcPtr->defined = FALSE;

    return BSTInsert(&(SymTable->root), key->str, funcPtr);
}

tBSTNodePtr SymTableInsertVariable(tSymtable* SymTable, string *key){
    tDataVariable* varPtr = (tDataVariable*) malloc(sizeof(struct tDataVariable));
    varPtr->dataType = -1;
    return BSTInsert(&(SymTable->root), key->str, varPtr);
}

tBSTNodePtr SymTableSearch(tSymtable* SymTable, string *key){
    return BSTSearch(SymTable->root, key->str);
}

void SymTableDelete(tSymtable* SymTable, string *key){
    BSTDelete(&SymTable->root, key->str);
}

void SymTableDispose(tSymtable* Symtable){
    BSTDispose(&(Symtable->root));
}
