
#include "expressionParser.h"

tVarDataType getDatatypefromId(tToken *token, tLinkedList *linkedL){
    tDataVariable *var;
    if(token == NULL){ 
   // printf("token text  %s", token->text->str);
    }
   var = TableLLGetSingleVariable(linkedL, token->text->str); //or just token
      printf("null\n");

   if(var == NULL){
       exit(SYN_ERROR);
   } else { 
   printf("\n*******var->dataType %d\n", var->dataType);
   }
   return var->dataType;
}

tVarDataType getDataType(tToken *token, tLinkedList *linkedL){
   // tLinkedList *linkedL;
    switch (token->type)
    {
    case tInteger:
        return IntType;
        
    case tId:
    case unknown_identifier:
    printf("tID *****************:::::::::::::\n");
        return getDatatypefromId(token, linkedL);
        
    case tFloat:
        return Float64Type;
    case tString:
        return StringType;

    default:    //not handled in switch
    break;

    }
}


void _save_to_token(tToken *token, string *string, tState type, int end_index, tLinkedList *list){
    token->text = malloc(sizeof(string));
    int str_len = strlen(string->str);
    token->text->str = malloc(str_len + 1);
    strncpy(token->text->str, string->str, str_len);
    token->text->str[str_len] = '\0';
    token->text->length = string->length;
    token->text->allocSize = string->allocSize;
    token->endIndex = end_index;
    token->type = type;
    printf("///////////");
    token->dataType = getDataType(token, list);
    printf("token datatyp %d sta %d\n", token->dataType, token->type);
}

void get_tokenExp(tToken* token, string *input, int startIndex){
    tState state = start;
    tLinkedList *L;
    char c;
    int i = 0;
    string tokenText;
    init_string(&tokenText);
    while (TRUE) {
        c = input->str[startIndex + i];
        //printf("char %c, startIndex %d, and i %d", c, startIndex, i);
        switch (state) {
            case start:
                if (is_whitespace(c)) {
                    i++;
                    continue;
                }
                else if (is_alpha(c)) {
                    add_to_string(&tokenText, c);
                    token->type = unknown_identifier;
                    state = unknown_identifier;
                }
                else if (c == '_') {
                    add_to_string(&tokenText, c);
                    state = tId;
                }
                else if (is_num(c)) {
                    add_to_string(&tokenText, c);
                    state = tInteger;
                }
                else if (c == '"') {
                    state = string_start;
                }
                else if (c == '+') {
                    add_to_string(&tokenText, c);
                    _save_to_token(token, &tokenText, tPlus, startIndex + i, L);
                    return;
                }
                else if (c == '-') {
                    add_to_string(&tokenText, c);
                    _save_to_token(token, &tokenText, tMinus, startIndex + i, L);
                    return;
                }
                else if (c == '*') {
                    add_to_string(&tokenText, c);
                    _save_to_token(token, &tokenText, tMultiply, startIndex + i, L);
                    return;
                }
                else if (c == '/') {
                    add_to_string(&tokenText, c);
                    state = tDivide;
                }
                else if (c == ':') {
                   exit(SYN_ERROR);  //declaration not supported in expression
                }
                else if (c == '!') {
                    add_to_string(&tokenText, c);
                    state = tNotEqual;
                }
                else if (c == '=') {
                    add_to_string(&tokenText, c);
                    state = tAssignment;
                }
                else if (c == '<') {
                    add_to_string(&tokenText, c);
                    state = tSmallerThan;
                }
                else if (c == '>') {
                    add_to_string(&tokenText, c);
                    state = tBiggerThan;
                }
                else if (c == ';') {
                    exit(SYN_ERROR);; //semicolon is not part of expression
                }
                else if (c == ',') {
                    exit(SYN_ERROR);; //comma is not part of expression
                }
                else if (c == '(') {
                    add_to_string(&tokenText, c);
                    _save_to_token(token, &tokenText, tOpeningSimpleBrace, startIndex + i, L);
                    return;

                }
                else if (c == ')') {
                    add_to_string(&tokenText, c);
                    _save_to_token(token, &tokenText, tClosingSimpleBrace, startIndex + i, L);
                    return;
                }
                else if (c == '{') {
                    exit(SYN_ERROR);;
                }
                else if (c == '}') {
                    exit(SYN_ERROR);;
                }
                else if (c == EOF) {
                    add_to_string(&tokenText, '$');
                    _save_to_token(token, &tokenText, tEOF, startIndex + i, L);;
                    return;
                }
                break;
            case tBiggerThan:
                if (c == '=') {
                    add_to_string(&tokenText, c);
                    _save_to_token(token, &tokenText, tBiggerOrEqual, startIndex + i, L);
                    return;
                }
                else if (!comparison_assumption(c)) {
                    exit(LEX_ERROR);
                }
                _save_to_token(token, &tokenText, tBiggerThan, startIndex + i - 1, L);
                    return;
            case tSmallerThan:
                if (c == '=') {
                    add_to_string(&tokenText, c);
                    _save_to_token(token, &tokenText, tSmallerOrEqual, startIndex + i, L);
                    return;
                }
                else if (!comparison_assumption(c)) {
                    exit(LEX_ERROR);
                }
                _save_to_token(token, &tokenText, tSmallerThan, startIndex + i - 1, L);
                return;
            case tAssignment:
                if (c == '=') {
                    add_to_string(&tokenText, c);
                    _save_to_token(token, &tokenText, tEqual, startIndex + i, L);
                    return;
                }
                else {
                    exit(SYN_ERROR);; //assignment not supported in expression
                }
                break;
            case tNotEqual:
                if (c == '=') {
                    add_to_string(&tokenText, c);
                    _save_to_token(token, &tokenText, tNotEqual, startIndex + i, L);
                    return;
                }
                exit(LEX_ERROR);
            case string_start:
                if (c == '"') {
                    _save_to_token(token, &tokenText, tString, startIndex + i, L);
                    printf("save to %s\n", token->text->str);
                    return;
                }
                else if (c == '\\') {
                    add_to_string(&tokenText, c);
                    state = string_special;
                }
                else {
                    add_to_string(&tokenText, c);
                }
                break;
            case string_special:
                if (c == 'x') {
                    add_to_string(&tokenText, c);
                    state = string_hex1;
                }
                else if (c == 'n' ||
                         c == 't' ||
                         c == '"' ||
                         c == '\\') {
                    add_to_string(&tokenText, c);
                    state = string_start;
                }
                else {
                    exit(LEX_ERROR);
                }
                break;
            case string_hex1:
                if (is_hex_char(c)) {
                    add_to_string(&tokenText, c);
                    state = string_hex2;
                }
                else {
                    exit(LEX_ERROR);
                }
                break;
            case string_hex2:
                if (is_hex_char(c)) {
                    add_to_string(&tokenText, c);
                    state = string_start;
                }
                else {
                    exit(LEX_ERROR);
                }
                break;
            case tInteger:
                if (is_num(c)) {
                    add_to_string(&tokenText, c);
                    printf("\n add to string\n");
                }
                else if (c == '.') {
                    add_to_string(&tokenText, c);
                    state = float_point;
                }
                else if (c == 'e' || c == 'E') {
                    add_to_string(&tokenText, c);
                    state = float_exponent;
                }
                else if (is_alpha(c)) {
                     exit(LEX_ERROR);
                }
                else {
                    _save_to_token(token, &tokenText, tInteger, startIndex + i - 1, L);
                    return;
                }
                break;
            case float_point:
                // 25. 22222.
                if (is_num(c)) {
                    add_to_string(&tokenText, c);
                    state = float_point_number;
                }
                else {
                    exit(LEX_ERROR);
                }
                break;
            case float_point_number:
                // 25.2 222.2
                if (is_num(c)) {
                    add_to_string(&tokenText, c);
                }
                else if (c == 'e' || c == 'E') {
                    add_to_string(&tokenText, c);
                    state = float_exponent;
                }
                else {
                    _save_to_token(token, &tokenText, tFloat, startIndex + i - 1, L);
                    return;
                }
                break;
            case float_exponent:
                // 25.2E 25E
                if (is_num(c)) {
                    add_to_string(&tokenText, c);
                    state = float_exponent_number;
                }
                else if (c == '+' || c == '-') {
                    add_to_string(&tokenText, c);
                    state = float_singed_exponent;
                }
                else if (!number_ending(c)) {
                    exit(LEX_ERROR);
                }
                else {
                    _save_to_token(token, &tokenText, tFloat, startIndex + i - 1, L);
                    return;
                }
                break;
            case float_exponent_number:
                // 25E2 25.2E2
                if (is_num(c)) {
                    add_to_string(&tokenText, c);
                }
                else if (!number_ending(c)) {
                    exit(LEX_ERROR);
                }
                else {
                    _save_to_token(token, &tokenText, tFloat, startIndex + i - 1, L);
                    return;
                }
                break;
            case float_singed_exponent:
                if (is_num(c)) {
                    add_to_string(&tokenText, c);
                    state = tFloat;
                }
                else {
                    
                    exit(LEX_ERROR);
                }
                break;
            case tFloat:
                if (is_num(c)) {
                    add_to_string(&tokenText, c);
                }
                else {
                    _save_to_token(token, &tokenText, tFloat, startIndex + i - 1, L);
                    return;
                }
                break;
            case unknown_identifier:
                if (c == '_') {
                    add_to_string(&tokenText, c);
                    state = tId;
                }
                else if (is_alphanumeric(c)) {
                    add_to_string(&tokenText, c);
                }
                else {
                    if (is_keyword(&tokenText)) {
                        state = tKeyword;
                    }
                    else if (is_built_in_func(&tokenText)) {
                        state = tBuiltIn;
                    } else {
                        state = tId;
                        
                    }
                    printf("save\n");
                    _save_to_token(token, &tokenText, state, startIndex + i - 1, L);
                    printf("notsave\n");
                    return;
                }
                break;
            case tId:
                if (is_alphanumeric(c) || c == '_') {
                    add_to_string(&tokenText, c);
                }
                else{
                    _save_to_token(token, &tokenText, tId, startIndex + i - 1, L);
                    return;

                }
                break;
            case tDivide:
                if (c == '*') {
                    exit(SYN_ERROR);;  //block comment not supported in expression
                }
                else if (c == '/') {
                    exit(SYN_ERROR);;  //line comment not supported in expression
                    
                }
                else {
                    _save_to_token(token, &tokenText, tDivide, startIndex + i - 1, L);
                    return;
                    
                }
                break;
            default:
                continue;
        }
        i++;
        
    }
    
} 

void TokenLLInsert(tLinkedList *L, tToken *token){
    tListItem *new_node = malloc(sizeof(tListItem));
    new_node->Content = malloc(sizeof(tToken));

    ((tToken*)new_node->Content)->text = token->text;
    ((tToken*)new_node->Content)->endIndex = token->endIndex;
    ((tToken*)new_node->Content)->type = token->type;
    ((tToken*)new_node->Content)->dataType = token->dataType;

    new_node->nextItem = NULL;
    
    if (!L->first){
        L->first = new_node;
      //  printf("Token test %s\n", token->text->str);
 //   printf("first %s\n", ((tToken*)(L->first->Content))->text.str);
        return;
    }
    //printf("Token test %s\n", token->text->str);
    //printf("%d\n", ix);
    tListItem *curr = L->first;
    while (curr->nextItem){
        curr = curr->nextItem;
    }
    
    curr->nextItem = new_node;
}

tLinkedList* get_tokens(string *s){
    tLinkedList *L = malloc(sizeof(tLinkedList));
    StrLLInit(L);
    int i = 0;  
    tToken *t = malloc(sizeof(tToken));
    do {
        printf("index %d %s\n", i, s->str);
        get_tokenExp(t, s, i);
        printf("BEFOREINSERT---\n");
        TokenLLInsert(L, t);
        //printf("------tdatatype %d\n", t->dataType);
        i = t->endIndex + 1;
        printf("endindex %d, s.len %d\n", t->endIndex, s->length);
        
        //printf("%s %d %d %d\n", t.text.str, t.type, t.endIndex, s->length);
    } while (t->endIndex + 1 < s->length);
    /*tToken t = get_tokenExp(s, i);
    printf("Token %s\n", t.text->str);
    while(t.type != EOL || t.type != tEOF){ 
    TokenLLInsert(&L, t);
    //printf("\n %p\n", &t);
    t = get_tokenExp(s,t.endIndex + 1);
    if(i>1){ 
        tToken* test;
        test = (tToken*)L.first->Content;
        printf("L: %s and %s\n", (test->text)->str, ((tToken*)(L.first->nextItem->nextItem->Content))->text->str);
    }
    printf("Token %s\n", t.text->str);
    i++;
    }*/
    printf("nextt\n");
  
    //printf("%s", ((tToken*)L.first->Content)->text.str);
    return L;
}
