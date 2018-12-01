#ifndef SORTING_PREFIXSUM_H
#define SORTING_PREFIXSUM_H

#include <cstdlib>
#include <chrono>
#include <iostream>
#include <omp.h>
#include <vector>
#include <cmath>

template <typename T>
void prefix_sum_sequential(T * data, const size_t size);

template <typename T>
void prefix_sum_parallel(T * data, const size_t size);



template <typename T>
void prefix_sum_sequential(T * data, const size_t size) {
    for(size_t i = 1 ; i < size ; i++) {
        data[i] += data[i-1];
    }
}



template <typename T>
void prefix_sum_parallel(T * data, const size_t size) {
    #pragma omp parallel
    {
        // Rozdelte vypocet prefixni sumy pole mezi jednotliva vlakna
        // Zamyslete se, jak byste se mohli "vyhnout" zavislosti sumy
        // na prefixni sume predchazejicich prvku. (Teto zavislosti se
        // nelze vyhnout - ale mozna by nam stacilo "vysbirat" vysledky
        // ostatnich vlaken a nemuseli bychom prochazet cele pole?)
        //
        // Zkuste se nad timto problemem zamyslet - my se Vam budeme
        // snazit pokladat otazky, ktere Vas doufame dovedou k reseni.
    }

    throw "Not implemented yet";
}


#endif //SORTING_PREFIXSUM_H
