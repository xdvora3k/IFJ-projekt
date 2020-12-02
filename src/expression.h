/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 */

#ifndef IFJ_PROJEKT_EXPRESSION_H
#define IFJ_PROJEKT_EXPRESSION_H

#include "scanner.h"
#include "str.h"

typedef struct{
    string *text;
    tState type;
    int endIndex;
} tToken;

typedef enum{
    expPLUSepx,  //E -> E + E;
    expMINUSepx, //E -> E - E;
    expMULepx,   //E -> E * E;
    expDIVepx,   //E -> E / E;
    expOPepx,    //E -> E o E;
    expBrackets, //E -> (E);
    expIdentity  //E -> i;
} ruleType;

#endif
