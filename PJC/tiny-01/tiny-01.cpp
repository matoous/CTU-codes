#include "tiny-01.hpp"

#include <set>
#include <regex>
#include <exception>

static const std::set<std::string> uncountables {
    "equipment",
    "information",
    "rice",
    "money",
    "species",
    "series",
    "fish",
    "sheep",
    "deer",
    "aircraft",
    "trout",
    "swine"
};

struct rule {
    std::regex m_pattern;
    std::string m_replacement;
    rule(std::string pattern, std::string replacement):
        m_pattern(pattern, std::regex_constants::icase),
        m_replacement(std::move(replacement))
        {}
        
    bool matches(std::string const& word) const {
        return std::regex_search(word, m_pattern);
    }
    
    std::string pluralize(std::string const& word) const {
        return std::regex_replace(word, m_pattern, m_replacement);
    }
};

std::vector<rule> rules {
    rule("person", "people"),
    rule("^ox$", "oxen"),
    rule("^criterion$", "criteria"),
    rule("child$", "children"),
    rule("foot$", "feet"),
    rule("tooth$", "teeth"),
    rule("^goose$", "geese"),
    rule("(.*[^af])fe?$", "$1ves"),         // ie, wolf, wife, but not giraffe, gaffe, safe
    rule("(hu|talis|otto|Ger|Ro|brah)man$", "$1mans"), // Exceptions from man -> men
    rule("(.*)man$", "$1men"),
    rule("(.+[^aeiou])y$", "$1ies"),
    rule("(.+zz)$", "$1es"),        // Buzz -> Buzzes
    rule("(.+z)$", "$1zes"),        // Quiz -> Quizzes
    rule("([m|l])ouse$", "$1ice"),
    rule("(append|matr|ind)(e|i)x$", "$1ices"),    // ie, Matrix, Index
    rule("(octop|vir|radi|fung)us$", "$1i"),
    rule("(phyl|milleni|spectr)um$", "$1a"),
    rule("(cris|ax)is$", "$1es"),
    rule("(.+(s|x|sh|ch))$", "$1es"),
    rule("(.+)ies$", "$1ies"),
    rule("(.+)", "$1s")
}; 


std::string pluralize(std::string const& word) {
    if (uncountables.count(word) > 0) {
        return word;
    }
    
    for (auto const& r : rules) {
        if (r.matches(word)) {
            return r.pluralize(word);
        }
    }
    
    // The last rule is fully generic "append s" rule, so we cannot
    // get here unless something is seriously wrong.
    throw std::runtime_error("Word '" + word + "' did not match any rule");
}

std::vector<std::string> pluralize(std::vector<std::string> const& words) {
    std::vector<std::string> plurelized(0);
    for (auto word : words) {
        plurelized.push_back(pluralize(word));
    }
    return plurelized;
}
