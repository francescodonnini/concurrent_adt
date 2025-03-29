#include "set.h"
#include <stddef.h>

int set_init(
    Set *set,
    ListHead *head,
    ListHead *tail,
    int (*default_cmp)(ListHead *a, ListHead *b)) {
    __set_init(set, head, tail, default_cmp);
    int err = pthread_mutex_init(&set->lock, NULL);
    return err;
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
    int err = pthread_mutex_lock(&set->lock);
    if (err) {
        return false;
    }
    ListHead *prec, *succ;
    bool b = __set_find(set, key, &prec, &succ);
    pthread_mutex_unlock(&set->lock);
    return b;
}

bool set_insert(Set *set, ListHead *key) {
    int err = pthread_mutex_lock(&set->lock);
    if (err) {
        return false;
    }
    ListHead *prec, *succ;
    bool b = __set_find(set, key, &prec, &succ);
    if (!b) {
        prec->next = key;
        key->next = succ;
    }
    pthread_mutex_unlock(&set->lock);
    return b ? false : true;
}

ListHead* set_remove(Set *set, ListHead *key) {
    int err = pthread_mutex_lock(&set->lock);
    if (err) {
        return false;
    }
    ListHead *prec, *succ;
    bool b = __set_find(set, key, &prec, &succ);
    if (b) {
        prec->next = succ->next;
    }
    pthread_mutex_unlock(&set->lock);
    return b ? succ : NULL;
}