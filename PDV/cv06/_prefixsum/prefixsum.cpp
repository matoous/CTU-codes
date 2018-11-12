#include "prefixsum.h"

#include <string>
#include <vector>
#include <chrono>

using elem_t = unsigned int;
constexpr unsigned int N = 1<<28;

unsigned long eval(std::string name, std::vector<elem_t> & counts,
                   void (*ps)(elem_t *, const size_t), unsigned long ref_time);

int main() {
    // Nagenerujeme si nahodna data z intervalu [0 .. 3]. Pro ty budeme pocitat
    // prefix-sum.
    std::vector<elem_t> data(N);
    for(unsigned int i = 0 ; i < N ; i++) {
        data[i] = rand() % 4;
    }

    
    long ref_time = eval("prefix_sum_sequential(...)", data,
                         &prefix_sum_sequential, 0);
    eval("prefix_sum_parallel(...)", data, &prefix_sum_parallel, ref_time);

    return 0;
}



// Evaluacni funkce:
unsigned long eval(std::string name, std::vector<elem_t> & counts,
                void (*ps)(elem_t *, const size_t), unsigned long ref_time) {

    using namespace std::chrono;

    std::vector<elem_t> copy = counts;
    unsigned long elapsed = 0ULL;

    try {
        size_t size = counts.size();
        auto begin = steady_clock::now();
        ps(&copy[0], size);
        auto end = steady_clock::now();

        elapsed = duration_cast<milliseconds>(end - begin).count();

        double speedup = 1.0;
        if (ref_time > 0) {
            speedup = ref_time / (double) elapsed;
        }

        elem_t acc = 0;
        for(unsigned int i = 0 ; i < size ; i++) {
            acc += counts[i];
            if(copy[i] != acc) {
                printf("%s    --- wrong solution ---\n", name.c_str());
                return elapsed;
            }
        }

        printf("%s   %ld ms (speedup %6.4fx)\n", name.c_str(), elapsed, speedup);
    } catch(...) {
        printf("%s   --- not implemented ---\n", name.c_str());
    }

    return elapsed;
}
