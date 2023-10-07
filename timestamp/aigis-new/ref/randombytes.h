#ifndef RANDOMBYTES_H
#define RANDOMBYTES_H

void rand_init(const char* filename);
void randombytes(unsigned char *x, int xlen);//always assume that xlen%8 == 0

#endif
