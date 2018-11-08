#include "sort.h"
#include <iostream>

using namespace std;

// implementace vaseho radiciho algoritmu. Detalnejsi popis zadani najdete v "sort.h"
void radix_par(std::vector<std::string *> &vector_to_sort, const MappingFunction &mappingFunction,
               unsigned long alphabet_size, unsigned long string_lengths) {

    // init buckets
    std::vector< std::vector<string *> > buckets(alphabet_size, vector<string*>(0));
    std::vector<unsigned long> starts_at(alphabet_size, 0);
    unsigned long strings_to_sort = vector_to_sort.size();
    unsigned long ss = vector_to_sort.at(0)->size();

    // sort pointers to buckets
    for(unsigned long i = 0; i < strings_to_sort; i++){
        std::string * sp = vector_to_sort[i];
        const unsigned long mix = mappingFunction(sp->at(ss - string_lengths));
        buckets[mix].push_back(sp);
    }

    // recursively sort children
    if (string_lengths > 1){
#pragma omp parallel for schedule(dynamic)
        for(unsigned int i = 0; i < alphabet_size; i++){
            if(buckets[i].size() > 1){
                std::vector<std::string *> &bp = (buckets[i]);
                radix_par(bp, mappingFunction, alphabet_size, string_lengths-1);
            }
        }
    }

    for(unsigned int i = 1; i < alphabet_size; i++) {
        starts_at[i] = starts_at[i-1] + buckets[i-1].size();
    }

    // put back together
#pragma omp parallel for
    for(unsigned int i = 0; i < alphabet_size; i++){
        const unsigned long offset = starts_at[i];
        const std::vector<std::string *> bckt = buckets[i];
        for(unsigned int u = 0; u < bckt.size(); u++){
            vector_to_sort[offset + u] = bckt[u];
        }
    }
}