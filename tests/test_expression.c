#include "../src/expression.h"
#include "../src/scanner.h"
#include <stdio.h>


int main(){
   string input;
   init_string(&input);
   input.str = "a+a*a";
   input.length = 5;
   precedencSA(&input);
   return 0;
}
