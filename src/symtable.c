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

    if (K > RootPtr->Key){
        return BSTSearch(RootPtr->RPtr, K, Content);
    }
    else if (K < RootPtr->Key) {
        return BSTSearch(RootPtr->LPtr, K, Content);
    }
}

tBSTNodePtr BSTCreateNode(char* K, void* Data, tNodeDataType dataType){
    tBSTNodePtr new_node = (tBSTNodePtr) malloc(sizeof(struct tBSTNode));
    new_node->Content = Data;
    new_node->LPtr = NULL;
    new_node->RPtr = NULL;
    new_node->Key = K;
    return new_node;
}

tBSTNodePtr BSTInsert(tBSTNodePtr* RootPtr, char* K, void* Data, tNodeDataType dataType){
    if (!(*RootPtr)){
        *RootPtr = BSTCreateNode(L, Data, dataType);
        return *RootPtr;
    }

    if (K == (*RootPtr)->Key){
        (*RootPtr)->Content = Data;
        (*RootPtr)->Content.position = position;
    }
    else if (K > (*RootPtr)->Key){
        if ((*RootPtr)->RPtr){
            BSTInsert(&(*RootPtr)->RPtr, K, type, position);
        }
        else {
            (*RootPtr)->RPtr = BSTCreateNode(L, Data, dataType);
            return (*RootPtr)->RPtr;
        }
    }
    else if (K < (*RootPtr)->Key){
        if ((*RootPtr)->LPtr){
            BSTInsert(&(*RootPtr)->LPtr, K, type, position);
        }
        else {
            (*RootPtr)->LPtr = BSTCreateNode(L, Data, dataType);
            return (*RootPtr)->LPtr;
        }
    }
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

tBSTNodePtr SymTableInsertFunction(tSymtable* SymTable, string key){
    tDataFunction* funcPtr = (tDataFunction*) malloc(sizeof(struct tDataFunction));
    string params;
    init_string(&params);

    funcPtr->params = params;
    funcPtr->declared = false;
    funcPtr->defined = false;
    funcPtr->returnType = NULL;

    return BSTInsert(SymTable, key.str, funcPtr, Function);
}

tBSTNodePtr SymTableInsertVariable(tSymtable* SymTable, string key){
    tDataVariable* varPtr = (tDataVariable*) malloc(sizeof(struct tDataVariable));

    varPtr->dataType = -1;

    return BSTInsert(SymTable, key.str, varPtr, Variable);
}

tBSTNodePtr SymTableSearch(tSymtable* SymTable, string key){
    return BSTSearch(SymTable->root, key.str);
}

void SymTableDelete(tSymtable* SymTable, string key){
    BSTDelete(SymTable->root, key.str);
}

void SymTableDispose(tSymtable* Symtable){
    BSTDispose(&(Symtable->root));
}

void InsertBuiltInFuncs(tSymtable* SymTable){
    tBSTNodePtr node;
    tDataFunction* func;

    // Built-in inputi

    string inputi;
    init_string(&inputi);

    adds_to_string(&inputi, "inputi");
    tBSTNodePtr new_node = SymTableInsertFunction(SymTable, inputi);
    func = (tDataFunction*)(new_node->Content);
    func->defined = true;
    func->declared = true;
    func->returnType[0] = 'i';
    func->returnType[1] = 'i';
    #TODO
}
