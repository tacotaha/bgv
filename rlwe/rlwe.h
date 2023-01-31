#ifndef RLWE_H
#define RLWE_H

#include "params/params.h"
#include "ring/rq.h"

/* rlwe key pair */
typedef struct rlwe_key_t {
  rq_t s, a, b;
} rlwe_key_t;

/* ciphertext in R_q^2 */
typedef struct rlwe_ct_t {
  rq_t a, b;
} rlwe_ct_t;

void rlwe_keygen(rlwe_key_t *);
void rlwe_encrypt(rlwe_ct_t *, rlwe_key_t *, rq_t *);
void rlwe_decrypt(rq_t *, rlwe_ct_t *, rlwe_key_t *);

void rlwe_key_free(rlwe_key_t *);
void rlwe_ct_free(rlwe_ct_t *);

#endif                          /* RLWE_H */
