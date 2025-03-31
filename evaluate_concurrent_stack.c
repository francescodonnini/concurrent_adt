#include "container_of.h"
#include "list.h"
#include "random.h"
#include "stack.h"
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

typedef struct ThreadState {
    short int x16v[3];
    long      stats;
    Stack     *stack;
    bool      quit;
} ThreadState;

typedef struct LongList {
    ListHead list;
    long key;
} LongList;

static inline int random_action(ThreadState *s) {
    return jrand48(s->x16v) & 1;
}

static void *thread_fn(void *args) {
    ThreadState *s = (ThreadState*)args;
    while (!s->quit) {
        int a = random_action(s);
        if (a) {
            LongList *node = malloc(sizeof(LongList));
            if (node) {
                node->key = randlong(s->x16v, 0, 100);
                node->list.next = NULL;
                stack_push(s->stack, &node->list);
                s->stats++;
            }
        } else {
            ListHead *list = stack_pop(s->stack);
            if (list) {
#if defined(MUTEX_VERSION) || defined(SPINLOCK_VERSION)
                // L'algoritmo non funziona se si libera la memoria (potrebbe
                // portare a segmentation fault).
                LongList *n = container_of(n, LongList, list);
                free(n);
#endif
            }
            s->stats++;
        }
    }
    return NULL;
  }

static inline int strtol_error(int n) {
    return n == 0 || n == LONG_MIN || n == LONG_MAX;
}

static void pop_all(Stack *stack) {
    ListHead *it = stack->head.next;
    while (it != NULL) {
        ListHead *next = it->next;
        LongList *node = container_of(it, LongList, list);
        free(node);
        it = next;
    }
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
#ifdef BACKOFF_VERSION
    BackoffSpec spec = {.max_no_tries=4, .max_sleep_time=0.00005};
    stack.ctx = &spec;
#elif  MUTEX_VERSION
    pthread_mutex_t mutex;
    stack.ctx = &mutex;
#elif  SPINLOCK_VERSION
    pthread_spinlock_t lock;
    stack.ctx = &lock;
#endif
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
        state[i].quit = false;
        state[i].stack = &stack;
        state[i].stats = 0;
        if (pthread_create(&tid[i], NULL, thread_fn,  (void*)&state[i])) {
            break;
        }
        c++;
    }
    if (c < n) {
        for (int i = 0; i < c; ++i) {
            state[i].quit = true;
        }
        while (--c >= 0) {
            pthread_join(tid[c], NULL);
        }
        free(tid);
        free(state);
        return -1;
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
        state[i].quit = true;
    }
    for (int i = 0; i < n; ++i) {
        pthread_join(tid[i], NULL);
    }
    long ops = 0L;
    for (size_t i = 0; i < n; ++i) {
        ops += state[i].stats;
    }
    printf("total number of ops in %d seconds is %ld\n", observation_time, ops);
    free(tid);
    free(state);
    pop_all(&stack);
    return 0;
}