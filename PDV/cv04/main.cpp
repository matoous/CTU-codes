//
// Created by karel on 3/11/18.
//

#include <vector>
#include <iostream>
#include <atomic>
#include <chrono>
#include <numeric>
#include <algorithm>

#include "sequential.h"
#include "lockBased.h"
#include "lockFree.h"

using namespace std::chrono;

constexpr int N = 40000;

template<typename T>
bool check(const T & list, int elems) {
    auto current = list.head;
    current = current->next; // Skip first "dummy" element
    current = current->next; // Skip second "dummy" element
    int i = 0;

    while(current != nullptr) {
        if(current->value != i) return false;
        else {
            current = current->next;
            ++i;
        }
    }

    return i == elems;
}

template <typename LLType>
class is_concurrent {
public:
    static constexpr bool value = true;
};

template<>
class is_concurrent<Sequential> {
public:
    static constexpr bool value = false;
};

template <typename LLType>
double eval(std::string name, double ref_time) {
    LLType ll;

    long elapsed = -1;

    try {
        std::vector<long long> data(N);
        std::iota(data.begin(), data.end(), 0);
        std::random_shuffle(data.begin(), data.end());

        // Cas zacatku behu metody
        auto begin = steady_clock::now();

        // Zkontrolujeme jestli uz je add naimplementovany pomoci dummy insertu
        ll.add(0);
        
        #pragma omp parallel for if(is_concurrent<LLType>::value)
        for(int i = 0 ; i < N ; i++) ll.add(data[i]);

        // Cas konce behu metody
        auto end = steady_clock::now();

        elapsed = duration_cast<microseconds>(end - begin).count();
        if(ref_time < 0) ref_time = elapsed;
        double speedup = ref_time / (double)elapsed;

        printf("%s %9ldus (speedup %6.3fx)           result %d\n", name.c_str(), elapsed, speedup, check(ll, N));
    } catch(...) {
        printf("%s               ----- not implemented yet -----\n", name.c_str());
    }

    return elapsed;
}

int main() {
    double sequential_time = eval<Sequential>("Sequential linked-list   ", -1);
                             eval<Concurrent>("Lock-based linked-list   ", sequential_time);
                             eval<Lockfree>  ("Lock-free linked-list    ", sequential_time);

    printf("\n\n");
    return 0;
}