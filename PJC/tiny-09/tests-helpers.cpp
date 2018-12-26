#include "tests-helpers.hpp"

#include <ostream>

bool operator==(const counter& c1, const counter& c2) {
    return c1.default_constructors == c2.default_constructors &&
        c1.copy_constructors == c2.copy_constructors &&
        c1.copy_assignments == c2.copy_assignments &&
        c1.move_constructors == c2.move_constructors &&
        c1.move_assignments == c2.move_assignments &&
        c1.destructors == c2.destructors &&
        c1.other == c2.other;
}
counter operator-(counter c1, const counter& c2) {
    c1.default_constructors -= c2.default_constructors;
    c1.copy_constructors -= c2.copy_constructors;
    c1.copy_assignments -= c2.copy_assignments;
    c1.move_constructors -= c2.move_constructors;
    c1.move_assignments -= c2.move_assignments;
    c1.destructors -= c2.destructors;
    c1.other -= c2.other;
    return c1;
}

std::ostream & operator<<(std::ostream & out, const counter & cnt) {
    out << "| dc: " << cnt.default_constructors
        << " | cc: " << cnt.copy_constructors
        << " | ca: " << cnt.copy_assignments
        << " | mc: " << cnt.move_constructors
        << " | ma: " << cnt.move_assignments
        << " | ~d: " << cnt.destructors
        << " | o:  " << cnt.other
        << " |";
    return out;
}

tracker::tracker():value(0) {
    cnt.default_constructors++;
}

tracker::tracker(double val):value(val) {
    cnt.other++;
}

tracker::tracker(const tracker& rhs):value(rhs.value) {
    cnt.copy_constructors++;
}
tracker& tracker::operator=(const tracker& rhs) {
    value = rhs.value;
    cnt.copy_assignments++;
    return *this;
}
tracker::tracker(tracker&& rhs):value(rhs.value) {
    cnt.move_constructors++;
}
tracker& tracker::operator=(tracker&& rhs) {
    value = rhs.value;
    cnt.move_assignments++;
    return *this;
}
tracker::~tracker() {
    cnt.destructors++;
}

counter tracker::cnt = {};


bool operator==(const tracker& c1, const tracker& c2) {
    return c1.value == c2.value;
}

noexcept_tracker::noexcept_tracker() :value(0) {
    cnt.default_constructors++;
}

noexcept_tracker::noexcept_tracker(double val) : value(val) {
    cnt.other++;
}

noexcept_tracker::noexcept_tracker(const noexcept_tracker& rhs) : value(rhs.value) {
    cnt.copy_constructors++;
}
noexcept_tracker& noexcept_tracker::operator=(const noexcept_tracker& rhs) {
    value = rhs.value;
    cnt.copy_assignments++;
    return *this;
}
noexcept_tracker::noexcept_tracker(noexcept_tracker&& rhs) noexcept:
    value(rhs.value) {
    cnt.move_constructors++;
}
noexcept_tracker& noexcept_tracker::operator=(noexcept_tracker&& rhs) noexcept {
    value = rhs.value;
    cnt.move_assignments++;
    return *this;
}
noexcept_tracker::~noexcept_tracker() {
    cnt.destructors++;
}

counter noexcept_tracker::cnt = {};


bool operator==(const noexcept_tracker& c1, const noexcept_tracker& c2) {
    return c1.value == c2.value;
}

trivially_copyable_tracker::trivially_copyable_tracker() :value(0) {
    cnt.default_constructors++;
}

trivially_copyable_tracker::trivially_copyable_tracker(double val) : value(val) {
    cnt.other++;
}

trivially_copyable_tracker::trivially_copyable_tracker(const trivially_copyable_tracker& rhs) : value(rhs.value) {
    cnt.copy_constructors++;
}
trivially_copyable_tracker& trivially_copyable_tracker::operator=(const trivially_copyable_tracker& rhs) {
    value = rhs.value;
    cnt.copy_assignments++;
    return *this;
}
trivially_copyable_tracker::trivially_copyable_tracker(trivially_copyable_tracker&& rhs) noexcept:
    value(rhs.value) {
    cnt.move_constructors++;
}
trivially_copyable_tracker& trivially_copyable_tracker::operator=(trivially_copyable_tracker&& rhs) noexcept {
    value = rhs.value;
    cnt.move_assignments++;
    return *this;
}
trivially_copyable_tracker::~trivially_copyable_tracker() {
    cnt.destructors++;
}

counter trivially_copyable_tracker::cnt = {};


bool operator==(const trivially_copyable_tracker& c1, const trivially_copyable_tracker& c2) {
    return c1.value == c2.value;
}
