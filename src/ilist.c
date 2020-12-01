/*
 * IFJ project 2020
 * Author: xdvora3k, Jakub Dvorak
 */

#include "ilist.h"





/*
#define LABEL_S     "$"
#define MAIN        "$$main"*/

extern tLinkedList *L;


void CreateInstruction (INSTRUCTION InstrType, void *addr1, void *addr2, void *addr3)
{
    tInstr  Instruciton;
    Instruciton.instType = InstrType;
    Instruciton.addr1 = addr1;
    Instruciton.addr2 = addr2;
    Instruciton.addr3 = addr3;
    InstructionPrint(Instruciton);
}

tInstructionOperand CreateOperand (char* name, char* value, tVarDataType type, FRAME f)
{
    tInstructionOperand o;
    o.name = name;
    o.value = value;
    o.type = type;
    o.frame = f;
    return o;
}
void Instruction0(INSTRUCTION InstrType)
{
    CreateInstruction(InstrType,"","",""); //maybe insted of NULL has to be "";
}

void Instruction1(INSTRUCTION InstrType, tInstructionOperand op)
{
    char* tmp;
    if(op.frame == Frame_NaN)
    {
        // func, konst, spec, labels
        if(op.type == Unknown_type)
        {
            // func, spec, labels
            if(op.name[0] == '$')
            {
                //funcs
                if(op.name[1] == '$')
                {
                    // main
                    tmp = op.name;
                }
                else
                {
                    // func
                    tmp = op.name;
                }
            }
            else
            {
                // labels, specs
                if((strcmp(op.name,"for") == 0) || (strcmp(op.name,"if") == 0))
                {
                    //specs
                    //well this gona be fun...
                    tmp = op.name;
                }
                else
                {
                    //labels
                    tmp = op.name;
                }
            }
        }
        else
        {
            // konst
            switch(op.type) {
            case IntType:
                tmp = "int@";
                break;
            case Float64Type:
                tmp = "float@";
                break;
            case StringType:
                tmp = "string@";
                break;
            case UnderscoreType:
                tmp = "nil@";
                break;
            default:
                tmp = "";
            }
            tmp = strcat(tmp,op.value);
        }
    }
    else // if (op.frame != Frame_NaN)
    {
        //variables
        if(op.frame == Frame_GF)
            tmp = "GF@";
        else if(op.frame == Frame_LF)
            tmp = "LF@";
        else if(op.frame == Frame_TF)
            tmp = "TF@";
        tmp = strcat(tmp,op.name);
    }

    CreateInstruction(InstrType,tmp,"","");
}

void Instruction2(INSTRUCTION InstrType, tInstructionOperand op, tInstructionOperand op2)
{
    char* tmp;
    if(op.frame == Frame_NaN)
    {
        // func, konst, spec, labels
        if(op.type == Unknown_type)
        {
            // func, spec, labels
            if(op.name[0] == '$')
            {
                //funcs
                if(op.name[1] == '$')
                {
                    // main
                    tmp = op.name;
                }
                else
                {
                    // func
                    tmp = op.name;
                }
            }
            else
            {
                // labels, specs
                if((strcmp(op.name,"for") == 0) || (strcmp(op.name,"if") == 0))
                {
                    //specs
                    //well this gona be fun...
                    tmp = op.name;
                }
                else
                {
                    //labels
                    tmp = op.name;
                }
            }
        }
        else
        {
            // konst
            switch(op.type) {
                case IntType:
                    tmp = "int@";
                    break;
                case Float64Type:
                    tmp = "float@";
                    break;
                case StringType:
                    tmp = "string@";
                    break;
                case UnderscoreType:
                    tmp = "nil@";
                    break;
                default:
                    tmp = "";
            }
            tmp = strcat(tmp,op.value);
        }
    }
    else // if (op.frame != Frame_NaN)
    {
        //variables
        if(op.frame == Frame_GF)
            tmp = "GF@";
        else if(op.frame == Frame_LF)
            tmp = "LF@";
        else if(op.frame == Frame_TF)
            tmp = "TF@";
        tmp = strcat(tmp,op.name);
    }
    //////////////////////
    char* tmp2;
    if(op2.frame == Frame_NaN)
    {
        // func, konst, spec, labels
        if(op2.type == Unknown_type)
        {
            // func, spec, labels
            if(op2.name[0] == '$')
            {
                //funcs
                if(op2.name[1] == '$')
                {
                    // main
                    tmp2 = op2.name;
                }
                else
                {
                    // func
                    tmp2 = op2.name;
                }
            }
            else
            {
                // labels, specs
                if((strcmp(op2.name,"for") == 0) || (strcmp(op2.name,"if") == 0))
                {
                    //specs
                    //well this gona be fun...
                    tmp2 = op2.name;
                }
                else
                {
                    //labels
                    tmp2 = op2.name;
                }
            }
        }
        else
        {
            // konst
            switch(op2.type) {
                case IntType:
                    tmp2 = "int@";
                    break;
                case Float64Type:
                    tmp2 = "float@";
                    break;
                case StringType:
                    tmp2 = "string@";
                    break;
                case UnderscoreType:
                    tmp2 = "nil@";
                    break;
                default:
                    tmp2 = "";
            }
            tmp2 = strcat(tmp2,op2.value);
        }
    }
    else // if (op.frame != Frame_NaN)
    {
        //variables
        if(op2.frame == Frame_GF)
            tmp2 = "GF@";
        else if(op2.frame == Frame_LF)
            tmp2 = "LF@";
        else if(op2.frame == Frame_TF)
            tmp2 = "TF@";
        tmp2 = strcat(tmp2,op2.name);
    }

    CreateInstruction(InstrType,tmp,tmp2,"");
}

void Instruction3(INSTRUCTION InstrType, tInstructionOperand op, tInstructionOperand op2, tInstructionOperand op3)
{
    char* tmp;
    if(op.frame == Frame_NaN)
    {
        // func, konst, spec, labels
        if(op.type == Unknown_type)
        {
            // func, spec, labels
            if(op.name[0] == '$')
            {
                //funcs
                if(op.name[1] == '$')
                {
                    // main
                    tmp = op.name;
                }
                else
                {
                    // func
                    tmp = op.name;
                }
            }
            else
            {
                // labels, specs
                if((strcmp(op.name,"for") == 0) || (strcmp(op.name,"if") == 0))
                {
                    //specs
                    //well this gona be fun...
                    tmp = op.name;
                }
                else
                {
                    //labels
                    tmp = op.name;
                }
            }
        }
        else
        {
            // konst
            switch(op.type) {
                case IntType:
                    tmp = "int@";
                    break;
                case Float64Type:
                    tmp = "float@";
                    break;
                case StringType:
                    tmp = "string@";
                    break;
                case UnderscoreType:
                    tmp = "nil@";
                    break;
                default:
                    tmp = "";
            }
            tmp = strcat(tmp,op.value);
        }
    }
    else // if (op.frame != Frame_NaN)
    {
        //variables
        if(op.frame == Frame_GF)
            tmp = "GF@";
        else if(op.frame == Frame_LF)
            tmp = "LF@";
        else if(op.frame == Frame_TF)
            tmp = "TF@";
        tmp = strcat(tmp,op.name);
    }
    //////////////////////
    char* tmp2;
    if(op2.frame == Frame_NaN)
    {
        // func, konst, spec, labels
        if(op2.type == Unknown_type)
        {
            // func, spec, labels
            if(op2.name[0] == '$')
            {
                //funcs
                if(op2.name[1] == '$')
                {
                    // main
                    tmp2 = op2.name;
                }
                else
                {
                    // func
                    tmp2 = op2.name;
                }
            }
            else
            {
                // labels, specs
                if((strcmp(op2.name,"for") == 0) || (strcmp(op2.name,"if") == 0))
                {
                    //specs
                    //well this gona be fun...
                    tmp2 = op2.name;
                }
                else
                {
                    //labels
                    tmp2 = op2.name;
                }
            }
        }
        else
        {
            // konst
            switch(op2.type) {
                case IntType:
                    tmp2 = "int@";
                    break;
                case Float64Type:
                    tmp2 = "float@";
                    break;
                case StringType:
                    tmp2 = "string@";
                    break;
                case UnderscoreType:
                    tmp2 = "nil@";
                    break;
                default:
                    tmp2 = "";
            }
            tmp2 = strcat(tmp2,op2.value);
        }
    }
    else // if (op.frame != Frame_NaN)
    {
        //variables
        if(op2.frame == Frame_GF)
            tmp2 = "GF@";
        else if(op2.frame == Frame_LF)
            tmp2 = "LF@";
        else if(op2.frame == Frame_TF)
            tmp2 = "TF@";
        tmp2 = strcat(tmp2,op2.name);
    }
    ////////////////////
    char* tmp3;
    if(op3.frame == Frame_NaN)
    {
        // func, konst, spec, labels
        if(op3.type == Unknown_type)
        {
            // func, spec, labels
            if(op3.name[0] == '$')
            {
                //funcs
                if(op3.name[1] == '$')
                {
                    // main
                    tmp3 = op3.name;
                }
                else
                {
                    // func
                    tmp3 = op3.name;
                }
            }
            else
            {
                // labels, specs
                if((strcmp(op3.name,"for") == 0) || (strcmp(op3.name,"if") == 0))
                {
                    //specs
                    //well this gona be fun...
                    tmp3 = op3.name;
                }
                else
                {
                    //labels
                    tmp3 = op3.name;
                }
            }
        }
        else
        {
            // konst
            switch(op3.type) {
                case IntType:
                    tmp3 = "int@";
                    break;
                case Float64Type:
                    tmp3 = "float@";
                    break;
                case StringType:
                    tmp3 = "string@";
                    break;
                case UnderscoreType:
                    tmp3 = "nil@";
                    break;
                default:
                    tmp3 = "";
            }
            tmp3 = strcat(tmp3,op3.value);
        }
    }
    else // if (op.frame != Frame_NaN)
    {
        //variables
        if(op3.frame == Frame_GF)
            tmp3 = "GF@";
        else if(op3.frame == Frame_LF)
            tmp3 = "LF@";
        else if(op3.frame == Frame_TF)
            tmp3 = "TF@";
        tmp3 = strcat(tmp3,op3.name);
    }

    CreateInstruction(InstrType,tmp,tmp2,tmp3);
}




void StrLLInit(tLinkedList *L){
    L->first = NULL;
}

void StrLLInsert(tLinkedList *L, char *K){
    tListItem *new_item = malloc(sizeof(tListItem));
    int str_len = strlen(K);
    char *content = malloc(str_len + 1);
    strncpy(content, K, str_len);
    content[str_len] = '\0';
    new_item->Content = content;
    new_item->nextItem = NULL;

    if (!L->first){
        L->first = new_item;
        return;
    }

    tListItem *curr = L->first;
    while (curr->nextItem){
        curr = curr->nextItem;
    }
    curr->nextItem = new_item;
}

int StrLLStringAlreadyOccupied(tLinkedList *L, char *S){
    tListItem *node = L->first;
    while (node){
        if (!strcmp((char*) node->Content, S)){
            return TRUE;
        }
        node = node->nextItem;
    }
    return FALSE;
}

void StrLLDeleteLast(tLinkedList *L){
    if (!L->first){
        return;
    }

    tListItem **to_delete = &L->first;
    while ((*to_delete)->nextItem){
        to_delete = &(*to_delete)->nextItem;
    }

    free((*to_delete)->Content);
    free(*to_delete);
    *to_delete = NULL;
}

void StrLLDispose(tLinkedList *L){
    while (L->first){
        StrLLDeleteLast(L);
    }
}

tListItem* StrLLLocateNthElem(tLinkedList *L, int index){
    tListItem *item = L->first;
    for (int i = 0; i < index; i++){
        item = item->nextItem;
    }
    return item;
}

int StrLLLen(tLinkedList *L) {
    if (!L->first) {
        return 0;
    }
    int i = 0;

    tListItem *item = L->first;
    while (item) {
        i++;
        item = item->nextItem;
    }
    return i;
}
// Symtable List
//---------------------------------------------------------
void TableLLInit(tLinkedList *L){
    L->first = NULL;
}

void TableLLDeleteFirst(tLinkedList *L){
    if (!L->first){
        return;
    }
    tListItem *to_delete = L->first;
    L->first = L->first->nextItem;
    SymTableDispose(to_delete->Content);
    free(to_delete);
}

void TableLLInsertFirst(tLinkedList *L, tSymtable *local_var_table){
    tListItem *new_node = malloc(sizeof(tListItem));
    tSymtable *new_content = malloc(sizeof(tSymtable));
    new_node->Content = local_var_table;
    if (!L->first){
        new_node->nextItem = NULL;
    }
    else {
        new_node->nextItem = L->first;
    }
    new_node->Content = new_content;
    L->first = new_node;
}

tListItem* TableLLLocateNthElem(tLinkedList *L, int index){
    tListItem *item = L->first;
    for (int i = 0; i < index; i++){
        item = item->nextItem;
    }
    return item;
}

int TableLLLen(tLinkedList *L){
    if (!L->first){
        return 0;
    }
    int i = 0;
    tListItem *item = L->first;
    while (item){
        i++;
        item = item->nextItem;
    }
    return i;
}

int TableLLFindAllVariables(tLinkedList *func_variable_list, tLinkedList *variables){
    if (!func_variable_list || !func_variable_list->first){
        return 100;
    }
    if (!variables || !variables->first){
        return 1000;
    }
    int not_found = 0;
    for (int i = 0; i < StrLLLen(variables); i++){
        for (int j = 0; j < TableLLLen(func_variable_list); j++){
            tSymtable *curr_table = (tSymtable*) TableLLLocateNthElem(func_variable_list, j)->Content;
            tListItem *var_node = StrLLLocateNthElem(variables, i);
            if (SymTableSearch(curr_table, ((string*) var_node->Content)->str)){
                continue;
            }
        }
        not_found++;
    }
    return not_found;
}

int TableLLGetNumOfNests(tLinkedList *func_variable_list, char* var){
    if (!func_variable_list || !func_variable_list->first){
        return -1;
    }
    int table_len = TableLLLen(func_variable_list);
    for (int i = 0; i < table_len; i++){
        tSymtable *curr_table = (tSymtable*) TableLLLocateNthElem(func_variable_list, i)->Content;
        if (SymTableSearch(curr_table, var)){
            return table_len - i;
        }
    }
    return -1;
}

tDataVariable* TableLLGetSingleVariable(tLinkedList *func_variable_list, char* var){
    if (!func_variable_list || !func_variable_list->first){
        return NULL;
    }
    for (int i = 0; i < TableLLLen(func_variable_list); i++){
        tSymtable *curr_table = (tSymtable*) TableLLLocateNthElem(func_variable_list, i)->Content;
        if (!SymTableSearch(curr_table, var)){
            return NULL;
        }
        tDataVariable *var_node = (tDataVariable*) SymTableSearch(curr_table, var)->Content;
        if (var_node){
            return var_node;
        }
    }
    return NULL;
}

// Instruction List
//---------------------------------------------------------
void InstrLLInit(tLinkedList *L){
    L->first = NULL;
}

void InstrLLDisposeAll(tLinkedList *L){
    while (L->first){
        tListItem *to_delete = L->first;
        L->first = L->first->nextItem;
        free(to_delete);
    }
    L->first = NULL;
}

void InstrLLInsertFirst(tLinkedList *L, tInstr* Instruction){
    tListItem *new_node = (tListItem*) malloc(sizeof(tListItem));
    new_node->nextItem = L->first;
    new_node->Content = Instruction;
    L->first = new_node;
}


void InstrLLDeleteFirst(tLinkedList *L){
    if (!L->first){
        return;
    }
    tListItem *to_delete = L->first;
    L->first = L->first->nextItem;
    free(to_delete);
}




void len(char* s, tInstructionOperand *out)
{
    tInstructionOperand counter;
    tInstructionOperand navesti_begin;
    navesti_begin = CreateOperand("$len","",Unknown_type,Frame_NaN);
    Instruction1(I_LABEL,navesti_begin);
    Instruction0(I_PUSHFRAME);
    counter = CreateOperand("",s,StringType,Frame_NaN);
    Instruction2(I_STRLEN,*out,counter);
    Instruction0(I_POPFRAME);
    Instruction0(I_RETURN);

}

void chr()
{
    tInstructionOperand navesti_begin;
    navesti_begin = CreateOperand("$chr","",Unknown_type,Frame_NaN);
    Instruction1(I_LABEL,navesti_begin);
    Instruction0(I_PUSHFRAME);


}


void inputi(tInstructionOperand o)
{
    char* frame;
    if(o.frame == Frame_GF)
        frame = "GF";
    else if(o.frame == Frame_LF)
        frame = "LF";
    else if(o.frame == Frame_TF)
        frame = "TF";
    else if(o.frame == Frame_NaN)
        frame = "";
    printf("READ %s@%s int\n",frame,o.name);
}
void inputs(tInstructionOperand o)
{
    char* frame;
    if(o.frame == Frame_GF)
        frame = "GF";
    else if(o.frame == Frame_LF)
        frame = "LF";
    else if(o.frame == Frame_TF)
        frame = "TF";
    else if(o.frame == Frame_NaN)
        frame = "";
    printf("READ %s@%s string\n",frame,o.name);
}
void inputf(tInstructionOperand o)
{
    char* frame;
    if(o.frame == Frame_GF)
        frame = "GF";
    else if(o.frame == Frame_LF)
        frame = "LF";
    else if(o.frame == Frame_TF)
        frame = "TF";
    else if(o.frame == Frame_NaN)
        frame = "";
    printf("READ %s@%s float\n",frame,o.name);
}

/* Built-in functions */
/*
void len()
{   tInstructionOperand operand1;tInstructionOperand operand2;
    operand1 = CreateOperand(operand1,"len",Unknown_type,Frame_GF,true,false);
    InstructionWith1operand(L,I_LABEL,operand1);
    InstructionWithNoOperand(L,I_PUSHFRAME);
    operand1 = CreateOperand(operand1,"retval",IntType,Frame_GF,false,true);
    operand2 = CreateOperand(operand2,"s",StringType,Frame_LF,false,false);
    InstructionWith2operand(L,I_STRLEN,operand1,operand2);
    InstructionWithNoOperand(L,I_RETURN);
}
*/


/*
int len (char* s)
{
    tInstructionOperand operandS;
    tInstructionOperand operandI;
    operandS = CreateOperand(operandS,s,StringType,Frame_GF,false,false);
    operandI = CreateOperand(operandI,"0",IntType,Frame_GF,false,false);
    InstructionWith2operand(L,I_STRLEN,operandI,operandS);
    return *(operandI.value.str);
}*/


/*
void for_function()
{
    tInstructionOperand iterator,for_begin,for_end;
    iterator = CreateOperand(iterator,"0",IntType,Frame_TF,false,true);
    InstructionWith1operand(L,I_DEFVAR,iterator);
    InstructionWithNoOperand(L,I_PUSHFRAME);
    for_begin = CreateOperand(for_begin,"for_begin",Unknown_type,Frame_TF,true,)

}
*/
















void InstructionPrint(tInstr i)
{
    switch (i.instType) {
        case I_MOVE:
            Instr_I_MOVE(i);
            break;
        case I_CREATEFRAME:
            Instr_I_CREATEFRAME(i);
            break;
        case I_PUSHFRAME:
            Instr_I_PUSHFRAME(i);
            break;
        case I_POPFRAME:
            Instr_I_POPFRAME(i);
            break;
        case I_DEFVAR:
            Instr_I_DEFVAR(i);
            break;
        case I_CALL:
            Instr_I_CALL(i);
            break;
        case I_RETURN:
            Instr_I_RETURN(i);
            break;
        case I_PUSHS:
            Instr_I_PUSHS(i);
            break;
        case I_POPS:
            Instr_I_POPS(i);
            break;
        case I_CLEARS:
            Instr_I_CLEARS(i);
            break;
        case I_ADD:
            Instr_I_ADD(i);
            break;
        case I_SUB:
            Instr_I_SUB(i);
            break;
        case I_MUL:
            Instr_I_MUL(i);
            break;
        case I_DIV:
            Instr_I_DIV(i);
            break;
        case I_IDIV:
            Instr_I_IDIV(i);
            break;
        case I_ADDS:
            Instr_I_ADDS(i);
            break;
        case I_SUBS:
            Instr_I_SUBS(i);
            break;
        case I_MULS:
            Instr_I_MULS(i);
            break;
        case I_DIVS:
            Instr_I_DIVS(i);
            break;
        case I_IDIVS:
            Instr_I_IDIVS(i);
            break;
        case I_LT:
            Instr_I_LT(i);
            break;
        case I_GT:
            Instr_I_GT(i);
            break;
        case I_EQ:
            Instr_I_EQ(i);
            break;
        case I_LTS:
            Instr_I_LTS(i);
            break;
        case I_GTS:
            Instr_I_GTS(i);
            break;
        case I_EQS:
            Instr_I_EQS(i);
            break;
        case I_AND:
            Instr_I_AND(i);
            break;
        case I_OR:
            Instr_I_OR(i);
            break;
        case I_NOT:
            Instr_I_NOT(i);
            break;
        case I_ANDS:
            Instr_I_ANDS(i);
            break;
        case I_ORS:
            Instr_I_ORS(i);
            break;
        case I_NOTS:
            Instr_I_NOTS(i);
            break;
        case I_FLOAT2INT:
            Instr_I_FLOAT2INT(i);
            break;
        case I_INT2FLOAT:
            Instr_I_INT2FLOAT(i);
            break;
        case I_INT2CHAR:
            Instr_I_INT2CHAR(i);
            break;
        case I_STRI2INT:
            Instr_I_STRI2INT(i);
            break;
        case I_INT2FLOATS:
            Instr_I_INT2FLOATS(i);
            break;
        case I_FLOAT2INTS:
            Instr_I_FLOAT2INTS(i);
            break;
        case I_INT2CHARS:
            Instr_I_INT2CHARS(i);
            break;
        case I_STRI2INTS:
            Instr_I_STRI2INTS(i);
            break;
        case I_READ:
            Instr_I_READ(i);
            break;
        case I_WRITE:
            Instr_I_WRITE(i);
            break;
        case I_CONCAT:
            Instr_I_CONCAT(i);
            break;
        case I_STRLEN:
            Instr_I_STRLEN(i);
            break;
        case I_GETCHAR:
            Instr_I_GETCHAR(i);
            break;
        case I_SETCHAR:
            Instr_I_SETCHAR(i);
            break;
        case I_TYPE:
            Instr_I_TYPE(i);
            break;
        case I_LABEL:
            Instr_I_LABEL(i);
            break;
        case I_JUMP:
            Instr_I_JUMP(i);
            break;
        case I_JUMPIFEQ:
            Instr_I_JUMPIFEQ(i);
            break;
        case I_JUMPIFNEQ:
            Instr_I_JUMPIFNEQ(i);
            break;
        case I_JUMPIFEQS:
            Instr_I_JUMPIFEQS(i);
            break;
        case I_JUMPIFNEQS:
            Instr_I_JUMPIFNEQS(i);
            break;
        case I_EXIT:
            Instr_I_EXIT(i);
            break;
        case I_BREAK:
            Instr_I_BREAK(i);
            break;
        case I_DPRINT:
            Instr_I_DPRINT(i);
            break;
        default:
            break;
    }
}



void Instr_I_MOVE(tInstr i){            printf("MOVE %s %s\n",i.addr1,i.addr2);}
void Instr_I_CREATEFRAME(){             printf("CREATEFRAME\n");}
void Instr_I_PUSHFRAME(){               printf("PUSHFRAME\n");}
void Instr_I_POPFRAME(){                printf("POPFRAME\n");}
void Instr_I_DEFVAR(tInstr i){          printf("DEFVAR %s\n",i.addr1);}
void Instr_I_CALL(tInstr i){            printf("LABEL %s\n",i.addr1);}
void Instr_I_RETURN(){                  printf("RETURN\n");}
void Instr_I_PUSHS(tInstr i){           printf("PUSHS %s\n",i.addr1);}
void Instr_I_POPS(tInstr i){            printf("POPS %s\n",i.addr1);}
void Instr_I_CLEARS(){                  printf("CLEARS\n");}
void Instr_I_ADD(tInstr i){             printf("ADD %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_SUB(tInstr i){             printf("SUB %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_MUL(tInstr i){             printf("MUL %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_DIV(tInstr i){             printf("DIV %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_IDIV(tInstr i){            printf("IDIV %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_ADDS(){                    printf("ADDS\n");}
void Instr_I_SUBS(){                    printf("SUBS\n");}
void Instr_I_MULS(){                    printf("MULS\n");}
void Instr_I_DIVS(){                    printf("DIVS\n");}
void Instr_I_IDIVS(){                   printf("IDIVS\n");}
void Instr_I_LT(tInstr i){              printf("LT %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_GT(tInstr i){              printf("GT %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_EQ(tInstr i){              printf("EQ %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_LTS(){                     printf("LTS\n");}
void Instr_I_GTS(){                     printf("GTS\n");}
void Instr_I_EQS(){                     printf("EQS\n");}
void Instr_I_AND(tInstr i){             printf("AND %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_OR(tInstr i){              printf("OR %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_NOT(tInstr i){             printf("NOT %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_ANDS(){                    printf("ANDS\n");}
void Instr_I_ORS(){                     printf("ORS\n");}
void Instr_I_NOTS(){                    printf("NOTS\n");}
void Instr_I_FLOAT2INT(tInstr i){       printf("FLOAT2INT %s %s\n",i.addr1,i.addr2);}
void Instr_I_INT2FLOAT(tInstr i){       printf("INT2FLOAT %s %s\n",i.addr1,i.addr2);}
void Instr_I_INT2CHAR(tInstr i){        printf("INT2CHAR %s %s\n",i.addr1,i.addr2);}
void Instr_I_STRI2INT(tInstr i){        printf("STRI2INT %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_INT2FLOATS(){              printf("INT2FLOATS\n");}
void Instr_I_FLOAT2INTS(){              printf("FLOAT2INTS\n");}
void Instr_I_INT2CHARS(){               printf("INT2CHARS\n");}
void Instr_I_STRI2INTS(){               printf("STRI2INTS\n");}
void Instr_I_READ(tInstr i){            printf("READ %s %s\n",i.addr1,i.addr2);}
void Instr_I_WRITE(tInstr i){           printf("WRITE %s",i.addr1);}
void Instr_I_CONCAT(tInstr i){          printf("CONCAT %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_STRLEN(tInstr i){          printf("STRLEN %s %s\n",i.addr1,i.addr2);}
void Instr_I_GETCHAR(tInstr i){         printf("GETCHAR %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_SETCHAR(tInstr i){         printf("SETCHAR %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_TYPE(tInstr i){            printf("TYPE %s %s\n",i.addr1,i.addr2);}
void Instr_I_LABEL(tInstr i){           printf("LABEL %s\n",i.addr1);}
void Instr_I_JUMP(tInstr i){            printf("JUMP %s\n",i.addr1);}
void Instr_I_JUMPIFEQ(tInstr i){        printf("JUMPIFEQ %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_JUMPIFNEQ(tInstr i){       printf("JUMPIFNEQ %s %s %s\n",i.addr1,i.addr2,i.addr3);}
void Instr_I_JUMPIFEQS(tInstr i){       printf("JUMPIFEQS %s\n",i.addr1);}
void Instr_I_JUMPIFNEQS(tInstr i){      printf("JUMPIFNEQS %s\n",i.addr1);}
void Instr_I_EXIT(tInstr i){            printf("EXIT %s\n",i.addr1);}
void Instr_I_BREAK(){                   printf("BREAK\n");}
void Instr_I_DPRINT(tInstr i){          printf("DPRINT %s\n",i.addr1);}


void Print_BuiltIn_Functions()
{
    printf(".IFJcode20\n\n");
    //DEF 2x tmp
    printf("JUMP $$main\n\n");

    //printf inputi
    printf("LABEL $inputi\n"
           "PUSHFRAME\n"
           "DEFVAR LF@reval\n"
           "READ LF@retval int\n"
           "POPFRAME\n"
           "RETURN\n");

    //printf inputf
    printf("LABEL $inputf\n"
           "PUSHFRAME\n"
           "DEFVAR LF@retval"
           "READ LF@retval float\n"
           "POPFRAME\n"
           "RETURN\n");

    //printf inputs
    printf("LABEL $inputs\n"
           "PUSHFRAME\n"
           "DEFVAR LF@retval\n"
           "READ LF@retval string\n"
           "POPFRAME\n"
           "RETURN\n");

    //printf int2float
    printf("LABEL $int2float\n"
           "PUSHFRAME\n"
           "DEFVAR LF@retval\n"
           "INT2FLOAT LF@retval LF@i\n"
           "POPFRAME\n"
           "RETURN\n");

    //printf float2int
    printf("LABEL $float2int\n"
           "PUSHFRAME\n"
           "DEFVAR LF@retval\n"
           "FLOAT2INT LF@retval LF@f\n"
           "POPFRAME\n"
           "RETURN\n");

    //Print len
    printf("LABEL $len\n"
           "PUSHFRAME\n"
           "DEFVAR LF@retval\n"
           "STRLEN LF@retval LF@s\n"
           "POPFRAME \n"
           "RETURN\n");

    //print substr
    printf("LABEL $substr\n"
           "PUSHFRAME\n"
           "DEFVAR LF@retval\n"
           "MOVE LF@retval string@\n"
           "DEFVAR LF@length\n"
           "CREATEFRAME\n"
           "DEFVAR TF%%0\n"
           "MOVE TF@%%0 LF%%0\n"
           "CALL $len\n"
           "MOVE LF@length TF@%%retval\n"
           "DEFVAR LF@ret\n"
           "LT LF@ret LF@length int@0\n"
           "JUMPIFEQ $substr$return LF@ret bool@true\n"
           "EQ LF@ret LF@length int@0\n"
           "JUMPIFEQ $substr$return LF@ret bool@true\n"
           "LT LF@ret LF@%%1 int@0\n"
           "JUMPIFEQ $substr$return LF@ret bool@true\n"
           "EQ LF@ret LF@%%1 int@0\n"
           "JUMPIFEQ $substr$return LF@ret bool@true\n"
           "GT LF@ret LF@%%1 LF@length\n"
           "JUMPIFEQ $substr$return LF@ret bool@true\n"
           "EQ LF@ret LF@%%2 int@0\n"
           "JUMPIFEQ $substr$return LF@ret bool@true\n"
           "DEFVAR LF@max\n"
           "MOVE LF@max LF@length\n"
           "SUB LF@max LF@max LF@%%1\n"
           "ADD LF@max LF@max int@1\n"
           "DEFVAR LF@edit\n"
           "LT LF@edit LF@%%2 int@0\n"
           "JUMPIFEQ $substr$edit LF@edit bool@true\n"
           "GT LF@edit LF@%%2 LF@max\n"
           "JUMPIFEQ $substr$edit LF@edit bool@true\n"
           "JUMP $substr$process\n"
           "LABEL $substr$edit\n"
           "MOVE LF@%%2 LF@max\n"
           "LABEL $substr$process\n"
           "DEFVAR LF@index\n"
           "MOVE LF@index LF@%%1\n"
           "SUB LF@index LF@index int@1\n"
           "DEFVAR LF@char\n"
           "DEFVAR LF@processloop\n"
           "LABEL $substr$loop\n"
           "GETCHAR LF@char LF@%%0 LF@index\n"
           "CONCAT LF@retval LF@retval LF@char\n"
           "ADD LF@index LF@index int@1\n"
           "SUB LF@%%2 LF@%%2 int@1\n"
           "GT LF@processloop LF@%%2 int@0\n"
           "JUMPIFEQ $substr$loop LF@processloop bool@true\n"
           "LABEL $substr$return\n"
           "POPFRAME\n"
           "RETURN\n");

    //printf chr
    printf("LABEL $chr\n"
           "PUSHFRAME\n"
           "DEFVAR LF@retval\n"
           "MOVE LF@retval string@\n"
           "DEFVAR LF@cond\n"
           "LT LF@cond LF@%%0 int@0\n"
           "JUMPIFEQ $chr$return LF@cond bool@true\n"
           "GT LF@cond LF@%%0 int@255\n"
           "JUMPIFEQ $chr$return LF@cond bool@true\n"
           "INT2CHAR LF@retval LF@%%0\n"
           "LABEL $chr$return\n"
           "POPFRAME\n"
           "RETURN\n");

    //printf ord
    printf("LABEL $ord\n"
           "PUSHFRAME\n"
           "DEFVAR LF@retval\n"
           "MOVE LF@retval int@0\n"
           "DEFVAR LF@cond\n"
           "LT LF@cond LF@%%1 int@1\n"
           "JUMPIFEQ $ord$return LF@cond bool@true\n"
           "DEFVAR LF@length\n"
           "CREATEFRAME\n"
           "DEFVAR TF@%%0\n"
           "MOVE TF@%%0 LF@%%0\n"
           "CALL $len\n"
           "MOVE LF@length TF@retval\n"
           "GT LF@cond LF@%%1 LF@length\n"
           "JUMPIFEQ $ord$return LF@cond bool@true\n"
           "SUB LF@%%1 LF@%%1 int@1\n"
           "STRI2INT LF@retval LF@%%0 LF@%%1\n"
           "LABEL $ord$return\n"
           "POPFRAME\n"
           "RETURN\n");


    //printf begin of main
    printf("LABEL $$main\n");
}