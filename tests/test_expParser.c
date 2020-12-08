#include <stdio.h>
#include "../src/expressionParser.h"


int main(){
    tToken* myToken;
    string s;
    init_string(&s);
    adds_to_string(&s, "\"hej\" != \"op\""); //"\"hej\" != \"op\""
    printf("length str %d\n", s.length);
    for(int i = 0; i < 18; i++){
        printf("on index %d is %c\n", i, s.str[i]);
    }

    int i = 0;
    tLinkedList *L = malloc(sizeof(tLinkedList));
    StrLLInit(L);
    tLinkedList *List = get_tokens(&s, L);
    tListItem* listItem = List->first;
    myToken = (tToken*)listItem->Content;
    printf("I %d %s\n", i, (myToken->text)->str);

    while (listItem->nextItem)
    {
        i++;
        listItem = listItem->nextItem;
        myToken = (tToken*)listItem->Content;
        printf("I %d %s\n", i, (myToken->text)->str);

    }
    
    return 0;

}