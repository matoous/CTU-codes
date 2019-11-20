#pragma once

#include "trie.hpp"
#include <algorithm>
#include <vector>
#include <string>

#include <chrono>
#include <stdint.h>

#define VALIDATE_SETS(lhs, rhs) \
    do {\
        auto l = lhs; \
        auto r = rhs; \
        std::sort(begin(l), end(l)); \
        std::sort(begin(r), end(r)); \
        REQUIRE(l == r);             \
   } while(0)


std::vector<std::string> as_vec(std::vector<std::string> v);
std::vector<std::string> extract_all(const trie& t);
void insert_all(trie& t, const std::vector<std::string>& strings);

char generate_char();
std::string generate_string(std::size_t size = 20);

std::vector<std::string> generate_data(std::size_t sz);

std::ostream& operator<<(std::ostream &os, trie::const_iterator const &i);

struct cpu_clock {
    using rep = int64_t;
    using period = std::nano;
    using duration = std::chrono::duration<rep, period>;
    using time_point = std::chrono::time_point<cpu_clock>;
    static constexpr bool is_steady = true;
    static time_point now();
};
