#include "../src/expression.h"
#include "../src/scanner.h"
#include <stdio.h>


int main(){
   string input;
   init_string(&input);
   printf("\n");
   adds_to_string(&input, "1+2+5");   //a+v*c //"58-o+1+9"
   printf("input %s . %d\n", input, input.length);
   precedencSA(&input);
   printf("\n");

   string input3;
   init_string(&input3);
   printf("\n");
   adds_to_string(&input3, "e*d-3");
   printf("Input3 %s . %d\n", input3.str, input3.length);
   precedencSA(&input3);
   printf("\n");

   /*string input4;
   init_string(&input4);
   adds_to_string(&input4, "b+4, r*t, 4-8");
   printf("Input4 %s . %d\n", input4.str, input4.length);
   precedencSA(&input4);*/

   string input5;
   init_string(&input5);
   printf("\n");
   adds_to_string(&input5, "e*d/3");
   printf("Input3 %s . %d\n", input5.str, input5.length);
   precedencSA(&input5);

   string input6;
   init_string(&input6);
   printf("\n");
   adds_to_string(&input6, "e+d/3-4+7/3");
   printf("Input3 %s . %d\n", input6.str, input6.length);
   precedencSA(&input6);

   string input2;
   init_string(&input2);
   printf("\n");
   adds_to_string(&input2, "(b+4) > (c-o)");    //b < c  || (b+4) > (c-o)
   printf("Input2 %s . %d\n", input2.str, input2.length);
   precedencSA(&input2);

   string input9;
   init_string(&input9);
   printf("\n");
   adds_to_string(&input9, "b+4 > c-o");    //tested: !=, <, == 
   printf("Input2 %s . %d\n", input9.str, input9.length);
   precedencSA(&input9);

   string input10;
   init_string(&input10);
   printf("\n");
   adds_to_string(&input10, "4 == c-o");    
   printf("Input2 %s . %d\n", input10.str, input10.length);
   precedencSA(&input10);

 /*  string input11;
   init_string(&input11);
   printf("\n");
   adds_to_string(&input11, "e = 9"); //error   
   printf("Input2 %s . %d\n", input11.str, input11.length);
   precedencSA(&input11);
*/
   string input12;
   init_string(&input12);
   printf("\n");
   adds_to_string(&input12, "+p = 9"); //error   
   printf("Input2 %s . %d\n", input12.str, input12.length);
   precedencSA(&input12);
   return 0;
}
