#include "set.h"
#include <stdatomic.h>

int set_init(
    Set *set,
    ListHead *head,
    ListHead *tail,
    int (*cmp)(ListHead *a, ListHead *b)) {
    __set_init(set, head, tail, cmp);
    set->ctx = NULL;
    return 0;
}

static inline ListHead* get_marked_ref(ListHead *ptr) {
    return (ListHead*)((long)ptr | 1);
}

static inline ListHead* get_unmarked_ref(ListHead *ptr) {
    return (ListHead*)((long)ptr & ~1);
}

static inline bool is_marked_ref(ListHead *ptr) {
    return (long)ptr & 1;
}

ListHead* __search(Set *set, ListHead *key, ListHead **right) {
retry:
    // invarianti:
    // left  = HEAD
    // right = TAIL
    *right = set->tail;
    ListHead *left = set->head;
    // it_raw è il puntatore al nodo successivo a HEAD che può essere marcato oppure no
    // it     è it_raw a cui è stata applicata la maschera per portare il primo bit a 0 (in modo tale
    //        che si possa usare come indirizzo valido).
    ListHead *it_raw = left->next;
    ListHead *it = get_unmarked_ref(left->next);
    while (it != set->tail) {
        if (!is_marked_ref(it_raw)) {
            if (set->cmp(it, key) < 0) {
                left = it;
            } else {
                *right = it;
                break;
            }
        }
        it_raw = it->next;
        it = get_unmarked_ref(it_raw);
    }
    // In generale, quando il loop termina:
    // 1. left == HEAD or left == n s.t. n.key < key
    // 2. right == TAIL or right == n s.t. n.key >= key
    // Tra left e right possono esserci nodi non attivi oppure no. Se i nodi non sono direttamente
    // adiacenti si prova subito a sganciare quelli in mezzo, se CAS fallisce allora si ricomincia la ricerca
    if (left->next != *right && __sync_bool_compare_and_swap(&left->next, left->next, *right)) {
        goto retry;
    }
    return left;
}

bool set_find(Set *set, ListHead *key) {
    return false;
}

bool set_insert(Set *set, ListHead *key) {
retry:
    ListHead *right;
    ListHead *left = __search(set, key, &right);
    if (!set->cmp(key, right)) {
        return false;
    }
    key->next = right;
    if (!__sync_bool_compare_and_swap(&left->next, right, key)) {
        goto retry;
    }
    return true;
}

ListHead* set_remove(Set *set, ListHead *key) {
    ListHead *right;
    do {
        __search(set, key, &right);
        if (right != set->tail || set->cmp(key, right)) {
            return NULL;
        }
        // Abbiamo trovato la chiave corretta! Si proceda all'eliminazione logica
        ListHead *n = right->next;
        if (!is_marked_ref(n)) {
            if (__sync_bool_compare_and_swap(&right->next, n, get_marked_ref(n))) {
                ListHead *t;
                __search(set, key, &t);
                return right;
            }
        }
    } while (true);
}