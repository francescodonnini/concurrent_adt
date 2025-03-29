#include "stack.h"
#include <stdatomic.h>
#include <stdbool.h>

int stack_init(Stack *s) {
    s->head.next = NULL;
    return 0;
}

ListHead *stack_pop(Stack *s) {
    for (;;) {
        ListHead *old_head = s->head.next;
        if (!old_head) {
            break;
        }
        if (atomic_compare_exchange_strong(&s->head.next, &old_head, old_head->next)) {
            return old_head;
        }
    }
    return NULL;
}

void stack_push(Stack *s, ListHead *item) {
    for (;;) {
        ListHead *old_head = s->head.next;
        if (atomic_compare_exchange_strong(&s->head.next, &old_head, item)) {
            break;
        }
    }
}