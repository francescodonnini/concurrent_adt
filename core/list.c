#include "list.h"

void list_add(ListHead *l, ListHead *m) {
    m->next = l->next;
    l->next = m;
}