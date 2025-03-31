#ifndef SET_H
#define SET_H
#include "list.h"
#include <stdbool.h>

typedef struct Set {
    ListHead *head;
    ListHead *tail;
    int (*cmp)(ListHead *a, ListHead *b);
    void *ctx;
} Set;

static inline void __set_init(
    Set *set,
    ListHead *head,
    ListHead *tail,
    int (*cmp)(ListHead *a, ListHead *b)) {
    set->head = head;
    set->tail = tail;
    head->next = tail;
    set->cmp = cmp;
}

int set_init(
    Set *set,
    ListHead *head,
    ListHead *tail,
    int (*cmp)(ListHead *a, ListHead *b));

bool set_find(Set *set, ListHead *key);

bool set_insert(Set *set, ListHead *key);

ListHead* set_remove(Set *set, ListHead *key);
#endif