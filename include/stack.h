#ifndef STACK_H
#define STACK_H
#include "list.h"
#include <pthread.h>

typedef struct Stack {
    ListHead head;
#ifdef SPINLOCK_VERSION
    pthread_spinlock_t lock;
#elif  MUTEX_VERSION
    pthread_mutex_t lock;
#endif
} Stack;

int stack_init(Stack *s);

ListHead *stack_pop(Stack *s);

void stack_push(Stack *s, ListHead *item);
#endif