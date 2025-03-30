#ifndef RANDOM_H
#define RANDOM_H
#include <math.h>
#include <stdlib.h>

static inline double randf(unsigned short x16v[3], double lo, double hi) {
    return lo + fmod(erand48(x16v), (hi - lo + 1));
}

static inline long randlong(unsigned short x16v[3], int lo, int hi) {
    return lo + (nrand48(x16v) % (hi - lo + 1));
}
#endif