// *****************************************************************************
// Anthony Tatowicz
//
// list.c - Linked list implementation
// *****************************************************************************

#include "list.h"


void new_list(list_t* list, int elm_size, free_fn fn) {
    assert(elm_size > 0);
    list->length = 0;
    list->elm_size = elm_size;
    list->head = list->tail = NULL;
    list->fn = fn;
}

void delete_list(list_t* list) {
    node_t* current;

    while(list->head != NULL) {
        current = list->head;
        list->head = current->next;

        if(list->fn) {
            list->fn(current->data);
        }
        else {
            free(current->data);
        }

        free(current);
    }
}

void push(list_t* list, void* data) {
  node_t* node = malloc(sizeof(node_t));
  if(node == NULL) exit(1);

  node->data = malloc(list->elm_size);
  if(node->data == NULL) exit(1);

  memcpy(node->data, data, list->elm_size);

  node->next = list->head;
  node->prev = NULL;
  list->head = node;

  // first node?
  if(!list->tail) {
    list->tail = list->head;
  }

  list->length++;
}

void append(list_t* list, void* data) {
  node_t *node = malloc(sizeof(node_t));
  if(node == NULL) exit(1);
  node->data = malloc(list->elm_size);
  if(node->data == NULL) exit(1);
  node->next = NULL;
  node->prev = NULL;

  memcpy(node->data, data, list->elm_size);

  if(list->length == 0) {
    list->head = list->tail = node;
  }
  else {
    list->tail->next = node;
    node->prev = list->tail;
    list->tail = node;
  }

  list->length++;
}

void list_for_each(list_t* list, list_iter iter) {
  assert(iter != NULL);

  node_t* node = list->head;
  bool result = true;

  while(node != NULL && result) {
    result = iter(node->data);
    node = node->next;
  }
}

void list_head(list_t* list, void* data, bool rm) {
  assert(list->head != NULL);

  node_t *node = list->head;
  memcpy(data, node->data, list->elm_size);

  if(rm) {
    list->head = node->next;
    list->length--;

    free(node->data);
    free(node);
  }
}

void list_tail(list_t *list, void *element){
  assert(list->tail != NULL);

  node_t *node = list->tail;
  memcpy(element, node->data, list->elm_size);
}

int list_size(list_t *list) {
  return list->length;
}

node_t* get(list_t *list, size_t i) {

    node_t *current = list->head;
    size_t count = 0;

    assert(current != NULL);

    while(current->next != NULL) {

        if(count == i) {
            return current;
        }

        count++;
        current = current->next;
    }

    return NULL;
}
