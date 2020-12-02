//
// Created by xkvasn14 on 02.12.20.
//

#ifndef IFJ_CONNECTOR_H
#define IFJ_CONNECTOR_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ilist.h"
#include "parser.h"
#include "expression.h"

int Calc_Int_Expression(tExpressionNode *Rules);
float Calc_Float_Expression(tExpressionNode *Rules);
void print(tPassedSide *Frases);
#endif //IFJ_CONNECTOR_H
