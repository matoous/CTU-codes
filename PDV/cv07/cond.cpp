#include <iostream>
#include <chrono>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <immintrin.h>

using namespace std::chrono;

unsigned long elapsedScalar = 0ULL;
unsigned long elapsedVectorized = 0ULL;

void condswap_vec(float * data, size_t N) {
    auto begin = steady_clock::now();
    size_t half = N / 2;
    try {

        for (unsigned int i = 0; i < half; i += 8) {
            // load
            __m256 A = _mm256_loadu_ps(&data[i]);
            __m256 B = _mm256_loadu_ps(&data[i+half]);

            // 1 where B is smaller than A, 0 where A is smaller than B
            __m256 mask = _mm256_cmp_ps(B, A, _CMP_LT_OQ);

            // smaller elements first
            __m256 res = _mm256_blendv_ps(A, B, mask);
            _mm256_storeu_ps(&data[i], res);

            // bigger elements to second half
            res = _mm256_blendv_ps(B, A, mask);
            _mm256_storeu_ps(&data[i+half], res);
        }
        auto end = steady_clock::now();
        elapsedVectorized = duration_cast<microseconds>(end - begin).count();

        double speedup = (double) elapsedScalar / elapsedVectorized;
        printf("Cas vektorizovane verze:        %dms  \tspeedup %.2fx\n", elapsedVectorized, speedup);
    }
    catch (...){
        printf("Cas vektorizovane verze:        --- not implemented ---\n");
    }
}

void condswap_scalar(float * data, size_t N) {
    auto begin = steady_clock::now();
    size_t half = N / 2;
    for(unsigned int i = 0 ; i < half ; i++) {
        if(data[i] > data[i+half]) std::swap(data[i], data[i+half]);
    }
    auto end = steady_clock::now();
    elapsedScalar = duration_cast<microseconds>(end - begin).count();

    printf("Cas skalarni verze:             %dms  \tspeedup 1x\n",elapsedScalar);
}


int main() {

    // Vygenerujeme data
	constexpr size_t N = 1 << 27;
	constexpr float NEG = 0.5;
	std::vector<float> data(N);
	for(unsigned int i = 0 ; i < N ; i++) {
        data[i] = -NEG + 3 * rand() / (float)RAND_MAX;
    }

    // Vytvorime kopie dat
    auto data1 = data;
    auto data2 = data;

    // Rozdelime data na dve casti tak, ze splnuji data[i] <= data[ i + data.size()/2 ]
    // Vypocet provedeme skalarne
    condswap_scalar(&data1[0], N);

    // Vypocet provedeme s vektory
    condswap_vec(&data2[0], N);

    // Zkontrolujeme, zda vypocet probehl spravne
    unsigned int numMistakes = 0;
    for(unsigned int i = 0 ; i < N ; i++) {
        double cdiff = (double)data1[i] - (double)data2[i];
        if(cdiff > 0.001) numMistakes++;
    }
    printf("Pocet chyb ve vypoctu:          %d\n", numMistakes);

	return 0;
}