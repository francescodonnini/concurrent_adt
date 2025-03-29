#ifndef __SET_H__
#define __SET_H__
#include "list.h"
#include <pthread.h>
#include <stdbool.h>

typedef struct Set {
    ListHead *head;
    ListHead *tail;
    int (*cmp)(ListHead *a, ListHead *b);
#ifdef SPINLOCK_VERSION
    pthread_spinlock_t lock;
#elif  MUTEX_VERSION
    pthread_mutex_t lock;    
#endif
} Set;

static void __set_init(
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
    int (*default_cmp)(ListHead *a, ListHead *b));

bool set_find(Set *set, ListHead *key);

bool set_insert(Set *set, ListHead *key);

ListHead* set_remove(Set *set, ListHead *key);


#endif