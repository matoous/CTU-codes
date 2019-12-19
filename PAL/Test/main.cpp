#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>

using namespace std;

class Node {
    map<char, Node *> nexts;
    set<int> finishes;
public:
    Node() : nexts(map<char, Node *>()), finishes(set<int>()) {}

    void add(const char *s, int owner) {
        if (*s == '\0') {
            this->finishes.insert(owner);
        } else if (this->nexts.find(*s) != this->nexts.end()) {
            this->nexts.find(*s)->second->add(s + 1, owner);
        } else {
            auto n = new Node;
            n->add(s + 1, owner);
            this->nexts.insert(make_pair(*s, n));
        }
    }

    vector<string> prefixes(long long l, long long N, int finishesMet) {
        auto sub = vector<string>();
        if (finishes.size() + finishesMet >= N) {
            sub.push_back("\0");
            return sub;
        }
        for (const auto &n : nexts) {
            auto subprefixes = n.second->prefixes(l - 1, N, finishesMet + finishes.size());
            for (const auto &subprefix : subprefixes)
                sub.push_back(n.first + subprefix);
        }
        return sub;
    }
};

int lpow(long long base, long long exp) {
    int result = 1;
    while (true) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            return result;
        base *= base;
    }
}

int main() {
    auto *root = new Node;
    string SA, tmp;
    long long N, D, K, stringsNum;

    cin >> SA >> N >> D >> K;

    for (int i = 0; i < N; ++i) {
        cin >> stringsNum;
        for (int u = 0; u < stringsNum; ++u) {
            cin >> tmp;
            root->add(tmp.c_str(), i);
        }
    }

    vector<string> prefixes = root->prefixes(K, D, 0);

    long long alphabetSize = SA.size();
    long long result = 0;
    for (const auto &pref : prefixes) {
        auto lengthDifference = K - pref.size();
        result += lpow(alphabetSize, lengthDifference) % 100000;
    }
    cout << result << endl;
    return 0;
}
