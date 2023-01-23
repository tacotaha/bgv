#ifndef PARAMS_H
#define PARAMS_H

#include <stdint.h>
#include <gmp.h>

/* Polynomial degree */
#define LGD 14
#define D (1 << LGD)

/* Initial bit length */
#define LGQ 237

/* RNS bit length */
#define M 25

#define L (1 << 4)

typedef struct level_t {
  /* polynomial degree */
  size_t d;

  /* size of the RNS basis of q */
  size_t m;

  /* RNS basis for q */
  uint32_t *basis;

  /* powers of a primitive 2nth root of unity (reverse order) */
  int32_t *roots;

  /* inverse powers of a primitive 2nth root of unity (reverse order) */
  int32_t *iroots;

  /* CRT coefficients used to convert back from an RNS representation */
  mpz_t Q, *crt_coefs;
} level_t;

typedef struct bgv_t {
  /* Circuit depth */
  size_t l;

  /* Level parameters */
  level_t *levels;
} bgv_t;

/* Initialize parameters */
void bgv_init(bgv_t *, size_t, size_t, int);

/* Free parameters */
void bgv_free(bgv_t *);

#endif                          /* PARAMS_H */
