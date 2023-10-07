#pragma once

#include "api.h"
typedef unsigned char* puchar_t;
typedef unsigned long long puchar_byts_t;

puchar_byts_t sig_get_pk_byts()
{
    return SIG_PUBLICKEYBYTES;
}
puchar_byts_t sig_get_sk_byts()
{
    return SIG_SECRETKEYBYTES;
}
puchar_byts_t sig_get_sn_byts()
{
    return SIG_BYTES;
}

int sig_keygen(puchar_t pk, puchar_byts_t* pk_byts,
    puchar_t sk, puchar_byts_t* sk_byts)
{
    *pk_byts = SIG_PUBLICKEYBYTES;
    *sk_byts = SIG_SECRETKEYBYTES;
    return msig_keygen(pk, sk);
}

int sig_sign(puchar_t sk, puchar_byts_t sk_byts,
    puchar_t m, puchar_byts_t m_byts,
    puchar_t sn, puchar_byts_t* sn_byts)
{
    return msig_sign(sk, m, m_byts, sn, sn_byts);
}

int sig_verf(
    puchar_t pk, puchar_byts_t pk_byts,
    puchar_t sn, puchar_byts_t sn_byts,
    puchar_t m, puchar_byts_t m_byts)
{
    return msig_verf(pk, sn, sn_byts, m, m_byts);
}
