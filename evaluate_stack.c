#include "container_of.h"
#include "stack.h"
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#define ACTION_PUSH 0

typedef struct ThreadState {
    short int x16v[3];
    long      stats;
    Stack     *stack;
} ThreadState;

typedef struct IntList {
    struct ListHead list;
    int key;
} IntList;

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

static inline int random_action(ThreadState *s) {
    return jrand48(s->x16v) & 1;
}

static void *thread_fn(void *args) {
    ThreadState *s = (ThreadState*)args;
    for (;;) {
        int a = random_action(s);
        if (a == ACTION_PUSH) {
            IntList *node = malloc(sizeof(IntList));
            stack_push(s->stack, &node->list);
        } else {
            ListHead *list = stack_pop(s->stack);
            if (list) {
                IntList *node = container_of(list, IntList, list);
                free(node);
            }
        }
        s->stats++;
    }
    return NULL;
  }

static inline int strtol_error(int n) {
    return n == 0 || n == LONG_MIN || n == LONG_MAX;
}

int main(int argc, const char **argv) {
    if (--argc != 2) {
        printf("see usage:\n%s <number of threads> <seconds>\n", argv[0]);
        return -1;
    }
    long n = strtol(argv[1], NULL, 10);
    if (strtol_error(n) || n <= 0) {
        printf("expected a positive number >= 1 but got %s\n", argv[1]);
        return -1;
    }
    long observation_time = strtol(argv[2], NULL, 10);
    if (strtol_error(n) || n <= 0) {
        printf("expected a positive number >= 1 but got %s\n", argv[1]);
        return -1;
    }
    Stack stack;
    if (stack_init(&stack)) {
        return -1;
    }
    pthread_t *tid = malloc(n * sizeof(pthread_t));
    if (!tid) {
        printf("malloc failed, got error (%d) %s\n", errno, strerror(errno));
        return -1;
    }
    ThreadState *state = malloc(n * sizeof(ThreadState));
    if (!state) {
        printf("malloc failed, got error (%d) %s\n", errno, strerror(errno));
        free(tid);
        return -1;
    }
    int c = 0;
    for (int i = 0; i < n; ++i) {
        state[i].stack = &stack;
        state[i].stats = 0;
        if (pthread_create(&tid[i], NULL, thread_fn,  (void*)&state[i])) {
            break;
        }
        c++;
    }
    if (c < n) {
        while (--c >= 0) {
            pthread_cancel(tid[c]);
        }
    }
    struct timeval start;
    int err = gettimeofday(&start, NULL);
    if (err) {
        printf("gettimeofday failed bacause of error (%d) %s\n", errno, strerror(errno));
    }
    struct timespec duration = {.tv_sec=observation_time, .tv_nsec=0L};
    if (nanosleep(&duration, NULL)) {
        printf("nanosleep failed because of error (%d) %s", errno, strerror(errno));
    }
    if (!err) {
        struct timeval end;
        err = gettimeofday(&end, NULL);
        if (!err) {
            double elapsed = end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec) / 1e6;
            printf("real elapsed time %f\n", elapsed);
        }
    }
    for (int i = 0; i < n; i++) {
        pthread_cancel(tid[i]);
    }
    
    long ops = 0;
    for (int i = 0; i < n; ++i) {
        ops += state->stats;
    }
    printf("total number of ops in %d seconds is %ld\n", observation_time, ops);
    free(tid);
    free(state);
    return 0;
}