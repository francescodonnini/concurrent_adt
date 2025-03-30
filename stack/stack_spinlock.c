#include "stack.h"
#include <bits/pthreadtypes.h>

int stack_init(Stack *s) {
    pthread_spinlock_t *lock = (pthread_spinlock_t*)s->ctx;
    int err = pthread_spin_init(lock, PTHREAD_PROCESS_PRIVATE);
    if (err) {
        return err;
    }
    s->head.next = NULL;
    return 0;
}

ListHead *stack_pop(Stack *s) {
    pthread_spinlock_t *lock = (pthread_spinlock_t*)s->ctx;
    int err = pthread_spin_lock(lock);
    if (err) {
        return NULL;
    }
    ListHead *n = s->head.next;
    if (n != NULL) {
        s->head.next = n->next;
    }
    pthread_spin_unlock(lock);
    return n;
}

void stack_push(Stack *s, ListHead *item) {
    pthread_spinlock_t *lock = (pthread_spinlock_t*)s->ctx;
    int err = pthread_spin_lock(lock);
    if (err) {
        return;
    }
    item->next = s->head.next;
    s->head.next = item;
    pthread_spin_unlock(lock);
}