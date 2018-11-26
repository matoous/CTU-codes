#ifndef PJC_LIST_HPP
#define PJC_LIST_HPP

#include <cstddef>
#include <vector>
#include <iterator>
#include <set>
#include <iosfwd>

using std::size_t;

namespace pjc {
    const size_t dist(const std::string &, const std::string &);

    class autocorrect {
    private:
        std::set<std::string> dictionary;

    public:
        autocorrect() = default;
        explicit autocorrect(std::istream&);

        size_t size();
        void add_word(std::string w);
        void remove_word(std::string w);

        std::vector<std::string> correct(std::string word);
        std::vector<std::string> p_correct(std::string word);
        std::vector<std::string> suggest(std::string word);
        std::vector<std::string> suggest(std::string word, int n);
    };
}

#endif
