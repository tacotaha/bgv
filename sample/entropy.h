#ifndef ENTROPY_H
#define ENTROPY_H

#include <stdint.h>
#include <stdlib.h>

#ifdef CSPRNG

#include <float.h>
#include <math.h>
#include <sys/random.h>

static inline size_t rng(void *buffer, size_t len) {
  return getrandom(buffer, len, GRND_RANDOM);
}

static inline uint32_t uniform32() {
  uint32_t r;
  rng(&r, sizeof(uint32_t));
  return r;
}

static inline double uniformr() {
  uint64_t r;
  rng(&r, sizeof(uint64_t));
  return ldexpf(r & ((1UL << DBL_MANT_DIG) - 1), -DBL_MANT_DIG);
}

#else

static inline uint32_t uniform32() { return (uint32_t)random(); }

static inline double uniformr() { return drand48(); }

#endif /* CSPRNG */

static inline uint8_t uniformb() { return uniform32() & 1; }

#endif /* ENTROPY_H */
