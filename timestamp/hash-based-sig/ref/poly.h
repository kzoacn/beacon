#ifndef POLY_H
#define POLY_H

#include <stdint.h>
#include "params.h"
#include "fips202.h"

typedef __declspec(align(32)) struct {
  uint32_t coeffs[PARAM_N];
} poly;

void poly_copy(poly *b, const poly *a);
void poly_freeze2q(poly *a);
void poly_freeze4q(poly *a);

void poly_add(poly *c, const poly *a, const poly *b);
void poly_sub(poly *c, const poly *a, const poly *b);
void poly_neg(poly *a);
void poly_shiftl(poly *a, unsigned int k);

void poly_ntt(poly *a);
void poly_invntt_montgomery(poly *a);
void poly_pointwise_invmontgomery(poly *c, const poly *a, const poly *b);

int  poly_chknorm(const poly *a, uint32_t B);
void poly_uniform(poly *a, unsigned char *buf);
void poly_uniform_eta1(poly *a,
                      const unsigned char seed[SEEDBYTES],
                      unsigned char nonce);
void poly_uniform_eta2(poly *a,
                      const unsigned char seed[SEEDBYTES],
                      unsigned char nonce);
void poly_uniform_gamma1m1(poly *a,
                           const unsigned char seed[SEEDBYTES + CRHBYTES],
                           uint16_t nonce);

void polyeta1_pack(unsigned char *r, const poly *a);
void polyeta1_unpack(poly *r, const unsigned char *a);
void polyeta2_pack(unsigned char *r, const poly *a);
void polyeta2_unpack(poly *r, const unsigned char *a);

void polyt1_pack(unsigned char *r, const poly *a);
void polyt1_unpack(poly *r, const unsigned char *a);

void polyt0_pack(unsigned char *r, const poly *a);
void polyt0_unpack(poly *r, const unsigned char *a);

void polyz_pack(unsigned char *r, const poly *a);
void polyz_unpack(poly *r, const unsigned char *a);

void polyw1_pack(unsigned char *r, const poly *a);
#endif
