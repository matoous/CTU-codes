#include <iostream>
#include <string>
#include <chrono>
#include <omp.h>

#include "_tests/tests.h"

template<class Test>
void eval(std::string test_name) {
    using namespace std::chrono;

    // Nejprve si vytvorime instanci testu
    Test test;

    try {
        // Cas zacatku behu testu
        auto begin = steady_clock::now();
        // Beh testu
        test.run_test();
        // Konec behu testu
        auto end = steady_clock::now();

        // Kontrola spravnosti vysledku
        if (!test.verify()) {
            printf("%s       --- wrong result ---\n", test_name.c_str());
        } else {
            printf("%s          %7ldms\n", test_name.c_str(), duration_cast<milliseconds>(end - begin).count());
        }
    } catch (...) {
        printf("%s      --- not implemented ---\n", test_name.c_str());
    }
}

int main() {
    if (!omp_get_cancellation()) {
        std::cout << "-----------------------------------------------------------------------------" << std::endl;
        std::cout << "| WARNING: OpenMP cancellations are not enabled                             |" << std::endl;
        std::cout << "| You can enable them by setting environment variable OMP_CANCELLATION=true |" << std::endl;
        std::cout << "-----------------------------------------------------------------------------" << std::endl
                  << std::endl;
    }

    // V tomto testovacim pripade testujeme rychlost vyhodnocovani dotazu typu:
    //  "Jsou vsechny dilci dotazy splnene?"
    // tj., existuje pro kazdy dilci dotaz alespon jeden radek v tabulce, pro
    // ktery predikat plati?
    eval<TestAll<true>>("true = is_satisfied_for_all(...)");

    // Analogie predchoziho dotazu, ale v tomto pripade ma dotaz formu:
    //  "Existuje alespon jeden dilci dotaz, ktery je splneny?"
    // tj., existuje alespon jeden predikat a jeden radek v tabulce takovy, ze
    // je pro tento radek predikat pravdivy?
    eval<TestAny<true>>("true = is_satisfied_for_any(...)");

    // Hodnota <true> udava, zda ma byt dotaz pravdivy nebo nikoliv. Vykon Vasi
    // implementace ale budeme testovat pouze na <true> dotazech.
    //
    // POZOR! Vase implementace ale samozrejme musi byt funkcni i pro <false>
    // dotazy! (tj., return true neni reseni ;-) To si muzete otestovat na techto
    // testech:
    std::cout << std::endl;
    eval<TestAll<false>>("false = is_satisfied_for_all(...)");
    eval<TestAny<false>>("false = is_satisfied_for_any(...)");

    // Parametry generovani dat si muzete upravit v souboru params.h

    return 0;
}