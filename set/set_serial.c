#include "set.h"
#include <stddef.h>

int set_init(
    Set *set,
    ListHead *head,
    ListHead *tail,
    int (*cmp)(ListHead *a, ListHead *b)) {
    __set_init(set, head, tail, cmp);
    return 0;
}

static bool __set_find(Set *set, ListHead *key, ListHead **prec, ListHead **succ) {
    if (set->head == set->tail) {
        return false;
    }
    *prec = set->head;
    *succ = (*prec)->next;
    while (*succ != set->tail) {
        switch (set->cmp(*succ, key)) {
            case -1:
                *prec = *succ;
                *succ = (*prec)->next;
                break;
            case 0:
                return true;
            case 1:
                return false;
        }
    }
    return false;
}

bool set_find(Set *set, ListHead *key) {
    ListHead *prec, *succ;
    return __set_find(set, key, &prec, &succ);
}

bool set_insert(Set *set, ListHead *key) {
    ListHead *prec, *succ;
    bool found = __set_find(set, key, &prec, &succ);
    if (!found) {
        prec->next = key;
        key->next = succ;
        return true;
    } else {
        return false;
    }
}

ListHead* set_remove(Set *set, ListHead *key) {
    ListHead *prec, *succ;
    bool found = __set_find(set, key, &prec, &succ);
    if (found) {
        prec->next = succ->next;
        return succ;
    }
    return NULL;
}