#include <utility>

#include "autocorrect.hpp"

#include <utility>
#include <algorithm>


// TODO DELETE
#include <iostream>

namespace pjc {

    pjc::autocorrect::autocorrect(std::istream& stream) {
        std::string word;
        while(stream >> word)
            dictionary.push_back(word);
    }

    size_t pjc::autocorrect::size() {
        return dictionary.size();
    }

    void autocorrect::add_word(std::string w) {
        dictionary.push_back(w);
    }

    void autocorrect::remove_word(std::string w) {
        return;
    }

    std::vector<std::string> autocorrect::correct(const std::string& word) {
        auto corrections = std::vector<std::string>(0);
        auto bestDistance = word.length();
        for(const auto& w : dictionary){
            auto d = dist(word, w);
            if(d < bestDistance){
                if(d == 0) {
                    corrections.clear();
                    break;
                }
                bestDistance = d;
                corrections.clear();
                corrections.push_back(w);
            }
            else if(d == bestDistance){
                corrections.push_back(w);
            }
        }
        return corrections;
    }

    std::vector<std::vector<std::string>> autocorrect::correct(const std::vector<std::string>& words) {
        std::vector<std::vector<std::string>> corrections(words.size());
        for(auto i = 0; i < words.size(); i++){
            const auto& word = words[i];
            auto bestDistance = word.length();
            for(const auto& w : dictionary){
                auto d = dist(word, w);
                if(d < bestDistance){
                    if(d == 0) {
                        corrections[i].clear();
                        break;
                    }
                    bestDistance = d;
                    corrections[i].clear();
                    corrections[i].push_back(w);
                }
                else if(d == bestDistance){
                    corrections[i].push_back(w);
                }
            }
        }
        return corrections;
    }

    std::vector<std::string> corrector(std::vector<std::string> dict, std::string word){
        auto corrections = std::vector<std::string>(0);
        auto bestDistance = word.length();
        for(const auto& w : dict){
            auto d = dist(word, w);
            if(d < bestDistance){
                if(d == 0) {
                    corrections.clear();
                    break;
                }
                bestDistance = d;
                corrections.clear();
                corrections.push_back(w);
            }
            else if(d == bestDistance){
                corrections.push_back(w);
            }
        }
        return corrections;
    }

    std::vector<std::vector<std::string>> autocorrect::p_correct(const std::vector<std::string>& words) {
        std::vector<std::future<std::vector<std::string>>> futures;
        std::vector<std::vector<std::string>> corrections(words.size());

        // NEW
        for(const auto &word : words){
            futures.push_back(std::async(std::launch::async, corrector, dictionary, word));
        }
        for(auto &e: futures){
            corrections.push_back(e.get());
        }
        return corrections;


        // OLD
        for(auto i = 0; i < words.size(); i++){
            const auto& word = words[i];
            auto bestDistance = word.length();
            for(int u = 0; u < dictionary.size(); u++){
                const auto& w = dictionary[u];
                auto d = dist(word, w);
                if(d < bestDistance){
                    if(d == 0) {
                        corrections[i].clear();
                        break;
                    }
                    bestDistance = d;
                    corrections[i].clear();
                    corrections[i].push_back(w);
                }
                else if(d == bestDistance){
                    corrections[i].push_back(w);
                }
            }
        }
        return corrections;
    }

    std::vector<std::string> autocorrect::suggest(std::string word, int n) {
        auto l = std::vector<std::pair<int, std::string>>(0);

        auto res = std::vector<std::string>();
        return res;
    }

    std::vector<std::string> autocorrect::suggest(std::string word) {
        return autocorrect::suggest(std::move(word), 10);
    }

    const size_t dist(const std::string &s1, const std::string &s2)
    {
        const size_t m(s1.size());
        const size_t n(s2.size());

        if( m==0 ) return n;
        if( n==0 ) return m;

        auto *costs = new size_t[n + 1];
        for( size_t k=0; k<=n; k++ ) costs[k] = k;
        size_t i = 0;
        for ( std::string::const_iterator it1 = s1.begin(); it1 != s1.end(); ++it1, ++i ) {
            costs[0] = i+1;
            size_t corner = i;

            size_t j = 0;
            for ( std::string::const_iterator it2 = s2.begin(); it2 != s2.end(); ++it2, ++j ) {
                size_t upper = costs[j+1];
                if( *it1 == *it2 ) {
                    costs[j+1] = corner;
                } else {
                    size_t t(upper<corner?upper:corner);
                    costs[j+1] = (costs[j]<t?costs[j]:t)+1;
                }
                corner = upper;
            }
        }

        size_t result = costs[n];
        delete [] costs;

        return result;
    }

} // end namespace pjc
