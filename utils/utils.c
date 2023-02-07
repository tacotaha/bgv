#include "utils/utils.h"

#include <math.h>

#include "sample/sample.h"

int is_prime(uint_t p) {
  uint_t x, r, s, t;

  if (p < 2 || (p != 2 && p % 2 == 0)) return 0;

  s = p - 1;
  x = __builtin_ctzll(s);
  s >>= x;

  for (int i = 0; i < MR_RUNS; ++i) {
    r = uniform32() % (p - 1) + 1;
    t = modexp(r, s, p);

    uint_t j = 0;
    while (j++ < x && t != 1 && t != p - 1) t = modmul(t, t, p);

    if (t != p - 1 && j > 1) return 0;
  }
  return 1;
}

/* Generates a sequence of l-bit primes of the form
 * p = k * 2^m + 1
 */
void gen_primes(uint_t l, uint_t m, uint_t* p, size_t n) {
  static int k = 0;

  m = (1UL << m);
  l = (1UL << l);

  for (size_t i = 0; i < n; ++i) do {
      p[i] = l + k++ * m + 1;
    } while (!is_prime(p[i]));
}

/* Find a primitive 2^m-th root of unity */
uint_t find_proot(uint_t p, uint_t lgn) {
  uint_t r;

  do {
    r = (uniform32() % (p - 1)) + 2;
  } while (modexp(r, (p - 1) >> 1, p) == 1);

  return modexp(r, (p - 1) >> lgn, p);
}

int modinv(int x, int y) {
  int r = 1, s = 0, q, t, p = y, pos = 1;
  x %= y;
  while (p) {
    q = x / p;
    t = r + s * q;
    r = s;
    s = t;
    t = x - p * q;
    x = p;
    p = t;
    pos = !pos;
  }
  return pos ? r : y - r;
}

uint_t modexp(uint_t x, uint_t y, uint_t p) {
  uint_t r = 1, z = x;
  for (uint_t i = y; i; i >>= 1) {
    if (i & 1) r = modmul(r, z, p);
    z = modmul(z, z, p);
  }

  return r;
}
