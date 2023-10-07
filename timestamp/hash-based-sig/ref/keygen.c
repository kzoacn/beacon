#include <stdio.h>
#include "api.h"
#include "randombytes.h"
#include "speed.h"
#include "cpucycles.h"

#define MLEN 59
#define NTESTS 10000


//�޸�random_bytes.c �ļ��Ը������Դ

int main(void)
{
    unsigned int i;
    int ret;
    unsigned long long mlen = MLEN, smlen;
    unsigned char m[64];
    unsigned char sm[SIG_BYTES];
    unsigned char pk[SIG_PUBLICKEYBYTES];
    unsigned char sk[SIG_SECRETKEYBYTES];
    unsigned long long t0[NTESTS], t1[NTESTS], t2[NTESTS];


    printf("���ɹ�˽��Կ��\n");
    sig_keygen(pk, sk);
    printf("�������\n");

    FILE* fpk, * fsk;
    fpk = fopen("alice.pk", "wb");
    fsk = fopen("alice.sk", "wb");
     

    fwrite(pk, SIG_PUBLICKEYBYTES, 1, fpk);
    fwrite(sk, SIG_SECRETKEYBYTES, 1, fsk);

    fclose(fpk);
    fclose(fsk);
    printf("�ѱ������ļ�");


    return 0;
}
