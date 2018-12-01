#ifndef SORTING_GENERATORS_H
#define SORTING_GENERATORS_H

#include <vector>
#include <cstdlib>
#include <limits>

// Metoda 'generate_data_numeric' nageneruje data do vektoru 'target' z rozsahu
// [min_value .. max_value].
template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
generate_data_numeric(std::vector<T> & target, T min_value, T max_value) {
    const T range = max_value - min_value + 1;
    const size_t size = target.size();
    for(int i = 0 ; i < size ; i++) {
        target[i] = min_value + (rand() % range);
    }
}

void generate_data_strings(std::vector<std::string> & target) {
}

#endif //SORTING_GENERATORS_H
