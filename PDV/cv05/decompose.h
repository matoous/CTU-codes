#include <functional>
#include <vector>

#ifndef CODE_DECOMPOSE_H
#define CODE_DECOMPOSE_H

unsigned long findmin_sequential(const std::vector<unsigned long> &data);
unsigned long findmin_parallel(const std::vector<unsigned long> &data);

unsigned long findn_sequential(unsigned long criteria);
unsigned long findn_parallel(unsigned long criteria);

unsigned long long fibonacci_sequential(unsigned int n);
unsigned long long fibonacci_parallel(unsigned int n);

unsigned long collatz(unsigned long k);

#endif //CODE_DECOMPOSE_H
