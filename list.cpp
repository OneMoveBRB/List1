#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// FIXME enum error -> string

ListErr_t ListInit(List_t* list) {
    assert( list != NULL );

    list->arr.size = 0;//FIXME
    list->arr.capacity = FIRST_SIZE;
    list->arr.data = (Node*)calloc(FIRST_SIZE, sizeof(Node));
    if (list->arr.data == NULL) {
        return LIST_OVERFLOW;
    }

    list->free = 1;

    for (size_t i = list->free; i < FIRST_SIZE - 1; i++) {
        list->arr.data[i].next = i + 1;
        list->arr.data[i].prev = UNINITIALIZED;
    }   
    
    list->arr.data[FIRST_SIZE - 1].next = FakeElemIdx;
    list->arr.data[FIRST_SIZE - 1].prev = UNINITIALIZED;
    list->arr.data[FakeElemIdx].next = FakeElemIdx;
    list->arr.data[FakeElemIdx].prev = FakeElemIdx;

    return LIST_OK;
}

ListErr_t ListUpdate(List_t* list) {
    assert( list != NULL );
    fprintf(stderr, "YESS\n");

    const size_t exp_multiplier = 2;
    
    Node* new_data = NULL;

    new_data = (Node*)realloc(list->arr.data, list->arr.capacity * exp_multiplier * sizeof(Node));

    if (new_data == NULL) {
        return LIST_OVERFLOW;
    }

    list->arr.capacity *= exp_multiplier;
    list->arr.data = new_data;
    new_data = NULL;

    return LIST_OK;
}

ListErr_t ListDestroy(List_t* list) {
    assert( list != NULL );

    list->arr.size = 0;
    list->arr.capacity = 0;
    FREE(list->arr.data);

    list->free = 0;

    return LIST_OK;
}

type ListGet(List_t* list, size_t position) {
    assert( list != NULL );
    assert( position < list->arr.size );

    return list->arr.data[position].value;
}

size_t ListNext(const List_t* list, size_t position) {
    assert( list != NULL );
    // assert( position <= list->arr.size );

    return list->arr.data[position].next;
}

size_t ListPrev(const List_t* list, size_t position) {
    assert( list != NULL );
    // assert( position < list->arr.size );

    return list->arr.data[position].prev;
}

size_t ListFront(const List_t* list) {
    assert( list != NULL );

    return list->arr.data[FakeElemIdx].next;
}

size_t ListEnd(const List_t* list) {
    assert( list != NULL );

    return list->arr.data->prev;
}

ListErr_t ListSet(List_t* list, size_t position, type value) {
    assert( list != NULL );
    assert( position < list->arr.size );

    list->arr.data[position].value = value;

    return LIST_OK;
}

ListErr_t ListInsertBefore(List_t* list, size_t position, type value) {
    assert( list != NULL );

    if ( list->arr.data[position].prev == UNINITIALIZED ) {
        return LIST_INSERT_BEFORE_UNINITIALIZED;// FIXME
    }

    return ListInsertAfter(list, list->arr.data[position].prev, value);
}

ListErr_t ListInsertAfter(List_t* list, size_t position, type value) {
    assert( list != NULL );

    if ( list->arr.data[position].prev == UNINITIALIZED ) {
        return LIST_INSERT_AFTER_UNINITIALIZED; // FIXME
    }

    size_t prev_position = position;
    Node*  prev_node     = &list->arr.data[prev_position];

    size_t next_position = prev_node->next;
    Node*  next_node     = &list->arr.data[next_position];

    size_t free_position = list->free;
    Node*  free_node     = &list->arr.data[free_position];

    list->free = free_node->next;

    free_node->value = value;

    prev_node->next = free_position;
    free_node->prev = prev_position;

    next_node->prev = free_position;
    free_node->next = next_position;

    ++list->arr.size;

    printf("%zu : %zu\n", list->arr.size, list->arr.capacity);

    if ( list->arr.size + 2 == list->arr.capacity ) { //FIXME
        return ListUpdate(list);
    }

    return LIST_OK;
}

ListErr_t ListDeleteAt(List_t* list, size_t position) {
    assert( list != NULL );
    
    if (position == 0 || list->arr.data[position].prev == UNINITIALIZED) {
        return LIST_INDEX_OUT_OF_RANGE;
    }

    size_t del_position = position;
    Node*  del_node     = &list->arr.data[del_position];
    
    size_t prev_position = del_node->prev;
    Node*  prev_node     = &list->arr.data[prev_position];
    
    size_t next_position = del_node->next;
    Node*  next_node     = &list->arr.data[next_position];

    del_node->value = 0;
    del_node->prev = UNINITIALIZED;

    del_node->next = list->free;
    // list->arr.data[list->free].prev = UNINITIALIZED;
    list->free = del_position;
    
    prev_node->next = next_position;
    next_node->prev = prev_position;

    --list->arr.size;

    return LIST_OK;
}


// ListErr_t ListInsertBefore(List_t* list, size_t position, type value) { // FIXME insertAfter after prev
//     assert( list != NULL );
    
//     if ( list->arr.data[position].prev == UNINITIALIZED ) {
//         return LIST_INSERT_BEFORE_UNINITIALIZED;// FIXME
//     }

//     size_t next_position = position;
//     Node*  next_node     = &list->arr.data[next_position];

//     size_t prev_position = next_node->prev;
//     Node*  prev_node     = &list->arr.data[prev_position];

//     size_t free_position = list->free;
//     Node*  free_node     = &list->arr.data[free_position];

//     list->free = free_node->next;

//     free_node->value = value;

//     prev_node->next = free_position;
//     free_node->prev = prev_position;
    
//     next_node->prev = free_position;
//     free_node->next = next_position;

//     ++list->arr.size;

//     return LIST_OK;
// }