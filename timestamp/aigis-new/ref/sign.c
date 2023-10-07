#include <stdint.h>
#include "api.h"
#include "params.h"
#include "sign.h"
#include "randombytes.h" 
#include "stdlib.h" 
#include "picosha2.h"
#include <cstring>


void prf(unsigned char* buf, int buflen,unsigned char *sk,unsigned char *input,int inlen)
{
	while (buflen > 0)
	{
		unsigned char buffer[64],tmp[32];
		memset(buffer, 0, sizeof(buffer));
		memcpy(buffer, sk, sizeof(buffer));
		memcpy(buffer+32, input, inlen);
		picosha2::hash256(buffer, buffer + 32+inlen, tmp, tmp + 32); 

		memcpy(buf, tmp, 8);
		buf += 8;
		buflen -= 8;
	}
}
void wots_keygen(unsigned char* wots_pk, unsigned char* wots_sk, unsigned char* sk,unsigned long long index) {
	for (int i = 0; i < PARAM_N; i++) {
		index ^= i << 16;
		prf(wots_sk + i * HASHBYTES, HASHBYTES, sk, (unsigned char*)&index, sizeof(index));
		index ^= i << 16;
		picosha2::hash256(wots_sk + i * HASHBYTES, wots_sk + (i + 1) * HASHBYTES, wots_pk + i * HASHBYTES, wots_pk + (i + 1) * HASHBYTES);
	}
}


int msig_keygen(unsigned char *pk, unsigned char *sk) 
{ 
	unsigned char wots_sk[HASHBYTES * PARAM_N];
	randombytes(sk, HASHBYTES);
	wots_keygen(pk, wots_sk, sk, 0);
  return 0;
}


int msig_sign(unsigned char *sk, 
                   unsigned char *m, unsigned long long mlen, 
                   unsigned char *sm, unsigned long long *smlen)
{ 
	unsigned char digest[HASHBYTES];
	picosha2::hash256(m, m+ mlen, digest, digest + 32);
	for (int i = 0; i < PARAM_N/8; i++) {
		for (int j = 0; j < 8; j++) {
			int k = (i * 8 + j);
			int bit = digest[i] >> j & 1;
			unsigned char wots_pk[HASHBYTES * PARAM_N];
			unsigned char wots_sk[HASHBYTES * PARAM_N];
			wots_keygen(wots_pk, wots_sk, sk, k);
			if (bit)
				memcpy(sm + k * HASHBYTES * PARAM_N, wots_pk, HASHBYTES * PARAM_N);
			else
				memcpy(sm + k * HASHBYTES * PARAM_N, wots_sk, HASHBYTES * PARAM_N);
		}
	}
	memcpy(sm, digest, HASHBYTES);
  return 0;
}


int msig_verf(unsigned char *pk,
                   unsigned char *sm, unsigned long long smlen,
                   unsigned char *m, unsigned long long mlen)
{ 
	unsigned char digest[HASHBYTES];
	picosha2::hash256(m, m + mlen, digest, digest + 32);
	if (memcmp(sm, digest, HASHBYTES))
		return -1;
	for (int i = 0; i < PARAM_N / 8; i++) {
		for (int j = 0; j < 8; j++) {
			int k = (i * 8 + j);
			int bit = digest[i] >> j & 1;
			unsigned char wots_pk[HASHBYTES * PARAM_N];
			unsigned char wots_sk[HASHBYTES * PARAM_N];
			unsigned char buff[HASHBYTES];
			picosha2::hash256(sm + k * HASHBYTES * PARAM_N, sm + (k + 1) * HASHBYTES * PARAM_N, buff, buff + 32);
			if (!memcmp(buff, wots_pk, HASHBYTES))
				return -1;
		}
	}
	return 0;
}
