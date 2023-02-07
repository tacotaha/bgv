#include "bgv.h"

void bgv_ksgen(bgv_t *b, bgv_key_t *k, poly_t *s) {
  bgv_keypair_t *eval = &k->eval;
  poly_t e;

  poly_init_rand(&b->r, &e, ERR);
  poly_cmul(&e, &e, b->t);
  poly_ntt(&e);

  poly_init_rand(&b->r, &eval->a, UNIFORM);
  poly_ntt(&eval->a);

  poly_clone(&eval->b, &eval->a);
  poly_mul(&eval->b, &eval->b, &k->s);
  poly_sub(&eval->b, &e, &eval->b);
  poly_add(&eval->b, &eval->b, s);

  poly_free(&e);
}

void bgv_keygen(bgv_t *b, bgv_key_t *k) {
  bgv_keypair_t *pub = &k->pub;
  poly_t e;

  poly_init_rand(&b->r, &k->s, TERNARY);
  poly_ntt(&k->s);

  poly_init_rand(&b->r, &pub->a, UNIFORM);
  poly_ntt(&pub->a);

  poly_init_rand(&b->r, &e, ERR);
  poly_cmul(&e, &e, b->t);
  poly_ntt(&e);

  poly_clone(&pub->b, &pub->a);
  poly_mul(&pub->b, &pub->b, &k->s);
  poly_sub(&pub->b, &e, &pub->b);

  poly_mul(&e, &k->s, &k->s);
  bgv_ksgen(b, k, &e);

  poly_free(&e);
}

void bgv_encrypt(bgv_t *b, bgv_ct_t *c, bgv_keypair_t *k, poly_t *m) {
  poly_t u, e1, e2;

  bgv_ct_init(b, c, 2);

  poly_init_rand(&b->r, &u, TERNARY);
  poly_ntt(&u);

  poly_init_rand(&b->r, &e1, ERR);
  poly_cmul(&e1, &e1, b->t);
  poly_ntt(&e1);

  poly_init_rand(&b->r, &e2, ERR);
  poly_cmul(&e2, &e2, b->t);
  poly_ntt(&e2);

  poly_mul(c->c + 1, &u, &k->a);
  poly_add(c->c + 1, c->c + 1, &e1);

  poly_mul(c->c, &u, &k->b);
  poly_add(c->c, c->c, &e2);
  poly_add(c->c, c->c, m);

  poly_free(&u);
  poly_free(&e1);
  poly_free(&e2);
}

void bgv_decrypt(poly_t *m, bgv_ct_t *c, poly_t *s) {
  poly_clone(m, c->c + c->n - 1);
  for (size_t i = c->n - 1; i > 0; --i) {
    poly_mul(m, m, s);
    poly_add(m, m, c->c + i - 1);
  }
  poly_intt(m);
}

void bgv_ct_add(bgv_t *b, bgv_ct_t *out, bgv_ct_t *x, bgv_ct_t *y) {
  bgv_ct_init(b, out, x->n);
  for (size_t i = 0; i < out->n; ++i)
      poly_add(out->c + i, x->c + i, y->c + i);
}

void bgv_ct_mul(bgv_t *b, bgv_ct_t *c, bgv_ct_t *x, bgv_ct_t *y) {
  poly_t tmp;

  bgv_ct_init(b, c, x->n + 1);
  poly_init(c->c[0].r, &tmp);

  poly_mul(c->c, x->c, y->c);
  poly_mul(c->c + 2, x->c + 1, y->c + 1);

  poly_mul(c->c + 1, x->c, y->c + 1);
  poly_mul(&tmp, x->c + 1, y->c);
  poly_add(c->c + 1, c->c + 1, &tmp);

  poly_free(&tmp);
}

void bgv_ct_relin(bgv_ct_t *c, bgv_keypair_t *k) {
  poly_t tmp;

  poly_clone(&tmp, c->c + 2);
  poly_mul(&tmp, &tmp, &k->b);
  poly_mul(c->c + 2, c->c + 2, &k->a);

  poly_add(c->c, c->c, &tmp);
  poly_add(c->c + 1, c->c + 1, c->c + 2);

  c->n = 2;
  poly_free(c->c + 2);
  poly_free(&tmp);
}

void bgv_init(bgv_t *b, size_t lgd, size_t lgq, size_t lgm, size_t t) {
  b->t = t;
  ring_init(&b->r, lgd, lgq, lgm);
}

void bgv_ct_init(bgv_t *b, bgv_ct_t *c, size_t n) {
  c->c = malloc_or_die(sizeof(poly_t) * n);
  c->n = n;
  for (size_t i = 0; i < n; ++i) poly_init(&b->r, c->c + i);
}

void bgv_key_free(bgv_key_t *k) {
  poly_free(&k->s);
  poly_free(&k->pub.a);
  poly_free(&k->pub.b);
  poly_free(&k->eval.a);
  poly_free(&k->eval.b);
}

void bgv_ct_free(bgv_ct_t *c) {
  for (size_t i = 0; i < c->n; ++i) poly_free(c->c + i);
  free(c->c);
  c->c = NULL;
  c->n = 0;
}

void bgv_free(bgv_t *b) {
  b->t = 0;
  ring_free(&b->r);
}
