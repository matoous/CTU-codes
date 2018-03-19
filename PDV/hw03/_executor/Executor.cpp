
#include <omp.h>
#include "Executor.h"
#include <iostream>
#include <climits>
#include <omp.h>

// Komentare najdete v header souboru

// Sekvencni implementace pocitani jednotlivych sum
void sumsOfVectors_sequential(const vector<vector<int8_t>> &data, vector<long> &solution,
                              unsigned long minVectorSize) {
    std::cout << "reference";
    for (unsigned long i = 0; i < data.size(); i++) {
        long sum = 0;
        for (const int8_t &j : data[i]) {
                sum += j;
        }
        solution[i] = sum;
    }
    std::cout << " - done\n";
}

long Executor::executeMethod(
        void (*functionPtr)(const vector<vector<int8_t>> &, vector<long> &, const unsigned long minVectorSize),
        const vector<long> &solution, const vector<vector<int8_t>> &data, unsigned long minVectorSize) const {
    try {
        vector<long> result(data.size());

        // Cas zacatku behu metody
        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        // Beh metody
        (*functionPtr)(data, result, minVectorSize);
        // Cas konce behu metody
        chrono::steady_clock::time_point end = chrono::steady_clock::now();

        // Kontrola spravnosti
        if (result == solution) {
            // V pripade spravneho vysledku vraci cas behu
            return chrono::duration_cast<chrono::nanoseconds>(end - begin).count();
        } else {
            std::cout << "wrong solution\n";
        }
    } catch (...) {
        //ignored
    }

    // V pripade spatneho vysledku vraci -1L
    return -1L;
}

results_t Executor::executeMethods(const vector<long> &solution, const vector<vector<int8_t>> &data) const {
    unsigned long shortestVectorLength = findLengthOfShortestVector(data);
    long referenceTime = executeMethod(&sumsOfVectors_sequential, solution, data, shortestVectorLength);
    long perVectorTime = executeMethod(&sumsOfVectors_omp_per_vector, solution, data, shortestVectorLength);
    long withShuffleTime = executeMethod(&sumsOfVectors_omp_shuffle, solution, data, shortestVectorLength);
    long dynamicSchedTime = executeMethod(&sumsOfVectors_omp_dynamic, solution, data, shortestVectorLength);
    long staticSchedulingTime = executeMethod(&sumsOfVectors_omp_static, solution, data,
                                              shortestVectorLength);
    return {referenceTime, perVectorTime, withShuffleTime, dynamicSchedTime, staticSchedulingTime};
}

unsigned long Executor::findLengthOfShortestVector(const vector<vector<int8_t>> &data) const {
    unsigned long minNumber = ULONG_MAX;
    const unsigned long size = data.size();

    // Redukce pomaha najit nejkratsi vektor
    // V pripade ze je datova sada mensi nez (pocet vlaken x 1000), vykonavame hledani sekvencne
#pragma omp parallel for reduction(min: minNumber) if(size > omp_get_max_threads() * 1000)
    for (unsigned long i = 0; i < size; i++) {
        if (data[i].size() < minNumber)
            minNumber =  data[i].size();
    }
    return minNumber;
}