#ifndef HW_VECTORFINDER_H
#define HW_VECTORFINDER_H

#include <vector>

using namespace std;

void
sumsOfVectors_omp_per_vector(const vector<vector<int8_t>> &data, vector<long> &solution, unsigned long minVectorSize);

void
sumsOfVectors_omp_static(const vector<vector<int8_t>> &data, vector<long> &solution,
                         unsigned long minVectorSize);

void
sumsOfVectors_omp_shuffle(const vector<vector<int8_t>> &data, vector<long> &solution,
                          unsigned long minVectorSize);

void
sumsOfVectors_omp_dynamic(const vector<vector<int8_t>> &data, vector<long> &solution,
                          unsigned long minVectorSize);

#endif //HW_VECTORFINDER_H
