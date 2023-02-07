#include "poly.h"

#include <string.h>

#include "sample/sample.h"

void poly_init(ring_t *r, poly_t *p) {
  p->b = malloc_or_die(sizeof(int_t) * r->n * r->d);
  p->r = r;
  p->is_ntt = 0;
}

void poly_clone(poly_t *dst, const poly_t *src) {
  poly_init(src->r, dst);
  memcpy(dst->b, src->b, sizeof(int_t) * src->r->d * src->r->n);
  dst->is_ntt = src->is_ntt;
}

void poly_init_rand(ring_t *r, poly_t *p, DISTRIBUTION d) {
  poly_init(r, p);
#pragma omp parallel for
  for (size_t j = 0; j < r->d; ++j) {
    int_t s = sample(d);
    for (size_t i = 0; i < r->n; ++i) p->b[i * r->d + j] = s % r->m[i];
  }
}

void poly_binop(poly_t *c, const poly_t *a, const poly_t *b,
                poly_binop_t binop) {
  ring_t *r = c->r;
#pragma omp parallel for
  for (size_t i = 0; i < r->n; ++i) {
    for (size_t j = 0; j < r->d; ++j) {
      int index = i * r->d + j;
      c->b[index] = binop(a->b[index], b->b[index], r->m[i]);
    }
  }
  c->is_ntt = a->is_ntt | b->is_ntt;
}

void poly_cmul(poly_t *c, const poly_t *a, int_t b) {
  ring_t *r = c->r;
#pragma omp parallel for
  for (size_t i = 0; i < r->n; ++i) {
    for (size_t j = 0; j < r->d; ++j) {
      int index = i * r->d + j;
      c->b[index] = modmul(a->b[index], b, r->m[i]);
    }
  }
  c->is_ntt = a->is_ntt;
}

void poly_neg(poly_t *p) {
  ring_t *r = p->r;
#pragma omp parallel for
  for (size_t i = 0; i < r->n; ++i) {
    for (size_t j = 0; j < r->d; ++j) {
      int index = i * r->d + j;
      p->b[index] = modsub(r->m[i], p->b[index], r->m[i]);
    }
  }
}

void poly_encode(ring_t *r, poly_t *p, const uint_t *x) {
  poly_init(r, p);
#pragma omp parallel for
  for (size_t i = 0; i < r->n; ++i) {
    for (size_t j = 0; j < r->d; ++j) p->b[i * r->d + j] = x[j] % r->m[i];
  }
  poly_ntt(p);
}

void poly_crt(uint_t *out, const poly_t *p, uint_t mod) {
  ring_t *r = p->r;
#pragma omp parallel for
  for (size_t i = 0; i < r->d; ++i) {
    mpz_t v, x;
    mpz_init(x);
    mpz_init(v);

    for (size_t j = 0; j < r->n; ++j) {
      mpz_mul_ui(v, r->ms[j], r->invms[j]);
      mpz_mul_ui(v, v, p->b[j * r->d + i]);
      mpz_add(x, x, v);
    }
    mpz_mod(x, x, r->M);
    if (mpz_cmp(x, r->M_half) > 0) mpz_sub(x, x, r->M);
    out[i] = mpz_fdiv_ui(x, mod);

    mpz_clear(v);
    mpz_clear(x);
  }
}

void poly_free(poly_t *r) {
  free(r->b);
  r->b = NULL;
  r->r = NULL;
  r->is_ntt = 0;
}
