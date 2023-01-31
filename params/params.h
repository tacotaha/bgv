#ifndef PARAMS_H
#define PARAMS_H

#include <stdint.h>
#include <gmp.h>

/* Polynomial degree */
#define LGD 14
#define D (1 << LGD)

/* Q_0 bit length */
#define LGQ 237

/* RNS bit length */
#define LGM 25

/* Number of RNS coefficients */
#define M (LGQ/LGM + 1)

/* Sampling parameters */
#define MU 0
#define SIGMA 3.19

/* Plaintext modulus */
#define T 65537

/* RNS basis for Q */
extern uint32_t _basis[M];

/* N-th Roots of unity */
extern int32_t _roots[M << LGD], _iroots[M << LGD];

/*  CRT coefficients */
extern mpz_t _Q, _Q_half, _crt_coefs[M];

/* Init parameters */
void bgv_init();
void bgv_free();

#endif                          /* PARAMS_H */
