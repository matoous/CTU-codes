#ifndef DATABASEQUERIES_QUERY_H
#define DATABASEQUERIES_QUERY_H

#include <vector>
#include <functional>
#include <atomic>

template<typename row_t>
using predicate_t = std::function<bool(const row_t &)>;



template<typename row_t>
bool is_satisfied_for_all(std::vector<predicate_t<row_t>> predicates, std::vector<row_t> data_table);

template<typename row_t>
bool is_satisfied_for_any(std::vector<predicate_t<row_t>> predicates, std::vector<row_t> data_table);



template<typename row_t>
bool is_satisfied_for_all(std::vector<predicate_t<row_t>> predicates, std::vector<row_t> data_table) {
    auto row_count = data_table.size();
    auto predicates_count = predicates.size();
    std::atomic<bool> ok { true };

#pragma omp parallel for
    for(unsigned int u = 0; u < predicates_count; u++){
        auto & predicate = predicates[u];
        auto this_ok = false;
        for(unsigned int i = 0 ; i < row_count ; i++) {
            auto & row = data_table[i];
            bool is_satisfied = predicate(row);

            if(is_satisfied) {
                this_ok = true;
                break;
            }
        }
        if(!this_ok){
            ok.store(false);
#pragma omp cancel for
        }
    }

    return ok.load();
}

template<typename row_t>
bool is_satisfied_for_any(std::vector<predicate_t<row_t>> predicates, std::vector<row_t> data_table) {
    auto row_count = data_table.size();
    auto predicates_count = predicates.size();
    std::atomic<bool> ok { false };

#pragma omp parallel for
    for(unsigned int i = 0 ; i < row_count ; i++) {
        auto & row = data_table[i];
        for(unsigned int u = 0; u < predicates_count; u++){
            auto & predicate = predicates[u];
            bool is_satisfied = predicate(row);
            if(is_satisfied) {
                ok.store(true);
                break;
            }
        }
        if(ok.load()){
#pragma omp cancel for
        }
    }

    return ok.load();
}


#endif //DATABASEQUERIES_QUERY_H
