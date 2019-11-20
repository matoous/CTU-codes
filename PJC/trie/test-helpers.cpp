#include "test-helpers.hpp"
#include "trie.hpp"

#include <chrono>
#include <random>
#include <iostream>
#include <algorithm>
#include <functional>

std::vector<std::string> as_vec(std::vector<std::string> v) {
    return v;
}

std::vector<std::string> extract_all(const trie& t) {
    return { t.begin(), t.end() };
}

void insert_all(trie& t, const std::vector<std::string>& strings) {
    for (const auto& string : strings) {
        t.insert(string);
    }
}

char generate_char() {
    // Note: The proper way to init the generator would be to use a std::random_device
    //       but that would lead to every run being different and I would prefer to keep
    //       the tests deterministic.
    static std::mt19937 gen;
    char chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUWXYZ0123456789,.:;/\\\"'";
    // -1 because the dist is [a, b], -1 because there is a \0 at the end
    static std::uniform_int_distribution<std::size_t> dist(0, sizeof(chars) - 2);
    return chars[dist(gen)];
}

std::string generate_string(std::size_t size) {
    std::string ret;
    ret.reserve(size);
    std::generate_n(std::back_inserter(ret), size, generate_char);
    return ret;
}

std::vector<std::string> generate_data(std::size_t sz) {
    std::vector<std::string> res;
    res.reserve(sz);
    std::generate_n(std::back_inserter(res), sz, std::bind(generate_string, 20));
    return res;
}

std::ostream& operator<<(std::ostream &os, trie::const_iterator const &i) {
    return os << "trie::const_iterator {" << i.current_node << "}";
}

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
void throw_windows_error(std::string msg)
{
    DWORD dwErrVal = GetLastError();
    std::error_code ec (dwErrVal, std::system_category());
    throw std::system_error(ec, std::move(msg));
}

cpu_clock::time_point cpu_clock::now()
{
    FILETIME createTime;
    FILETIME exitTime;
    FILETIME kernelTime;
    FILETIME userTime;

    if (!GetProcessTimes(GetCurrentProcess(),
        &createTime, &exitTime, &kernelTime, &userTime))
        throw_windows_error("GetProcessTimes failed");

    uint64_t d = (static_cast<uint64_t>(userTime.dwHighDateTime) << 32)
                 | userTime.dwLowDateTime;
    duration res { std::chrono::duration<uint64_t, std::ratio<1, 100'000>>(d) }; // 100ns
    return time_point(res);
}
#else // -> POSIX systems
#include <time.h>
cpu_clock::time_point cpu_clock::now()
{
    struct timespec ts;
    if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts) == -1)
        throw std::system_error(errno, std::generic_category());
    using namespace std::chrono;
    duration res = duration(seconds(ts.tv_sec)) +
                   duration_cast<duration>(nanoseconds(ts.tv_nsec));
    return time_point(res);
}
#endif
