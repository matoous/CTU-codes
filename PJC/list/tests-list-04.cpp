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
        list l({ 1, 2 });
        auto iter = l.begin();
        std::advance(iter, 1);
        auto lists = l.split(iter);
        REQUIRE(l.empty());
        REQUIRE(l.size() == 0);
        REQUIRE(lists.first.size() == 1);
        REQUIRE(lists.second.size() == 1);
        REQUIRE(list_equal(lists.first, { 1 }));
        REQUIRE(list_equal(lists.second, { 2 }));
    }
    SECTION("Many elements") {
        auto elems = std::vector<double>({ 1, 2, 3, 4, 5, 6, 7, 8, 9 });
        list l{ elems };
        SECTION("Split all elements to the LHS list") {
            auto lists = l.split(l.end());
            REQUIRE(list_equal(lists.first, elems));
            REQUIRE(lists.second.size() == 0);
            REQUIRE(lists.second.empty());
        }
        SECTION("Split all elements to the RHS list") {
            auto lists = l.split(l.begin());
            REQUIRE(lists.first.size() == 0);
            REQUIRE(lists.first.empty());
            REQUIRE(list_equal(lists.second, elems));
        }
        SECTION("Split the list somewhere in the middle") {
            auto offset = 4;
            auto iter = l.begin();
            std::advance(iter, offset);
            auto lists = l.split(iter);
            REQUIRE(list_equal(lists.first, { begin(elems), begin(elems) + offset }));
            REQUIRE(list_equal(lists.second, { begin(elems) + offset, end(elems) }));
        }
    }
}

TEST_CASE("Split: degenerate cases", "[stage4][split]") {
    SECTION("Splliting empty list results in two empty lists") {
        list l;
        auto lists = l.split(l.begin());
        REQUIRE(list_equal(lists.first, {}));
        REQUIRE(list_equal(lists.first, {}));
    }
    SECTION("Splitting apart 1 element") {
        list l({ 1 });
        SECTION("Split into the LHS list") {
            auto lists = l.split(l.end());
            REQUIRE(lists.first.size() == 1);
            REQUIRE(lists.first.front() == 1);
            REQUIRE(lists.second.size() == 0);
        }
        SECTION("Split into the RHS list") {
            auto lists = l.split(l.begin());
            REQUIRE(lists.first.size() == 0);
            REQUIRE(lists.second.size() == 1);
            REQUIRE(lists.second.front() == 1);
        }
    }
}

TEST_CASE("Merge: common cases", "[stage4][merge]") {
    SECTION("All of LHS list comes before RHS list") {
        list lhs({ 0, 1, 2 });
        list rhs({ 3, 4, 5 });
        lhs.merge(rhs);
        REQUIRE(rhs.empty());
        REQUIRE(rhs.size() == 0);
        CHECK(lhs.size() == 6);
        REQUIRE(list_equal(lhs, { 0, 1, 2, 3, 4, 5 }));
    }
    SECTION("All of RHS list comes before LHS list") {
        list lhs({ 3, 4, 5 });
        list rhs({ 0, 1, 2 });
        lhs.merge(rhs);
        CHECK(rhs.empty());
        CHECK(rhs.size() == 0);
        CHECK(lhs.size() == 6);
        REQUIRE(list_equal(lhs, { 0, 1, 2, 3, 4, 5 }));
    }
    SECTION("Merge together 2 lists consisting of the same elements") {
        list lhs({ 0, 0, 0, 0 });
        list rhs({ 0, 0, 0, 0 });
        lhs.merge(rhs);
        CHECK(lhs.size() == 8);
        REQUIRE(list_equal(lhs, { 0, 0, 0, 0, 0, 0, 0, 0 }));
    }
    SECTION("Merge together 2 equal lists") {
        list lhs({ 0, 1, 2, 3 });
        list rhs({ 0, 1, 2, 3 });
        lhs.merge(rhs);
        REQUIRE(list_equal(lhs, { 0, 0, 1, 1, 2, 2, 3, 3 }));
    }
    SECTION("Plain merge #1") {
        list lhs({ 0, 1, 3, 4, 5 });
        list rhs({ 1, 2, 2 });
        lhs.merge(rhs);
        REQUIRE(list_equal(lhs, { 0, 1, 1, 2, 2, 3, 4, 5 }));
    }
    SECTION("Plain merge #2") {
        list lhs({ 1, 2, 3 });
        list rhs({ -2, -2, 4, 4, 5 });
        lhs.merge(rhs);
        REQUIRE(list_equal(lhs, { -2, -2, 1, 2, 3, 4, 4, 5 }));
    }
}

TEST_CASE("Merge: degenerated cases", "[stage4][merge]") {
    SECTION("Merge empty lists") {
        list lhs, rhs;
        lhs.merge(rhs);
        REQUIRE(lhs.empty());
        REQUIRE(lhs.size() == 0);
        REQUIRE(rhs.empty());
        REQUIRE(rhs.size() == 0);
    }
    SECTION("Merge into an empty list") {
        list lhs;
        list rhs({ 0, 1, 2, 3 });
        lhs.merge(rhs);
        REQUIRE(lhs.size() == 4);
        REQUIRE(list_equal(lhs, { 0, 1, 2, 3 }));
        REQUIRE(rhs.empty());
        REQUIRE(rhs.size() == 0);
    }
    SECTION("Merge with an empty list") {
        list lhs({ 0, 1, 2, 3 });
        list rhs;
        lhs.merge(rhs);
        REQUIRE(lhs.size() == 4);
        REQUIRE(list_equal(lhs, { 0, 1, 2, 3 }));
    }
}

TEST_CASE("Sort: crafted inputs", "[stage4][sort]") {
    SECTION("sort can be called on an empty list") {
        list l;
        l.sort();
        CHECK(l.empty());
        CHECK(l.size() == 0);
        REQUIRE(list_equal(l, {}));
    }
    SECTION("sort can be called on an list with single element") {
        list l({ 42 });
        l.sort();
        CHECK(l.size() == 1);
        REQUIRE(list_equal(l, { 42 }));
    }
    SECTION("sorting a list of equal elements") {
        list l({ 3.3, 3.3, 3.3, 3.3, 3.3 });
        l.sort();
        CHECK(l.size() == 5);
        REQUIRE(list_equal(l, { 3.3, 3.3, 3.3, 3.3, 3.3 }));
    }
}

TEST_CASE("Sort: generated inputs", "[stage4][sort]") {
    // The input sizes are intentionally kept small to avoid
    // overly long runtime under Debug builds.
    auto size = GENERATE(1234, 1345, 5522, 1287);
    CAPTURE(size);
    auto elems = generate_data(size);
    list l{ elems };
    l.sort();
    std::sort(elems.begin(), elems.end());
    REQUIRE(list_equal(l, elems));
}

TEST_CASE("Reverse: Degenerate cases", "[stage4][reverse]") {
    SECTION("Can be called on an empty list") {
        list l;
        l.reverse();
        CHECK(l.empty());
        CHECK(l.size() == 0);
    }
    SECTION("Can be called on a list with single element") {
        list l({ 1 });
        l.reverse();
        CHECK(l.size() == 1);
        REQUIRE(list_equal(l, { 1 }));
    }
    SECTION("Can be called on a list with two elements") {
        list l({ 1, 2 });
        l.reverse();
        CHECK(l.size() == 2);
        REQUIRE(list_equal(l, { 2, 1 }));
    }
}

TEST_CASE("Reverse: larger list", "[stage4][reverse]") {
    auto elems = std::vector<double>{ 0., 1., 2., 3., 4., 5., 6., 7., 8., 9. };
    list l{ elems };
    l.reverse();
    std::reverse(begin(elems), end(elems));
    REQUIRE(list_equal(l, elems));
    SECTION("Two reverses cancel each other out") {
        l.reverse();
        std::reverse(begin(elems), end(elems));
        REQUIRE(list_equal(l, elems));
    }
}

TEST_CASE("Remove: basic tests", "[stage4][remove]") {
    auto elements = std::vector<double>{ 0, 2, 2, 3, 2, 2, 3, 3, 2, 0, 1 };
    list l(elements);
    SECTION("Remove a common element (2)") {
        l.remove(2);
        elements.erase(std::remove(elements.begin(), elements.end(), 2),
                       elements.end());
        REQUIRE(list_equal(l, elements));
    }
    SECTION("Remove an element not in list") {
        l.remove(5);
        REQUIRE(list_equal(l, elements));
    }
    SECTION("Remove from an empty list leaves the list in a valid state") {
        list l2;
        l2.remove(1);
        l2.push_back(1.23);
        l2.push_front(1.22);
        REQUIRE(list_equal(l2, { 1.22, 1.23 }));
    }
}

TEST_CASE("Remove: complex tests", "[stage4][remove]") {
    auto elements = std::vector<double>{ 0, 2, 2, 3, 2, 2, 3, 3, 2, 0, 1 };
    list l{ elements };
    SECTION("Remove the first element in the list") {
        l.remove(0);
        elements.erase(std::remove(begin(elements), end(elements), 0),
                       end(elements));
        REQUIRE(list_equal(l, elements));
    }
    SECTION("Remove the last element in list") {
        l.remove(1);
        elements.erase(std::remove(begin(elements), end(elements), 1),
                       end(elements));
        REQUIRE(list_equal(l, elements));
    }
    SECTION("Remove all elements from a list") {
        for (int i = 0; i < 4; ++i) {
            l.remove(i);
        }
        REQUIRE(l.empty());
        REQUIRE(l.size() == 0);
    }
}
