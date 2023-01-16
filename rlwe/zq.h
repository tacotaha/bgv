#ifndef ZQ_H
#define ZQ_H

#include "params/params.h"

typedef struct zq_t {
  uint32_t b[M];
} zq_t;

/* Montgomery encoding */

static inline int32_t redc(uint64_t t, uint8_t i) {
  uint64_t m = (uint32_t) t * r_minus_q_inv[i];
  return (m * rns_basis[i] + t) >> 32;
}

static inline int32_t modmul(uint32_t a, uint32_t b, uint8_t i) {
  return redc((uint64_t) a * b, i);
}

static inline int32_t to_mont(uint32_t x, uint8_t i) {
  return redc((uint64_t) x * r_squared_mod_q[i], i);
}

/* Compute x^-1 mod y */
int inverse_mod(int, int);

/* Init CRT coefficients */
void crt_init();

/* Read a ring element from a bigint */
void zq_from_mpz(zq_t *, const mpz_t);

/* Write a ring element to a bigint */
void zq_to_mpz(mpz_t, const zq_t *);

#endif                          /* ZQ_H */
