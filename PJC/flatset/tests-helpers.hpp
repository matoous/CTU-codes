#pragma once

#include <algorithm>
#include <functional>
#include <iosfwd>
#include <chrono>
#include <stdint.h>
#include <set>
#include "flatset.hpp"

template <typename T>
bool ordered_equals(flat_set<T> const& fs, std::vector<T> const& target) {
    if (fs.size() != target.size()) {
        return false;
    }
    return std::equal(fs.begin(), fs.end(), target.begin(), target.end());
}

template <typename T>
void insert_many(flat_set<T>& fs, std::vector<T> const& target) {
    for (auto const& elem : target) {
        fs.insert(elem);
    }
}

template <typename T> struct is_flat_set : std::false_type {};
template <typename T> struct is_flat_set<flat_set<T>> : std::true_type {};

template <typename T, typename = is_flat_set<T>>
bool set_equal(const T &a, const std::set<typename T::value_type> &b) {
    return std::equal(a.begin(), a.end(), b.begin(), b.end());
}
template <typename T, typename = is_flat_set<T>>
bool set_equal(const T &a, std::initializer_list<typename T::value_type> &&b) {
    return set_equal(a, std::set<typename T::value_type>(std::move(b)));
}
template <typename T, typename = is_flat_set<T>>
bool set_equal(const T &a, const std::vector<typename T::value_type> &vb) {
    std::set<typename T::value_type> b(vb.begin(), vb.end());
    return std::equal(a.begin(), a.end(), b.begin(), b.end());
}
template <typename T>
flat_set<T> make_flat_set(std::vector<T> const &v) {
    return flat_set<T>(v.begin(), v.end());
}


// Used to silence warnings about unused expression result
template <typename... T>
void unused(T&&...) {}

struct counter {
    counter() = default;
    counter(int dc, int cc, int ca, int mc, int ma, int d, int o):default_constructors(dc),
        copy_constructors(cc), copy_assignments(ca), move_constructors(mc), move_assignments(ma),
        destructors(d), other(o) {}
    int default_constructors = 0;
    int copy_constructors = 0;
    int copy_assignments = 0;
    int move_constructors = 0;
    int move_assignments = 0;
    int destructors = 0;
    int other = 0;
};

bool operator==(const counter& c1, const counter& c2);
counter operator-(counter c1, const counter& c2);
std::ostream& operator<<(std::ostream& out, const counter& cnt);

struct tracker {
    tracker();
    tracker(double val);
    tracker(const tracker&);
    tracker& operator=(const tracker&);
    tracker(tracker&&);
    tracker& operator=(tracker&&);
    ~tracker();

    double value;
    static counter cnt;
};

bool operator==(const tracker& c1, const tracker& c2);
bool operator<(const tracker& c1, const tracker& c2);

struct ordered {
    ordered() = default;
    ordered(int sortby, size_t order):sortby(sortby), order(order) {}

    int sortby;
    size_t order;
};

bool operator==(const ordered& o1, const ordered& o2);
bool operator<(const ordered& o1, const ordered& o2);

struct thrower {
    thrower(int, bool);
    thrower(const thrower&);
    thrower& operator=(const thrower&);
    thrower(thrower&&);
    thrower& operator=(thrower&&);

    void check() const;
    int value;
    bool should_throw;
};

bool operator==(const thrower& t1, const thrower& t2);


template <typename Iterator, bool ForceInvalidate = true>
struct fake_input_iterator {
    using difference_type = typename Iterator::difference_type;
    using value_type = typename Iterator::value_type;
    using reference = typename Iterator::reference;
    using pointer = typename Iterator::pointer;
    using iterator_category = std::input_iterator_tag;

    fake_input_iterator() = default;
    fake_input_iterator(Iterator it) :place(it) {}

    bool operator==(const fake_input_iterator& rhs) const {
        return place == rhs.place;
    }
    bool operator!=(const fake_input_iterator& rhs) const {
        return place != rhs.place;
    }
    reference operator*() {
        return *place;
    }
    fake_input_iterator& operator++() {
        if (ForceInvalidate) {
            *place = value_type{};
        }
        ++place;
        return *this;
    }
    fake_input_iterator operator++(int) {
        if (ForceInvalidate) {
            *place = value_type{};
        }
        auto tmp(*this);
        ++(*this);
        return tmp;
    }
    pointer operator->() {
        return place;
    }

    Iterator place;
};

struct cpu_clock {
    using rep = int64_t;
    using period = std::nano;
    using duration = std::chrono::duration<rep, period>;
    using time_point = std::chrono::time_point<cpu_clock>;
    static constexpr bool is_steady = true;
    static time_point now();
};
