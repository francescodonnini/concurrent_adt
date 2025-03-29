#include "stack.h"
#include <stdatomic.h>
#include <stdbool.h>

int stack_init(Stack *s) {
    return 0;
}

ListHead *stack_pop(Stack *s) {
    bool b = true;
    do {
        ListHead *old_head = s->head.next;
        b = atomic_compare_exchange_strong(&s->head.next, old_head, old_head->next);
    } while (!b);
    return NULL;
}

void stack_push(Stack *s, ListHead *item) {
    bool b = true;
    do {
        ListHead *old_head = s->head.next;
        b = atomic_compare_exchange_strong(&s->head.next, old_head, item);
    } while (!b);
}