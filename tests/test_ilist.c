/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 */

#include "../src/ilist.h"
#define I_ANOTHER 1
#define I_FUNC 2


int main(){
    tLinkedList List;
    InstrLLInit(&List);
    printf("Starting test.\n");
    tInstr Instr;
    Instr.addr1 = NULL;
    Instr.addr2 = NULL;
    Instr.addr3 = NULL;
    Instr.instType = I_ANOTHER;
    InstrLLInsertFirst(&List, &Instr);
    printf("1.Expecting: %d, Actual: %d\n\n", I_ANOTHER, ((tInstr*) List.first->Content)->instType);
    if (I_ANOTHER != ((tInstr*) List.first->Content)->instType){
        return 1;
    }

    Instr.instType = I_FUNC;
    InstrLLInsertFirst(&List, &Instr);
    printf("2.Expecting: %d, Actual: %d\n", I_FUNC, ((tInstr*) List.first->Content)->instType);
    if (I_FUNC != ((tInstr*) List.first->Content)->instType){
        return 1;
    }
    // TODO
}
