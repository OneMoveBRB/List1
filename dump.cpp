#include "dump.h"

#include <stdio.h>
#include <assert.h>

void ListDump(List_t* list) { // FIXME используй написанные функции
    FILE* fp = fopen("List_test.txt", "a"); // FIXME file name в константу
    if (fp == NULL) {
        fprintf(stderr, "Couldn't open  List_test.txt\n");
        return;
    }

    fprintf(fp, "\n############## LIST DUMP ##############\n");
    fprintf(fp, "List size: %zu\nList capacity: %zu\nList data: ", 
                list->arr.size, list->arr.capacity);
    
    for (size_t i = 0; i < list->arr.capacity; i++) {
        fprintf(fp, "%d ", list->arr.data[i].value);
    } fprintf(fp, "\n");

    fprintf(fp, "List next: ");

    for (size_t i = 0; i < list->arr.capacity; i++) {
        fprintf(fp, "%zu ", list->arr.data[i].next);
    } fprintf(fp, "\n");

    fprintf(fp, "List prev: ");

    for (size_t i = 0; i < list->arr.capacity; i++) {
        fprintf(fp, "%zu ", list->arr.data[i].prev);
    } fprintf(fp, "\n");

    fprintf(fp, "List HEAD: %zu\nList TAIL: %zu\nList FREE: %zu\n", 
                list->arr.data->next, list->arr.data->prev, list->free);

    return;
}

ListErr_t DotVizualizeList(const List_t* list, const char* filename) {
    assert( list != NULL );
    assert( filename != NULL );

    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        return LIST_FILE_OPEN_FAILED; 
    }

    fprintf(fp, "digraph List {\n\t");
    fprintf(fp, "rankdir=LR;\n\t");
    fprintf(fp, "node [shape=record, style=filled, fillcolor=lightblue];\n\t");
    fprintf(fp, "edge [fontsize=10,  color=black];\n\n\t");

    for (size_t i = 0; i != list->arr.capacity; i++) {
        Node* cur_node = &list->arr.data[i];
        
        if (cur_node->prev == UNINITIALIZED) {
            fprintf(fp, "node%zu [label=\"<f0> value: %d | <f1> idx: %zu | {<f2>  prev: POIZON | <f3> next: %zu}\", fillcolor=pink];\n\t",
                    i, cur_node->value, i, cur_node->next);
        } else {
            fprintf(fp, "node%zu [label=\"<f0> value: %d | <f1> idx: %zu | {<f2>  prev: %zu | <f3> next: %zu}\"];\n\t",
                    i, cur_node->value, i, cur_node->prev, cur_node->next);
        }

    } 
    
    fprintf(fp, "\n\t");

    // for (size_t i = 0; i != list->arr.capacity - 1; i++) {
    //     fprintf(fp, "node%zu:f3 -> node%zu:f2 [color=white, arrowhead=normal];\n    ",
    //                     i, i+1);
    //     fprintf(fp, "node%zu:f3 -> node%zu:f2 [color=white, arrowhead=normal];\n    ",
    //                     i+1, i);
    // }

    // fprintf(fp, "\n    ");

    int is_arrows_diverge = 0;

    // list->arr.data[2].prev = 3;

    for (size_t cur_pos = FakeElemIdx; cur_pos != ListEnd(list); cur_pos = ListNext(list, cur_pos)) {
        Node* cur_node  = &list->arr.data[cur_pos];
        Node* next_node = &list->arr.data[cur_node->next];

        if (cur_pos == next_node->prev) {
            fprintf(fp, "node%zu:f3 -> node%zu:f2 [color=green, dir=both, arrowhead=normal];\n\t",
                        cur_pos, cur_node->next);
        } else {
            is_arrows_diverge = 1;

            fprintf(fp, "node%zu:f3 -> node%zu:f2 [color=blue, arrowhead=normal];\n\t",
                        cur_pos, cur_node->next);
        }
    } 
    
    fprintf(fp, "\n\t");

    if (is_arrows_diverge) {
        size_t cur_pos = ListFront(list);

        do {
            cur_pos = ListNext(list, cur_pos);

            Node* cur_node  = &list->arr.data[cur_pos];
            Node* prev_node = &list->arr.data[cur_node->prev];

            if (cur_pos != prev_node->next) {
                fprintf(fp, "node%zu:f2 -> node%zu:f3 [color=red, arrowhead=normal];\n\t",
                            cur_pos, cur_node->prev);
            }

        } while (cur_pos != ListEnd(list));

        fprintf(fp, "\n\t");
    }

    fprintf(fp, "head [shape=plaintext, label=\"HEAD\"];\n\t");
    fprintf(fp, "head -> node%zu [color=green, style=bold];\n\t", ListFront(list));

    fprintf(fp, "tail [shape=plaintext, label=\"TAIL\"];\n\t");
    fprintf(fp, "tail -> node%zu:f0 [color=orange, style=bold];\n\t", ListEnd(list));

    fprintf(fp, "free [shape=plaintext, label=\"FREE\"];\n\t");
    fprintf(fp, "free -> node%zu [color=purple, style=bold];\n\t", list->free);

    fprintf(fp, "\n\t");

    if (list->free != FakeElemIdx) {
        for (size_t cur_pos = list->free; ListNext(list, cur_pos) != FakeElemIdx; cur_pos = ListNext(list, cur_pos)) {
            fprintf(fp, "node%zu -> node%zu [color=\"purple\", style=bold];\n\t", 
                        cur_pos, ListNext(list, cur_pos));
        }
    }

    if (list->arr.data[FakeElemIdx].next != FakeElemIdx) {
        fprintf(fp, "node0:f3 -> node%zu:f2 [color=white, dir=both, arrowhead=normal];\n\t",
                    ListFront(list));
        fprintf(fp, "node%zu:f3 -> node%zu:f2 [color=white, arrowhead=normal];\n\t",
                    ListPrev(list, ListEnd(list)), ListEnd(list));
    }

    fprintf(fp, "node%zu:f3 -> node%zu:f2 [color=white, arrowhead=normal];\n\t",
                ListEnd(list), list->free);
    fprintf(fp, "node%zu:f3 -> node%zu:f2 [color=white, arrowhead=normal];\n}",
                ListEnd(list), list->free);

    return LIST_OK;
}
