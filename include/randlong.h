#ifndef RANDLONG_H
#define RANDLONG_H
#include <stdlib.h>

static inline long randlong(unsigned short x16v[3], int lo, int hi) {
    return lo + (nrand48(x16v) % (hi - lo + 1));
}
#endif