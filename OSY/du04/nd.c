#include "nd.h"

#include <math.h>
#include <stdlib.h>

int nd(int a) {
	// vypocet nejvetsiho delitele cisla a
	int ret;
	int min_sqrt;

	if (a == 1) {
		ret = 1;
	} else {
		min_sqrt = abs(a) / 2;
		for (ret = min_sqrt; ret > 1; ret--) {
			if ((a % ret) == 0) {
				break;
			}
		}
	}
	return ret;
}
