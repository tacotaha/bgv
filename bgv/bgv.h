#ifndef BGV_H
#define BGV_H

#include "ring/poly.h"
#include "ring/ring.h"

typedef struct bgv_t {
  size_t t;
  ring_t r;
} bgv_t;

typedef struct bgv_keypair_t {
  poly_t a, b;
} bgv_keypair_t;

typedef struct bgv_key_t {
  poly_t s;
  bgv_keypair_t pub;
  bgv_keypair_t eval;
} bgv_key_t;

typedef struct bgv_ct_t {
  size_t n;
  poly_t *c;
} bgv_ct_t;

void bgv_init(bgv_t *, size_t, size_t, size_t, size_t);
void bgv_free(bgv_t *);

void bgv_keygen(bgv_t *, bgv_key_t *);
void bgv_ksgen(bgv_t *, bgv_key_t *, poly_t *);
void bgv_encrypt(bgv_t *, bgv_ct_t *, bgv_keypair_t *, poly_t *);
void bgv_decrypt(poly_t *, bgv_ct_t *, poly_t *);
void bgv_key_free(bgv_key_t *);

void bgv_ct_init(bgv_t *, bgv_ct_t *, size_t);
void bgv_ct_add(bgv_t *, bgv_ct_t *, bgv_ct_t *, bgv_ct_t *);
void bgv_ct_mul(bgv_t *, bgv_ct_t *, bgv_ct_t *, bgv_ct_t *);
void bgv_ct_relin(bgv_ct_t *, bgv_keypair_t *);
void bgv_ct_free(bgv_ct_t *);

#endif /* BGV_H */
