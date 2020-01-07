/*
 * File name: nsd_main.c
 * Date:      2017/10/21 09:20
 * Author:    Jan Chudoba
 */

#include <stdio.h>

#include "nd.h"
#include "nsd.h"

int main(int argc, char ** argv)
{
	char line[256];

	while (fgets(line, sizeof(line), stdin)) {
		int a, b;
		if (sscanf(line, "%d %d", &a, &b) == 2) {
			int nd_a = nd(a);
			int nd_b = nd(b);
			int prime_a = (a != 1 && nd_a == 1);
			int prime_b = (b != 1 && nd_b == 1);
			if (prime_a && prime_b) {
				printf("Both %d and %d are prime\n", a, b);
			} else {
				printf("nsd(%d, %d) = %d\n", a, b, nsd(a, b));
			}
		} else {
			fprintf(stderr, "Invalid input: %s\n", line);
		}
	}
	fprintf(stderr, "NSD DONE\n");
	return 0;
}

/* end of nsd_main.c */
