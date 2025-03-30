#include "container_of.h"
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

#define ACTION_PUSH 0

typedef struct ThreadState {
    short int x16v[3];
    long      stats;
    Stack     *stack;
    bool      quit;
} ThreadState;

typedef struct LongList {
    struct ListHead list;
    long key;
} LongList;

static inline int random_action(ThreadState *s) {
    return jrand48(s->x16v) & 1;
}

static void *thread_fn(void *args) {
    ThreadState *s = (ThreadState*)args;
    while (!s->quit) {
        int a = random_action(s);
        if (a == ACTION_PUSH) {
            LongList *node = malloc(sizeof(LongList));
            if (node) {
                node->key = randlong(s->x16v, 0, 100);
                stack_push(s->stack, &node->list);
                s->stats++;
            }
        } else {
            ListHead *list = stack_pop(s->stack);
            if (list) {
                LongList *node = container_of(list, LongList, list);
                free(node);
            }
            s->stats++;
        }
    }
    return NULL;
  }

static inline int strtol_error(int n) {
    return n == 0 || n == LONG_MIN || n == LONG_MAX;
}

int main(int argc, const char **argv) {
    if (--argc != 1) {
        printf("see usage:\n%s <seconds>\n", argv[0]);
        return -1;
    }
    long observation_time = strtol(argv[1], NULL, 10);
    if (strtol_error(observation_time) || observation_time <= 0) {
        printf("expected a positive number >= 1 but got %s\n", argv[1]);
        return -1;
    }
    Stack stack;
    if (stack_init(&stack)) {
        return -1;
    }
    pthread_t tid;
    ThreadState state = {.quit=false, .stack=&stack, .stats=0};
    if (pthread_create(&tid, NULL, thread_fn,  (void*)&state)) {
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
    state.quit = true;
    pthread_join(tid, NULL);
    printf("total number of ops in %d seconds is %ld\n", observation_time, state.stats);
    ListHead *it = stack.head.next;
    while (it != NULL) {
        ListHead *next = it->next;
        LongList *node = container_of(it, LongList, list);
        free(node);
        it = next;
    }
    return 0;
}