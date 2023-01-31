#include <gmp.h>
#include <assert.h>
#include <string.h>

#include "sample/sample.h"
#include "utils/utils.h"
#include "rq.h"

void rq_init(rq_t * r) {
  r->b = malloc_or_die(sizeof(int32_t) * (M << LGD));
}

void rq_clone(rq_t * dst, const rq_t * src) {
  rq_init(dst);
  memcpy(dst->b, src->b, sizeof(int32_t) * (M << LGD));
}

void rq_init_rand(rq_t * r, DISTRIBUTION d) {
  int32_t s;
  rq_init(r);
  for (size_t j = 0; j < D; ++j) {
    s = sample(d);
    for (size_t i = 0; i < M; ++i)
      r->b[i * D + j] = s % _basis[i];
  }
}

void rq_add(rq_t * r, const rq_t * a, const rq_t * b) {
  int index;
  for (size_t i = 0; i < M; ++i)
    for (size_t j = 0; j < D; ++j) {
      index = i * D + j;
      r->b[index] = modadd(a->b[index], b->b[index], _basis[i]);
    }
}

void rq_sub(rq_t * r, const rq_t * a, const rq_t * b) {
  int index;
  for (size_t i = 0; i < M; ++i)
    for (size_t j = 0; j < D; ++j) {
      index = i * D + j;
      r->b[index] = modsub(a->b[index], b->b[index], _basis[i]);
    }
}

void rq_mul(rq_t * r, const rq_t * a, const rq_t * b) {
  int index;
  for (size_t i = 0; i < M; ++i)
    for (size_t j = 0; j < D; ++j) {
      index = i * D + j;
      r->b[index] = modmul(a->b[index], b->b[index], _basis[i]);
    }
}

void rq_cmul(rq_t * r, const rq_t * a, int32_t b) {
  int index;
  for (size_t i = 0; i < M; ++i)
    for (size_t j = 0; j < D; ++j) {
      index = i * D + j;
      r->b[index] = modmul(a->b[index], b, _basis[i]);
    }
}

void rq_neg(rq_t * r) {
  int index;
  for (size_t i = 0; i < M; ++i)
    for (size_t j = 0; j < D; ++j) {
      index = i * D + j;
      r->b[index] = modsub(_basis[i], r->b[index], _basis[i]);
    }
}

int rq_eq(const rq_t * a, const rq_t * b) {
  int index;
  for (size_t i = 0; i < M; ++i)
    for (size_t j = 0; j < D; ++j) {
      index = i * D + j;
      if (modsub(a->b[index], b->b[index], _basis[i]) != 0)
        return 0;
    }
  return 1;
}

void rq_encode(rq_t * r, int32_t * x) {
  for (int i = 0; i < M; ++i)
    for (int j = 0; j < D; ++j)
      r->b[i * D + j] = x[j] % _basis[i];
  rq_ntt(r);
}

void rq_crt(int32_t * out, rq_t * r) {
  mpz_t b, x;

  mpz_init(x);
  mpz_init(b);

  for (int i = 0; i < D; ++i) {
    mpz_set_ui(x, 0);
    for (int j = 0; j < M; ++j) {
      mpz_mul_ui(b, _crt_coefs[j], r->b[j * D + i]);
      mpz_mod(b, b, _Q);
      mpz_add(x, x, b);
    }
    mpz_mod(x, x, _Q);
    if (mpz_cmp(x, _Q_half) > 0)
      mpz_sub(x, x, _Q);
    out[i] = mpz_fdiv_ui(x, T);
  }

  mpz_clear(b);
  mpz_clear(x);
}

void rq_free(rq_t * r) {
  free(r->b);
  r->b = NULL;
}
