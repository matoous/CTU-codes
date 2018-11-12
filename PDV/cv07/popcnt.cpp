#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <immintrin.h>

int popcnt_intrinsic(uint64_t x) {
//  throw "Not implemented yet";
	return _mm_popcnt_u64(x);
  // Doimplementujte vypocet poctu jednickovych bitu pomoci intrinsic operace

	return 0;
}

int popcnt_naive(uint64_t x) {
	int count = 0;
	for(uint64_t i = 1ULL << 63 ; i > 0 ; i >>= 1) {
		if(x & i) count++;
	}
	return count;
}


int main() {
	using namespace std::chrono;

  // Vygenerujeme data
  // Vektory 'unsigned long long' nam mohou reprezentovat napriklad
  // mnozinu. Bit nastaveny na 1 znamena, ze prvek je obsazeny
  // v mnozine, 0 naopak ze prvek chybi.
	constexpr size_t N = 10000000;
	std::vector<unsigned long long> x(N);
	std::vector<unsigned long long> y(N);

	for(unsigned int i = 0 ; i < N ; i++) {
		x[i] = rand() + ((unsigned long long)rand() << 32);
		y[i] = rand() + ((unsigned long long)rand() << 32);
	}

	unsigned int count_popcnt = 0;
	unsigned int count_naive = 0;

	unsigned long elapsedIntristic = 0ULL;
	unsigned long elapsedNaive = 0ULL;

    // Spocitame naivni verzi
	{
		auto begin = steady_clock::now();
		for(unsigned int i = 0 ; i < N ; i++) {
			// V nasem pripade nas zajima, kolik prvku se nachazi v pruniku
			// mnozin x a y. Prunik ziskame jednoduse tak, ze provedeme ope-
			// raci 'and' (&) nad binarnimi vektory.
			// Kolik prvku se ve vysledne mnozine nachazi zjistime tak, ze
			// spocitame, kolik bitu je nastavenych na 1.
			count_naive += popcnt_naive(x[i] & y[i]);
		}
		auto end = steady_clock::now();
		elapsedNaive = duration_cast<microseconds>(end-begin).count();
		printf("Cas naivni verze:               %dms  \tspeedup 1x\n",elapsedNaive);
	}

    // Spocitame intrinsickou verzi
	{
		try {
			auto begin = steady_clock::now();
			for (unsigned int i = 0; i < N; i++) {
				count_popcnt += popcnt_intrinsic(x[i] & y[i]);
			}
			auto end = steady_clock::now();
			elapsedIntristic = duration_cast<microseconds>(end - begin).count();
			double speedup = (double) elapsedNaive / elapsedIntristic;
			printf("Cas intrinsicke verze:          %dms  \tspeedup %.2fx\n", elapsedIntristic, speedup);
		}
		catch(...){
			printf("Cas intrinsicke verze:          --- not implemented ---\n");
		}
	}

    // Zkontrolujeme spravnost vysledku
	printf("Vysledek je %s\n", (count_naive == count_popcnt) ? "spravny" : "spatny");

	return 0;
}