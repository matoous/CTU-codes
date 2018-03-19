#include <cstdio>
#include <chrono>

#include "tests.h"

constexpr unsigned int N1 = 1000000;      // Kolik prvku budeme do BVS vkladat "napreskacku"
constexpr unsigned int N2 = 40000;         // Kolik prvku budeme do BVS vkladat "poporade"

template <typename Test>
void run_test(std::string test_name) {
    using namespace std::chrono;

    Test test;

    try {
        auto begin = steady_clock::now();
        test.run_test();
        auto end = steady_clock::now();

        if(!test.verify()) {
            printf("%s       --- wrong result ---\n", test_name.c_str());
        } else {
            printf("%s          %7ldms\n", test_name.c_str(), duration_cast<milliseconds>(end-begin).count());
        }
    } catch(...) {
        printf("%s      --- not implemented ---\n", test_name.c_str());
    }
}

int main() {
    run_test<sorted_data<N2>> ("Sorted data  ");
    run_test<shuffled_data<N1>> ("Shuffled data");

    return 0;
}