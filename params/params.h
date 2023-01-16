#ifndef PARAMS_H
#define PARAMS_H

#include <stdint.h>
#include <gmp.h>

/* Sampling parameters */
#define SIGMA 3.19
#define MU 0

/* Polynomial degree */
#define LGD 14
#define D (1 << LGD)

/* Plaintext modulus */
#define T 2

/* The length of the RNS decomposition of q */
#define M 8

/* Ciphertext modulus q */
extern const char *Q_str;
extern mpz_t Q;

/* RNS basis for q */
extern uint32_t rns_basis[M];

/* Used to convert back from an RNS representation */
extern mpz_t crt_coefs[M];

/* Montgomery encoding */

/* 1 / (R-q_i) mod R */
extern uint32_t r_minus_q_inv[M];

/* R^2 mod q_i */
extern uint32_t r_squared_mod_q[M];

#endif                          /* PARAMS_H */
