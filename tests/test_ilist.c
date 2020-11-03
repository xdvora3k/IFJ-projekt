/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 */

#include "../src/ilist.h"

int main(){
    tListOfInstr List;
    LLInit(&List);
    printf("Starting test.\n");
    tInstr Instr;
    Instr.addr1 = NULL;
    Instr.addr2 = NULL;
    Instr.addr3 = NULL;
    Instr.instType = I_ANOTHER;
    LLInsertFirst(&List, Instr);
    printf("1.Expecting: %d, Actual: %d\n\n", I_ANOTHER, List.first->Instruction.instType);
    if (I_ANOTHER != List.first->Instruction.instType){
        return 1;
    }

    Instr.instType = I_DIVIDE;
    LLInsertLast(&List, Instr);
    printf("2.Expecting: %d, Actual: %d\n", I_ANOTHER, List.first->Instruction.instType);
    printf("3.Expecting: %d, Actual: %d\n\n", I_DIVIDE, List.last->Instruction.instType);
    if (I_ANOTHER != List.first->Instruction.instType || I_DIVIDE != List.last->Instruction.instType){
        return 1;
    }

    Instr.instType = I_FUNC;
    LLInsertFirst(&List, Instr);
    printf("4.Expecting: %d, Actual: %d\n", I_FUNC, List.first->Instruction.instType);
    printf("5.Expecting: %d, Actual: %d\n", I_ANOTHER, List.first->nextItem->Instruction.instType);
    printf("6.Expecting: %d, Actual: %d\n\n", I_DIVIDE, List.last->Instruction.instType);
    if (I_FUNC != List.first->Instruction.instType){
        return 1;
    }
    if (I_ANOTHER != List.first->nextItem->Instruction.instType){
        return 1;
    }
    if (I_DIVIDE != List.last->Instruction.instType){
        return 1;
    }

    LLDeleteFirst(&List);
    printf("7.Expecting: %d, Actual: %d\n", I_ANOTHER, List.first->Instruction.instType);
    printf("8.Expecting: %d, Actual: %d\n\n", I_DIVIDE, List.last->Instruction.instType);
    if (I_ANOTHER != List.first->Instruction.instType){
        return 1;
    }
    if (I_DIVIDE != List.last->Instruction.instType){
        return 1;
    }

    Instr.instType = I_MINUS;
    LLSetActiveToFirst(&List);
    LLActualize(&List, Instr);
    printf("9.Expecting: %d, Actual: %d\n", I_MINUS, List.first->Instruction.instType);
    printf("10.Expecting: %d, Actual: %d\n\n", I_DIVIDE, List.last->Instruction.instType);
    if (I_MINUS != List.first->Instruction.instType){
        return 1;
    }
    if (I_DIVIDE != List.last->Instruction.instType){
        return 1;
    }

    LLDisposeAll(&List);
    printf("11.Expecting: 0, Actual: %s\n", List.first ? "1" : "0");
    printf("12.Expecting: 0, Actual: %s\n\n", List.last ? "1" : "0");
    if (List.last || List.first){
        return 1;
    }

}
