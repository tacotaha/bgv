#include <assert.h>
#include <string.h>

#include "params/params.h"
#include "utils/utils.h"
#include "rlwe/rlwe.h"
#include "ring/rq.h"

int main() {
  rq_t m, m2, p;
  rlwe_ct_t c, c2;
  rlwe_key_t r;
  int32_t x[D] = { 0 }, y[D] = { 0 }, z[D] = { 0 };

  bgv_init();

  rq_init(&p);
  rq_init(&m);
  rq_init(&m2);

  for (int i = 0; i < D; ++i) {
    x[i] = rand() % T;
    y[i] = rand() % T;
  }
  rq_encode(&m, x);
  rq_encode(&m2, y);

  rlwe_keygen(&r);
  rlwe_encrypt(&c, &r, &m);
  rlwe_encrypt(&c2, &r, &m2);

  rq_add(&c2.a, &c2.a, &c.a);
  rq_add(&c2.b, &c2.b, &c.b);

  rlwe_decrypt(&p, &c2, &r);
  rq_crt(z, &p);
  for (int i = 0; i < D; ++i)
    assert(z[i] == (x[i] + y[i]) % T);

  rq_free(&m);
  rq_free(&m2);
  rq_free(&p);

  rlwe_ct_free(&c);
  rlwe_ct_free(&c2);
  rlwe_key_free(&r);

  bgv_free();

  return 0;
}
