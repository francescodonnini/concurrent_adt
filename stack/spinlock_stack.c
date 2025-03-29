#include "stack.h"

int init(Stack *s) {
    int err = pthread_spin_init(&s->lock, PTHREAD_PROCESS_PRIVATE);
    if (err) {
        return err;
    }
    return 0;
}

ListHead *pop(Stack *s) {
    int err = pthread_spin_lock(&s->lock);
    if (err) {
        return NULL;
    }
    ListHead *p = s->head;
    if (p != NULL) {
        s->head = p->next;
    }
    pthread_spin_unlock(&s->lock);
    return p;
}

void push(Stack *s, ListHead *item) {
    int err = pthread_spin_lock(&s->lock);
    if (err) {
        return;
    }
    item->next = s->head;
    s->head = item;
    pthread_spin_unlock(&s->lock);
}