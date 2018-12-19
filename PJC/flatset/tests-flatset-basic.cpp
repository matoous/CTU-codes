#include "flatset.hpp"
#include "catch.hpp"

#include "tests-helpers.hpp"

TEST_CASE("Default constructed flatset passes basic accessors", "[basics]") {
    flat_set<int> fs;
    REQUIRE(fs.empty());
    REQUIRE(fs.size() == 0);
}

TEST_CASE("Trivial use of insert", "[basics]") {
    flat_set<int> fs;
    REQUIRE(fs.insert(123).second);
    REQUIRE(fs.size() == 1);
    REQUIRE_FALSE(fs.empty());

    // Second insertion of the same type does nothing
    REQUIRE_FALSE(fs.insert(123).second);
    REQUIRE(fs.size() == 1);
    CHECK_FALSE(fs.empty());

    REQUIRE(fs.insert(124).second);
    REQUIRE(fs.size() == 2);

    REQUIRE(fs.insert(125).second);
    REQUIRE(fs.size() == 3);
}

TEST_CASE("Trivial use of find", "[basics]") {
    flat_set<int> fs;
    REQUIRE(fs.find(0) == fs.end());

    fs.insert(1); fs.insert(2); fs.insert(3);

    REQUIRE(fs.find(1) == fs.begin());
    REQUIRE(fs.find(2) == fs.begin() + 1);
    REQUIRE(fs.find(3) == fs.begin() + 2);

    REQUIRE(fs.find(4) == fs.end());
}

TEST_CASE("Inserted elements are kept sorted", "[basics]") {
    flat_set<int> fs;
    fs.insert(3);
    fs.insert(1);
    fs.insert(2);

    CHECK(fs.size() == 3);
    REQUIRE(ordered_equals(fs, { 1, 2, 3 }));
}

TEST_CASE("Erasing elements works", "[basics]") {
    flat_set<int> fs;
    SECTION("Erasing non-existing elements") {
        REQUIRE(fs.erase(3) == 0);
    }
    SECTION("Erasing existing elements... ") {
        insert_many(fs, {1, 3, 2, 4, 5, 6, 0, -1});
        CHECK(fs.size() == 8);
        SECTION("... by value") {
            REQUIRE(fs.erase(3) == 1);
            REQUIRE(fs.erase(5) == 1);
            CHECK(fs.size() == 6);
            REQUIRE(ordered_equals(fs, { -1, 0, 1, 2, 4, 6 }));
        }
        SECTION("... by iterator") {
            REQUIRE(*fs.erase(fs.begin()) == 0);
            REQUIRE(*fs.erase(fs.begin() + 2) == 3);
            CHECK(fs.size() == 6);
            REQUIRE(ordered_equals(fs, { 0, 1, 3, 4, 5, 6 }));
        }
    }
}

TEST_CASE("Basic usage of lower_bound", "[basics]") {
    flat_set<int> fs;
    insert_many(fs, { 5, 0, 0, 1, 1, 9, 2 });
    // These two are present in the set
    REQUIRE(fs.lower_bound(0) == fs.begin());
    REQUIRE(fs.lower_bound(9) == fs.begin() + 4);
    // These aren't
    REQUIRE(fs.lower_bound(42) == fs.end()); // greater than max
    REQUIRE(fs.lower_bound(-1) == fs.begin()); // lesser than min
    REQUIRE(fs.lower_bound(3) == fs.begin() + 3);  // in range, but not present
}

TEST_CASE("Basic usage of upper_bound", "[basics]") {
    flat_set<int> fs;
    insert_many(fs, { 5, 0, 0, 1, 1, 9, 2 });
    REQUIRE(fs.upper_bound(0) == fs.begin() + 1);
    REQUIRE(fs.upper_bound(9) == fs.end());
    REQUIRE(fs.lower_bound(42) == fs.end()); // greater than max
    REQUIRE(fs.upper_bound(-1) == fs.begin()); // lesser than min
    REQUIRE(fs.upper_bound(3) == fs.begin() + 3);  // in range, but not present
}

TEST_CASE("Range insert", "[basics]") {
    flat_set<int> fs;
    std::vector<int> vec{ 3, 4, 5, 2, 1, 0 };
    fs.insert(vec.begin(), vec.end() - 1);
    REQUIRE(ordered_equals(fs, { 1, 2, 3, 4, 5 }));
}

TEST_CASE("Range constructor", "[basics]") {
    std::vector<int> vec{ 2, 4, 6, 8, 9, 7, 5, 3, 1 };
    flat_set<int> fs(vec.begin(), vec.end() - 2);
    REQUIRE(ordered_equals(fs, { 2, 4, 5, 6, 7, 8, 9 }));
}
