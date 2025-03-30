#include "stack.h"

int stack_init(Stack *s) {
    pthread_mutex_t *mutex = (pthread_mutex_t*)s->ctx;
    int err = pthread_mutex_init(mutex, NULL);
    if (err) {
        return err;
    }
    s->head.next = NULL;
    return 0;
}

ListHead *stack_pop(Stack *s) {
    pthread_mutex_t *mutex = (pthread_mutex_t*)s->ctx;
    int err = pthread_mutex_lock(mutex);
    if (err) {
        return NULL;
    }
    ListHead *n = s->head.next;
    if (n) {
        s->head.next = n->next;
    }
    pthread_mutex_unlock(mutex);
    return n;
}

void stack_push(Stack *s, ListHead *item) {
    pthread_mutex_t *mutex = (pthread_mutex_t*)s->ctx;
    int err = pthread_mutex_lock(mutex);
    if (err) {
        return;
    }
    item->next = s->head.next;
    s->head.next = item;
    pthread_mutex_unlock(mutex);
}