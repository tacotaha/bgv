#include <assert.h>

#include "params.h"
#include "utils/utils.h"

void rns_init(level_t * l) {
  uint32_t inv;
  l->crt_coefs = malloc_or_die(sizeof(mpz_t) * l->m);

  mpz_init_set_ui(l->Q, l->basis[0]);
  for (size_t i = 0; i < l->m; ++i)
    mpz_init(l->crt_coefs[i]);

  /* express Q as a product over the basis */
  for (size_t i = 1; i < l->m; ++i)
    mpz_mul_ui(l->Q, l->Q, l->basis[i]);

  /* generate CRT coefficients */
  for (size_t i = 0; i < l->m; ++i) {
    mpz_divexact_ui(l->crt_coefs[i], l->Q, l->basis[i]);
    inv = mpz_fdiv_ui(l->crt_coefs[i], l->basis[i]);
    inv = modinv(inv, l->basis[i]);
    mpz_mul_ui(l->crt_coefs[i], l->crt_coefs[i], inv);
  }
}

void rou_init(level_t * l, size_t lgd) {
  int32_t root, iroot, t, u;
  for (size_t i = 0; i < l->m; ++i) {
    l->roots[i * l->d] = 1;
    l->iroots[i * l->d] = 1;
  }

  for (uint32_t i = 0; i < l->m; ++i) {
    root = find_proot(l->basis[i], l->d << 1);
    iroot = modinv(root, l->basis[i]);

    /* primitive 2n-th root of unity */
    assert(modexp(root, l->d << 1, l->basis[i]) == 1);
    assert(modexp(iroot, l->d << 1, l->basis[i]) == 1);

    for (uint32_t j = 1; j < l->d; ++j) {
      t = i * l->d + (reverse(j) >> (32 - lgd));
      u = i * l->d + (reverse(j - 1) >> (32 - lgd));
      l->roots[t] = modmul(l->roots[u], root, l->basis[i]);
      l->iroots[t] = modmul(l->iroots[u], iroot, l->basis[i]);
    }
  }
}

void bgv_init(bgv_t * b, size_t lgq, size_t lgd, int l) {
  level_t *lvl;
  uint64_t nbits, m, d = (1UL << lgd);

  b->l = l;
  b->levels = malloc_or_die(sizeof(level_t) * l);

  for (int i = l - 1; i > -1; --i) {
    nbits = (i + 1) * lgq, m = (nbits / M) + 1;
    lvl = b->levels + i;
    lvl->m = m;
    lvl->d = d;
    if (i == l - 1) {
      lvl->basis = malloc_or_die(sizeof(uint32_t) * m);
      lvl->roots = malloc_or_die(sizeof(int32_t) * (m << lgd));
      lvl->iroots = malloc_or_die(sizeof(int32_t) * (m << lgd));
      gen_primes(nbits / m + 1, lgd + 1, lvl->basis, m);
      rou_init(lvl, lgd);
    } else {
      lvl->basis = b->levels[l - 1].basis;
      lvl->roots = b->levels[l - 1].roots;
      lvl->iroots = b->levels[l - 1].iroots;
    }
    rns_init(lvl);
  }
}

void bgv_free(bgv_t * b) {
  level_t *l;
  for (size_t i = 0; i < b->l; ++i) {
    l = b->levels + i;
    if (i == b->l - 1) {
      free(l->basis);
      free(l->roots);
      free(l->iroots);
    }

    mpz_clear(l->Q);
    for (size_t j = 0; j < l->m; ++j)
      mpz_clear(l->crt_coefs[j]);
    free(l->crt_coefs);

    l->basis = NULL;
    l->roots = NULL;
    l->iroots = NULL;
    l->crt_coefs = NULL;
  }

  free(b->levels);
}
