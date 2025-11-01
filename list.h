#ifndef LIST_H
#define LIST_H

#include <stddef.h>

// FIXME сделать реалок списка

#define FREE(ptr) free(ptr); ptr = NULL;

const int FIRST_SIZE    = 8;
const int UNINITIALIZED = 0xBAD;
const int FakeElemIdx   = 0;

typedef int type;

struct Node {
    type value;
    size_t next;
    size_t prev;
};

struct Nodes {
    Node* data;
    size_t size;
    size_t capacity;
};

struct List_t {
    Nodes  arr;
    size_t free;
};

enum ListErr_t {
    LIST_OK,
    LIST_OVERFLOW,
    LIST_INSERT_BEFORE_UNINITIALIZED,
    LIST_INSERT_AFTER_UNINITIALIZED,
    LIST_INDEX_OUT_OF_RANGE,
    LIST_FILE_OPEN_FAILED
};

ListErr_t ListInit(List_t* list);
ListErr_t ListUpdate(List_t* list);
ListErr_t ListDestroy(List_t* list);

type ListGet(List_t* list, size_t position);

size_t ListNext(const List_t* list, size_t position);
size_t ListPrev(const List_t* list, size_t position);
size_t ListFront(const List_t* list);
size_t ListEnd(const List_t* list);

ListErr_t ListSet(List_t* list, size_t position, type value);
ListErr_t ListInsertBefore(List_t* list, size_t position, type value);
ListErr_t ListInsertAfter(List_t* list, size_t position, type value);
ListErr_t ListDeleteAt(List_t* list, size_t position);


#endif // LIST_H