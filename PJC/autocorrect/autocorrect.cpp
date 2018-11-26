#include <utility>

#include "autocorrect.hpp"

#include <utility>
#include <algorithm>

namespace pjc {

    pjc::autocorrect::autocorrect(std::iostream stream) {
        std::string word;
        while(stream >> word)
            dictionary.insert(word);
    }

    size_t pjc::autocorrect::size() {
        return dictionary.size();
    }

    void autocorrect::add_word(std::string w) {
        dictionary.insert(w);
    }

    void autocorrect::remove_word(std::string w) {
        dictionary.erase(w);
    }

    std::vector<std::string> autocorrect::correct(std::string word) {
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

    std::vector<std::string> autocorrect::suggest(std::string word, int n) {
        auto l = std::vector<std::pair<int, std::string>>(0);
        
        auto res = std::vector<std::string>();
        return res;
    }

    std::vector<std::string> autocorrect::suggest(std::string word) {
        return autocorrect::suggest(std::move(word), 10);
    }

    const size_t dist(std::string s, std::string t) {
        size_t i,j, temp,tracker;
        auto m = s.length();
        auto n = t.length();
        size_t d[100][100];
        for(i=0;i<=m;i++)
            d[0][i] = i;
        for(j=0;j<=n;j++)
            d[j][0] = j;
        for (j=1;j<=m;j++) {
            for(i=1;i<=n;i++) {
                if(s[i-1] == t[j-1])
                    tracker = 0;
                else
                    tracker = 1;
                temp = std::min((d[i-1][j]+1),(d[i][j-1]+1));
                d[i][j] = std::min(temp,(d[i-1][j-1]+tracker));
            }
        }
        return d[n][m];
    }

} // end namespace pjc
