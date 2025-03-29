#include "stack.h"

int init(Stack *s) {
    return 0;
}

ListHead *pop(Stack *s) {
    if (s->head == NULL) {
        return NULL;
    } 
    for (;;) {
        ListHead *old_head = s->head;
        if (__sync_bool_compare_and_swap(&s->head, old_head, old_head->next)) {
            return old_head;
        }
    }
    return NULL; // unreachable!
}

void push(Stack *s, ListHead *item) {
    for (;;) {
        ListHead *old_head = s->head;
        item->next = old_head;
        if (__sync_bool_compare_and_swap(&s->head, old_head, item)) {
            break;
        }
    }
}