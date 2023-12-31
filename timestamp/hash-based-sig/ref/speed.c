#include <stdlib.h>
#include <stdio.h>
#include "speed.h"


static int cmp_llu(const void *a, const void*b)
{
  if(*(unsigned long long *)a < *(unsigned long long *)b) return -1;
  if(*(unsigned long long *)a > *(unsigned long long *)b) return 1;
  return 0;
}

static unsigned long long median(unsigned long long *l, size_t llen)
{
  qsort(l,llen,sizeof(unsigned long long),cmp_llu);

  if(llen%2) return l[llen/2];
  else return (l[llen/2-1]+l[llen/2])/2;
}

static unsigned long long average(unsigned long long *t, size_t tlen)
{
  unsigned long long acc=0;
  size_t i;
  for(i=0;i<tlen;i++)
    acc += t[i];
  return acc/(tlen);
}

void print_results(const char *s, unsigned long long *t, size_t tlen)
{
  printf("%s\n", s);
  printf("median: %llu\n", median(t, tlen));
  printf("average: %llu\n", average(t, tlen - 1));
  printf("\n");
}
