#include <assert.h>
#include <gmp.h>

#include "utils/utils.h"
#include "params.h"

uint32_t _basis[M] = { 0 };

int32_t _roots[M << LGD], _iroots[M << LGD];
mpz_t _Q, _Q_half, _crt_coefs[M];

void bgv_init() {
  int32_t root, iroot, t, u, inv;

  /* generate special primes */
  gen_primes(LGM, LGD + 1, _basis, M);

  /* primitive 2n-th roots of unity */
  for (size_t i = 0; i < M; ++i) {
    _roots[i * D] = 1;
    _iroots[i * D] = 1;
  }

  for (uint32_t i = 0; i < M; ++i) {
    root = find_proot(_basis[i], LGD + 1);
    iroot = modinv(root, _basis[i]);

    assert(modexp(root, D << 1, _basis[i]) == 1);
    assert(modexp(iroot, D << 1, _basis[i]) == 1);

    for (uint32_t j = 1; j < D; ++j) {
      t = i * D + (reverse(j) >> (32 - LGD));
      u = i * D + (reverse(j - 1) >> (32 - LGD));
      _roots[t] = modmul(_roots[u], root, _basis[i]);
      _iroots[t] = modmul(_iroots[u], iroot, _basis[i]);
    }
  }

  /* CRT coefficients */
  mpz_init_set_ui(_Q, _basis[0]);
  for (int i = 1; i < M; ++i)
    mpz_mul_ui(_Q, _Q, _basis[i]);

  mpz_init_set(_Q_half, _Q);
  mpz_cdiv_q_ui(_Q_half, _Q, 2);

  for (int i = 0; i < M; ++i) {
    mpz_init(_crt_coefs[i]);
    mpz_divexact_ui(_crt_coefs[i], _Q, _basis[i]);
    inv = mpz_fdiv_ui(_crt_coefs[i], _basis[i]);
    inv = modinv(inv, _basis[i]);
    mpz_mul_ui(_crt_coefs[i], _crt_coefs[i], inv);
  }
}

void bgv_free() {
  for (int i = 0; i < M; ++i)
    mpz_clear(_crt_coefs[i]);
  mpz_clear(_Q);
  mpz_clear(_Q_half);
}
