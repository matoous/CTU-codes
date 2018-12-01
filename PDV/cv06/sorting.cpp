#include <iostream>
#include <memory>
#include <algorithm>
#include <chrono>
#include <string>

#include "generators.h"

#include "_mergesort/mergesort.h"
#include "_countingsort/countingsort.h"

using elem_t = unsigned int;            // Typ prvku, ktere budeme radit
constexpr size_t N_ELEMS = 100000000;   // Pocet prvku k serazeni
constexpr elem_t MIN_VALUE = 0;         // Minimalni prvek v neserazenem poli
constexpr elem_t MAX_VALUE = 8191;      // Maximalni prvek v neserazenem poli

// Pomocny datovy typ uchovavajici vysledek (doufame, ze serazene pole) a cas,
// ktery byl nezbytny k serazeni.
using solution_pair_t = std::pair<std::vector<elem_t>, unsigned long>;
using solution_t = std::unique_ptr<solution_pair_t>;

solution_t eval(std::string name, std::vector<elem_t> & unsorted,
                void (*sorter)(std::vector<elem_t> &), solution_pair_t * reference);

int main() {
    // Nejprve si nagenerujeme pole velikosti N_ELEMS a ulozime do nej nahodne
    // prvky z rozsahu [MIN_VALUE .. MAX_VALUE].
    std::vector<elem_t> unsorted(N_ELEMS);
    generate_data_numeric(unsorted, MIN_VALUE, MAX_VALUE);

    // Nyni muzeme testovat Vase implementace. Nejprve si napocteme referencni
    // reseni pomoci knihovni funkce std::sort.
    solution_t reference = eval("std::sort(...)           ", unsorted,
                                [](std::vector<elem_t> & to_sort) {
                                    std::sort(to_sort.begin(), to_sort.end());
                                }, nullptr);

    eval("mergesort_parallel(...)  ", unsorted, &mergesort_parallel, reference.get());
    eval("mergesort_sequential(...)", unsorted, &mergesort_sequential, reference.get());
    eval("counting_parallel(...)   ", unsorted, &counting_parallel, reference.get());
    eval("counting_sequential(...) ", unsorted, &counting_sequential, reference.get());

    return 0;
}





// Evaluacni funkce:
solution_t eval(std::string name, std::vector<elem_t> & unsorted,
                void (*sorter)(std::vector<elem_t> &), solution_pair_t * reference) {

    using namespace std::chrono;

    std::vector<elem_t> copy = unsorted;
    unsigned long elapsed = 0ULL;

    try {
        auto begin = steady_clock::now();
        sorter(copy);
        auto end = steady_clock::now();

        elapsed = duration_cast<milliseconds>(end - begin).count();

        double speedup = 1.0;
        if (reference != nullptr && reference->first != copy) {
            printf("%s    --- wrong solution ---\n", name.c_str());
        } else {
            if(reference != nullptr) speedup = reference->second / (double) elapsed;
            printf("%s   %ld ms (speedup %6.4fx)\n", name.c_str(), elapsed, speedup);
        }
    } catch(...) {
        printf("%s   --- not implemented ---\n", name.c_str());
    }

    return std::unique_ptr<solution_pair_t>(new std::pair<std::vector<elem_t>,unsigned long>(
            std::move(copy), elapsed
    ));
}
