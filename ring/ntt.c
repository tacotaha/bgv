#include "utils/utils.h"
#include "rq.h"

void _ntt(int32_t * roots, int32_t * x, uint32_t q) {
  int S, U, V;
  for (size_t m = 1, t = D >> 1; m < D; m <<= 1, t >>= 1) {
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

void _intt(int32_t * iroots, int32_t * x, uint32_t q) {
  uint32_t S, U, V, invd;
  for (size_t m = D >> 1, t = 1; m > 0; m >>= 1, t <<= 1)
    for (size_t i = 0, k = 0; i < m; ++i, k += (t << 1)) {
      S = iroots[m + i];
      for (size_t j = k; j < k + t; ++j) {
        U = x[j];
        V = x[j + t];
        x[j] = modadd(U, V, q);
        x[j + t] = modmul(modsub(U, V, q), S, q);
      }
    }
  invd = modinv(D, q);
  for (size_t i = 0; i < D; ++i) {
    x[i] = modmul(x[i], invd, q);
    x[i] -= q;
    x[i] += (x[i] >> 31) & q;
  }
}

void rq_ntt(rq_t * r) {
  for (size_t i = 0; i < M; ++i) {
    int offset = i * D;
    _ntt(_roots + offset, r->b + offset, _basis[i]);
  }
}

void rq_intt(rq_t * r) {
  for (size_t i = 0; i < M; ++i) {
    int offset = i * D;
    _intt(_iroots + offset, r->b + offset, _basis[i]);
  }
}
