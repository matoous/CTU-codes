#include "catch.hpp"

#include "list.hpp"

#include "test-helpers.hpp"

#include <algorithm>

/*
    Testy pro stage-04.

    Tento krok testuje
      * split
      * merge
      * sort
      * reverse
      * remove

    Testy v tomto kroku předpokládají, že metody testované v minulém
    kroku fungují správně, a tudíž se na nich mohou stavět testy pro další
    funkcionalitu.
*/

using pjc::list;

TEST_CASE("Split: common cases", "[stage4][split]") {
    SECTION("Two elements") {

    }
    SECTION("Many elements") {

    }
}

TEST_CASE("Split: degenerate cases", "[stage4][split]") {
    SECTION("Splliting empty list results in two empty lists") {

    }
    SECTION("Splitting apart 1 element") {
        SECTION("Split into the LHS list") {

        }
        SECTION("Split into the RHS list") {

        }
    }
}

TEST_CASE("Merge: common cases", "[stage4][merge]") {
    SECTION("All of LHS list comes before RHS list") {

    }
    SECTION("All of RHS list comes before LHS list") {

    }
    SECTION("Merge together 2 lists consisting of the same elements") {

    }
    SECTION("Merge together 2 equal lists") {

    }
    SECTION("Plain merge #1") {

    }
    SECTION("Plain merge #2") {

    }
}
