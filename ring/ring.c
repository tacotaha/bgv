#include "ring.h"

#include <assert.h>

#include "utils/utils.h"

void ring_init(ring_t* r, size_t lgd, size_t lgq, size_t lgm) {
  int_t root, iroot, t, u;
  r->d = (1 << lgd);
  r->n = (lgq / lgm) + 1;

  r->m = malloc_or_die(sizeof(int_t) * r->n);
  r->ms = malloc_or_die(sizeof(mpz_t) * r->n);
  r->invms = malloc_or_die(sizeof(int_t) * r->n);
  r->roots = malloc_or_die(sizeof(int_t) * (r->n << lgd));
  r->iroots = malloc_or_die(sizeof(int_t) * (r->n << lgd));

  gen_primes(lgm, lgd + 1, r->m, r->n);

  for (size_t i = 0; i < r->n; ++i) {
    r->roots[i * r->d] = 1;
    r->iroots[i * r->d] = 1;
  }

  for (uint_t i = 0; i < r->n; ++i) {
    root = find_proot(r->m[i], lgd + 1);
    iroot = modinv(root, r->m[i]);
    assert(modexp(root, r->d << 1, r->m[i]) == 1);
    assert(modexp(iroot, r->d << 1, r->m[i]) == 1);
    for (uint_t j = 1; j < r->d; ++j) {
      t = i * r->d + (reverse(j) >> (32 - lgd));
      u = i * r->d + (reverse(j - 1) >> (32 - lgd));
      r->roots[t] = modmul(r->roots[u], root, r->m[i]);
      r->iroots[t] = modmul(r->iroots[u], iroot, r->m[i]);
    }
  }

  mpz_init_set_ui(r->M, r->m[0]);
  for (size_t i = 1; i < r->n; ++i) mpz_mul_ui(r->M, r->M, r->m[i]);

  mpz_init_set(r->M_half, r->M);
  mpz_cdiv_q_ui(r->M_half, r->M_half, 2);

  for (size_t i = 0; i < r->n; ++i) {
    mpz_init(r->ms[i]);
    mpz_divexact_ui(r->ms[i], r->M, r->m[i]);
    r->invms[i] = mpz_fdiv_ui(r->ms[i], r->m[i]);
    r->invms[i] = modinv(r->invms[i], r->m[i]);
  }
}

void ring_free(ring_t* r) {
  mpz_clear(r->M);
  mpz_clear(r->M_half);
  for (size_t i = 0; i < r->n; ++i) mpz_clear(r->ms[i]);
  free(r->m);
  free(r->ms);
  free(r->invms);
  free(r->roots);
  free(r->iroots);
}
