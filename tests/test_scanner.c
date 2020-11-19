#include <stdio.h>
#include "../src/scanner.h"
#include "../src/str.h"
#include "../src/error_codes.h"

char* get_name(int i){
    switch (i){
        case 2: return "tId";
        case 3: return "tKeyword";
        case 4: return "tBuiltIn";
        case 5: return "tInteger";
        case 11: return "tDouble";
        case 16: return "tString";
        case 17: return "tPlus";
        case 18: return "tMinus";
        case 19: return "tMultiply";
        case 20: return "tDivide";
        case 21: return "tDeclaration";
        case 22: return "tAssignment";
        case 23: return "tEqual";
        case 24: return "tNotEqual";
        case 25: return "tBiggerThan";
        case 26: return "tSmallerThan";
        case 27: return "tBiggerOrEqual";
        case 28: return "tSmallerOrEqual";
        case 29: return "tComma";
        case 30: return "tSemicolon";
        case 31: return "tOpeningSimpleBrace";
        case 32: return "tClosingSimpleBrace";
        case 33: return "tOpeningCurlyBrace";
        case 34: return "tClosingCurlyBrace";
        case 41: return "";
        case 1000: return "LEX_ERROR";
    }
    return "Unknown";
}

int main(){
    //FILE *file = fopen(argv[1], "r");
    set_source_file(stdin);

    string token;
    init_string(&token);
    int token_type;
    while (1){
        token_type = get_token(&token);
        if (token_type == tEOF){
            return 0;
        }
        else if (token_type == tLEX_ERROR){
            printf("%s\n", token.str);
            return 1;
        }
        printf("%s\n", get_name(token_type));
    }

}
