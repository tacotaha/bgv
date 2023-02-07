#ifndef RING_H
#define RING_H

#include <gmp.h>

#include "utils/utils.h"

typedef struct ring_t {
  size_t d, n;
  uint_t *m, *invms;
  int_t *roots, *iroots;
  mpz_t M, M_half, *ms;
} ring_t;

void ring_init(ring_t *, size_t, size_t, size_t);
void ring_free(ring_t *);

#endif /* RING_H */
