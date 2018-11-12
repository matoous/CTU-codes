#ifndef SORTING_COUNTINGSORT_H
#define SORTING_COUNTINGSORT_H

#include <vector>
#include <limits>
#include <cstdlib>
#include <chrono>
#include <omp.h>
#include "../_prefixsum/prefixsum.h"

template <typename elem_t>
void counting_sequential(std::vector<elem_t> & data) {
    elem_t min = std::numeric_limits<elem_t>::max();
    elem_t max = std::numeric_limits<elem_t>::min();

    for(auto && el : data) {
        if(el < min) min = el;
        if(el > max) max = el;
    }

    const size_t range = max - min + 1;
    std::vector<unsigned int> counts(range);

    for(auto && el : data) {
        ++counts[el-min];
    }

    unsigned int idx = 0;
    for(unsigned int i = 0 ; i < range ; i++) {
        unsigned int count = counts[i];
        unsigned int number = min + i;
        for(unsigned int k = 0 ; k < count ; k++) {
            data[idx++] = number;
        }
    }
}

template <typename elem_t>
void counting_parallel(std::vector<elem_t> & data) {
    elem_t min = std::numeric_limits<elem_t>::max();
    elem_t max = std::numeric_limits<elem_t>::min();

#pragma omp parallel for shared(data)
    for(int i = 0; i < data.size(); i++) {
        const auto & el = data[i];
        if(el < min) min = el;
        if(el > max) max = el;
    }

    const unsigned int size = data.size();
    const size_t range = max - min + 1;
    std::vector<unsigned int> counts(range);
    const unsigned int threads = omp_get_max_threads();
    std::vector<std::vector<unsigned int>> tmpc(threads, std::vector<unsigned int>(range));

#pragma omp parallel
    {
        unsigned int thread = omp_get_thread_num();
        auto & my_counts = tmpc[thread];
#pragma omp for
        for(unsigned int i = 0; i < size; i++) {
            auto el = data[i];
            ++tmpc[thread][el-min];
        }
    };

#pragma omp parallel for
    for(unsigned int i = 0; i < range; i++){
        for(int u = 0; u < threads; u++){
            counts[i] += tmpc[u][i];
        }
    }

    for(unsigned int i = 1; i < range; i++){
        counts[i] += counts[i-1];
    }

#pragma omp parallel for
    for(unsigned int i = 0 ; i < range ; i++) {
        unsigned int begin = i == 0 ? 0 : counts[i-1];
        unsigned int end = counts[i];
        unsigned int number = min + i;
        for(unsigned int k = begin ; k < end ; k++) {
            data[k] = number;
        }
    }
}

#endif //SORTING_COUNTINGSORT_H
