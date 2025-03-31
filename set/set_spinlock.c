#include "set.h"
#include <pthread.h>
#include <stddef.h>

int set_init(
    Set *set,
    ListHead *head,
    ListHead *tail,
    int (*cmp)(ListHead *a, ListHead *b)) {
    __set_init(set, head, tail, cmp);
    pthread_spinlock_t *lock = (pthread_spinlock_t*)set->ctx;
    int err = pthread_spin_init(lock, PTHREAD_PROCESS_PRIVATE);
    if (err) {
        return err;
    }
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
    pthread_spinlock_t *lock = (pthread_spinlock_t*)set->ctx;
    int err = pthread_spin_lock(lock);
    if (err) {
        return false;
    }
    ListHead *prec, *succ;
    bool b = __set_find(set, key, &prec, &succ);
    pthread_spin_unlock(lock);
    return b;
}

bool set_insert(Set *set, ListHead *key) {
    pthread_spinlock_t *lock = (pthread_spinlock_t*)set->ctx;
    int err = pthread_spin_lock(lock);
    if (err) {
        return false;
    }
    ListHead *prec, *succ;
    bool b = __set_find(set, key, &prec, &succ);
    if (!b) {
        prec->next = key;
        key->next = succ;
    }
    pthread_spin_unlock(lock);
    return b ? false : true;
}

ListHead* set_remove(Set *set, ListHead *key) {
    pthread_spinlock_t *lock = (pthread_spinlock_t*)set->ctx;
    int err = pthread_spin_lock(lock);
    if (err) {
        return false;
    }
    ListHead *prec, *succ;
    bool b = __set_find(set, key, &prec, &succ);
    if (b) {
        prec->next = succ->next;
    }
    pthread_spin_unlock(lock);
    return b ? succ : NULL;
}