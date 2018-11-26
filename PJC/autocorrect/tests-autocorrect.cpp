#include "catch.hpp"

#include "autocorrect.hpp"
#include "test-helpers.hpp"

#include <vector>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iostream>

using pjc::autocorrect;

TEST_CASE("Levahnstein distance implementation"){
    REQUIRE(pjc::dist("broccoli", "brocoli") == 1);
    REQUIRE(pjc::dist("broccoli", "broccoli") == 0);
    REQUIRE(pjc::dist("abcd", "efgh") == 4);

    SECTION("levahnstein distance function handles empty words correctly"){
        REQUIRE(pjc::dist("", "") == 0);
        REQUIRE(pjc::dist("abcd", "") == 4);
    }

    SECTION("order doesn't matter when calculating levahnstein distance"){
        REQUIRE(pjc::dist("broccoli", "brooli") == pjc::dist("brooli", "broccoli"));
    }

//    SECTION("levahnstein distance can handle UTF-8 characters as well"){
//        REQUIRE(pjc::dist("čaj", "caj") == 1);
//    }
}


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
    std::fstream myfile;
    myfile.open("words.txt");
    REQUIRE(myfile.is_open());
    autocorrect ac(myfile);
    myfile.close();

    SECTION("autocorrect loaded dictionary file correctly") {
        REQUIRE(ac.size() == lines_in_file("words.txt"));
    }

    SECTION("autocorrecy corrects some common english words") {
        auto correction = ac.correct("icefalll");
        REQUIRE(!correction.empty());
        REQUIRE(correction[0] == "icefall");

        correction = ac.correct("brocoli");
        REQUIRE(!correction.empty());
        REQUIRE(correction[0] == "broccoli");

        correction = ac.correct("thunderblt");
        REQUIRE(!correction.empty());
        REQUIRE(correction[0] == "thunderbolt");
    }
}
