
#include "../src/expression.h"
#include "../src/scanner.h"
#include <stdio.h>


int main(){
bool test1;
string text;
init_string(&text);
//adds_to_string(&text,"a");
text.str = "abb";

/*FILE *source;
set_source_file(source);*/

//text.str = "a + b";
   // test1 = chceckBracket(&text);
    /*printf("text %d\n", test1);
    //text.str = "(a+b)";
    clear_str(&text);
    adds_to_string(&text,"(a+b)");
    test1 = chceckBracket(&text);
    printf("text %d\n", test1);
    //text.str = "(a+b";
    clear_str(&text);
    adds_to_string(&text,"(a+b");
    test1 = chceckBracket(&text);
    printf("text %d\n", test1);*/
  get_token(&text);

 /* printf("result %d\n", result);*/
    return 0;
}
