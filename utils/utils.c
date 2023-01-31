#include <math.h>

#include "utils/utils.h"

int is_prime(uint64_t p) {
  uint64_t x, r, s, t;

  if (p < 2 || (p != 2 && p % 2 == 0))
    return 0;

  s = p - 1;
  x = __builtin_ctzll(s);
  s >>= x;

  for (int i = 0; i < MR_RUNS; ++i) {
    r = rand64() % (p - 1) + 1;
    t = modexp(r, s, p);

    uint64_t j = 0;
    while (j++ < x && t != 1 && t != p - 1)
      t = modmul(t, t, p);

    if (t != p - 1 && j > 1)
      return 0;
  }
  return 1;
}

/* Generates a sequence of l-bit primes of the form
 * p = k * 2^m + 1
 */
void gen_primes(uint32_t l, uint32_t m, uint32_t * p, size_t n) {
  int k = 0;

  m = (1UL << m);
  l = (1UL << l);

  for (size_t i = 0; i < n; ++i)
    do {
      p[i] = l + k++ * m + 1;
    } while (!is_prime(p[i]));
}


/* Find a primitive 2^m-th root of unity */
uint64_t find_proot(uint64_t p, uint64_t lgn) {
  uint64_t r;

  do {
    r = (rand64() % (p - 1)) + 2;
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

uint64_t modexp(uint64_t x, uint64_t y, uint64_t p) {
  uint64_t r = 1, z = x;
  for (uint64_t i = y; i; i >>= 1) {
    if (i & 1)
      r = modmul(r, z, p);
    z = modmul(z, z, p);
  }

  return r;
}

uint64_t modsqrt(uint64_t x, uint64_t p) {
  uint64_t b, bb, i, u, m, c, t, r, z = 1, q = p - 1, s = __builtin_ctzll(q);

  // p = 3 mod 4
  if (s == 1)
    return modexp(x, (p + 1) >> 2, p);

  q >>= s;

  // find a qnr mod p
  while (modexp(++z, (p - 1) >> 1, p) != p - 1);

  m = s;
  c = modexp(z, q, p);
  t = modexp(x, q, p);
  r = modexp(x, (q + 1) >> 1, p);

  while (t != 1) {

    for (i = 0, u = t; u != 1; u = modmul(u, u, p))
      if (++i == m)
        return 0;               // qnr

    b = modexp(c, modexp(2, m - i - 1, p - 1), p);
    bb = modmul(b, b, p);

    m = i;
    c = bb;
    t = modmul(t, bb, p);
    r = modmul(r, b, p);
  }

  return r;
}
