#ifndef PJC_LIST_HPP
#define PJC_LIST_HPP

#include <cstddef>
#include <vector>
#include <iterator>
#include <set>
#include <omp.h>

using std::size_t;

namespace pjc {
    // compute Levenshtein distance of two words
    const size_t dist(const std::string &, const std::string &);

    class autocorrect {
    private:
        std::set<std::string> dictionary;

    public:
        autocorrect() = default;

        // initialize the autocorrect with dictionary of words from file
        explicit autocorrect(std::istream&);

        // initialize the autocorrect with dictionary of words provided as vector if strings
        explicit autocorrect(std::vector<std::string>&);

        size_t size();
        void add_word(std::string w);
        void remove_word(std::string w);

        std::vector<std::string> correct(const std::string&);
        std::vector<std::vector<std::string>> correct(const std::vector<std::string>&);
        std::vector<std::vector<std::string>> p_correct(const std::vector<std::string>&);
    };
}

#endif
