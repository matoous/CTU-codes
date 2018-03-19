#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int nthreads, tid, procs, maxt, inpar, nested;

/* Zacatek regionu */
#pragma omp parallel private(nthreads, tid)
    {

        /* Jake vlakno vykonava kod */
        tid = omp_get_thread_num();

        /* Jenom master bude vypisovat informace */
        if (tid == 0) {
            printf("Thread %d getting environment info...\n", tid);

            /* Informace o prostredi */
            //pocet procesoru, ktere jsou dostupne pri volani funkce
            procs = omp_get_num_procs();
            //pocet vlaken v paralelnim bloku
            nthreads = omp_get_num_threads();
            //maximalni pocet vlaken, ktere by mohly byt v paralelnim bloku
            maxt = omp_get_max_threads();
            //pokud je volano v paralelnim bloku, vrati nenulovou hodnotu
            inpar = omp_in_parallel();
            //hodnota rika, zda je povolena vnorena paralelizace
            nested = omp_get_nested();

            /* Vypis... */
            printf("Number of processors = %d\n", procs);
            printf("Number of threads = %d\n", nthreads);
            printf("Max threads = %d\n", maxt);
            printf("In parallel? = %d\n", inpar);
            printf("Nested parallelism enabled? = %d\n", nested);

        }

    }

}
