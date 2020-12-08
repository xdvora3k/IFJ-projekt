#include "../src/expression.h"
#include "../src/scanner.h"
#include <stdio.h>


int main(){
  tExpressionList *L = malloc(sizeof(tExpressionList));
  tLinkedList *list;
  ExprLLInit(L);
  //TableLLInit(list);
  printf("here\n");
  string input;         //noda a node b = a+9 , t*5-9
   init_string(&input);
   printf("\n");
   adds_to_string(&input, "8<0");   //a+v*c //"58-o+1+9" //8-9+5*8/(86+9)
   printf("input %s . %d\n", input.str, input.length);
  // L->first->data_type = IntType;
   precedencSA(&input, L ,list);
   printf("\n");

 /* string errorstr;
  int i;
   init_string(&errorstr);
   printf("\n");
   adds_to_string(&errorstr, "8+9.2");   //a+v*c //"58-o+1+9"
   printf("input %s . %d\n", errorstr.str, errorstr.length);
  // L->first->data_type = IntType;
  list = get_tokens(&errorstr);
      tListItem* listItem = list->first;
   tToken* myToken = (tToken*)listItem->Content;
    //printf("List: %s", myToken.text->str);
    printf("I %d %s \n", i, (myToken->text)->str);
   // printf("Index = 2\n");//%d and %s\n", i, (myToken->text).str);

    while (listItem->nextItem)
    {
        i++;
        listItem = listItem->nextItem;
        myToken = (tToken*)listItem->Content;
        printf("I %d %s %d\n", i, (myToken->text)->str, myToken->dataType);

    }


   precedencSA(&errorstr, L ,list);
   printf("\n");*/

   /*string input3;
   init_string(&input3);
   printf("\n");
   adds_to_string(&input3, "e*d-3");
   printf("Input3 %s . %d\n", input3.str, input3.length);
   precedencSA(&input3, L ,list);
   printf("\n");*/

/*   string input4;
   init_string(&input4);
   adds_to_string(&input4, "b+4, r*t, 4-8");
   printf("Input4 %s . %d\n", input4.str, input4.length);
   precedencSA(&input4);
*/
   /*string input5;
   init_string(&input5);
   printf("\n");
   adds_to_string(&input5, "e*d/3");
   printf("Input3 %s . %d\n", input5.str, input5.length);
   precedencSA(&input5, L, list);

   string input6;
   init_string(&input6);
   printf("\n");
   adds_to_string(&input6, "e+d/3-4+7/3");
   printf("Input3 %s . %d\n", input6.str, input6.length);
   precedencSA(&input6, L, list);

   string input2;
   init_string(&input2);
   printf("\n");
   adds_to_string(&input2, "(b+4) > (c-o)");    //b < c  || (b+4) > (c-o)
   printf("Input2 %s . %d\n", input2.str, input2.length);
   precedencSA(&input2, L, list);

   string input9;
   init_string(&input9);
   printf("\n");
   adds_to_string(&input9, "b+4 > c-o");    //tested: !=, <, == 
   printf("Input2 %s . %d\n", input9.str, input9.length);
   precedencSA(&input9, L, list);

   string input10;
   init_string(&input10);
   printf("\n");
   adds_to_string(&input10, "4 == c-o");    
   printf("Input2 %s . %d\n", input10.str, input10.length);
   precedencSA(&input10, L, list);*/

 /*  string input11;
   init_string(&input11);
   printf("\n");
   adds_to_string(&input11, "e = 9"); //error   
   printf("Input2 %s . %d\n", input11.str, input11.length);
   precedencSA(&input11);
*/
/*tToken * tok;
string testinp;
init_string(&testinp);
add_to_string(&testinp, "p");
tok->text = &testinp;
    StrLLInsert(list, tok->text);*/


   string input12;
   init_string(&input12);
   printf("\n");
   adds_to_string(&input12, "+8 = 9"); //error   
   printf("Input2 %s . %d\n", input12.str, input12.length);
   precedencSA(&input12,L, list);
/*
  string input13;
   init_string(&input13);
   printf("\n");
   adds_to_string(&input13, "5/0"); //error   
   printf("Input2 %s . %d\n", input13.str, input13.length);
   precedencSA(&input13, L, list);*/
   
 /* string inputa;
   init_string(&inputa);
   printf("\n");
   adds_to_string(&inputa, "\"hej\" != \"op\"");    
   printf("Input2 %s . %d\n", inputa.str, inputa.length);
   precedencSA(&inputa, L, list);
*/
   return 0;
}
