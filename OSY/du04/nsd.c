#include "nsd.h"

#include <math.h>
#include <stdlib.h>

int nsd(int a, int b) {
	// urci nejvetsi spolecny delitel cisel a, b
	int ret;
	int min_sqrt = (abs(a) < abs(b)) ? abs(a) : abs(b);

	for (ret = min_sqrt; ret > 1; ret--) {
		if (((a % ret) == 0) && ((b % ret) == 0)) {
			break;
		}
	}
	return ret;
}
