#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>

#include "_generator/generator.h"
#include "sort.h"
#include "_tests/test.h"

// nase prvky, ktere budeme radit, jsou retezce
using element_t = std::string;
// bude jich celkem tolik
const int count_of_elements = 2500000;
// vsechny stringy maji prave tuto delku. nemusite osetrovat zadne specialni pripady, ze by retezce byli jinak dlouhe.
// toto vam usetri osetrovani specialnich pripadu
const int max_length = 5;
// znaky retezcu jsou tvoreny touto abecedou
namespace Alphabet {

    // pouzivame nekolika prvnich velkych pismen z anglicke abecedy
    const std::string characters = "ABCDEFGHIJ";
    // velikost nasi abecedy
    const unsigned long alphabet_size = characters.size();
    // zjisteni offsetu pro nasi abecedu z ASCII - podle prvniho znaku. Napr. A zacina v ASCII na pozici 65.
    // chceme, aby prvni znak nasi abecedy byl mapovan na 0
    const unsigned long offset = characters.at(0);

    // rychla mapovaci funkce, ktera vam pro znak v abecede vrati jeho poradi
    const unsigned long get_bucket(const char &character_of_alphabet) {

        // poradi nastavujeme od 0. odecitame od cisla offset
        return character_of_alphabet - offset;
    }
}

// metoda ktera generuje retezce podle parametru vyse. retezce maji uniformni delku a jsou tvoreny znaky z abecedy
template<typename element_t>
std::vector<element_t> sample_elements_from_distribution(const unsigned long count_of_elements) {

    // distribuce moznych zacatku abecedy
    std::uniform_int_distribution<unsigned long> alphabet_start_dis(0, Alphabet::alphabet_size - 1);
    std::vector<element_t> strings(count_of_elements);
    // nageneruj data
    for (int i = 0; i < count_of_elements; ++i) {
        std::string str;
        // retezce maji uniformni delku
        for (int j = 0; j < max_length; ++j) {
            // pridame na pozici v retezci nahodny znak z abecedy
            str.push_back(Alphabet::characters.at(alphabet_start_dis(rng)));
        }
        strings[i] = str;
    }
    return strings;
}

// instance radiciho algoritmu - instance vasho radiciho algoritmu
// volani implementace vaseho radiciho algoritmu. vsimnete si, ze promena je funkce, kterou inicializujeme lambdou.
// lambda ma jako vstup vektor odkazu na retezce, ktere maji byt serazeny. do vaseho radiciho algoritmu je vlozen tento
// vektor spolecne s mapovaci funkci, ktera vam pro znak z abecedy vrati jeho poradi. toto se hodi pro urceni spravneho
// bucketu, kam by mel tento znak patrit. vstupem pro vas algoritmus je take delka retezcu. vsechny retezce jsou stejne
// dlouhe
template<typename element_t>
SortingAlgorithm<element_t> radix_sort = [](std::vector<element_t *> &vector_to_sort) {
    // metoda, kterou budete implementovat. viz soubor "sort.h"
    radix_par(vector_to_sort, Alphabet::get_bucket, Alphabet::alphabet_size, max_length);
};

// instance radiciho algoritmu
// pouziti radiciho algoritmu ze standardni knihovny
template<typename element_t>
SortingAlgorithm<element_t> std_sort = [](std::vector<element_t *> &vector_to_sort) {
    // pracujeme s pointers. aby byly retezce spravne setridene je treba vlozit lambda funkcy, ktera porovnava retezce
    // na adresach, kam pointery ukazuji. bez lambda funkce bychom porovnavali adresy
    std::sort(vector_to_sort.begin(), vector_to_sort.end(), [&](element_t *first, element_t *second) {
        return (*first) < (*second);
    });
};

// evaluacni skript. v prvnim kroku preda data vasemu algoritmu. algoritmus spusti a zmeri cas.
// pokud vas algoritmus retezce spravne seradil, vypise cas. v opacnem pripade evaluace vypise chybovou hlasku
template<typename element_t>
void
eval(const std::string &test_name, SortingAlgorithm<element_t> sorting_algorithm,
     std::vector<element_t> &data_to_sort) {
    using namespace std::chrono;

    // Nejprve si vytvorime instanci testu
    SortingTest<element_t> sortingTest{data_to_sort, sorting_algorithm};

    try {
        // Cas zacatku behu testu
        auto begin = steady_clock::now();
        // Beh testu
        sortingTest.run_sort();
        // Konec behu testu
        auto end = steady_clock::now();

        // Kontrola spravnosti vysledku
        if (!sortingTest.verify(data_to_sort)) {
            printf("%s       --- wrong result ---\n", test_name.c_str());
        } else {
            printf("%s          %7ldms\n", test_name.c_str(), duration_cast<milliseconds>(end - begin).count());
        }
    } catch (...) {
        printf("%s      --- not implemented ---\n", test_name.c_str());
    }
}

// hlavni metoda, ve ktere se vygeneruji data. data jsou serazena vasim algoritmem, nasledne je zkontrolovana
// spravnost razeni
int main() {

    // pripraveni data setu. data set obsahuje count_of_elements retezcu
    // retezce jsou vsechny uniformne dlouhe. delka je velice male cislo. retezce se skladaji jen z nekolika
    // znaku abecedy
    // podrobnejsi informace viz "metoda sample_elements_from_distribution"
    std::vector<element_t> data_to_sort = generate_data(count_of_elements,
                                                        sample_elements_from_distribution<element_t>);

    // spusti evaluaci vaseho radiciho algoritmu s vygenerovanymi daty. informace se zadanim viz "sort.h"
    eval<element_t>("student's radix sort", radix_sort<element_t>, data_to_sort);

    // razeni za pouziti std::sort. na vygenerovane datove sade by mel byt znatelne pomalejsi nez vase reseni. radek
    // pro urychleni testovani muzete zakomentovat
    eval<element_t>("std::sort", std_sort<element_t>, data_to_sort);

    return 0;
}