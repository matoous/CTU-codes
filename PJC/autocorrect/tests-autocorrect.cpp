#include "catch.hpp"

#include "autocorrect.hpp"

#include <vector>
#include <algorithm>
#include <sstream>
#include <fstream>

using pjc::autocorrect;


TEST_CASE("Base methods of autocorrect work", "[stage1]") {
    autocorrect ac;
    REQUIRE(ac.size() == 0);

    SECTION("adding words to autocorrect works"){
        ac.add_word("one");
        ac.add_word("two");
        ac.add_word("three");
        REQUIRE(ac.size() == 3);
    }

    SECTION("adding duplicate words does nothing"){
        ac.add_word("three");
        REQUIRE(ac.size() == 3);
    }

    SECTION("removing words works"){
        ac.remove_word("three");
        REQUIRE(ac.size() == 2);
    }
}

TEST_CASE("Correction of word works", "[stage2]") {
    autocorrect ac;
    ac.add_word("one");
    ac.add_word("two");
    ac.add_word("three");

    SECTION("autocorrect suggest at least something"){
        auto correction = ac.correct("threa");
        REQUIRE(!correction.empty());
    }

    SECTION("autoccorect returns empty suggestions if the word is in the dictionary"){
        auto correction = ac.correct("three");
        REQUIRE(correction.empty());
    }
}

TEST_CASE("Autocorrect can be constructed from file of words", "[stage3]") {

}
