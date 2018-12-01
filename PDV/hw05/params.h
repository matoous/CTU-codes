//
// Parametry ovlivnujici generovana data
//



// Kvuli replikovatelnosti vysledku pouzivame zafixovany seed pouzivany pri generaovani nahodne
// databaze. Budeme samozrejme radi, pokud si Vase reseni otestujete i na jinych seedech.
constexpr unsigned int SEED = 15;



// nastaveni parametru pro generovani konjunkci/disjunkci
// ocekavana pravdepodobnost, ze predikat v disjunkci/konjunkci bude pro vygenerovana data pravdivy
const double t_probability_disjunction_predicate = 0.8;
const double t_probability_conjunction_predicate = 0.99;




//
// Parametry pro generovani datasetu, pro ktere ma vyhodnocovany dotaz platit (tj. <true>)
//

// parametry data setu pro dotazy typu GENERATE ALL (konjunkce predikatu)
// pocet predikatu jako delka dotazu
const int length_of_query_all_true = 100000;
// pocet radku tabulky
const int count_of_rows_all_true = 100000;

// parametry data setu pro dotazy typu GENERATE ANY (disjunkce predikatu)
// pocet predikatu jako delka dotazu
const int length_of_query_any_true = 500;
// pocet radku tabulky
const int count_of_rows_any_true = 500000;





//
// Parametry pro generovani datasetu, pro ktere nema vyhodnocovany dotaz platit (tj. <false>)
//

// pocet predikatu jako delka dotazu
const int length_of_query_all_false = 30;
// pocet radku tabulky
const int count_of_rows_all_false = 30000;


// pocet predikatu jako delka dotazu
const int length_of_query_any_false = 300;
// pocet radku tabulky
const int count_of_rows_any_false = 2000;