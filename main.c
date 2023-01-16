#include <stdio.h>

#include "params/params.h"
#include "rlwe/zq.h"

int main() {
  zq_t x;
  mpz_t out;

  mpz_init(out);
  mpz_init_set_str(Q, Q_str, 10);

  for (int i = 0; i < M; ++i)
    mpz_init(crt_coefs[M]);

  crt_init();

  zq_from_mpz(&x, Q);
  zq_to_mpz(out, &x);

  mpz_out_str(stdout, 10, out);
  printf("\n");

  for (int i = 0; i < M; ++i)
    mpz_clear(crt_coefs[M]);

  mpz_clear(out);
  mpz_clear(Q);

  return 0;
}
