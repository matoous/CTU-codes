#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

// seed pro nahodny generator. pouzivame konstantu - kvuli replikovatelnosti vysledku
const unsigned int seed = 15;

// generator nahodnych cisel (Mersenne-Twister)
std::mt19937 rng(seed);

// predpis metody, ktera generuje retezce
template<typename element_t>
using GenerateElements = std::vector<element_t> (*)(const unsigned long count_of_elements);

// metoda k vygenerovani testovacich dat
template<typename element_t>
std::vector<element_t>
generate_data(const unsigned long count_of_elements, GenerateElements<element_t> generator) {
    // nejdriv s pouzitim generatoru vygenerujeme data
    std::vector<element_t> generated_data = generator(count_of_elements);
    // data jeste zamichame
    std::shuffle(generated_data.begin(), generated_data.end(), rng);
    return generated_data;
}

#endif //CODE_GENERATOR_H
