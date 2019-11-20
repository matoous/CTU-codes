#include "trie.hpp"
#include "catch.hpp"

/*
   Testy pro stage-01.

   Tyto testy po kouskách zkontrolují metody
     * trie() (základní konstruktor)
     * ~trie() (destruktor)
     * insert()
     * contains()
     * erase()
     * trie(std::vector<std::string> const &strings) (konstruktor z vektoru stringů)

   V dalších krocích se očekává, že vám metody z tohoto kroku fungují.
*/


namespace {
    void insert_all(trie& t, const std::vector<std::string>& strings) {
        for (const auto& string : strings) {
            t.insert(string);
        }
    }
}

TEST_CASE("Basics: inserts", "[stage1]") {
    trie trie;

    SECTION("Empty trie") {
        REQUIRE_FALSE(trie.contains(""));
        REQUIRE(trie.size() == 0);
        REQUIRE(trie.empty());
    }

    SECTION("Trivial inserts") {
        REQUIRE(trie.insert("abcd"));
        REQUIRE(trie.contains("abcd"));
        REQUIRE_FALSE(trie.contains(""));
        REQUIRE_FALSE(trie.contains("a"));
        REQUIRE_FALSE(trie.contains("ab"));
        REQUIRE_FALSE(trie.contains("abc"));
        REQUIRE(trie.size() == 1);

        REQUIRE(trie.insert("abc"));
        REQUIRE(trie.contains("abc"));
        REQUIRE(trie.size() == 2);

        REQUIRE_FALSE(trie.empty());
    }
}

TEST_CASE("Basics: erase", "[stage1]") {
    trie trie;

    SECTION("Trivial erase") {
        REQUIRE(trie.insert("a"));
        REQUIRE(trie.erase("a"));
        REQUIRE_FALSE(trie.contains("a"));
        REQUIRE(trie.size() == 0);
        REQUIRE(trie.empty());

        REQUIRE(trie.insert("bcd"));
        REQUIRE(trie.erase("bcd"));
        REQUIRE_FALSE(trie.contains("bcd"));
        REQUIRE(trie.size() == 0);
        REQUIRE(trie.empty());
    }

    SECTION("Erase in the middle of a link") {
        insert_all(trie, { "", "a", "ab", "abc", "abcd" });
        REQUIRE(trie.erase("ab"));
        REQUIRE(trie.size() == 4);
        REQUIRE_FALSE(trie.contains("ab"));

        REQUIRE(trie.erase("abc"));
        REQUIRE(trie.size() == 3);
        REQUIRE_FALSE(trie.contains("abc"));

        REQUIRE(trie.contains("abcd"));
    }
}

TEST_CASE("Complex: inserts", "[stage1]") {
    trie trie;

    SECTION("Repeated inserts do not reinsert given string") {
        REQUIRE(trie.insert("abcd"));
        REQUIRE_FALSE(trie.insert("abcd"));
        REQUIRE(trie.insert("abc"));
        REQUIRE_FALSE(trie.insert("abc"));

        REQUIRE(trie.contains("abcd"));
        REQUIRE(trie.contains("abc"));
        REQUIRE(trie.size() == 2);
    }
}

TEST_CASE("Complex: repeated interaction", "[stage1]") {
    trie trie;

    SECTION("Reinserting and reerasing the same element works properly") {
        insert_all(trie, { "a", "aa", "ab", "ac" });
        REQUIRE(trie.erase("a"));
        REQUIRE_FALSE(trie.contains("a"));
        REQUIRE(trie.insert("a"));
        REQUIRE(trie.contains("a"));
        REQUIRE(trie.erase("a"));
        REQUIRE_FALSE(trie.erase("a"));
        REQUIRE(trie.contains("aa"));
        REQUIRE(trie.contains("ab"));
        REQUIRE(trie.contains("ac"));

        REQUIRE(trie.size() == 3);
    }
}

TEST_CASE("Vector constructor", "[stage1]") {
    trie trie({ "abc", "bc", "a", "bc", "d", "", "d", "abcd", "abc" });
    REQUIRE(trie.size() == 6);
    REQUIRE(trie.contains(""));
    REQUIRE(trie.contains("a"));
    REQUIRE(trie.contains("d"));
    REQUIRE_FALSE(trie.contains("b"));
    REQUIRE(trie.contains("bc"));
    REQUIRE_FALSE(trie.contains("ab"));
    REQUIRE(trie.contains("abc"));
    REQUIRE(trie.contains("abcd"));
}
