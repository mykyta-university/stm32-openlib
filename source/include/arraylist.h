#ifndef __ARRAYLIST_H
#define __ARRAYLIST_H

#include <malloc.h>

typedef struct arraylist ArrayList;


/*
 * Function prototypes
 */
ArrayList * arraylist_create(int length);

static void arraylist_allocate(ArrayList * list);

void arraylist_add(ArrayList * list, void * value);

void arraylist_insert(ArrayList * list, int index, void * value);

void arraylist_remove(ArrayList * list, int index);

int arraylist_size(ArrayList * list);

int arraylist_length(ArrayList * list);

int arraylist_set(ArrayList * list, int index, void * value);

void * arraylist_get(ArrayList * list, int index);

#endif