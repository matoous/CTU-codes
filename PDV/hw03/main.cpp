#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
#include <omp.h>
#include <unistd.h>
#include "_dataGenerator/DataGenerator.h"
#include "_executor/Executor.h"
#include "_outputGenerator/TextTable.h"

using namespace std;

// Instance trid, ktere se staraji o generovani dat a naslednou exekuci metod implementovanych v "SumsOfVectors.cpp"
DataGenerator generator;
Executor executor;

// Prevod na retezec s fixni presnosti
string as_string(double x, unsigned int precision) {
    stringstream s;
    s << fixed << setprecision(precision) << x;
    return s.str();
}

// Konverze na cas
string convertTimeToString(long nanos) {
    if (nanos <= 0) return "N/A";
    else if (nanos >= 100000000) return as_string(nanos / 1e9, 2) + "s";
    else if (nanos >= 100000) return as_string(nanos / 1e6, 2) + "ms";
    else if (nanos >= 100) return as_string(nanos / 1e3, 2) + "us";
    else return to_string(nanos) + "ns";
}

// Naplneni radku tabulky vysledku
void addRowWithResultsToTable(const string &typeOfData, const results_t &results, TextTable &table) {
    table.add(typeOfData);
    table.add(convertTimeToString(results.referenceTime));
    table.add(convertTimeToString(results.perVectorTime));
    table.add(convertTimeToString(results.withShuffleTime));
    table.add(convertTimeToString(results.dynamicSchedulingTime));
    table.add(convertTimeToString(results.staticSchedulingTime));
    table.endOfRow();
}

// Prvni sada dat, ktera obsahuje velice malo hodne dlouhych vektoru
// Tato sada je vhodna pro paralelizaci na urovni vektoru. Tento zpusob paralelizace by mel byt v tomto pripade nejrychlejsi
void generateAndSolveFirstDataSet(TextTable &table) {
    //vytvarime 2 vektory, ktere maji kazdy 500000000 pseudonahodne generovanych cisel
    vector<vector<int8_t>> data(2, vector<int8_t>(25000000));
    //spravne reseni
    vector<long> solution(2);
    //nagenerujeme data a ulozime si spravne reseni
    generator.generateData(solution, data);

    // Zavolame metody, ktere jste naimplementovali a pridame radek do tabulky
    auto results = executor.executeMethods(solution, data);
    addRowWithResultsToTable("Malo hodne dlouhych vektoru", results, table);
}

// Druha sada dat, kde delka vektoru je generovana z normalni distribuce podle parametru "mean" a "sigma".
// V tomto pripade jsou vektory serazene podle delky sestupne.
// Nejrychlejsim resenim by melo byt pouziti dynamickeho schedulingu. Alternativne si vyzkousite data pred
// pouzitim statickeho schedulingu promichat. Zkuste se zamyslet proc bychom chteli v nekterych pripadech
// pridavat dalsi rezii v podobe michani
void generateAndSolveSecondDataSet(TextTable &table) {
    constexpr int N = 24;

    vector<int> lengths(N);

    // Delky vektoru vygenerujeme ze dvou normalnich rozdeleni. Vetsina dat pochazi z rozdeleni s malou
    // stredni hodnotou (a dane vektory tedy zpracujeme velmi rychle). Cast dat ale trva velmi dlouho
    // (jsou generovana z rozdeleni z vysokou stredni hodnotou), a budou tedy narocne na zpracovani.
    // V nasem pripade se data narocna na zpracovani nachazi na zacatku datove sady - ale v obecnosti
    // mohou byt kdekoliv.

    double longWeight = 1.0 / std::thread::hardware_concurrency();
    int longData = std::floor(longWeight * N);

    generator.generateDistribution(lengths, 50000000, 100, 0);
    generator.generateDistribution(lengths, 2, 1, longData);

    //serazeni delek vektoru vzestupne
    //sort(lengths.begin(), lengths.end());
    vector<vector<int8_t>> data;
    for (int &j : lengths) {
        vector<int8_t> vec(static_cast<unsigned long>(j));
        data.push_back(vec);
    }
    vector<long> solution(N);
    generator.generateData(solution, data);
    auto results = executor.executeMethods(solution, data);
    addRowWithResultsToTable("Delky s velkym rozptylem", results, table);
}

// Treti sada dat, ktera obsahuje velky pocet vektoru male konstatntni velikost.
// Vase reseni s redukci by melo byt napsane tak, aby bylo srovnatelne se sekvencim resenim. To znamena, ze
// se vypocet v pripade redukce nebude vykonavat paralelne.
// Proc v tomto pripade muze byt dynamicke rozvrhovani horsi nez staticke?
void generateAndSolveThirdDataSet(TextTable &table) {
    vector<vector<int8_t>> data(10000000, vector<int8_t>(2));
    vector<long> solution(10000000);
    generator.generateData(solution, data);
    auto results = executor.executeMethods(solution, data);
    addRowWithResultsToTable("Hodne kratkych vektoru", results, table);
}

// Ctvrta sada obsahuje data, ktera jsou nevhodna k paralelizaci. Vektoru je malo a navic jsou
// velmi kratke. V tomto pripade by mela mit na vrch sekvencni verze, ktera nema zadnou rezii
// na paralelizaci.
void generateAndSolveForthDataSet(TextTable &table) {
    vector<vector<int8_t>> data(10, vector<int8_t>(10));
    vector<long> solution(10);
    generator.generateData(solution, data);
    auto results = executor.executeMethods(solution, data);
    addRowWithResultsToTable("Data nevhodna k paralelizaci", results, table);
}

int main() {
    //vytvor tabulku, parametry jsou znaky pro ohraniceni
    TextTable table('-', '|', '+');

    //zahlavi tabulky
    table.add("");
    table.add("Sekvencni reseni");
    table.add("P. na urovni vektoru");
    table.add("Reseni s michanim");
    table.add("Reseni s dyn. rozvrhovanim");
    table.add("Reseni se statickym rozvrhovanim");
    table.endOfRow();

    //pro blizsi informace se podivejte na samotne metody s komentarem
    generateAndSolveFirstDataSet(table);
    generateAndSolveSecondDataSet(table);
    generateAndSolveThirdDataSet(table);
    generateAndSolveForthDataSet(table);

    //vytisknuti tabulky s vysledky
    table.setAlignment(10, TextTable::Alignment::RIGHT);
    std::cout << table;

    return 0;
}