#include "../src/expression.h"
#include "../src/scanner.h"
#include <stdio.h>


int main(){
   string input;
   init_string(&input);
   adds_to_string(&input, "a+v*c");
   precedencSA(&input);
   return 0;
}
