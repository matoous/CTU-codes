#include "trie.hpp"
#include "test-helpers.hpp"
#include "catch.hpp"

/*
    Testy pro stage-03.

    Tento krok testuje
      * kopie
      * přesuny
      * swap
      * operátory ==, !=
      * operátory <, <=, >, >=

    Testy v tomto kroku předpokládají, že metody testované v minulém
    kroku fungují správně, a tudíž se na nich mohou stavět testy pro další
    funkcionalitu.
*/


TEST_CASE("Copy", "[stage3]") {
    SECTION("Copy constructor") {
        SECTION("Verify that deep copy is performed") {
            trie t1({ "a", "aa", "ab", "ad", "cd", "c", "q" });
            trie t2(t1);
            SECTION("Add to old trie") {
                t1.insert("xXxXx");
                VALIDATE_SETS(extract_all(t1), as_vec({ "a", "aa", "ab", "ad", "cd", "c", "q", "xXxXx" }));
                VALIDATE_SETS(extract_all(t2), as_vec({ "a", "aa", "ab", "ad", "cd", "c", "q" }));
            }
            SECTION("Add to new trie") {
                t2.insert("xXxXx");
                VALIDATE_SETS(extract_all(t1), as_vec({ "a", "aa", "ab", "ad", "cd", "c", "q" }));
                VALIDATE_SETS(extract_all(t2), as_vec({ "a", "aa", "ab", "ad", "cd", "c", "q", "xXxXx" }));
            }
            SECTION("Add to both tries") {
                t1.insert("xXxXx");
                t2.insert("YYY");
                VALIDATE_SETS(extract_all(t1), as_vec({ "a", "aa", "ab", "ad", "cd", "c", "q", "xXxXx" }));
                VALIDATE_SETS(extract_all(t2), as_vec({ "a", "aa", "ab", "ad", "cd", "c", "q", "YYY" }));
            }
        }
        SECTION("Copying an empty trie must work properly") {
            trie t1;
            trie t2(t1);
            REQUIRE(t1.empty());
            REQUIRE(t2.empty());
        }
    }
    SECTION("Copy assignment") {
        trie t1({ "a", "b", "c", "d" });
        trie t2({ "A", "B", "C", "D" });
        SECTION("Simple assignment") {
            t1 = t2;
            VALIDATE_SETS(extract_all(t1), as_vec({ "A", "B", "C", "D" }));
            VALIDATE_SETS(extract_all(t2), as_vec({ "A", "B", "C", "D" }));
        }
        SECTION("Self assignment") {
            t1 = t1;
            VALIDATE_SETS(extract_all(t1), as_vec({ "a", "b", "c", "d" }));
            REQUIRE(t1.size() == 4);
        }
        SECTION("Repeated assignments") {
            t2 = t1;
            VALIDATE_SETS(extract_all(t2), as_vec({ "a", "b", "c", "d" }));
            t1.insert("A");
            t2 = t1;
            VALIDATE_SETS(extract_all(t2), as_vec({ "a", "b", "c", "d", "A" }));
            t1.insert("B");
            t2 = t1;
            VALIDATE_SETS(extract_all(t2), as_vec({ "a", "b", "c", "d", "A", "B" }));
            t1.insert("C");
            t2 = t1;
            VALIDATE_SETS(extract_all(t2), as_vec({ "a", "b", "c", "d", "A", "B", "C" }));
            t1.insert("D");
            t2 = t1;
            VALIDATE_SETS(extract_all(t2), as_vec({ "a", "b", "c", "d", "A", "B", "C", "D" }));
        }
    }
}

TEST_CASE("Move operations", "[stage3]") {
    SECTION("Move constructor") {
        SECTION("From an empty trie") {
            trie t1;
            trie t2(std::move(t1));
            REQUIRE(t2.empty());
        }
        SECTION("From non-empty trie") {
            trie t1({ "a", "b", "c", "d", "e" });
            trie t2(std::move(t1));
            VALIDATE_SETS(extract_all(t2), as_vec({ "a", "b", "c", "d", "e" }));
        }
    }
    SECTION("Move assignment") {
        SECTION("From empty trie") {
            trie t1;
            SECTION("To empty trie") {
                trie t2;
                t2 = std::move(t1);
                REQUIRE(t2.empty());
            }
            SECTION("To non-empty trie") {
                trie t2({ "a", "b", "c", "d", "e" });
                t2 = std::move(t1);
                REQUIRE(t2.empty());
            }
        }
        SECTION("From non-empty trie") {
            trie t1({ "ab", "bc", "deg" });
            SECTION("To empty trie") {
                trie t2;
                t2 = std::move(t1);
                VALIDATE_SETS(extract_all(t2), as_vec({ "ab", "bc", "deg" }));
            }
            SECTION("To non-empty trie") {
                trie t2({ "kfa", "bfg", "id" });
                t2 = std::move(t1);
                VALIDATE_SETS(extract_all(t2), as_vec({ "ab", "bc", "deg" }));
            }
        }
        SECTION("Back and forth") {
            trie t1({ "abc", "def", "eee" });
            trie t2({ "qwertz", "qwerty", "azerty" });
            t1 = std::move(t2);
            VALIDATE_SETS(extract_all(t1), as_vec({ "qwertz", "qwerty", "azerty" }));

            t2 = std::move(t1);
            VALIDATE_SETS(extract_all(t2), as_vec({ "qwertz", "qwerty", "azerty" }));

            SECTION("Trie can be used after being move assigned-to") {
                REQUIRE(t2.erase("azerty"));
                REQUIRE(t2.insert("qzerty"));
                VALIDATE_SETS(extract_all(t2), as_vec({ "qwertz", "qwerty", "qzerty" }));
            }
        }
    }
}

TEST_CASE("member swap", "[stage3]") {
    SECTION("Swapping empty tries leaves both empty") {
        trie t1, t2;
        t1.swap(t2);
        REQUIRE(t1.empty());
        REQUIRE(t1.empty());
    }
    SECTION("Swapping tries actually swaps elements") {
        trie t1({ "azerty", "qwerty", "qwertz" });
        SECTION("Non-empty and empty trie") {
            trie t2;
            t1.swap(t2);
            REQUIRE(t1.empty());
            VALIDATE_SETS(extract_all(t2), as_vec({ "azerty", "qwerty", "qwertz" }));

            SECTION("Roundtrip") {
                t2.swap(t1);
                REQUIRE(t2.empty());
                VALIDATE_SETS(extract_all(t1), as_vec({ "azerty", "qwerty", "qwertz" }));
            }
        }
        SECTION("Non-empty and non-empty trie") {
            trie t2({ "qzerty", "querty", "awerty" });
            t1.swap(t2);
            VALIDATE_SETS(extract_all(t1), as_vec({ "qzerty", "querty", "awerty" }));
            VALIDATE_SETS(extract_all(t2), as_vec({ "azerty", "qwerty", "qwertz" }));

            SECTION("Roundtrip") {
                t1.swap(t2);
                VALIDATE_SETS(extract_all(t1), as_vec({ "azerty", "qwerty", "qwertz" }));
                VALIDATE_SETS(extract_all(t2), as_vec({ "qzerty", "querty", "awerty" }));
            }
        }
    }
}

TEST_CASE("Operator==, !=", "[stage3]") {
    trie t1, t2;
    SECTION("Empty tries are equal") {
        REQUIRE(t1 == t2);
        REQUIRE_FALSE(t1 != t2);
    }
    SECTION("Empty and non empty tries aren't equal") {
        t1.insert("a");
        REQUIRE(t1 != t2);
        REQUIRE_FALSE(t1 == t2);
    }
    SECTION("Two tries with the same members") {
        insert_all(t1, {"aa", "bc", "efg", "xkcd"});
        insert_all(t2, {"efg", "xkcd", "aa", "bc"});
        REQUIRE(t1 == t2);
        REQUIRE_FALSE(t1 != t2);
    }
    SECTION("Two lists with different members") {
        trie t1({ "aa", "bc", "efg", "efgh"});
        trie t2({ "aa", "bcd", "efg"});
        REQUIRE(t1 != t2);
        REQUIRE_FALSE(t1 == t2);
    }
}

TEST_CASE("Operators <, <=, >, >=", "[stage3]") {
    SECTION("Empty tries are neither lesser nor greater") {
        trie t1, t2;
        REQUIRE_FALSE(t1 < t2);
        REQUIRE_FALSE(t1 > t2);
        REQUIRE(t1 <= t2);
        REQUIRE(t1 >= t2);
    }
    SECTION("An empty trie is smaller than any non-empty trie") {
        trie t1;
        trie t2({ "a", "b", "abc" });
        REQUIRE(t1 <  t2);
        REQUIRE(t1 <= t2);
        REQUIRE_FALSE(t1 >  t2);
        REQUIRE_FALSE(t1 >= t2);
    }
    SECTION("Equal tries are neither lesser nor greater") {
        trie t1({"aa", "bc", "efg", "xkcd"});
        trie t2({"efg", "xkcd", "aa", "bc"});
        REQUIRE_FALSE(t1 < t2);
        REQUIRE(t1 <= t2);
        REQUIRE_FALSE(t1 > t2);
        REQUIRE(t1 >= t2);
    }
    SECTION("Trie's prefix compares as smaller") {
        trie t1({"aa", "bc", "efg"});
        trie t2({"aa", "bc", "efg", "efgh"});
        REQUIRE(t1 <  t2);
        REQUIRE(t1 <= t2);
        REQUIRE_FALSE(t1 >  t2);
        REQUIRE_FALSE(t1 >= t2);
    }
    SECTION("Distinct tries") {
        trie t1({ "aa", "bc", "efg", "efgh"});
        trie t2({ "aa", "bcd", "efg"});
        REQUIRE(t1 <  t2);
        REQUIRE(t1 <= t2);
        REQUIRE_FALSE(t1 >  t2);
        REQUIRE_FALSE(t1 >= t2);
    }
}
