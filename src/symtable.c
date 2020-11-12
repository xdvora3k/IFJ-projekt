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
    new_node->Content = *new_content;
    new_node->Key = K;
    new_node->LPtr = NULL;
    new_node->RPtr = NULL;
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
    tDataFunction* func;


    // Built-in  //
    //func inputi
    //---------------------------------
    string inputi;
    init_string(&inputi);

    adds_to_string(&inputi, "inputi");
    tBSTNodePtr new_node_inputi = SymTableInsertFunction(SymTable, inputi);
    func = (tDataFunction*)(new_node_inputi->Content);
    func->defined = true;
    func->declared = true;
    //func->params = NONE
    func->list_initialized = false;
    init_string(&(func->returnType));
    adds_to_string(&(func->returnType),"ii");
    //---------------------------------

    //func inputs
    //---------------------------------
    string inputs;
    init_string(&inputs);

    adds_to_string(&inputs,"inputs");
    tBSTNodePtr new_node_inputs = SymTableInsertFunction(SymTable, inputs);
    func = (tDataFunction*)(new_node_inputs->Content);
    func->declared = true;
    func->defined = true;
    //func->params = NONE
    func->list_initialized = false;
    init_string(&(func->returnType));
    adds_to_string(&(func->returnType),"si");
    //---------------------------------

    //func inputf
    //---------------------------------
    string inputf;
    init_string(&inputf);

    adds_to_string(&inputf,"inputf");
    tBSTNodePtr new_node_inputf = SymTableInsertFunction(SymTable, inputf);
    func = (tDataFunction*)(new_node_inputf->Content);
    func->defined = true;
    func->declared = true;
    //func->params = NONE
    func->list_initialized = false;
    init_string(&(func->returnType));
    adds_to_string(&(func->returnType),"fi");
    //---------------------------------

    //func print
    //---------------------------------
    string print;
    init_string(&print);

    adds_to_string(&print,"print");
    tBSTNodePtr new_node_print = SymTableInsertFunction(SymTable, print);
    func = (tDataFunction*)(new_node_print->Content);
    func->defined = true;
    func->declared = true;
    func->list_initialized = false;
    //---------------------------------

    //func int2float
    //---------------------------------
    string int2float;
    init_string(&int2float);

    adds_to_string(&int2float,"int2float");
    tBSTNodePtr new_node_int2float = SymTableInsertFunction(SymTable, int2float);
    func = (tDataFunction*)(new_node_int2float->Content);
    func->defined = true;
    func->declared = true;
    func->list_initialized = true;

    string int2float_i;
    init_string(&int2float_i);
    add_to_string(&(int2float_i),'i');
    StrLLInit(&(func->paramNames));
    StrLLInsert(&(func->paramNames),&(int2float_i));
    init_string(&(func->params));
    add_to_string(&(func->params),'i');
    init_string(&(func->returnType));
    add_to_string(&(func->returnType),'f');
    //---------------------------------

    //func float2int
    //---------------------------------
    string float2int;
    init_string(&int2float);

    adds_to_string(&float2int,"float2int");
    tBSTNodePtr new_node_float2int = SymTableInsertFunction(SymTable, float2int);
    func = (tDataFunction*)(new_node_float2int->Content);
    func->defined = true;
    func->declared = true;
    func->list_initialized = true;

    string float2int_f;
    init_string(&(float2int_f));
    add_to_string(&(float2int_f),'f');
    StrLLInit(&(func->paramNames));
    StrLLInsert(&(func->paramNames),&(float2int_f));
    init_string(&(func->params));
    add_to_string(&(func->params),'f');
    init_string(&(func->returnType));
    add_to_string(&(func->returnType),'i');
    //---------------------------------

    //func len
    //---------------------------------
    string len;
    init_string(&len);

    adds_to_string(&len,"len");
    tBSTNodePtr new_node_len = SymTableInsertFunction(SymTable, len);
    func = (tDataFunction*)(new_node_len->Content);
    func->declared = true;
    func->defined = true;
    func->list_initialized = true;

    string len_s;
    init_string(&(len_s));
    add_to_string(&(len_s),'s');
    StrLLInit(&(func->paramNames));
    StrLLInsert(&(func->paramNames),&(len_s));
    init_string(&(func->params));
    add_to_string(&(func->params),'s');
    init_string(&(func->returnType));
    add_to_string(&(func->returnType),'i');
    //---------------------------------

    //func substr
    //---------------------------------
    string substr;
    init_string(&substr);

    adds_to_string(&substr,"substr");
    tBSTNodePtr new_node_substr = SymTableInsertFunction(SymTable, substr);
    func = (tDataFunction*)(new_node_substr->Content);
    func->defined = true;
    func->declared = true;
    func->list_initialized = true;

    string substr_s;                  string substr_i;                   string substr_n;
    init_string(&(substr_s));         init_string(&(substr_i));          init_string(&(substr_n));
    add_to_string(&(substr_s),'s'); add_to_string(&(substr_i),'i'); add_to_string(&(substr_n),'n');
    StrLLInit(&(func->paramNames));
    StrLLInsert(&(func->paramNames),&(substr_s));
    StrLLInsert(&(func->paramNames),&(substr_i));
    StrLLInsert(&(func->paramNames),&(substr_n));
    init_string(&(func->params));
    adds_to_string(&(func->params),"sii");
    init_string(&(func->returnType));
    adds_to_string(&(func->returnType),"si");
    //---------------------------------

    //func ord
    //---------------------------------
    string ord;
    init_string(&ord);

    adds_to_string(&ord,"ord");
    tBSTNodePtr new_node_ord = SymTableInsertFunction(SymTable, ord);
    func = (tDataFunction*)(new_node_ord->Content);
    func->declared = true;
    func->defined = true;
    func->list_initialized = true;

    string ord_s;                       string ord_i;
    init_string(&(ord_s));              init_string(&(ord_i));
    add_to_string(&(ord_s),'s');      add_to_string(&(ord_i),'i');
    StrLLInit(&(func->paramNames));
    StrLLInsert(&(func->paramNames),&(ord_s));
    StrLLInsert(&(func->paramNames),&(ord_i));
    init_string(&(func->params));
    adds_to_string(&(func->params),"si");
    init_string(&(func->returnType));
    adds_to_string(&(func->returnType),"ii");
    //---------------------------------

    //func chr
    //---------------------------------
    string chr;
    init_string(&chr);

    adds_to_string(&chr,"chr");
    tBSTNodePtr new_node_chr = SymTableInsertFunction(SymTable, chr);
    func = (tDataFunction*)(new_node_chr->Content);
    func->declared = true;
    func->defined = true;
    func->list_initialized = true;

    string chr_i;
    init_string(&(chr_i));
    add_to_string(&(chr_i),'i');
    StrLLInit(&(func->paramNames));
    StrLLInsert(&(func->paramNames),&(chr_i));
    init_string(&(func->params));
    add_to_string(&(func->params),'i');
    init_string(&(func->returnType));
    adds_to_string(&(func->returnType),"si");
    //---------------------------------


}
