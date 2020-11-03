/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 */

#include "symtable.h"

void BSTInit(tBSTNodePtr *RootPtr){
    *RootPtr = NULL;
}

int BSTSearch(tBSTNodePtr RootPtr, char* K, tBSTContent *Content){
    if (RootPtr == NULL){
        return FALSE;
    }

    if (RootPtr->Key == K){
        *Content = RootPtr->Content;
        return TRUE;
    }

    if (K > RootPtr->Key){
        return BSTSearch(RootPtr->RPtr, K, Content);
    }
    return BSTSearch(RootPtr->LPtr, K, Content);
}

tBSTNodePtr BSTCreateNode(char* K, char* type, double position){
    tBSTContent *new_content = (tBSTContent*) malloc(sizeof(tBSTContent));
    new_content->position = position;
    new_content->type = type;
    tBSTNodePtr new_node = (tBSTNodePtr) malloc(sizeof(struct tBSTNode));
    new_node->Content = *new_content;
    new_node->Key = K;
    new_node->LPtr = NULL;
    new_node->RPtr = NULL;
    return new_node;
}

void BSTInsert(tBSTNodePtr* RootPtr, char* K, char* type, double position){
    if (!(*RootPtr)){
        *RootPtr = BSTCreateNode(K, type, position);
    }

    if (K == (*RootPtr)->Key){
        (*RootPtr)->Content.type = type;
        (*RootPtr)->Content.position = position;
    }
    else if (K > (*RootPtr)->Key){
        if ((*RootPtr)->RPtr){
            BSTInsert(&(*RootPtr)->RPtr, K, type, position);
        }
        else {
            (*RootPtr)->RPtr = BSTCreateNode(K, type, position);
        }
    }
    else if (K < (*RootPtr)->Key){
        if ((*RootPtr)->LPtr){
            BSTInsert(&(*RootPtr)->LPtr, K, type, position);
        }
        else {
            (*RootPtr)->LPtr = BSTCreateNode(K, type, position);
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
