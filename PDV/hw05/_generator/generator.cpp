
#include <random>
#include <algorithm>
#include <thread>
#include <chrono>
#include "generator.h"
#include "../params.h"

// generator nahodnych cisel (Mersenne-Twister)
std::mt19937 rng(SEED);

// Funkce simulujici kontrolu predikatu
// V realnych podminkach neni kontrola platnosti predikatu okamzita, napriklad je potreba provest operaci
// s daty ulozenymi na disku.
double predicate_checking_simulation_function() {
    using namespace std::chrono;

    auto begin = steady_clock::now();
    while(true) {
        auto now = steady_clock::now();
        if(duration_cast<microseconds>(now-begin).count() >= 2L) break; // Pockame alespon 1us na data
    }
}

// naplni vektor boolovskych hodnot truth/false hodnotami a zajisti, ze aspon jedna hodnota bude true/false
// ppt_of_true - pravdepodobnost, ze libovolny prvek vektoru bude true
// to_fill - vektor, ktery se ma naplnit
// bool_value_to_ensure - zajisti ze ve vektoru bude aspon jedno true/false
void
fill_vector_by_bools_by_ppt(const double ppt_of_true, std::vector<bool> &to_fill, const bool bool_value_to_ensure) {

    // nase distribuce cisel
    std::uniform_real_distribution<> dis(0.0, 1.0);

    // chceme zarucit, ze se vyskytne aspon jednou bool_value_to_ensure v datech. tato prom. slouzi ke kontrole, zda se
    // uz tak stalo
    bool is_at_least_one_predicate_satisfied = false;

    // nagenerujeme, ktere prvky jsou pravdive
    long size = to_fill.size();
    for (int i = 0; i < size - 1; ++i) {

        // bude tento prvek nastaveny na true
        bool is_satisfied = dis(rng) < ppt_of_true;

        // kontrola, zda jsme jiz vygnerovali aspon jednu bool hodnotu, kterou ma vektor aspon jednou obsahovat
        if (!is_at_least_one_predicate_satisfied && is_satisfied == bool_value_to_ensure) {
            is_at_least_one_predicate_satisfied = true;
        }
        to_fill[i] = is_satisfied;
    }

    // vygenerovali jsme aspon jeden prvek, ktery odpovida bool_value_to_ensure
    if (!is_at_least_one_predicate_satisfied) {
        to_fill[size - 1] = bool_value_to_ensure;
    } else {
        to_fill[size - 1] = dis(rng) < ppt_of_true;
    }
}

// generuje data pro disjunkci/konjunkci. evaluace dotazu je true v pripade disjunkce a false u konjunkce
// zbyle parametry odpovidaji parametrum metody generate_instance_with_query v hlavickovem souboru
std::pair<std::vector<test_row_t>, std::vector<predicate_t<test_row_t>>>
generate_instance_with_query(const int length_of_query, const int count_of_rows, const bool bool_value_to_ensure,
                             const double t_probability_predicate) {
    if (length_of_query > count_of_rows) {
        throw std::invalid_argument("there should be more rows than queries");
    }

    // ktere query maji byt splnene
    std::vector<bool> is_predicate_true(length_of_query);
    fill_vector_by_bools_by_ppt(t_probability_predicate, is_predicate_true, bool_value_to_ensure);

    // vytvorime data v tabulce. v tabulce jsou hodnoty od 0 az (count_of_rows - 1). tyto hodnoty nejsou setridene
    std::vector<int> data(count_of_rows);
    std::iota(std::begin(data), std::end(data), 0);
    std::random_shuffle(data.begin(), data.end());

    // nahodne urcime, ktere hodnoty se maji brat pro predikat - na jakem radku tabulky bude predikat splnen,
    // pokud bude splnen
    std::vector<int> indexes(count_of_rows);
    std::iota(std::begin(indexes), std::end(indexes), 0);
    std::random_shuffle(indexes.begin(), indexes.end());

    // vytvorime predikaty pro nasi tabulku
    std::vector<predicate_t<test_row_t>> predicates(length_of_query);
    for (int i = 0; i < length_of_query; ++i) {

        // pokud ma byt predikat splnen, vybereme hodnotu v tabulce, kde ma byt splneny. V opacnem pripade bude hodnota
        // -1. Jelikoz hodnota -1 v tabulce neni, neexistuje radek s hodnotou, kde bude predikat splnen
        const int value_t = is_predicate_true[i] ? data[indexes[i]] : -1;

        // nas predikat ma podobu lambda funkce
        predicates[i] = [value_t](const test_row_t &value) {
            predicate_checking_simulation_function();

            // pouze pokud je hodnota rovna ocekavane hodnote, je predikat splnen
            return value == value_t;
        };
    }
    return std::make_pair(data, predicates);
}

// generuje data pro konjunkci, kdy evaluace dotazu je true
// zbyle parametry odpovidaji parametrum metody generate_instance_with_query v hlavickovem souboru
std::pair<std::vector<test_row_t>, std::vector<predicate_t<test_row_t>>>
generate_instance_with_query_conjunction_t(const int length_of_query, const int count_of_rows) {

    if (length_of_query > count_of_rows) {
        throw std::invalid_argument("there should be more rows than queries");
    }

    // vytvorime data v tabulce. v tabulce jsou hodnoty od 0 az (count_of_rows - 1). tyto hodnoty nejsou setridene
    std::vector<int> data(count_of_rows);
    std::iota(std::begin(data), std::end(data), 0);
    std::random_shuffle(data.begin(), data.end());

    // vytvorime predikaty pro nasi tabulku
    std::vector<predicate_t<test_row_t>> predicates(length_of_query);
    for (int i = 0; i < length_of_query; ++i) {

        // pro ktere hodnoty je predikat platny.
        std::uniform_int_distribution<> dis_index(0, (count_of_rows - 1) / 20);
        // zajima nas rozpeti hodnot - od - kam
        const int start_index_number = dis_index(rng), end_index_number = count_of_rows - dis_index(rng);
        // ale i jejich delitelnost nejakym cislem
        std::uniform_int_distribution<> dis_space(2, 5);
        const int space = dis_space(rng);

        // nas predikat ma podobu lambda funkce
        predicates[i] = [start_index_number, end_index_number, space](const test_row_t &value) {
            predicate_checking_simulation_function();

            // pokud hodnota odpovida pozadovane hodnote, vratime true. vetsina hodnot by mela byt true
            return value >= start_index_number && value <= end_index_number && (value % space == 0);
        };
    }
    return std::make_pair(data, predicates);
}

// generuje data pro dotaz s disjunkcemi - evaluace je false
// zbyle parametry odpovidaji parametrum metody generate_instance_with_query v hlavickovem souboru
std::pair<std::vector<test_row_t>, std::vector<predicate_t<test_row_t>>>
generate_instance_with_query_disjunction_f(const int length_of_query, const int count_of_rows) {

    // vytvorime data v tabulce. v tabulce jsou hodnoty od 0 az (count_of_rows - 1). tyto hodnoty nejsou setridene
    std::vector<int> data(count_of_rows);
    std::iota(std::begin(data), std::end(data), 0);
    std::random_shuffle(data.begin(), data.end());

    // vytvorime predikaty pro nasi tabulku. vsechny se budou evaluovat jako false
    std::vector<predicate_t<test_row_t>> predicates(length_of_query);
    for (int i = 0; i < length_of_query; ++i) {

        // nas predikat ma podobu lambda funkce
        predicates[i] = [](const test_row_t &value) {
            predicate_checking_simulation_function();

            // zadny predikat neni platny. -1 se v tabulce nevyskytuje
            return value == -1;
        };
    }
    return std::make_pair(data, predicates);
}

// implementace metody. komentar viz hlavickovy soubor
std::pair<std::vector<test_row_t>, std::vector<predicate_t<test_row_t>>>
generate_instance_with_query(Operation operation, bool is_query_evaluated_true, const int length_of_query,
                             const int count_of_rows) {

    // reset naseho generatoru nahodnych cisel. pouzivame konstantu - kvuli replikovatelnosti vysledku
    rng.seed(SEED);

    // na zaklade operace a ocekavane evaluace dotazu vybereme vhodnou metodu na generovani dat - implementace viz vyse
    switch (operation) {
        case conjunction:
            if (is_query_evaluated_true) {
                return generate_instance_with_query_conjunction_t(length_of_query, count_of_rows);
            } else {
                return generate_instance_with_query(length_of_query, count_of_rows, false,
                                                    t_probability_conjunction_predicate);
            }
        case disjunction:
            if (is_query_evaluated_true) {
                return generate_instance_with_query(length_of_query, count_of_rows, true,
                                                    t_probability_disjunction_predicate);
            } else {
                return generate_instance_with_query_disjunction_f(length_of_query, count_of_rows);
            }
    }
    throw std::invalid_argument("not supported operation");
}

// implementace metody. komentar viz hlavickovy soubor
std::pair<std::vector<test_row_t>, std::vector<predicate_t<test_row_t>>> generate_all(bool expected) {
    if (expected) {
        return generate_instance_with_query(conjunction, expected, length_of_query_all_true, count_of_rows_all_true);
    }
    else{
        return generate_instance_with_query(conjunction, expected, length_of_query_all_false, count_of_rows_all_false);
    }
}

// implementace metody. komentar viz hlavickovy soubor
std::pair<std::vector<test_row_t>, std::vector<predicate_t<test_row_t>>> generate_any(bool expected) {
    if (expected) {
        return generate_instance_with_query(disjunction, expected, length_of_query_any_true, count_of_rows_any_true);
    }
    else{
        return generate_instance_with_query(disjunction, expected, length_of_query_any_false, count_of_rows_any_false);
    }
}