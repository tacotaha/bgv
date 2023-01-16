#include <gmp.h>
#include "zq.h"

int inverse_mod(int x, int y) {
  int r = 1, s = 0, q, t, p = y, pos = 1;
  x %= y;
  while (p) {
    q = x / p;
    t = r + s * q;
    r = s;
    s = t;
    t = x - p * q;
    x = p;
    p = t;
    pos = !pos;
  }
  return pos ? r : y - r;
}

void crt_init() {
  uint32_t inv;
  for (int i = 0; i < M; ++i) {
    mpz_divexact_ui(crt_coefs[i], Q, rns_basis[i]);
    inv = mpz_fdiv_ui(crt_coefs[i], rns_basis[i]);
    inv = inverse_mod(inv, rns_basis[i]);
    mpz_mul_ui(crt_coefs[i], crt_coefs[i], inv);
  }
}

void zq_from_mpz(zq_t * r, const mpz_t z) {
  for (int i = 0; i < M; ++i)
    r->b[i] = to_mont(mpz_fdiv_ui(z, rns_basis[i]), i);
}

void zq_to_mpz(mpz_t z, const zq_t * r) {
  mpz_t b;
  mpz_init(b);

  for (int i = 0; i < M; ++i) {
    mpz_mul_ui(b, crt_coefs[i], redc(r->b[i], i));
    mpz_add(z, z, b);
  }

  mpz_mod(z, z, Q);
  mpz_clear(b);
}
