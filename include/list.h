#ifndef LIST_H
#define LIST_H
#include <pthread.h>

typedef struct ListHead {
    struct ListHead *next;
} ListHead;

typedef struct LListHead {
    struct ListHead *next;
    pthread_mutex_t  lock;
} LListHead;

void list_add(ListHead *l, ListHead *m);

#endif