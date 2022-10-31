#ifndef __ARRAYLIST_H
#define __ARRAYLIST_H

#include <malloc.h>

typedef struct arraylist {
    int length;
    int size;
    int multiplier;
    void ** array;
} ArrayList;



ArrayList * arraylist_create(int length) {
    if  (NULL == length) {
        length = 1;
    } else if (length <= 0) {
        return NULL;
    }
    ArrayList * list = malloc(sizeof(ArrayList));
    list->length = length;
    list->size = 0;
    list->multiplier = 2;
    list->array = malloc(length * sizeof(void*));
    return list;
}

static void arraylist_allocate(ArrayList * list) {
    int new_length = list->length * list->multiplier;
    void ** new_array = malloc(new_length * sizeof(void*));
    for (int index = 0; index < list->size; ++index) {
        new_array[index] = list->array[index];
    }
    list->length = new_length;
    list->array = new_array;
}

void arraylist_add(ArrayList * list, void * value) {
    int next_index = list->size;
    if  (list->size == list->length) {
        arraylist_allocate(list);
    }
    list->size += 1;
    list->array[next_index] = value;
}

void arraylist_insert(ArrayList * list, int index, void * value) {
    int index_src = list->size - 1;
    int index_dest = list->size;
    if  (list->size == list->length) {
        arraylist_allocate(list);
    }
    for (; index_src >= index ; index_src--, index_dest--) {
        list->array[index_dest] = list->array[index_src];
    }
    list->size += 1;
    list->array[index] = value;
}

void arraylist_remove(ArrayList * list, int index) {
    int index_src = index + 1;
    int index_dest = index;
    int index_max = list->size - 1;
    for (; index_src <= index_max; index_src++, index_dest++) {
        list->array[index_dest] = list->array[index_src];
    }
    list->size -= 1;
}

int arraylist_size(ArrayList * list) {
    return list->size;
}

int arraylist_length(ArrayList * list) {
    return list->length;
}

int arraylist_set(ArrayList * list, int index, void * value) {
    if  (index < 0 || index >= list->size) {
        return -1;
    }
    list->array[index] = value;
}

void * arraylist_get(ArrayList * list, int index) {
    if  (index < 0 || index >= list->size) {
        return NULL;
    }
    return list->array[index];
}



#endif