#include "../src/expression.h"
#include "../src/scanner.h"
#include <stdio.h>


int main(){
   string input;
   init_string(&input);
   adds_to_string(&input, "a+v*c");
   printf("input %s . %d\n", input, input.length);
   precedencSA(&input);
   return 0;
}
