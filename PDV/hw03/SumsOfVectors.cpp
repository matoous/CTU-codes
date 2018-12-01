// V teto domaci uloze se Vas budeme snazit presvedcit, ze vykon vasi implementace zavisi do znacne miry na podobe
// vstupnich dat. Pri navrhu efektivnich algoritmu byste se tedy meli rozhodovat i na zaklade datove sady.
//
// Vasim ukolem je doimplementovat 4 ruzne metody na pocitani sumy kazdeho vstupniho vektoru ("data").
// V kazde metode budete pouzitivat OpenMP, ale pokazde trochu jinym zpusobem. Rychlost vypoctu Vasi implementace
// budeme porovnavat s nasi referencni implementaci. Vychazet muzete z metody sumsOfVectors_sequential implementovane
// v _executor/Executor.cpp.

#include <iostream>
#include <numeric>
#include <algorithm>
#include <random>
#include "SumsOfVectors.h"

#define DEBUG false

void log(const string &x){
    if(DEBUG){
        std::cout << x << std::endl;
    }
}

void sumsOfVectors_omp_per_vector(const vector<vector<int8_t>> &data, vector<long> &solution, unsigned long minVectorSize) {
    log("per_vector");
    for(unsigned i = 0; i < data.size(); i++) {
        long tmp = 0;
#pragma omp parallel for reduction(+:tmp) shared(data)
        for (unsigned u = 0; u < data[i].size(); u++) {
            tmp += data[i][u];
        }
        solution[i] = tmp;
    }
}

void sumsOfVectors_omp_static(const vector<vector<int8_t>> &data, vector<long> &solution,
                              unsigned long minVectorSize) {
    // Pokud vektory, ktere zpracovavame nejsou prilis dlouhe, ale naopak jich musime zpracovat
    // velky pocet, muzeme zparalelizovat vnejsi for smycku, ktera prochazi pres jednotlive
    // vektory. Vyuzijte paralelizaci pres #pragma omp parallel for se statickym schedulingem.
    log("static");
#pragma omp parallel for schedule(static) shared(data, solution)
    for(unsigned i = 0; i < data.size(); i++) {
        long tmp = 0;
        for (signed char u : data[i]) {
            tmp += u;
        }
        solution[i] = tmp;
    }
}

void sumsOfVectors_omp_dynamic(const vector<vector<int8_t>> &data, vector<long> &solution,
                               unsigned long minVectorSize) {
    // Na cviceni jsme si ukazali, ze staticky scheduling je nevhodny, pokud praci mezi
    // jednotliva vlakna nedokaze rozdelit rovnomerne. V teto situaci muze byt vhodnym
    // resenim pouzitim dynamickeho schedulingu. Ten je rovnez vhodny v situacich, kdy
    // nevime predem, jak dlouho budou jednotlive vypocty trvat. Dani za to je vyssi
    // rezie pri zjistovani indexu 'i', ktery ma vlakno v dane chvili zpracovavat.
    log("dynamic");
#pragma omp parallel for schedule(dynamic) shared(data, solution)
    for(unsigned i = 0; i < data.size(); i++) {
        long tmp = 0;
        for (signed char u : data[i]) {
            tmp += u;
        }
        solution[i] = tmp;
    }
}

void sumsOfVectors_omp_shuffle(const vector<vector<int8_t>> &data, vector<long> &solution,
                               unsigned long minVectorSize) {
    // Dalsi moznosti, jak se vyhnout problemum pri pouziti statickeho schedulingu (tj.,
    // zejmena nevyvazenemu rozdeleni prace) je predzpracovani dat. V teto funkci zkuste
    // data pred zparalelizovanim vnejsi for smycky (se statickym schedulingem) nejprve
    // prohazet. To samozrejme predpoklada, ze cas potrebny na predzpracovani je radove
    // mensi, nez zisk, ktery ziskame nahrazenim dynamickeho schedulingu za staticky.
    //
    // Tip: Abyste se vyhnuli kopirovani vektoru pri "prohazovani" (ktere muze byt velmi
    // drahe!), muzete prohazovat pouze pointery na vektory. Alternativou je vytvorit si
    // nejprve pole nahodne serazenych indexu a ty pak pouzit pro pristup k poli.
    // Uzitecnymi metodami mohou byt metody std::iota(...) (ktera Vam vygeneruje posloupnost
    // indexu) a std::random_shuffle(...), ktera zajisti nahodne prohazeni prvku.
    log("shuffle");
    vector<int> indexes(data.size());
    std::iota (std::begin(indexes), std::end(indexes), 0);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indexes.begin(), indexes.end(), g);
#pragma omp parallel for schedule(static) shared(data, solution, indexes)
    for(unsigned i = 0; i < indexes.size(); i++) {
        long tmp = 0;
        int idx = indexes[i];
        for (signed char u : data[idx]) {
            tmp += u;
        }
        solution[idx] = tmp;
    }
}
