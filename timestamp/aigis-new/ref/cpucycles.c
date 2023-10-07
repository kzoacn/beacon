#include "cpucycles.h"
#include <intrin.h>

#pragma intrinsic(__rdtsc)

long long cpucycles(void)
{
	return __rdtsc();
}