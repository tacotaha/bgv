#include "utils/utils.h"
#include "rq.h"

void _ntt(int32_t * roots, int32_t * x, uint32_t q, uint32_t d) {
  int S, U, V;
  for (size_t m = 1, t = d >> 1; m < d; m <<= 1, t >>= 1) {
    for (size_t i = 0, k = 0; i < m; ++i, k += (t << 1)) {
      S = roots[m + i];
      for (size_t j = k; j < k + t; ++j) {
        U = x[j];
        V = modmul(x[j + t], S, q);
        x[j] = modadd(U, V, q);
        x[j + t] = modsub(U, V, q);
      }
    }
  }
}

void _intt(int32_t * iroots, int32_t * x, uint32_t q, uint32_t d) {
  uint32_t S, U, V, invd;
  for (size_t m = d >> 1, t = 1; m > 0; m >>= 1, t <<= 1)
    for (size_t i = 0, k = 0; i < m; ++i, k += (t << 1)) {
      S = iroots[m + i];
      for (size_t j = k; j < k + t; ++j) {
        U = x[j];
        V = x[j + t];
        x[j] = modadd(U, V, q);
        x[j + t] = modmul(modsub(U, V, q), S, q);
      }
    }
  invd = modinv(d, q);
  for (size_t i = 0; i < d; ++i) {
    x[i] = modmul(x[i], invd, q);
    x[i] -= q;
    x[i] += (x[i] >> 31) & q;
  }
}

void rq_ntt(rq_t * r) {
  level_t *l = r->l;
  if (!r->is_ntt) {
    for (size_t i = 0; i < r->l->m; ++i) {
      int offset = i * l->d;
      _ntt(l->roots + offset, r->b + offset, l->basis[i], l->d);
    }
    r->is_ntt |= 1;
  }
}

void rq_intt(rq_t * r) {
  level_t *l = r->l;
  if (r->is_ntt) {
    for (size_t i = 0; i < r->l->m; ++i) {
      int offset = i * l->d;
      _intt(l->iroots + offset, r->b + offset, l->basis[i], l->d);
    }
    r->is_ntt &= 0;
  }
}
