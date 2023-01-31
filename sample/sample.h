#ifndef SAMPLE_H
#define SAMPLE_H

#include <stdint.h>
#include <math.h>

#include "entropy.h"
#include "params/params.h"

/* exp(-0.5) */
#define P ((double)0.606530659712633)

typedef enum DISTRIBUTION {
  UNIFORM = 0,
  TERNARY,
  ERR,
} DISTRIBUTION;

/* half-exponential Bernoulli trial */
static inline uint8_t bernoulli() {
  return uniformr() < P;
}

/* sample k >= 0 w.p. exp(k/2)(1−1/sqrt(e)) */
static inline uint32_t G() {
  int n = 0;
  while (bernoulli())
    ++n;
  return n;
}

/* accept k w.p. exp(−k(k − 1)/2) */
static inline int32_t S() {
  for (;;) {
    int k = G();
    if (k < 2)
      return k;
    int z = k * (k - 1);
    while (z-- && bernoulli());
    if (z < 0)
      return k;
  }
}

static inline int32_t sample_err() {
  for (;;) {
    int k = S();

    int s = uniformb()? -1 : 1;

    double xn0 = k * SIGMA + s * MU;
    int i0 = ceil(xn0);
    xn0 = (i0 - xn0) / SIGMA;
    int j = uniform32() % (int) ceil(SIGMA);

    double x = xn0 + ((double) j) / SIGMA;
    if (x >= 1 || (x == 0 && s < 0 && k == 0))
      continue;

    xn0 = exp(-x * ((k << 1) + x) / 2);
    if (x == 0 || uniformr() <= xn0)
      return s * (i0 + j);
  }
}

static inline int32_t sample(DISTRIBUTION d) {
  switch (d) {
    case UNIFORM:
      return uniform32();
    case TERNARY:
      return (uniform32() % 3) - 1;
    case ERR:
      return sample_err();
  };
  return -1;
}

#endif                          /* SAMPLE_H */
