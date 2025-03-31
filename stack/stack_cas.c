#include "stack.h"

int stack_init(Stack *s) {
    s->head.next = NULL;
    return 0;
}

ListHead *stack_pop(Stack *s) {
    for (;;) {
        ListHead *old_head = s->head.next;
        ListHead *new_head = old_head != NULL ? old_head->next : NULL;
        if (__sync_bool_compare_and_swap((long*)&s->head.next, old_head, new_head)) {
            return old_head;
        }
    }
    return NULL;
}

void stack_push(Stack *s, ListHead *item) {
    for (;;) {
        item->next = s->head.next;
        if (__sync_bool_compare_and_swap((long*)&s->head.next, s->head.next, item)) {
            return;
        }
    }
}