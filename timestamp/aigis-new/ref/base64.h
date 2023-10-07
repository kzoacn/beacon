#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>
#include <ctime>
#include <cstdlib>
/*
   base64.cpp and base64.h

   base64 encoding and decoding with C++.

   Version: 1.02.00

   Copyright (C) 2004-2017, 2020 Ren�� Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   Ren�� Nyffenegger rene.nyffenegger@adp-gmbh.ch

*/

std::string base64_encode(unsigned char const*, unsigned int len);
std::string base64_decode(std::string const& s);
#include <cctype>

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

static std::size_t pos_of_char(const unsigned char chr)
{
    //
    // Return the position of chr within base64_chars.
    //

    if (chr >= 'A' && chr <= 'Z')
        return chr - 'A';
    else if (chr >= 'a' && chr <= 'z')
        return chr - 'a' + ('Z' - 'A') + 1;
    else if (chr >= '0' && chr <= '9')
        return chr - '0' + ('Z' - 'A') + ('z' - 'a') + 2;
    else if (chr == '+')
        return 62;
    else if (chr == '/')
        return 63;

    throw "If input is correct, this line should never be reached.";
}

static inline bool is_base64(unsigned char c)
{
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len)
{
    std::string ret;
    int i = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--)
    {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3)
        {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4); i++)
                ret.push_back(base64_chars[char_array_4[i]]);
            i = 0;
        }
    }

    if (i)
    {
        for (int j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

        for (int j = 0; (j < i + 1); j++)
            ret.push_back(base64_chars[char_array_4[j]]);

        while ((i++ < 3))
            ret.push_back('=');
    }

    return ret;
}

std::string base64_decode(std::string const& encoded_string)
{
    size_t in_len = encoded_string.size();
    int i = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    ret.reserve(encoded_string.size() / 4 * 3);

    while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_]))
    {
        char_array_4[i++] = encoded_string[in_];
        in_++;
        if (i == 4)
        {
            for (i = 0; i < 4; i++)
            {
                char_array_4[i] = pos_of_char(char_array_4[i]);
            }

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret.push_back(char_array_3[i]);
            i = 0;
        }
    }

    if (i)
    {
        for (int j = 0; j < i; j++)
        {
            char_array_4[j] = pos_of_char(char_array_4[j]);
        }

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);

        for (int j = 0; (j < i - 1); j++)
            ret.push_back(char_array_3[j]);
    }

    return ret;
}

#include "Alg.h"
using std::string;
string sign_base64(string sk_s, string msg)
{

    unsigned char* sk;
    unsigned long long sk_byts;
    unsigned char* sn;
    unsigned long long sn_byts;

    sk_byts = sig_get_sk_byts();
    sk = new unsigned char[sk_byts];
    memcpy(sk, base64_decode(sk_s).c_str(), sk_byts);

    unsigned long long m_byts = msg.length();
    unsigned char* m = new unsigned char[m_byts];
    memcpy(m, msg.c_str(), m_byts);

    sn_byts = sig_get_sn_byts();
    sn = new unsigned char[sn_byts];

    sig_sign(sk, sk_byts, m, m_byts, sn, &sn_byts);

    return base64_encode(sn, sn_byts);
}
bool verify_base64(string pk_s, string sn_s, string msg)
{
    unsigned char* pk;
    unsigned long long pk_byts;
    unsigned char* sn;
    unsigned long long sn_byts;
    pk_byts = sig_get_pk_byts();
    pk = new unsigned char[pk_byts];
    memcpy(pk, base64_decode(pk_s).c_str(), pk_byts);

    unsigned long long m_byts = msg.length();
    unsigned char* m = new unsigned char[m_byts];
    memcpy(m, msg.c_str(), m_byts);

    sn_byts = sig_get_sn_byts();

    sn = new unsigned char[sn_byts];
    memcpy(sn, base64_decode(sn_s).c_str(), sn_byts);

    bool res = sig_verf(pk, pk_byts, sn, sn_byts, m, m_byts);
    return res;
}