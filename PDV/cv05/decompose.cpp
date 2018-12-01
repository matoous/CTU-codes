#include "decompose.h"

#include <cmath>
#include <atomic>
#include <iostream>
#include <limits>
#include <map>

// Collatzuv problem (znamy take jako uloha 3n+1) je definovany jako
// posloupnost cisel generovana podle nasledujicich pravidel
//   1) Pokud je n liche, dalsi prvek posloupnosti je 3n+1
//   2) Pokud je n sude, dalsi prvek posloupnosti je n/2
// Plati, ze pro libovolne prirozene cislo n, posloupnost cisel
// "dosahne" cisla 1. Na dnesnim cviceni nas bude zajimat, po kolika
// krocich (tj. kolik operaci (1) a (2)) je pro to potreba. To muzeme
// zjistit pomoci nasledujici jednoduche funkce:

unsigned long collatz(unsigned long k) {
    long steps = 0;
    while (k > 1) {
        steps++;
        if (k % 2) k = 3 * k + 1;
        else k /= 2;
    }
    return steps;
}



// V prvni sade uloh 'findmin_*' hledame cislo 'n' z vektoru 'data', pro
// ktere Collatzova sekvence dosahne cisla 1 nejrychleji. Takova uloha
// se da dobre paralelizovat, protoze muzeme hodnotu funkce 'collatz(n)'
// spocitat pro kazdy prvek pole nezavisle. V nasem pripade hledame dane
// cislo v intervalu [MIN_ELEMENT .. MAX_ELEMENT]. Tyto hodnoty si muzete
// upravit v souboru 'main.cpp'.
//
// Mnoho optimalizacnich problemu jde resit efektivneji. Pokud zjistime,
// ze dany prvek garantovane neni optimalni, nemusime se snazit spocitat
// jeho presnou hodnotu (v tomto pripade pocet potrebnych kroku). Mohli
// bychom takoveto odrezavani pouzit i v tomto pripade?
unsigned long findmin_sequential(const std::vector<unsigned long> &data) {
    unsigned long min = std::numeric_limits<unsigned long>::max();

    for (int i : data){
        int idx = i;
        unsigned long steps = 0;
        while (idx > 1) {
            steps++;
            if (steps > min) break;
            if (idx % 2) idx = 3 * idx + 1;
            else idx /= 2;
        }
        if (steps < min) {
            min = steps;
        }
    }

    return min;

}

unsigned long findmin_parallel(const std::vector<unsigned long> &data) {
    std::atomic<unsigned long> min { std::numeric_limits<unsigned long>::max() };
    unsigned int size = data.size();

#pragma omp parallel for
    for (unsigned int j = 0; j < size; j++){
        // load data
        auto x = data[j];
        auto curr_min = min.load();

        // collatz
        unsigned long steps = 0;
        while (x > 1 && steps <= curr_min) {
            steps++;
            if (x % 2) x = 3 * x + 1;
            else x /= 2;
        }

        // set new minimum
        while ( steps < curr_min ){
            if (min.compare_exchange_strong(curr_min, steps)) {
                break;
            }
        }
    }

    return min;
}

// V druhe sade uloh 'findn_*' se zamerime na opacny problem. Dostaneme
// delku sekvence 'criteria' a nasim ukolem bude nalezt cislo 'n', pro
// ktere je hodnota collatz(n) >= criteria. Hodnotu parametru 'criteria'
// si muzete upravit v souboru main.cpp.
//
// Vsimnete si, ze v tomto pripade nevime predem kolik prvku budeme muset
// zpracovat, nez narazime na potrebne 'n'. Nevime proto predem, jak mame
// data rozdelit mezi jednotliva vlakna.

unsigned long findn_sequential(unsigned long criteria) {
    for (unsigned long i = 1 ; ; i++){
        unsigned long value = collatz(i);
        if (value > criteria) return i;
    }
}

unsigned long findn_parallel(unsigned long criteria) {
    std::atomic<unsigned long> i = { 1 };
    unsigned long res;
    std::atomic<bool> end = {false};
#pragma omp parallel
    while(!end.load()) {
        unsigned long current_i = i++;
        auto collatz_value = collatz(current_i);
        if(collatz_value >= criteria) {
            res = current_i;
            end.store(true);
        }
    }
    return res;
}

// V posledni sade uloh se zamerime na vypocet Fibonacciho cisla pomoci
// rekurze a budete si moci vyzkouset #pragma omp task pro rekurzivni
// paralelizaci. V testech pocitame cislo FIB_QUERY, ktere si muzete
// upravit v souboru main.cpp.

unsigned long long fibonacci_sequential(unsigned int n) {
    if(n <= 2) return 1;
    else return fibonacci_sequential(n-1) + fibonacci_sequential(n-2);
}

unsigned long long fibonacci_parallel_worker(unsigned int n);
unsigned long long fibonacci_parallel(unsigned int n) {
    throw "Not implemented yet";

    unsigned long long result = 0;

    #pragma omp parallel
    #pragma omp single
    {
        result = fibonacci_parallel_worker(n);
    };

    return result;
}

unsigned long long fibonacci_parallel_worker(unsigned int n) {

}