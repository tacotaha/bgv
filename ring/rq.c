#include "rq.h"
#include "utils/utils.h"

void rq_init(rq_t * r, level_t * l, int32_t * b) {
  r->is_ntt = 0;
  r->l = l;
  r->n = l->d * l->m;
  r->b = b ? b : malloc_or_die(sizeof(int32_t) * r->n);
}

void rq_add(rq_t * r, const rq_t * a, const rq_t * b) {
  int index;
  level_t *l = r->l;

  for (size_t i = 0; i < l->m; ++i)
    for (size_t j = 0; j < l->d; ++j) {
      index = i * l->d + j;
      r->b[index] = modadd(a->b[index], b->b[index], l->basis[i]);
    }
}

void rq_sub(rq_t * r, const rq_t * a, const rq_t * b) {
  int index;
  level_t *l = r->l;

  for (size_t i = 0; i < l->m; ++i)
    for (size_t j = 0; j < l->d; ++j) {
      index = i * l->d + j;
      r->b[index] = modsub(a->b[index], b->b[index], l->basis[i]);
    }
}

void rq_cmul(rq_t * r, const rq_t * a, int32_t b) {
  int index;
  level_t *l = r->l;

  for (size_t i = 0; i < l->m; ++i)
    for (size_t j = 0; j < l->d; ++j) {
      index = i * l->d + j;
      r->b[index] = modmul(a->b[index], b, l->basis[i]);
    }
}

void rq_mul(rq_t * r, rq_t * a, rq_t * b) {
  int index;
  level_t *l = r->l;

  rq_ntt(a);
  rq_ntt(b);

  for (size_t i = 0; i < l->m; ++i)
    for (size_t j = 0; j < l->d; ++j) {
      index = i * l->d + j;
      r->b[index] = modmul(a->b[index], b->b[index], l->basis[i]);
    }

  rq_intt(a);
  rq_intt(b);
  rq_intt(r);
}

void rq_free(rq_t * r) {
  free(r->b);
  r->is_ntt = 0;
  r->l = NULL;
  r->n = 0;
  r->b = NULL;
}
