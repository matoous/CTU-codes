#include "trie.hpp"
#include "test-helpers.hpp"
#include "catch.hpp"

/*
    Testy pro stage-04.

    Tento krok testuje
      * search_by_prefix
      * get_prefixes
      * sjednocení trií (operator|)
      * průnik trií (operator&)

    Testy v tomto kroku předpokládají, že metody testované v minulém
    kroku fungují správně, a tudíž se na nich mohou stavět testy pro další
    funkcionalitu.
*/


TEST_CASE("Search by prefix", "[search_by_prefix]") {
    trie trie;
    insert_all(trie, { "a", "aa", "aaa", "aabb", "aabab", "aaaab", "aaqqq" });
    SECTION("Search is inclusive") {
        VALIDATE_SETS(trie.search_by_prefix("aa"), as_vec({ "aa", "aaa", "aabb", "aabab", "aaaab", "aaqqq" }));
    }
    SECTION("Prefix not in the trie") {
        VALIDATE_SETS(trie.search_by_prefix(""), extract_all(trie));
    }
}

TEST_CASE("Get prefixes", "[get_prefixes]") {
    trie trie;
    insert_all(trie, { "a", "aa", "aaa", "aabb", "aabab", "aaaab", "aaqqq" });
    SECTION("Search is inclusive") {
        VALIDATE_SETS(trie.get_prefixes("aabab"), as_vec({ "aa", "a", "aabab" }));
    }
    SECTION("Input is not in the trie") {
        VALIDATE_SETS(trie.get_prefixes("aaaaa"), as_vec({ "a", "aa", "aaa" }));
    }
}

TEST_CASE("Trie union", "[union]") {
    trie t1, t2;
    SECTION("Empty tries") {
        auto res = t1 | t2;
        CHECK(res.empty());
        REQUIRE(res.size() == 0);
    }
    SECTION("One trie empty") {
        t1 = trie({ "aa", "aab", "aabb", "zzz" });
        SECTION("LHS trie empty") {
            auto res = t2 | t1;
            CHECK(res.size() == 4);
            REQUIRE(res == t1);
        }
        SECTION("RHS trie empty") {
            auto res = t1 | t2;
            CHECK(res.size() == 4);
            REQUIRE(res == t1);
        }
    }
    SECTION("No empty tries") {
        t1 = trie({ "queue", "quiz", "quizzical", "quilt" });
        SECTION("Identical tries") {
            auto res = t1 | t1;
            CHECK(res.size() == 4);
            REQUIRE(res == t1);
        }
        SECTION("Overlapping tries") {
            t2 = trie({ "quilt", "queue", "quitter" });
            auto res = t1 | t2;
            CHECK(res.size() == 5);
            REQUIRE(res == trie({ "queue", "quiz", "quizzical", "quilt", "quitter" }));
        }
        SECTION("Distinct tries") {
            t2 = trie({ "abra", "kadabra","abrakadabra" });
            auto res = t1 | t2;
            CHECK(res.size() == 7);
            REQUIRE(res == trie({ "queue", "quiz", "quizzical", "quilt", "abra", "kadabra", "abrakadabra" }));
        }
    }
    SECTION("Chained unions") {
        t1 = trie({ "aaa", "bbb", "ccc" });
        t2 = trie({ "bbb", "ccc", "ddd" });
        trie t3({ "eee", "fff", "ggg" });
        trie t4({ "ggg", "hhh", "iii" });
        auto res = t1 | t2 | t3 | t4;
        CHECK(res.size() == 9);
        REQUIRE(res == trie({ "aaa", "bbb", "ccc", "ddd", "eee", "fff", "ggg", "hhh" ,"iii" }));
    }
}

TEST_CASE("Trie intersection", "[intersection]") {
    trie t1, t2;
    SECTION("Empty tries") {
        auto res = t1 & t2;
        CHECK(res.empty());
        REQUIRE(res.size() == 0);
    }
    SECTION("One trie empty") {
        t1 = trie({ "aa", "aab", "aabb", "zzz" });
        SECTION("LHS trie empty") {
            auto res = t2 & t1;
            CHECK(res.size() == 0);
            REQUIRE(res == t2);
        }
        SECTION("RHS trie empty") {
            auto res = t1 & t2;
            CHECK(res.size() == 0);
            REQUIRE(res == t2);
        }
    }
    SECTION("No empty tries") {
        t1 = trie({ "queue", "quiz", "quizzical", "quilt" });
        SECTION("Identical tries") {
            auto res = t1 & t1;
            CHECK(res.size() == 4);
            REQUIRE(res == t1);
        }
        SECTION("Overlapping tries") {
            t2 = trie({ "quilt", "queue", "quitter" });
            auto res = t1 & t2;
            CHECK(res.size() == 2);
            REQUIRE(res == trie({ "queue", "quilt" }));
        }
        SECTION("Distinct tries") {
            t2 = trie({ "abra", "kadabra","abrakadabra" });
            auto res = t1 & t2;
            CHECK(res.size() == 0);
            REQUIRE(res == trie{});
        }
    }
    SECTION("Chained intersections") {
        t1 = trie({ "aaa", "bbb", "ccc", "ddd" });
        t2 = trie({ "bbb", "ccc", "ddd", "eee" });
        trie t3({ "ccc", "ddd", "eee", "fff" });
        trie t4({ "ddd", "eee", "fff", "ggg" });
        auto res = t1 & t2 & t3 & t4;
        CHECK(res.size() == 1);
        REQUIRE(res == trie({ "ddd" }));
    }
}
