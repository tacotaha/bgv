#ifndef RQ_H
#define RQ_H

#include "params/params.h"
#include "sample/sample.h"

/* Ciphertext ring element */
typedef struct rq_t {
  int32_t *b;
} rq_t;

/* Init a zero polynomial */
void rq_init(rq_t *);

/* Free a polynomial */
void rq_free(rq_t *);

/* Sample a random polynomial */
void rq_init_rand(rq_t *, DISTRIBUTION);

/* Clone a  polynomial */
void rq_clone(rq_t *, const rq_t *);

/* Addition */
void rq_add(rq_t *, const rq_t *, const rq_t *);

/* Subtraction */
void rq_sub(rq_t *, const rq_t *, const rq_t *);

/* Convolution */
void rq_mul(rq_t *, const rq_t *, const rq_t *);

/* Scalar multiplication */
void rq_cmul(rq_t *, const rq_t *, int32_t);

/* Negation */
void rq_neg(rq_t *);

/* Equality check */
int rq_eq(const rq_t *, const rq_t *);

/* Convert to ntt form */
void rq_ntt(rq_t *);

/* Convert to coefficient form */
void rq_intt(rq_t *);

/* Encode a polynomial */
void rq_encode(rq_t * r, int32_t * x);

/* Decode a polynomial */
void rq_crt(int32_t *, rq_t *);

#endif                          /* RQ_H */
