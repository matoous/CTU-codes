#include <ostream>
#include <cstdlib>
#include <utility>

#include "vector.hpp"
#include "tests-helpers.hpp"
#include "catch.hpp"


TEST_CASE("Trivially copyable type is memcpyied", "[all]") {
    vector<trivially_copyable_tracker> vec; vec.reserve(3);
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    CHECK(vec.size() == 3);
    CHECK(vec[0] == 1);
    CHECK(vec[1] == 2);
    CHECK(vec[2] == 3);
    SECTION("Copy constructor") {
        auto cnt1 = trivially_copyable_tracker::cnt;
        auto vec2(vec);
        auto cnt2 = trivially_copyable_tracker::cnt;
        CHECK(vec2.size() == 3);
        CHECK(vec2[0] == 1);
        CHECK(vec2[1] == 2);
        CHECK(vec2[2] == 3);
        // There should be 3 more default constructors in cnt2, equalize them
        cnt2.default_constructors -= 3;
        REQUIRE(cnt2 == cnt1);
    }
    SECTION("Reserve") {
        auto cnt1 = trivially_copyable_tracker::cnt;
        vec.reserve(123456);
        auto cnt2 = trivially_copyable_tracker::cnt;
        CHECK(vec.capacity() >= 123456);
        CHECK(vec.size() == 3);
        CHECK(vec[0] == 1);
        CHECK(vec[1] == 2);
        CHECK(vec[2] == 3);
        // counters should be the same modulo 3 destructors and 123456 default constructors
        cnt2.destructors -= 3;
        cnt2.default_constructors -= 123456;
        REQUIRE(cnt2 == cnt1);
    }
}

TEST_CASE("Noexcept movable types are moved", "[all]") {
    vector<noexcept_tracker> vec; vec.reserve(3);
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    CHECK(vec.size() == 3);
    CHECK(vec[0] == 1);
    CHECK(vec[1] == 2);
    CHECK(vec[2] == 3);
    SECTION("Copy constructor") {
        auto cnt1 = noexcept_tracker::cnt;
        auto vec2(vec);
        auto cnt2 = noexcept_tracker::cnt;
        CHECK(vec2.size() == 3);
        CHECK(vec2[0] == 1);
        CHECK(vec2[1] == 2);
        CHECK(vec2[2] == 3);
        // There should be 3 more default constructors in cnt2, equalize them
        cnt2.default_constructors -= 3;
        // We should also see 3 copy assignments, because we cannot move
        // the elements inside a copy constructor
        cnt2.copy_assignments -= 3;
        REQUIRE(cnt2 == cnt1);
    }
    SECTION("Reserve") {
        auto cnt1 = noexcept_tracker::cnt;
        vec.reserve(123456);
        auto cnt2 = noexcept_tracker::cnt;
        CHECK(vec.capacity() >= 123456);
        CHECK(vec.size() == 3);
        CHECK(vec[0] == 1);
        CHECK(vec[1] == 2);
        CHECK(vec[2] == 3);
        // counters should be the same modulo 3 destructors and 123456 default constructors
        cnt2.destructors -= 3;
        cnt2.default_constructors -= 123456;
        // We should also see 3 move assignments, because in reserve we
        // can actually move the elements over
        cnt2.move_assignments -= 3;
        REQUIRE(cnt2 == cnt1);
    }
}

TEST_CASE("types that could throw are copied", "[all]") {
    vector<tracker> vec; vec.reserve(3);
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    CHECK(vec.size() == 3);
    CHECK(vec[0] == 1);
    CHECK(vec[1] == 2);
    CHECK(vec[2] == 3);
    SECTION("Copy constructor") {
        auto cnt1 = tracker::cnt;
        auto vec2(vec);
        auto cnt2 = tracker::cnt;
        CHECK(vec2.size() == 3);
        CHECK(vec2[0] == 1);
        CHECK(vec2[1] == 2);
        CHECK(vec2[2] == 3);
        // There should be 3 more default constructors in cnt2, equalize them
        cnt2.default_constructors -= 3;
        // We should also see 3 copy assignments
        cnt2.copy_assignments -= 3;
        REQUIRE(cnt2 == cnt1);
    }
    SECTION("Reserve") {
        auto cnt1 = tracker::cnt;
        vec.reserve(123456);
        auto cnt2 = tracker::cnt;
        CHECK(vec.capacity() >= 123456);
        CHECK(vec.size() == 3);
        CHECK(vec[0] == 1);
        CHECK(vec[1] == 2);
        CHECK(vec[2] == 3);
        // counters should be the same modulo 3 destructors and 123456 default constructors
        cnt2.destructors -= 3;
        cnt2.default_constructors -= 123456;
        // We should see 3 copies, because without no-except move
        // assignments, we have to fallback to copies for safety
        cnt2.copy_assignments -= 3;
        REQUIRE(cnt2 == cnt1);
    }
}
