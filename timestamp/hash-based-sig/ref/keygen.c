#include <stdio.h>
#include "api.h"
#include "randombytes.h"
#include "speed.h"
#include "cpucycles.h"

#define MLEN 59
#define NTESTS 10000


//修改random_bytes.c 文件以更改随机源

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


    printf("生成公私密钥对\n");
    sig_keygen(pk, sk);
    printf("生成完毕\n");

    FILE* fpk, * fsk;
    fpk = fopen("alice.pk", "wb");
    fsk = fopen("alice.sk", "wb");
     

    fwrite(pk, SIG_PUBLICKEYBYTES, 1, fpk);
    fwrite(sk, SIG_SECRETKEYBYTES, 1, fsk);

    fclose(fpk);
    fclose(fsk);
    printf("已保存至文件");


    return 0;
}
