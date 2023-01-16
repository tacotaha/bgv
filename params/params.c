#include <stdint.h>
#include <gmp.h>

#include "params.h"

/* CRT coefficients */
mpz_t crt_coefs[M];

/* Ciphertext modulus q */
mpz_t Q;
const char *Q_str = "50679783804403462340256613581969530309612361497226257802787414186716561409";

/* RNS basis for q */
uint32_t rns_basis[] =
  { 0x65f38001, 0x552f0001, 0x65fc8001, 0x4af38001, 0x7a808001, 0x47e00001, 0x7f420001, 0x6c8b0001 };

// 1 / (R-q_i) mod R
uint32_t r_minus_q_inv[] =
  { 0x25f37fff, 0x552effff, 0x25fc7fff, 0xaf37fff, 0x3a807fff, 0x47dfffff, 0x7f41ffff, 0x6c8affff };

// R^2 mod q_i
uint32_t r_squared_mod_q[] =
  { 0x178a36b1, 0x3c8d64c9, 0x58c3f0c1, 0x470d0e72, 0x51c55180, 0x1dd1dd53, 0x23a39150, 0x45677e6d };
