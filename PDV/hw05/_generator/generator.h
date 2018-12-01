#ifndef DATABASEQUERY_GENERATOR_H
#define DATABASEQUERY_GENERATOR_H

#include <vector>

#include "../query.h"

// vsechny testy pouzite v teto uloze pouzivaji nasledujici generator dat, ktery generuje data (tabulku a dotaz nad ni)
// na zaklade zadanych parametru. V uloze generujeme 2 typy dotazu, ktere se jednou maji evaluovat jako pravdive a jindy
// jako nepravdive. V prvnim pripade je dotaz slozeny s predikatu, ktere jsou vsechny spojeny disjunkci. V druhem pripade
// jsou predikaty spojeny konjunkci. Aby dany dotaz byl vzhledem k dane tabulce splnen, musim byt vzdy pro konjunkci
// vsechny predikaty pravdive. U konjunkce staci, aby byl splnen nejaky.

// datovy typ radku v nasi simulovane databazi
using test_row_t = int;


// vygeneruje tabulku a dotaz v podobe konjunkce predikatu
// expected - zda ma byt dotaz a data vygenerovana tak, ze dotaz je nad danymi daty pravdivy
std::pair<std::vector<test_row_t>, std::vector<predicate_t<test_row_t>>> generate_all(bool expected);

// vygeneruje tabulku a dotaz v podobe disjunkce predikatu
// expected - zda ma byt dotaz a data vygenerovana tak, ze dotaz je nad danymi daty pravdivy
std::pair<std::vector<test_row_t>, std::vector<predicate_t<test_row_t>>> generate_any(bool expected);

// typ logicke spojky - jak jsou jednotlive predikaty spojeny
enum Operation {
    conjunction, disjunction
};

// metoda na zaklade parametru vygeneruje dotaz a tabulku, nad kterou se provadi evaluace dotazu
// operation - konjunkce/disjunkce predikatu
// is_query_evaluated_true - ma byt dotaz vzhledem k datum splnen
// length_of_query - kolik bude v dotazu predikatu
// count_of_rows - pocet radku v tabulce
std::pair<std::vector<test_row_t>, std::vector<predicate_t<test_row_t>>>
generate_instance_with_query(Operation operation, bool is_query_evaluated_true, const int length_of_query,
                             const int count_of_rows);

#endif