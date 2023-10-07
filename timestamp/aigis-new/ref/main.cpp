#include <iostream>
#include <fstream>
#include <cstdio>
#include "time.h"
#include "Alg.h"
#include "base64.h"
#include "randombytes.h"

using namespace std;
//�޸�randombytes.c�ļ��Ըı����Դ


void usage()
{
    printf("usage :\n\t aigis keygen <pk file> <sk file>\n");
    printf("\t aigis sign <sk file> <in file> <out file>\n");
    printf("\t aigis verify <pk file> <in file> <sign file>\n");
}

unsigned char* pk;
unsigned long long pk_byts;
unsigned char* sk;
unsigned long long sk_byts;
unsigned char* sn;
unsigned long long sn_byts;

void read_byts(ifstream& fin, unsigned char*& in, unsigned long long byts)
{
    string s;
    fin >> s;
    in = new unsigned char[byts];
    memcpy(in, base64_decode(s).c_str(), byts);
}

int main(int argc, char** argv)
{ 
    if (argc <= 1)
    {
        usage();
        return 0;
    }

    rand_init("random.txt");
    char op[50];
    sscanf(argv[1], "%s", op);
    if (op[0] == 'k')
    {
        pk = new unsigned char[sig_get_pk_byts()];
        sk = new unsigned char[sig_get_sk_byts()];
        sig_keygen(pk, &pk_byts, sk, &sk_byts);
        ofstream f1(argv[2]), f2(argv[3]);
        f1 << base64_encode(pk, pk_byts);
        f2 << base64_encode(sk, sk_byts);
        f1.close();
        f2.close();
    }
    else if (op[0] == 's')
    {
        ifstream f1(argv[2]), f2(argv[3], ios::binary | ios::in);
        ofstream f3(argv[4]);
        string sk_s;
        f1 >> sk_s;
        sk_byts = sig_get_sk_byts();
        sk = new unsigned char[sk_byts];
        memcpy(sk, base64_decode(sk_s).c_str(), sk_byts);

        f2.seekg(0, std::ios::end);
        unsigned long long m_byts = f2.tellg();
        f2.seekg(0, std::ios::beg);
        unsigned char* m = new unsigned char[m_byts];
        f2.read((char*)m, m_byts);

        sn_byts = sig_get_sn_byts();
        sn = new unsigned char[sn_byts];

        sig_sign(sk, sk_byts, m, m_byts, sn, &sn_byts);

        f3 << base64_encode(sn, sn_byts);
    }
    else if (op[0] == 'v')
    {
        ifstream f1(argv[2]), f2(argv[3], ios::binary | ios::in);
        ifstream f3(argv[4]);
        string pk_s;
        f1 >> pk_s;
        pk_byts = sig_get_pk_byts();
        pk = new unsigned char[pk_byts];
        memcpy(pk, base64_decode(pk_s).c_str(), pk_byts);

        f2.seekg(0, std::ios::end);
        unsigned long long m_byts = f2.tellg();
        f2.seekg(0, std::ios::beg);
        unsigned char* m = new unsigned char[m_byts];
        f2.read((char*)m, m_byts);

        string sn_s;
        sn_byts = sig_get_sn_byts();
        sn = new unsigned char[sn_byts];
        f3 >> sn_s;
        memcpy(sn, base64_decode(sn_s).c_str(), sn_byts);

        bool res = sig_verf(pk, pk_byts, sn, sn_byts, m, m_byts);
        cout << (res==0?"SUCCESS":"FAIL") << endl;
        return res;
    }
    else
    {
        usage();
    } 
    return 0;
}