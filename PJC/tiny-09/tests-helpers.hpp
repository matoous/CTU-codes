#pragma once

#include <iosfwd>
#include "pjc-traits.hpp"


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

struct noexcept_tracker {
    noexcept_tracker();
    noexcept_tracker(double val);
    noexcept_tracker(const noexcept_tracker&);
    noexcept_tracker& operator=(const noexcept_tracker&);
    noexcept_tracker(noexcept_tracker&&) noexcept;
    noexcept_tracker& operator=(noexcept_tracker&&) noexcept;
    ~noexcept_tracker();

    double value;
    static counter cnt;
};

bool operator==(const noexcept_tracker& c1, const noexcept_tracker& c2);

struct trivially_copyable_tracker {
    trivially_copyable_tracker();
    trivially_copyable_tracker(double val);
    trivially_copyable_tracker(const trivially_copyable_tracker&);
    trivially_copyable_tracker& operator=(const trivially_copyable_tracker&);
    trivially_copyable_tracker(trivially_copyable_tracker&&) noexcept;
    trivially_copyable_tracker& operator=(trivially_copyable_tracker&&) noexcept;
    ~trivially_copyable_tracker();

    double value;
    static counter cnt;
};

bool operator==(const trivially_copyable_tracker& c1, const trivially_copyable_tracker& c2);

namespace pjc {
template <>
struct is_trivially_copyable<trivially_copyable_tracker> : std::true_type {};
}

