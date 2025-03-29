#include "set.h"
#include "container_of.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct IntList {
    struct ListHead list;
    int key;
} IntList;

typedef struct StringList {
    struct ListHead list;
    char *key;
} StringList;

int icmp(struct ListHead *a, struct ListHead *b) {
    IntList *m = container_of(a, IntList, list);
    IntList *n = container_of(b, IntList, list);
    int d = m->key - n->key;
    if (d > 0) {
        return 1;
    } else if (!d) {
        return 0;
    } else {
        return -1;
    }
}

int lstrcmp(struct ListHead *a, struct ListHead *b) {
    StringList *m = container_of(a, StringList, list);
    StringList *n = container_of(b, StringList, list);
    int cmp = strcmp(m->key, n->key);
    if (cmp) {
        return cmp > 0 ? 1 : -1;
    } else {
        return 0;
    }
}

int main(int argc, const char *argv[]) {
    struct Set set;
    struct ListHead head, tail;
    set_init(&set, &head, &tail, icmp);
    StringList x = {.key = "10"};
    StringList y = {.key = "11"};
    set_insert(&set, &x.list);
    set_insert(&set, &y.list);
    StringList k = {.key = "10"};
    printf("%d\n", set_find(&set, &k.list));
    k.key = "5";
    printf("%d\n", set_find(&set, &k.list));
    k.key = "11";
    printf("%d\n", set_find(&set, &k.list));
    set_remove(&set, &k.list);
    printf("%d\n", set_find(&set, &k.list));
}