#include "random.h"
#include "stack.h"
#include <errno.h>
#include <math.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

int stack_init(Stack *s) {
    s->head.next = NULL;
    return 0;
}

static struct timespec* to_timespec(struct timespec *duration, double seconds) {
    long whole_part = (long) floor(seconds);
    double frac_part = seconds - whole_part;
    duration->tv_sec = whole_part;
    duration->tv_nsec = frac_part * 1e9;
    return duration;
}

static inline double get_initial_sleep_time(BackoffSpec *b) {
    return randf(b->x16v, 0., b->max_sleep_time);
}

static inline double get_sleep_time(double initial_sleep_time, long n) {
    return (1 << n) * initial_sleep_time;
}

static inline int nanosleepf(double secs) {
    struct timespec duration = {.tv_sec=0L, .tv_nsec=0L};
    return nanosleep(to_timespec(&duration, secs), NULL);
}

ListHead *stack_pop(Stack *s) {
    BackoffSpec *backoff_spec = (BackoffSpec*)s->ctx;
    double initial_sleep_time = get_initial_sleep_time(backoff_spec);
    long n = 0;
    for(;;) {
        double sleep_time = get_sleep_time(initial_sleep_time, n++);
        nanosleepf(sleep_time);
        ListHead *old_head = s->head.next;
        if (!old_head) {
            break;
        }
        if (atomic_compare_exchange_strong(&s->head.next, &old_head, old_head->next) || n >= backoff_spec->max_no_tries) {
            return old_head;
        }
    }
    return NULL;
}

void stack_push(Stack *s, ListHead *item) {
    BackoffSpec *backoff_spec = (BackoffSpec*)s->ctx;
    double initial_sleep_time = get_initial_sleep_time(backoff_spec);
    long n = 0;
    for (;;) {
        double sleep_time = get_sleep_time(initial_sleep_time, n++);
        if (nanosleepf(sleep_time)) {
            printf("nanosleep() failed, got error \"%s\" (%d)", strerror(errno), errno);
            break;
        }
        ListHead *old_head = s->head.next;
        item->next = old_head;
        if (atomic_compare_exchange_strong(&s->head.next, &old_head, item) || n >= backoff_spec->max_no_tries) {
            break;
        }
    }
}