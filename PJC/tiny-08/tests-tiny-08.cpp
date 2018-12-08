#include "tiny-08.hpp"

#include <ostream>
#include <cstdlib>
#include <utility>

namespace {
    struct only_lt {
        int value;
    };
    bool operator<(only_lt lhs, only_lt rhs) {
        return lhs.value < rhs.value;
    }

    std::ostream& operator<<(std::ostream& os, only_lt elem) {
        os << "only_lt: " << elem.value;
        return os;
    }

    struct with_identity {
        int value;
        int identity;
    };
    bool operator<(with_identity lhs, with_identity rhs) {
        return lhs.value < rhs.value;
    }
    bool operator==(with_identity lhs, with_identity rhs) {
        return lhs.value == rhs.value && lhs.identity == rhs.identity;
    }
    std::ostream& operator<<(std::ostream& os, with_identity elem) {
        os << "with_identity: " << elem.value << ", " << elem.identity;
        return os;
    }
}

#include "catch.hpp"


TEST_CASE("Basic clamping on ints", "[all]") {
    const int low = 0;
    const int high = 255;
    REQUIRE(clamp(-1, low, high) == 0);
    REQUIRE(clamp(256, low, high) == 255);
    int val = 10;
    REQUIRE(clamp(val, low, high) == 10);
}

TEST_CASE("Clamping a type that has only <", "[all]") {
    const only_lt low{0};
    const only_lt high{255};
    REQUIRE(clamp({-1}, low, high).value == 0);
    REQUIRE(clamp({256}, low, high).value == 255);
    only_lt val{10};
    REQUIRE(clamp(val, low, high).value == 10);
}

TEST_CASE("Clamping a type that has identity", "[all]") {
    const with_identity low{0, -1};
    const with_identity high{255, 1};
    with_identity val{10, 0};
    REQUIRE(clamp(val, low, high) == with_identity{10, 0});
    val = with_identity{0, 0};
    REQUIRE(clamp(val, low, high) == with_identity{0, 0});
    val = with_identity{255, 0};
    REQUIRE(clamp(val, low, high) == with_identity{255, 0});
}

// Check custom lambda
TEST_CASE("Clamping using a custom lambda", "[all]") {
    const int low = 255;
    const int high = 0;
    auto cmp = [] (int lhs, int rhs) { return std::abs(lhs) > std::abs(rhs); };
    int value, expected;
    std::tie(value, expected) = GENERATE(values<std::pair<int, int>>({
            {-1, -1},
            {256, 255},
            {-256, 255},
            {244, 244},
            {0, 0},
            {-22, -22},
            {-255, -255},
            {255, 255}
    }));
    REQUIRE(clamp(value, low, high, cmp) == expected);
}
