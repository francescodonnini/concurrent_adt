#ifndef STACK_H
#define STACK_H
#include "list.h"
#include <pthread.h>

#ifdef BACKOFF_VERSION
typedef struct BackoffSpec {
    long   max_no_tries;
    double max_sleep_time;
    short  x16v[3];
} BackoffSpec;
#endif

typedef struct Stack {
    ListHead head;
    void     *ctx;
} Stack;

int stack_init(Stack *s);

ListHead *stack_pop(Stack *s);

void stack_push(Stack *s, ListHead *item);
#endif