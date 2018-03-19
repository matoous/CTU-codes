#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <chrono>
#include <cmath>
#include <cstdio>
#include "integrate.h"

using namespace std::chrono;

typedef double (*solver_t)(std::function<double(double)>,double,double,int);

// parametry pro integraci:
// pocatek intervalu, kde se zacina integrovat
const double INTEGRATION_START = 0.0;
// velikost kroku behem integrace
const double STEP_SIZE = 0.01;
// pocet kroku integrace
const int STEP_COUNT = 300;

// Jednoducha integracni funkce - identita
// Vypocet trva velmi kratce
double identity(double x) {
    return x;
}

// Slozitejsi integracni funkce - integral ze sinu s pohyblivou horni mezi integrace
// Vypocet trva tim dele, cim vetsi je argument udavajici delku integrace
double integrated_function(double integrationRange) {
    const int num_steps = 1000 * std::ceil(integrationRange*integrationRange*integrationRange*integrationRange);
    const double step_size = (integrationRange*integrationRange)/num_steps;
    return integrate_sequential([](double x) { return std::sin(x); }, INTEGRATION_START, step_size, num_steps);
}

int main() {

    // Vektor metod pocitajicich integral ze zadane funkce
    std::vector<solver_t> solvers;
    std::vector<std::string> solver_names;

    solvers.push_back(&integrate_sequential); solver_names.push_back(     "integrate_sequential(...)     ");
    solvers.push_back(&integrate_omp_critical); solver_names.push_back(   "integrate_omp_critical(...)   ");
    solvers.push_back(&integrate_omp_atomic); solver_names.push_back(     "integrate_omp_atomic(...)     ");
    solvers.push_back(&integrate_omp_reduction); solver_names.push_back(  "integrate_omp_reduction(...)  ");
    solvers.push_back(&integrate_omp_for_static); solver_names.push_back( "integrate_omp_for_static(...) ");
    solvers.push_back(&integrate_omp_for_dynamic); solver_names.push_back("integrate_omp_for_dynamic(...)");


    {
        printf("Integrace identity(...): identity(x)=x\n");

        // Zapamatujeme si cas sekvencni verze (prvni solver)
        double sequential_time = -1;

        // Urcite postupne casy behu jednotlivych zpusobu integrace
        for(int i = 0 ; i < solvers.size() ; i++) {
            // Cas zacatku behu metody
            try {
            auto begin = steady_clock::now();
            // Vypocet integralu - uvazujeme 1000000x vice kroku, protoze funkce f(x)=x je opravdu jednoducha
            double result = solvers[i](&identity, INTEGRATION_START, STEP_SIZE, 1000000*STEP_COUNT);
            // Cas konce behu metody
            auto end = steady_clock::now();

            long elapsed = duration_cast<microseconds>(end - begin).count();
            if(sequential_time < 0) sequential_time = elapsed;
            double speedup = sequential_time / (double)elapsed;

            printf("%s %9ldus (speedup %6.3fx)           result %f\n", solver_names[i].c_str(), elapsed, speedup, result);

            } catch(...) {
                printf("%s               ----- not implemented yet -----\n", solver_names[i].c_str());
            }
        }

        printf("\n\n");
    }

    {
        printf("Integrace slozitejsi funkce integrated_function(...) - numericky aproximovany integral sin(x) s promenlivym poctem kroku\n");
        // Zapamatujeme si cas sekvencni verze (prvni solver)
        double sequential_time = -1;

        // Urcite postupne casy behu jednotlivych zpusobu integrace
        for(int i = 0 ; i < solvers.size() ; i++) {
            // Cas zacatku behu metody
            try {
            auto begin = steady_clock::now();
            // Vypocet integralu
            double result = solvers[i](&integrated_function, INTEGRATION_START, STEP_SIZE, STEP_COUNT);
            // Cas konce behu metody
            auto end = steady_clock::now();

            long elapsed = duration_cast<microseconds>(end - begin).count();
            if(sequential_time < 0) sequential_time = elapsed;
            double speedup = sequential_time / (double)elapsed;

            printf("%s %9ldus (speedup %6.3fx)           result %f\n", solver_names[i].c_str(), elapsed, speedup, result);

            } catch(...) {
                printf("%s               ----- not implemented yet -----\n", solver_names[i].c_str());
            }
        }
    }

    return 0;
}