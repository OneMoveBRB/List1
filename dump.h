#ifndef DUMP_H
#define DUMP_H

#include "list.h"

void ListDump(List_t* list);
ListErr_t DotVizualizeList(const List_t* list, const char* filename);

#endif