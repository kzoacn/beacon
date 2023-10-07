#include "randombytes.h"
#include "picosha2.h" 
#include <cstdio>
#include "cstring"

unsigned char buffer[256];
unsigned char tmp[32];
int counter;

void rand_init(const char *filename) {
	FILE* fr;
	fr = fopen(filename, "rb");
	fread(buffer, 1, 256, fr);
	counter = 0;
	fclose(fr);
}

void randombytes(unsigned char *buf, int buflen)//�޸Ĵ˺������滻����������ʹ�õ������
{
	while (buflen > 0)
	{
		//_rdrand64_step((unsigned long long *)buf);
		memcpy(buffer, &counter, sizeof(counter));
		picosha2::hash256(buffer, buffer + 32, tmp, tmp + 32);
		counter += 1;

		memcpy(buf, tmp, 8);
		buf += 8;
		buflen -= 8;
	}
}