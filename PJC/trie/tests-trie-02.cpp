#include "test-helpers.hpp"
#include "trie.hpp"
#include "catch.hpp"

/*
    Testy pro stage-02.

    Tento krok testuje metody
      * begin() const
      * end() const
    A metody iterátorů z nich vytvořených, tj.
      * const_iterator()
      * operátory prefix ++, postfix ++
      * operátory ==, !=
      * operátor *

    Testy v tomto kroku předpokládají, že metody testované v minulém
    kroku fungují správně, a tudíž se na nich mohou stavět testy pro další
    funkcionalitu.
*/


TEST_CASE("Iterator", "[stage2]") {
    SECTION("Default constructed iterators are equal") {
        trie::const_iterator iter1, iter2;
        REQUIRE(iter1 == iter2);
    }
    trie t({ "abc", "bcd", "cde", "edf" });
    auto it = t.begin();
    auto et = t.end();
    SECTION("End iterator and default constructed iterator are equal") {
        trie::const_iterator iter;
        REQUIRE(iter == et);
    }
    SECTION("Prefix increment") {
        REQUIRE(it != et);
        REQUIRE(*it == "abc");
        ++it;
        REQUIRE(it != et);
        REQUIRE(*it == "bcd");
        ++it;

        REQUIRE(it != et);
        REQUIRE(*it == "cde");
        ++it;

        REQUIRE(it != et);
        REQUIRE(*it == "edf");
        ++it;

        REQUIRE(it == et);
    }
    SECTION("Postfix increment") {
        REQUIRE(it != et);
        REQUIRE(*it == "abc");
        it++;
        REQUIRE(it != et);
        REQUIRE(*it == "bcd");
        it++;

        REQUIRE(it != et);
        REQUIRE(*it == "cde");
        it++;

        REQUIRE(it != et);
        REQUIRE(*it == "edf");
        it++;

        REQUIRE(it == et);
    }
    SECTION("Iterator can be reused") {
        auto it2 = it;
        ++it;
        REQUIRE(*it2 == "abc");
        ++it;
        REQUIRE(*it2 == "abc");
        ++it;
        REQUIRE(*it2 == "abc");
        ++it2;
        REQUIRE(*it2 == "bcd");
    }
    SECTION("Iterator equality") {
        auto it2 = it;
        REQUIRE(it == it2);
        while (it != et) {
            ++it;
            REQUIRE_FALSE(it == it2);
            REQUIRE(it != it2);
            ++it2;
            REQUIRE(it == it2);
            REQUIRE_FALSE(it != it2);
        }
    }
    SECTION("Empty string handling") {
        trie yes({ "", "a", "b", "aaa", "aab" });
        auto it = yes.begin();
        REQUIRE(*it == "");
        trie no({ "a", "b", "aaa", "aab" });
        it = no.begin();
        REQUIRE(*it == "a");
    }
}
