#include <assert.h>
#include <string.h>

#define LGD 14
#define D (1 << LGD)
#define LGQ 237
#define LGM 25
#define T 65537

#include "bgv/bgv.h"
#include "ring/ring.h"
#include "utils/utils.h"

int main() {
  bgv_t b;
  bgv_key_t k;
  uint_t x[D] = {0}, y[D] = {0}, z[D];
  poly_t m, m2, p;
  bgv_ct_t c, c2, c3;

  bgv_init(&b, LGD, LGQ, LGM, T);

  x[0] = rand() % T;
  y[0] = rand() % T;

  poly_encode(&b.r, &m, x);
  poly_encode(&b.r, &m2, y);

  bgv_keygen(&b, &k);
  bgv_encrypt(&b, &c, &k.pub, &m);
  bgv_encrypt(&b, &c2, &k.pub, &m2);

  bgv_ct_add(&b, &c3, &c2, &c);
  bgv_decrypt(&p, &c3, &k.s);
  poly_crt(z, &p, T);

  assert(z[0] == modadd(x[0], y[0], T));

  bgv_ct_free(&c3);
  poly_free(&p);

  bgv_ct_mul(&b, &c3, &c2, &c);
  bgv_ct_relin(&c3, &k.eval);
  bgv_decrypt(&p, &c3, &k.s);
  poly_crt(z, &p, T);

  assert(z[0] == modmul(x[0], y[0], T));

  bgv_ct_free(&c);
  bgv_ct_free(&c2);
  bgv_ct_free(&c3);

  bgv_key_free(&k);

  poly_free(&m);
  poly_free(&m2);
  poly_free(&p);

  bgv_free(&b);

  return 0;
}
