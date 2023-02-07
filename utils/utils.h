#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MR_RUNS (1 << 8)

typedef int32_t int_t;
typedef uint32_t uint_t;
typedef int64_t int_dt;
typedef uint64_t uint_dt;

int modinv(int, int);
int is_prime(uint_t);
uint_t modsqrt(uint_t, uint_t);
uint_t find_proot(uint_t, uint_t);
uint_t modexp(uint_t, uint_t, uint_t);
void gen_primes(uint_t, uint_t, uint_t *, size_t);

static inline uint_t modmul(uint_t a, uint_t b, uint_t m) {
  uint_dt mul = (uint_dt)(a % m) * (b % m);
  return mul % (uint_dt)(m);
}

static inline uint_t modadd(uint_t a, uint_t b, uint_t m) {
  return (a + b) % m;
}

static inline uint_t modsub(uint_t a, uint_t b, uint_t m) {
  return (a + m - (b % m)) % m;
}

static inline uint_t reverse(uint_t x) {
  x = (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1));
  x = (((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2));
  x = (((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4));
  x = (((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8));
  return ((x >> 16) | (x << 16));
}

static inline void *malloc_or_die(size_t size) {
  void *mem = calloc(1, size);
  if (!mem) {
    perror("calloc");
    exit(-1);
  }
  return mem;
}

#endif /* UTILS_H */
