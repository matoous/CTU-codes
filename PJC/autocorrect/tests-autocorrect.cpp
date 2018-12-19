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
}


TEST_CASE("Base methods of autocorrect work") {
    autocorrect ac;
    REQUIRE(ac.size() == 0);

    SECTION("adding words to autocorrect works"){
        ac.add_word("one");
        ac.add_word("two");
        ac.add_word("three");
        REQUIRE(ac.size() == 3);
    }

    SECTION("adding duplicate words does nothing"){
        ac.add_word("one");
        ac.add_word("two");
        ac.add_word("three");
        ac.add_word("three");
        REQUIRE(ac.size() == 3);
    }

    SECTION("removing words works"){
        ac.add_word("one");
        ac.add_word("two");
        ac.add_word("three");
        ac.remove_word("three");
        REQUIRE(ac.size() == 2);
    }
}

TEST_CASE("Correction of word works") {
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

TEST_CASE("Autocorrect can be constructed from file of words") {
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
        REQUIRE(vector_contains(correction, "icefall"));

        correction = ac.correct("brocoli");
        REQUIRE(!correction.empty());
        REQUIRE(vector_contains(correction, "broccoli"));

        correction = ac.correct("thunderblt");
        REQUIRE(!correction.empty());
        REQUIRE(vector_contains(correction, "thunderbolt"));
    }
}

TEST_CASE("Autocorrect more words in one call") {
    std::fstream myfile;
    myfile.open("words.txt");
    REQUIRE(myfile.is_open());
    autocorrect ac(myfile);
    myfile.close();

    std::vector<std::string> misspelledWords = {
            "icefalll",
            "brocoli",
            "thunderblt",
    };
    auto corrections = ac.correct(misspelledWords);
    REQUIRE(corrections.size() == misspelledWords.size());

    REQUIRE(!corrections[0].empty());
    REQUIRE(vector_contains(corrections[0], "icefall"));

    REQUIRE(!corrections[1].empty());
    REQUIRE(vector_contains(corrections[1], "broccoli"));

    REQUIRE(!corrections[2].empty());
    REQUIRE(vector_contains(corrections[2], "thunderbolt"));
}

TEST_CASE("Autocorrect parallel correction implementation") {
    std::fstream myfile;
    myfile.open("words.txt");
    REQUIRE(myfile.is_open());
    autocorrect ac(myfile);
    myfile.close();

    std::vector<std::string> misspelledWords = {
            "icefalll",
            "brocoli",
            "thunderblt",
    };
    auto normCorrections = ac.correct(misspelledWords);
    auto corrections = ac.p_correct(misspelledWords);
    REQUIRE(corrections.size() == misspelledWords.size());
    REQUIRE(corrections.size() == normCorrections.size());
    REQUIRE(corrections == normCorrections);

    REQUIRE(!corrections[0].empty());
    REQUIRE(vector_contains(corrections[0], "icefall"));

    REQUIRE(!corrections[1].empty());
    REQUIRE(vector_contains(corrections[1], "broccoli"));

    REQUIRE(!corrections[2].empty());
    REQUIRE(vector_contains(corrections[2], "thunderbolt"));
}