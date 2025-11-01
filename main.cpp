#include <stdio.h>

#include "list.h"
#include "dump.h"


int main() {
    List_t list;
    ListInit(&list);

    ListInsertAfter(&list, 0, 10);
    ListInsertAfter(&list, 1, 20);
    ListInsertAfter(&list, 2, 30);
    ListInsertAfter(&list, 3, 40);
    ListInsertAfter(&list, 4, 50);
    ListInsertAfter(&list, 5, 60);

    // ListInsertAfter(&list, 6, 555);
    // ListInsertBefore(&list, 7, 444);
    // ListDeleteAt(&list, 6);

    ListDump(&list);

    DotVizualizeList(&list, "dotstvo.txt");

    ListDestroy(&list);

    return 0;
}