#include "stack.h"

int stack_init(Stack *s) {
    int err = pthread_spin_init(&s->lock, PTHREAD_PROCESS_PRIVATE);
    if (err) {
        return err;
    }
    s->head.next = NULL;
    return 0;
}

ListHead *stack_pop(Stack *s) {
    int err = pthread_spin_lock(&s->lock);
    if (err) {
        return NULL;
    }
    ListHead *n = s->head.next;
    if (n != NULL) {
        s->head.next = n->next;
    }
    pthread_spin_unlock(&s->lock);
    return n;
}

void stack_push(Stack *s, ListHead *item) {
    int err = pthread_spin_lock(&s->lock);
    if (err) {
        return;
    }
    item->next = s->head.next;
    s->head.next = item;
    pthread_spin_unlock(&s->lock);
}