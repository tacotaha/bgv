#ifndef POLY_H
#define POLY_H

#include "ring.h"
#include "sample/sample.h"
#include "utils/utils.h"

typedef struct poly_t {
  int_t *b;
  ring_t *r;
  char is_ntt;
} poly_t;

typedef uint_t (*poly_binop_t)(uint_t, uint_t, uint_t);

void poly_neg(poly_t *);
void poly_ntt(poly_t *);
void poly_intt(poly_t *);
void poly_free(poly_t *);
void poly_crt(uint_t *, const poly_t *, uint_t);
void poly_init(ring_t *, poly_t *);
void poly_encode(ring_t *r, poly_t *, const uint_t *);
void poly_clone(poly_t *, const poly_t *);
void poly_cmul(poly_t *, const poly_t *, int_t);
void poly_init_rand(ring_t *, poly_t *, DISTRIBUTION);
void poly_binop(poly_t *, const poly_t *, const poly_t *, const poly_binop_t);

static inline void poly_add(poly_t *r, const poly_t *a, const poly_t *b) {
  poly_binop(r, a, b, modadd);
}

static inline void poly_sub(poly_t *r, const poly_t *a, const poly_t *b) {
  poly_binop(r, a, b, modsub);
}

static inline void poly_mul(poly_t *r, const poly_t *a, const poly_t *b) {
  poly_binop(r, a, b, modmul);
}

#endif /* POLY_H */
