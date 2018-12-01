#include <iostream>
#include <vector>
#include <functional>
#include <chrono>
#include <cmath>
#include <omp.h>
#include "decompose.h"

using namespace std::chrono;

constexpr unsigned long MIN_ELEMENT = 10000000;
constexpr unsigned long MAX_ELEMENT = 100000000;
constexpr unsigned long CRITERIA = 650;
constexpr unsigned int FIB_QUERY = 42;

typedef unsigned long (*solver_t)(const std::vector<unsigned long> &);
typedef unsigned long (*solver_e)(unsigned long);
typedef unsigned long long (*solver_fib)(unsigned int);

int main() {
    if(!omp_get_cancellation()) {
        std::cout << "-----------------------------------------------------------------------------" << std::endl;
        std::cout << "| WARNING: OpenMP cancellations are not enabled                             |" << std::endl;
        std::cout << "| You can enable them by setting environment variable OMP_CANCELLATION=true |" << std::endl;
        std::cout << "-----------------------------------------------------------------------------" << std::endl << std::endl;
    }

    std::vector<solver_t> solvers;
    std::vector<solver_e> solvers_exploratory;
    std::vector<solver_fib> solvers_fibonacci;
    std::vector<std::string> solver_names;

    solvers.push_back(&findmin_sequential); solver_names.push_back(     "findmin_sequential(...)     ");
    solvers.push_back(&findmin_parallel); solver_names.push_back(       "findmin_parallel(...)       ");

    std::vector<unsigned long> data;
    for(auto i = MIN_ELEMENT; i < MAX_ELEMENT; i++)
        data.push_back(i);


    {
        // Zapamatujeme si cas sekvencni verze (prvni solver)
        double sequential_time = -1;

        printf("Computing minimum of the Collatz function on the data:\n");
        for(unsigned int i = 0 ; i < solvers.size() ; i++) {
            // Cas zacatku behu metody
            try {
                auto begin = steady_clock::now();
                unsigned long min = solvers[i](data);
                // Cas konce behu metody
                auto end = steady_clock::now();

                long elapsed = duration_cast<microseconds>(end - begin).count();
                if(sequential_time < 0) {
                    sequential_time = elapsed;
                }
                double speedup = sequential_time / (double)elapsed;


                printf("%s %9ldus (speedup %6.3fx)           result %ld\n", solver_names[i].c_str(), elapsed, speedup, min);

            } catch(...) {
                printf("%s               ----- not implemented yet -----\n", solver_names[i].c_str());
            }
        }

        printf("\n\n");
    }

    solvers.clear(); solver_names.clear();
    solvers_exploratory.push_back(&findn_sequential); solver_names.push_back(     "findn_sequential(...)     ");
    solvers_exploratory.push_back(&findn_parallel);   solver_names.push_back(     "findn_parallel(...)       ");

    {

        // Zapamatujeme si cas sekvencni verze (prvni solver)
        double sequential_time = -1;

        printf("Finding parameter of the Collatz function with number of steps greater than %ld :\n", CRITERIA);
        for(unsigned int i = 0 ; i < solvers_exploratory.size() ; i++) {
            // Cas zacatku behu metody
            try {
                auto begin = steady_clock::now();
                long result = solvers_exploratory[i](CRITERIA);
                // Cas konce behu metody
                auto end = steady_clock::now();

                long elapsed = duration_cast<microseconds>(end - begin).count();
                if(sequential_time < 0) {
                    sequential_time = elapsed;
                }
                double speedup = sequential_time / (double)elapsed;

                printf("%s %9ldus (speedup %6.3fx)           result: Collatz( %ld ) =\t %ld\n", solver_names[i].c_str(), elapsed, speedup, result, collatz(result));

            } catch(...) {
                printf("%s               ----- not implemented yet -----\n", solver_names[i].c_str());
            }
        }

        printf("\n\n");
    }

    solvers.clear(); solver_names.clear();
    solvers_fibonacci.push_back(&fibonacci_sequential); solver_names.push_back(     "fibonacci_sequential(...)     ");
    solvers_fibonacci.push_back(&fibonacci_parallel);   solver_names.push_back(     "fibonacci_parallel(...)       ");

    {

        // Zapamatujeme si cas sekvencni verze (prvni solver)
        double sequential_time = -1;

        printf("Computing Fibonacci number F(%d) :\n", FIB_QUERY);
        for(unsigned int i = 0 ; i < solvers_fibonacci.size() ; i++) {
            // Cas zacatku behu metody
            try {
                auto begin = steady_clock::now();
                long result = solvers_fibonacci[i](FIB_QUERY);
                // Cas konce behu metody
                auto end = steady_clock::now();

                long elapsed = duration_cast<microseconds>(end - begin).count();
                if(sequential_time < 0) {
                    sequential_time = elapsed;
                }
                double speedup = sequential_time / (double)elapsed;

                printf("%s %9ldus (speedup %6.3fx)           result: F( %d ) =\t %ld\n", solver_names[i].c_str(), elapsed, speedup, FIB_QUERY, result);

            } catch(...) {
                printf("%s               ----- not implemented yet -----\n", solver_names[i].c_str());
            }
        }

        printf("\n\n");
    }

    return 0;
}