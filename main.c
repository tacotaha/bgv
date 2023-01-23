#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "params/params.h"
#include "utils/utils.h"
#include "ring/rq.h"

int main() {
  bgv_t b;
  rq_t r1;
  level_t *l;

  bgv_init(&b, LGQ, LGD, L);

  for (size_t i = 0; i < L; ++i) {
    l = b.levels + i;
    rq_init(&r1, l, NULL);

    srand(0);
    for (size_t j = 0; j < l->m; ++j)
      for (size_t k = 0; k < l->d; ++k)
        r1.b[j * l->d + k] = rand() % l->basis[j];

    rq_ntt(&r1);
    rq_intt(&r1);

    srand(0);
    for (size_t j = 0; j < l->m; ++j)
      for (size_t k = 0; k < l->d; ++k)
        assert(r1.b[j * l->d + k] == (int32_t) (rand() % l->basis[j]));

    rq_free(&r1);
  }

  bgv_free(&b);

  return 0;
}
