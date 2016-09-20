// *****************************************************************************
// Anthony Tatowicz
//
// list.h - header for linked list impl
// *****************************************************************************

#ifndef _LIST_H_
#define  _LIST_H_

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

typedef void (*free_fn) (void*);
typedef bool (*list_iter) (void*);

typedef struct _node {
    void *data;
    struct _node *next;
    struct _node *prev;
} node_t;

typedef struct {
    int length;
    int elm_size;
    node_t *head;
    node_t *tail;
    free_fn fn;
} list_t;

void new_list(list_t *list, int elm_size, free_fn fn);
void delete_list(list_t *list);

void push(list_t *list, void *data);
void append(list_t *list, void *data);
int list_size(list_t *list);

void list_for_each(list_t *list, list_iter iter);
void list_head(list_t *list, void *data, bool remove);
void list_tail(list_t *list, void *data);

node_t* get(list_t *list, size_t i);

#endif
