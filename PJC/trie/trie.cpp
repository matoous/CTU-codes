#include "trie.hpp"

#include <utility>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <queue>

bool empty(const trie_node *n) {
    for (const auto &child : n->children) {
        if (child) return false;
    }
    return true;
}

bool nerase(trie_node *n, const char *str) {
    if (!*str) {
        if (n->is_terminal) {
            n->is_terminal = false;
            return true;
        }
        return false;
    }
    if (n->children[*str] == nullptr) return false;
    if (nerase(n->children[*str], str + 1)) {
        if (empty(n->children[*str])) {
            delete n->children[*str];
            n->children[*str] = nullptr;
        }
        return true;
    }
    return false;
}

bool ninsert(trie_node *n, const char *str) {
    if (!*str && n->is_terminal) return false;
    if (!*str) {
        n->is_terminal = true;
        return true;
    }
    if (n->children[*str] == nullptr) {
        n->children[*str] = new trie_node{
                .parent = n,
                .payload = *str,
        };
    }
    return ninsert(n->children[*str], str + 1);
}

trie_node *find_next(const trie_node *n, const char prev) {
    for (const auto &child : n->children) {
        if (!child) continue;
        if (child->payload <= prev) continue;
        return child->is_terminal ? child : find_next(child, '\0');
    }
    return n->parent ? find_next(n->parent, n->payload) : nullptr;
}

trie::trie() {
    m_root = new trie_node{};
}

trie::trie(const std::vector<std::string> &strings) {
    if(m_root != nullptr) {
        std::vector<trie_node *> to_del;
        std::queue<trie_node *> q;
        q.push(m_root);
        while (!q.empty()) {
            auto curr = q.front();
            q.pop();
            if (curr == nullptr)continue;
            for (auto ch : curr->children) {
                if (ch != nullptr) q.push(ch);
            }
            to_del.push_back(curr);
        }
        for (auto r : to_del) {
            delete r;
        }
    }
    m_root = new trie_node{};
    for (const auto &s : strings) this->insert(s);
}


trie::trie(const trie &rhs) {
    m_root = new trie_node{};
    for (const auto &x : rhs) {
        insert(x);
    }
}

trie::~trie() {
    std::vector<trie_node *> to_del;
    std::queue<trie_node *> q;
    q.push(m_root);
    while (!q.empty()) {
        auto curr = q.front();
        q.pop();
        if (curr == nullptr)continue;
        for (auto ch : curr->children) {
            if (ch != nullptr) q.push(ch);
        }
        to_del.push_back(curr);
    }
    for (auto r : to_del) {
        delete r;
    }
}

bool trie::insert(const std::string &str) {
    if (ninsert(m_root, str.c_str())) {
        m_size++;
        return true;
    }
    return false;
}

bool trie::contains(const std::string &str) const {
    auto curr = m_root;
    const char *sptr = str.c_str();
    while (*sptr && curr != nullptr) {
        curr = curr->children[*sptr];
        sptr++;
    }
    if (*sptr) return false;
    if (!*sptr && curr != nullptr) return curr->is_terminal;
    return false;
}

bool trie::erase(const std::string &str) {
    const char *sptr = str.c_str();
    if (nerase(m_root, sptr)) {
        m_size--;
        return true;
    }
    return false;
}

size_t trie::size() const {
    return m_size;
}

bool trie::empty() const {
    return m_size == 0;
}

trie::const_iterator trie::begin() const {
    return {m_root->is_terminal ? m_root : find_next(m_root, '\0')};
}

trie::const_iterator trie::end() const {
    return nullptr;
}

trie &trie::operator=(const trie &rhs) {
    if (this->m_root == rhs.m_root)
        return *this;
    std::vector<trie_node *> to_del;
    std::queue<trie_node *> q;
    q.push(m_root);
    while (!q.empty()) {
        auto curr = q.front();
        q.pop();
        if (curr == nullptr)continue;
        for (auto ch : curr->children) {
            if (ch != nullptr) q.push(ch);
        }
        to_del.push_back(curr);
    }
    for (auto r : to_del) {
        delete r;
    }    this->m_size = 0;
    this->m_root = new trie_node{};
    for (const auto &x : rhs) {
        insert(x);
    }
    return *this;
}

std::vector<std::string> trie::search_by_prefix(const std::string &prefix) const {
    auto curr = m_root;
    auto s = prefix.c_str();
    while (*s && curr != nullptr) {
        curr = m_root->children[*s];
        s++;
    }
    std::vector<std::string> res;
    if (curr == nullptr) return res;
    return res;//todo
}

std::vector<std::string> trie::get_prefixes(const std::string &str) const {
    auto curr = m_root;
    auto s = str.c_str();
    std::vector<std::string> res;
    while (*s && curr != nullptr) {
        curr = m_root->children[*s];
        s++;
        if (curr && curr->payload) {
            auto tmp = std::string(1, curr->payload);
            res.push_back(res.size() > 0 ? res[res.size() - 1] + tmp : tmp);
        }
    }
    return res;
}

trie trie::operator|(trie const &rhs) const {
    auto nt = trie();
    for (auto const &x: *this) {
        nt.insert(x);
    }
    for (auto const &x: rhs) {
        nt.insert(x);
    }
    return nt;
}

trie trie::operator&(trie const &rhs) const {
    auto nt = trie();
    for (auto const &x: *this) {
        if (rhs.contains(x)) {
            nt.insert(x);
        }
    }
    return nt;
}

trie::trie(trie &&rhs) {
    std::vector<trie_node *> to_del;
    std::queue<trie_node *> q;
    q.push(m_root);
    while (!q.empty()) {
        auto curr = q.front();
        q.pop();
        if (curr == nullptr)continue;
        for (auto ch : curr->children) {
            if (ch != nullptr) q.push(ch);
        }
        to_del.push_back(curr);
    }
    for (auto r : to_del) {
        delete r;
    }
    this->m_root = rhs.m_root;
    this->m_size = rhs.m_size;
    rhs.m_root = nullptr;
    rhs.m_size = 0;
}

trie &trie::operator=(trie &&rhs) {
    std::vector<trie_node *> to_del;
    std::queue<trie_node *> q;
    q.push(m_root);
    while (!q.empty()) {
        auto curr = q.front();
        q.pop();
        if (curr == nullptr)continue;
        for (auto ch : curr->children) {
            if (ch != nullptr) q.push(ch);
        }
        to_del.push_back(curr);
    }
    for (auto r : to_del) {
        delete r;
    }
    this->m_root = rhs.m_root;
    this->m_size = rhs.m_size;
    rhs.m_root = nullptr;
    rhs.m_size = 0;
    return *this;
}

void trie::swap(trie &rhs) {
    auto tmp = this->m_root;
    this->m_root = rhs.m_root;
    rhs.m_root = tmp;
    auto n = this->m_size;
    this->m_size = rhs.m_size;
    rhs.m_size = n;
}

bool trie::operator==(const trie &rhs) const {
    auto rit = rhs.begin();
    auto lit = this->begin();
    while (rit != rhs.end() && lit != this->end()) {
        if (*rit != *lit) return false;
        rit++;
        lit++;
    }
    return rit == rhs.end() && lit == this->end();
}

bool trie::operator<(const trie &rhs) const {
    auto rit = rhs.begin();
    auto lit = this->begin();
    while (rit != rhs.end() && lit != this->end()) {
        if (*lit < *rit) return true;
        else if (*rit > *lit) return false;
        lit++;
        rit++;
    }
    return rit != rhs.end();
}

bool operator<=(const trie &lhs, const trie &rhs) {
    return lhs == rhs || lhs < rhs;
}

bool operator>(const trie &lhs, const trie &rhs) {
    return (lhs != rhs) && !(lhs < rhs);
}

bool operator>=(const trie &lhs, const trie &rhs) {
    return !(lhs < rhs);
}

bool operator!=(const trie &lhs, const trie &rhs) {
    return !(lhs == rhs);
}

std::ostream &operator<<(std::ostream &out, trie const &trie) {
    for (const auto &x: trie) {
        out << x << " ";
    }
    return out;
}

trie::const_iterator::const_iterator(const trie_node *node) {
    this->current_node = node;
}

trie::const_iterator &trie::const_iterator::operator++() {
    this->current_node = find_next(this->current_node, '\0');
    return *this;
}

trie::const_iterator trie::const_iterator::operator++(int x) {
    for (int i = 0; i < x + 1; ++i) {
        this->current_node = find_next(this->current_node, '\0');
    }
    return *this;
}

trie::const_iterator::reference trie::const_iterator::operator*() const {
    std::string x = "";
    auto curr = this->current_node;
    while (curr != nullptr && curr->payload) {
        x = curr->payload + x;
        curr = curr->parent;
    }
    return x;
}

bool trie::const_iterator::operator==(const trie::const_iterator &rhs) const {
    return this->current_node == rhs.current_node;
}

bool trie::const_iterator::operator!=(const trie::const_iterator &rhs) const {
    return this->current_node != rhs.current_node;
}
