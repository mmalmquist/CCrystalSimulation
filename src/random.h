#ifndef RANDOM_H_
#define RANDOM_H_

#include <inttypes.h>

#define CS_RAND_MAX 0x7fff
static uint_fast16_t __cs_next_rand__ = 1;
#define cs_rand() (((__cs_next_rand__ = __cs_next_rand__*0x41c64e6d + 0x3039) >> 16) & 0x7fff)
#define cs_drand() (cs_rand() / (double)0x8000)
#define cs_srand(seed) (__cs_next_rand__ = seed)

#endif //RANDOM_H_
