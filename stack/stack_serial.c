#include "stack.h"

int stack_init(Stack *s) {
    s->head.next = NULL;
    return 0;
}

ListHead *stack_pop(Stack *s) {
    ListHead *n = s->head.next;
    if (n != NULL) {
        s->head.next = n->next;
    }
    return n;
}

void stack_push(Stack *s, ListHead *item) {
    item->next = s->head.next;
    s->head.next = item;
}