
#include "expressionParser.h"



void get_tokenExp(tToken* token, string *input, int startIndex){
    tState state = start;
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
                    token->text = tokenText;
                    token->type = tPlus;
                    token->endIndex = startIndex + i;
                    return;
                }
                else if (c == '-') {
                    add_to_string(&tokenText, c);
                    token->text = tokenText;
                    token->type = tMinus;
                    token->endIndex = startIndex + i;
                    return;
                }
                else if (c == '*') {
                    add_to_string(&tokenText, c);
                    token->text = tokenText;
                    token->type = tMultiply;
                    token->endIndex = startIndex + i;
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
                    token->text = tokenText;
                    token->type = tOpeningSimpleBrace;
                    token->endIndex = startIndex + i;
                    return;

                }
                else if (c == ')') {
                    add_to_string(&tokenText, c);
                    token->text = tokenText;
                    token->type = tClosingSimpleBrace;
                    token->endIndex = startIndex + i;
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
                    token->text = tokenText;
                    token->type = tEOF;
                    token->endIndex = startIndex + i;
                    return;
                }
                break;
            case tBiggerThan:
                if (c == '=') {
                    add_to_string(&tokenText, c);
                    token->text = tokenText;
                    token->type = tBiggerOrEqual;
                    token->endIndex = startIndex + i;
                    return;
                }
                else if (!comparison_assumption(c)) {
                    exit(LEX_ERROR);
                }
                    token->text = tokenText;
                    token->type = tBiggerThan;
                    token->endIndex = startIndex + i - 1;
                    return;
            case tSmallerThan:
                if (c == '=') {
                    add_to_string(&tokenText, c);
                    token->text = tokenText;
                    token->type = tSmallerOrEqual;
                    token->endIndex = startIndex + i;
                    return;
                }
                else if (!comparison_assumption(c)) {
                    exit(LEX_ERROR);
                }
                token->text = tokenText;
                    token->type = tSmallerOrEqual;
                    token->endIndex = startIndex + i - 1;
                    return;
            case tAssignment:
                if (c == '=') {
                    add_to_string(&tokenText, c);
                    token->text = tokenText;
                    token->type = tEqual;
                    token->endIndex = startIndex + i;
                    return;
                }
                else {
                    exit(SYN_ERROR);; //assignment not supported in expression
                }
                break;
            case tNotEqual:
                if (c == '=') {
                    add_to_string(&tokenText, c);
                    token->text = tokenText;
                    token->type = tNotEqual;
                    token->endIndex = startIndex + i;
                    return;
                }
                exit(LEX_ERROR);
            case string_start:
                if (c == '"') {
                    token->text = tokenText;
                    token->endIndex = startIndex + i -1; //-1 == nepatri token ktery
                    token->type = tString;
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
                    token->text = tokenText;
                    token->endIndex = startIndex + i -1; //-1 == nepatri token ktery
                    token->type = tInteger;
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
                    token->text = tokenText;
                    token->endIndex = startIndex + i -1; //-1 == nepatri token ktery
                    token->type = tFloat;
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
                    token->text = tokenText;
                    token->endIndex = startIndex + i -1; //-1 == nepatri token ktery
                    token->type = tFloat;
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
                    token->text = tokenText;
                    token->endIndex = startIndex + i -1; //-1 == nepatri token ktery
                    token->type = tFloat;
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
                    token->text = tokenText;
                    token->endIndex = startIndex + i -1; //-1 == nepatri token ktery
                    token->type = tFloat;
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
                        token->type = tKeyword;
                    }
                    else if (is_built_in_func(&tokenText)) {
                        token->type = tBuiltIn;
                    } else {
                        token->type = tId;
                        
                    }
                    token->text = tokenText;
                    token->endIndex = startIndex + i -1; //-1 == nepatri token ktery
                    return;

                }
                break;
            case tId:
                if (is_alphanumeric(c) || c == '_') {
                    add_to_string(&tokenText, c);
                }
                else{
                    token->type = tId;
                    token->text = tokenText;
                    token->endIndex = startIndex + i -1; //-1 == nepatri token ktery
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
                    token->text = tokenText;
                    token->type = tDivide;
                    token->endIndex = startIndex + i - 1;
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

    new_node->nextItem = NULL;
    
    if (!L->first){
        L->first = new_node;
//        printf("Token test %s\n", token->text.str);
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
        printf("index %d\n", i);
        get_tokenExp(t, s, i);
        TokenLLInsert(L, t);
        i = t->endIndex + 1;
        
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
