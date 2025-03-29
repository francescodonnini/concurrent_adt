#ifndef __STACK_H__
#define __STACK_H__
#include "list.h"
#include <pthread.h>

typedef struct Stack {
    ListHead *head;
#ifdef SPINLOCK_VERSION
    pthread_spinlock_t lock;
#elif MUTEX_VERSION
    pthread_mutex_t lock;
#endif
} Stack;

int init(Stack *s);

ListHead *pop(Stack *s);

void push(Stack *s, ListHead *item);

#endif