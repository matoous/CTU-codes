#ifndef SORTING_MERGESORT_H
#define SORTING_MERGESORT_H

#include <cstdlib>
#include <cstring>
#include <memory>

// Metoda pro sliti dvou serazenych poli [a .. b-1] a [b .. end-1].
template <typename elem_t>
void merge(elem_t *a, elem_t *b, elem_t *end, elem_t *tmp) {
    elem_t *orig_a = a;
    elem_t *orig_b = b;
    elem_t *orig_tmp = tmp;

    // Nejprve hodnoty z poli 'a' a 'b' vypisujeme do pomocneho pole
    // 'tmp'.
    while (a < orig_b && b < end) {
        if (*a < *b) *tmp++ = *a++;
        else *tmp++ = *b++;
    }
    while (a < orig_b) *tmp++ = *a++;
    while (b < end) *tmp++ = *b++;

    // Pote, co vsechny prvky vypiseme do pole 'tmp', prekopirujeme
    // je zpet na puvodni misto - tj. do pole [a .. end-1]. Vysledne
    // pole [a .. end-1] je serazene.
    memcpy(orig_a, orig_tmp, (tmp - orig_tmp) * sizeof(elem_t));
}

#include "mergesort_sequential.h"
#include "mergesort_parallel.h"

#endif //SORTINGCODEFINALMERGE_SORT_H
