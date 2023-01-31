#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define MR_RUNS (1 << 8)

static inline uint32_t reverse(uint32_t x) {
  x = (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1));
  x = (((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2));
  x = (((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4));
  x = (((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8));
  return ((x >> 16) | (x << 16));
}

int is_prime(uint64_t);
void gen_primes(uint32_t, uint32_t, uint32_t *, size_t);
uint64_t find_proot(uint64_t, uint64_t);

int modinv(int, int);
uint64_t modexp(uint64_t, uint64_t, uint64_t);
uint64_t modsqrt(uint64_t, uint64_t);

static inline uint64_t modmul(uint64_t a, uint64_t b, uint64_t m) {
  __int128 mul = (__int128) (a % m) * (b % m);
  return mul % (__int128) (m);
}

static inline uint64_t modadd(uint64_t a, uint64_t b, uint64_t m) {
  return (a + b) % m;
}

static inline uint64_t modsub(uint64_t a, uint64_t b, uint64_t m) {
  return (a + m - (b % m)) % m;
}

static inline uint64_t rand64() {
  return ((((uint64_t) rand()) << 32) | rand());
}

static inline void *malloc_or_die(size_t size) {
  void *mem = calloc(1, size);
  if (!mem) {
    perror("calloc:");
    exit(-1);
  }
  return mem;
}

#endif                          /* UTILS_H */
