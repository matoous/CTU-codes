#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>
#include "sparse.h"
#include "multiply.h"

constexpr double NZ_ROW_PROB = 0.75;
constexpr double NZ_PROB = 0.05;

constexpr unsigned int NUM_ROWS = 500000;
constexpr unsigned int NUM_COLS = 1000;

std::mt19937_64 gen(0);
std::uniform_real_distribution<double> prob_dist(0.0, 1.0);
std::uniform_int_distribution<unsigned int> ind_dist(0, NUM_COLS-1);
std::uniform_real_distribution<double> value_dist(0.0, 5.0);

void fill(matrix_row & row);

int main() {
    using namespace std::chrono;

    sparse_matrix A;
    sparse_vector x;

    // Nejprve vygenerujeme obsah matice A a vektoru x nahodne:
    for(unsigned int i = 0 ; i < NUM_ROWS ; i++) {
        if(prob_dist(gen) <= NZ_ROW_PROB) {
            matrix_row row(i);
            fill(row);

            A.push_back(std::move(row));
        }
    }
    fill(x);

    // Zbytek uz je jenom mereni a porovnavani spravnosti reseni, ktere uz dobre znate:
    auto seq_begin = steady_clock::now();
    sparse_vector reference = multiply_sequential(A, x);
    auto seq_end = steady_clock::now();
    std::cout << "Sequential computation of A*x          " << duration_cast<microseconds>(seq_end-seq_begin).count()
              << "us" << std::endl;

    try {
        auto par_begin = steady_clock::now();
        sparse_vector computed = multiply_parallel(A, x);
        auto par = steady_clock::now();
        std::cout << "Parallel computation of A*x            " << duration_cast<microseconds>(par-par_begin).count()
                  << "us";

        std::cout << " ; speedup="
                  << duration_cast<microseconds>(seq_end-seq_begin).count()/(double)duration_cast<microseconds>(par-par_begin).count();

        std::cout << "  ;  result is ";
        if(computed == reference) std::cout << "correct" << std::endl;
        else std::cout << "incorrect" << std::endl;
    } catch(std::string s) {
        std::cout << "Parallel computation of A*x            " << s << std::endl;
    }

    return 0;
}

void fill(matrix_row & row) {
    constexpr unsigned int NZ_COUNT = (int)std::ceil(NZ_PROB * NUM_COLS);

    // Nejprve nagenerujeme indexy nenulovych prvku
    std::vector<unsigned int> indices(NZ_COUNT);
    for(unsigned int i = 0 ; i < NZ_COUNT ; i++) {
        indices[i] = ind_dist(gen);
    }

    // Jelikoz musime vkladat poporade, indexy si seradime a vyhazime duplicity
    std::sort(indices.begin(), indices.end());
    indices.erase(std::unique(indices.begin(), indices.end()), indices.end());

    // Pro nenulove indexy nagenerujeme prvky do vektoru
    for(auto idx : indices) {
        row.set(idx, value_dist(gen));
    }
}