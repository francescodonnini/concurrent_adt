#include "stack.h"

int stack_init(Stack *s) {
    int err = pthread_mutex_init(&s->lock, NULL);
    if (err) {
        return err;
    }
    return 0;
}

ListHead *stack_pop(Stack *s) {
    int err = pthread_mutex_lock(&s->lock);
    if (err) {
        return NULL;
    }
    ListHead *n = s->head.next;
    if (n != NULL) {
        s->head.next = n->next;
    }
    pthread_mutex_unlock(&s->lock);
    return n;
}

void stack_push(Stack *s, ListHead *item) {
    int err = pthread_mutex_lock(&s->lock);
    if (err) {
        return;
    }
    item->next = s->head.next;
    s->head.next = item;
    pthread_mutex_unlock(&s->lock);
}