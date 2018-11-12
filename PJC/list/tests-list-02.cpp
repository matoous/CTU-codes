#include "catch.hpp"

#include "list.hpp"

#include "test-helpers.hpp"

/*
    Testy pro stage-02.

    Tento krok testuje metody
      * begin()
      * begin() const
      * cbegin()
      * end()
      * end() const
      * cend()
    A metody iterátorů z nich vytvořených, tj
      * iterator()
      * operátory prefix ++, postfix ++
      * operátory prefix --, postfix --
      * operátory ==, !=
      * operátory *, -> 


    Testy v tomto kroku předpokládají, že metody testované v minulém
    kroku fungují správně, a tudíž se na nich mohou stavět testy pro další
    funkcionalitu.

*/

using pjc::list;


TEST_CASE("Default constructed iterators are equal", "[stage2]") {
    list::iterator iter1, iter2;
    REQUIRE(iter1 == iter2);
    REQUIRE_FALSE(iter1 != iter2);
}

TEST_CASE("begin and end iterators of empty list are equal", "[stage2]") {
    list l;
    REQUIRE(l.begin() == l.end());
    REQUIRE_FALSE(l.begin() != l.end());
    REQUIRE(l.cbegin() == l.cend());
    REQUIRE_FALSE(l.cbegin() != l.cend());
}

TEST_CASE("Begin and end iterators of non-empty list are not equal", "[stage2]") {
    list l;
    l.push_back(1.23);
    REQUIRE(l.begin() != l.end());
    REQUIRE_FALSE(l.begin() == l.end());
    REQUIRE(l.cbegin() != l.cend());
    REQUIRE_FALSE(l.cbegin() == l.cend());
}

TEST_CASE("begin iterators of different lists are not equal", "[stage2]") {
    list l1, l2;
    l1.push_back(1); l2.push_back(1);
    REQUIRE(l1.begin() != l2.begin());
    REQUIRE(l1.cbegin() != l2.cbegin());
}

TEST_CASE("Dereferencing begin iterator returns first element", "[stage2]") {
    list l;
    l.push_back(1.234);
    auto it = l.begin();
    REQUIRE(*it == 1.234);
    auto cit = l.cbegin();
    REQUIRE(*cit == 1.234);
}

TEST_CASE("Increment operator moves the iterator forward", "[stage2]") {
    list l;
    append_to_list(l, { 1.11, 2.22, 3.33, 4.44 });

    auto it = l.begin();
    CHECK(*it == 1.11);
    SECTION("Prefix") {
        REQUIRE(*(++it) == 2.22);
        REQUIRE(*(++it) == 3.33);
        REQUIRE(*(++it) == 4.44);
        REQUIRE(++it == l.end());
    }
    SECTION("Postfix") {
        it++;
        REQUIRE(*(it++) == 2.22);
        REQUIRE(*(it++) == 3.33);
        REQUIRE(*(it++) == 4.44);
        REQUIRE(it == l.end());
    }
}

TEST_CASE("Increment operator moves the (const) iterator forward", "[stage2]") {
    list l;
    append_to_list(l, { 1.11, 2.22, 3.33, 4.44 });

    auto it = l.cbegin();
    CHECK(*it == 1.11);
    SECTION("Prefix") {
        REQUIRE(*(++it) == 2.22);
        REQUIRE(*(++it) == 3.33);
        REQUIRE(*(++it) == 4.44);
        REQUIRE(++it == l.cend());
    }
    SECTION("Postfix") {
        it++;
        REQUIRE(*(it++) == 2.22);
        REQUIRE(*(it++) == 3.33);
        REQUIRE(*(it++) == 4.44);
        REQUIRE(it == l.cend());
    }
}


TEST_CASE("Decrement operator moves the iterator backward", "[stage2]") {
    list l;
    append_to_list(l, { 5.55, 6.66, 7.77, 8.88 });

    auto it = l.end();
    SECTION("Prefix") {
        REQUIRE(*(--it) == 8.88);
        REQUIRE(*(--it) == 7.77);
        REQUIRE(*(--it) == 6.66);
        REQUIRE(*(--it) == 5.55);
        REQUIRE(it == l.begin());
    }

    SECTION("Postfix") {
        it--;
        REQUIRE(*(it--) == 8.88);
        REQUIRE(*(it--) == 7.77);
        REQUIRE(*(it--) == 6.66);
        REQUIRE(*it == 5.55);
        REQUIRE(it == l.begin());
    }
}

TEST_CASE("Decrement operator moves the (const) iterator backward", "[stage2]") {
    list l;
    append_to_list(l, { 5.55, 6.66, 7.77, 8.88 });

    auto it = l.cend();
    SECTION("Prefix") {
        REQUIRE(*(--it) == 8.88);
        REQUIRE(*(--it) == 7.77);
        REQUIRE(*(--it) == 6.66);
        REQUIRE(*(--it) == 5.55);
        REQUIRE(it == l.cbegin());
    }

    SECTION("Postfix") {
        it--;
        REQUIRE(*(it--) == 8.88);
        REQUIRE(*(it--) == 7.77);
        REQUIRE(*(it--) == 6.66);
        REQUIRE(*it == 5.55);
        REQUIRE(it == l.cbegin());
    }
}


TEST_CASE("Dereference operator can modify the underlying list", "[stage2]") {
    list l;
    append_to_list(l, { 1, 2, 3, 4 });
    auto it = l.begin();
    for (size_t i = 0; i < 4; ++i) {
        *(it++) *= 1.11;
    }
    it = l.begin();
    REQUIRE(*(it++) == 1.11);
    REQUIRE(*(it++) == 2.22);
    REQUIRE(*(it++) == 3.33);
    REQUIRE(*(it++) == 4.44);
    CHECK(it == l.end());
}
