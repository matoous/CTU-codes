
#ifndef HW_EXECUTOR_H
#define HW_EXECUTOR_H

#include <vector>
#include <chrono>
#include "../SumsOfVectors.h"

using namespace std;

/**
 *  Datova struktura, ktera obsahuje vysledky behu ruznych implementaci
 */
typedef struct results {
    long referenceTime;             // rychlost sekvencni implementace
    long perVectorTime;             // rychlost implementace pri paralelizaci na urovni jednotlivych vektoru
    long withShuffleTime;           // rychlost implementace s "michanim" a statickym rozvrhovanim
    long dynamicSchedulingTime;     // rychlost implementace s dynamickym rozvrhovanim
    long staticSchedulingTime;      // rychlost implementace s statickym rozvrhovanim

    results(long referenceTime, long perVectorTime, long withShuffleTime, long dynamicSchedulingTime,
            long staticSchedulingTime)
            : referenceTime(referenceTime), perVectorTime(perVectorTime), withShuffleTime(withShuffleTime),
              dynamicSchedulingTime(dynamicSchedulingTime), staticSchedulingTime(staticSchedulingTime) {};

    results() : results(-1L, -1L, -1L, -1L, -1L) {}

} results_t;

class Executor {

public:

    // Spoustec jednotlivych implementaci
    // Kontroluje spravnost vysledku. Pokud je vysledek spravny, vraci dobu behu
    // Jinak vraci -1L
    long executeMethod(void (*functionPtr)(const vector<vector<int8_t>> &, vector<long> &, unsigned long minVectorSize),
                       const vector<long> &solution,
                       const vector<vector<int8_t>> &data, unsigned long minVectorSize) const;

    results_t executeMethods(const vector<long> &solution, const vector<vector<int8_t>> &data) const;

    unsigned long findLengthOfShortestVector(const vector<vector<int8_t>> &data) const;
};


#endif //HW_EXECUTOR_H
