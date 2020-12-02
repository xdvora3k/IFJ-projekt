#include <stdio.h>
#include "../src/expressionParser.h"


int main(){
    tToken* myToken;
    string s;
    init_string(&s);
    adds_to_string(&s, "(46+ab/5)");

    int i = 0;
   /* int index = 0;
    myToken = get_tokenExp(&s, index);
    printf("(a+b)");
    printf("%s and %d\n", myToken.text->str, myToken.type);
    index = 1;
    myToken = get_tokenExp(&s, index);
    printf("%s and %d\n", myToken.text->str, myToken.type);*/

    tLinkedList *List = get_tokens(&s);
    //printf("%s %s", ((tToken*)List.first->Content)->text.str, "a");
    //printf("\n N\n");
    tListItem* listItem = List->first;
    myToken = (tToken*)listItem->Content;
    //printf("List: %s", myToken.text->str);
    printf("I %d %s\n", i, (myToken->text).str);
   // printf("Index = 2\n");//%d and %s\n", i, (myToken->text).str);

    while (listItem->nextItem)
    {
        i++;
        listItem = listItem->nextItem;
        myToken = (tToken*)listItem->Content;
        printf("I %d %s\n", i, (myToken->text).str);

    }
    return 0;

}