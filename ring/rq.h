#ifndef RQ_H
#define RQ_H

#include "params/params.h"

/* Ciphertext ring R/qR */
typedef struct rq_t {
  /* is in NTT form */
  char is_ntt;

  /* parameters for this level */
  level_t *l;

  /* polynomial degree */
  size_t n;

  /* polynomial coefficients */
  int32_t *b;
} rq_t;

/* Init a polynomial in coefficient form */
void rq_init(rq_t *, level_t *, int32_t *);

/* Addition */
void rq_add(rq_t *, const rq_t *, const rq_t *);

/* Subtraction */
void rq_sub(rq_t *, const rq_t *, const rq_t *);

/* Convolution */
void rq_mul(rq_t *, rq_t *, rq_t *);

/* Scalar multiplication */
void rq_cmul(rq_t *, const rq_t *, int32_t);

/* Convert to ntt form */
void rq_ntt(rq_t *);

/* Convert to coefficient form */
void rq_intt(rq_t *);

/* Free a polynomial */
void rq_free(rq_t *);
#endif                          /* RQ_H */
