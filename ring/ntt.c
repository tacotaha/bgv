#include "poly.h"
#include "ring.h"
#include "utils/utils.h"

void _ntt(int_t *roots, int_t *x, uint_t d, uint_t q) {
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

void _intt(int_t *iroots, int_t *x, uint_t d, uint_t q) {
  uint_t S, U, V, invd;
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

void poly_ntt(poly_t *p) {
  if (!p->is_ntt) {
    ring_t *r = p->r;
#pragma omp parallel for
    for (size_t i = 0; i < r->n; ++i) {
      int offset = i * r->d;
      _ntt(r->roots + offset, p->b + offset, r->d, r->m[i]);
    }
    p->is_ntt = 1;
  }
}

void poly_intt(poly_t *p) {
  if (p->is_ntt) {
    ring_t *r = p->r;
#pragma omp parallel for
    for (size_t i = 0; i < r->n; ++i) {
      int offset = i * r->d;
      _intt(r->iroots + offset, p->b + offset, r->d, r->m[i]);
    }
    p->is_ntt = 0;
  }
}
