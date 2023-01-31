#include "sample/sample.h"
#include "utils/utils.h"
#include "ring/rq.h"
#include "rlwe.h"

void rlwe_keygen(rlwe_key_t * r) {
  rq_t e;

  // sample secret key
  rq_init_rand(&r->s, TERNARY);
  rq_ntt(&r->s);

  // sample a
  rq_init_rand(&r->a, UNIFORM);

  // sample error vector
  rq_init_rand(&e, ERR);
  rq_cmul(&e, &e, T);
  rq_ntt(&e);

  // e - a*s
  rq_clone(&r->b, &r->a);
  rq_mul(&r->b, &r->b, &r->s);
  rq_sub(&r->b, &e, &r->b);

  rq_free(&e);
}

void rlwe_encrypt(rlwe_ct_t * c, rlwe_key_t * r, rq_t * m) {
  rq_t u, e1, e2;

  rq_init_rand(&u, TERNARY);
  rq_ntt(&u);

  rq_init_rand(&e1, ERR);
  rq_cmul(&e1, &e1, T);
  rq_ntt(&e1);

  rq_init_rand(&e2, ERR);
  rq_cmul(&e2, &e2, T);
  rq_ntt(&e2);

  rq_init(&c->a);
  rq_mul(&c->a, &u, &r->a);
  rq_add(&c->a, &c->a, &e1);

  rq_init(&c->b);
  rq_mul(&c->b, &u, &r->b);
  rq_add(&c->b, &c->b, &e2);
  rq_add(&c->b, &c->b, m);

  rq_free(&u);
  rq_free(&e1);
  rq_free(&e2);
}

void rlwe_decrypt(rq_t * m, rlwe_ct_t * c, rlwe_key_t * r) {
  rq_mul(m, &c->a, &r->s);
  rq_add(m, m, &c->b);
  rq_intt(m);
}

void rlwe_key_free(rlwe_key_t * c) {
  rq_free(&c->a);
  rq_free(&c->b);
  rq_free(&c->s);
}

void rlwe_ct_free(rlwe_ct_t * c) {
  rq_free(&c->a);
  rq_free(&c->b);
}
