#include "catch.hpp"

#include "list.hpp"

#include <vector>
#include <algorithm>

/*
   Testy pro stage-01.

   Tyto testy po kouskách zkontrolují metody
     * list() (základní konstruktor)
     * ~list() (destruktor)
     * push_back()
     * back()
     * pop_back()
     * push_front()
     * front()
     * pop_front()

   Kvůli omezení se na tuto sadu metod vám tyto testy neodhalí všechny
   chyby, ale měly by jich najít většinu. V dalších krocích se očekává,
   že vám metody z tohoto kroku fungují.
*/


using pjc::list;


namespace {
    void check_empty(list const& l) {
        REQUIRE(l.empty());
        REQUIRE(l.size() == 0);
    }

    enum class check_direction {
        forward,
        backward
    };

    // Kvůli omezení na API pro tento stage je kontrola destruktivní...
    void check_list_equal(list& l, std::vector<double> numbers,
                          check_direction direction) {
        REQUIRE(l.size() == numbers.size());

        if (direction == check_direction::forward) {
            for (auto const& number : numbers) {
                REQUIRE(l.front() == number);
                l.pop_front();
            }
        } else {
            std::reverse(numbers.begin(), numbers.end());
            for (auto const& number : numbers) {
                REQUIRE(l.back() == number);
                l.pop_back();
            }
        }
        REQUIRE(l.empty());
    }
}

TEST_CASE("Default constructed list passes basic accessors", "[stage1]") {
    list l;
    REQUIRE(l.empty());
    REQUIRE(l.size() == 0);
}

TEST_CASE("Trivial use of push_back", "[stage1]") {
    list l;
    l.push_back(1);
    REQUIRE(l.back() == 1);
    REQUIRE(l.size() == 1);
    REQUIRE_FALSE(l.empty());

    l.push_back(3);
    REQUIRE(l.back() == 3);
    REQUIRE(l.size() == 2);

    l.push_back(7.234);
    REQUIRE(l.back() == 7.234);
    REQUIRE(l.size() == 3);
}

TEST_CASE("push_backs and pop_backs", "[stage1]") {
    list l;
    l.push_back(33);
    l.push_back(22);
    l.push_back(11);
    CHECK(l.size() == 3);

    l.pop_back();
    REQUIRE(l.size() == 2);
    REQUIRE(l.back() == 22);

    l.pop_back();
    REQUIRE(l.size() == 1);
    REQUIRE(l.back() == 33);

    l.pop_back();
    REQUIRE(l.size() == 0);
    REQUIRE(l.empty());
}

TEST_CASE("Interspersed push_backs and pop_backs", "[stage1]") {
    list l;
    l.push_back(123);
    l.push_back(132);
    l.push_back(213);
    l.pop_back();
    l.push_back(231);
    CHECK(l.size() == 3);
    CHECK_FALSE(l.empty());
    REQUIRE(l.back() == 231);

    l.push_back(312);
    REQUIRE(l.size() == 4);
    REQUIRE(l.back() == 312);

    l.pop_back();
    l.pop_back();

    l.push_back(321);
    REQUIRE(l.size() == 3);
    REQUIRE(l.back() == 321);
    REQUIRE_FALSE(l.empty());
}

TEST_CASE("Trivial use of push_front", "[stage1]") {
    list l;
    l.push_front(12);
    REQUIRE(l.front() == 12);
    REQUIRE(l.size() == 1);
    REQUIRE_FALSE(l.empty());

    l.push_front(22.2);
    REQUIRE(l.front() == 22.2);
    REQUIRE(l.size() == 2);

    l.push_front(11.1);
    REQUIRE(l.front() == 11.1);
    REQUIRE(l.size() == 3);
}

TEST_CASE("push_fronts and pop_fronts", "[stage1]") {
    list l;
    l.push_front(39);
    l.push_front(29);
    l.push_front(19);
    CHECK(l.size() == 3);

    l.pop_front();
    REQUIRE(l.size() == 2);
    REQUIRE(l.front() == 29);

    l.pop_front();
    REQUIRE(l.size() == 1);
    REQUIRE(l.front() == 39);

    l.pop_front();
    REQUIRE(l.size() == 0);
    REQUIRE(l.empty());
}

TEST_CASE("Interspersed push_fronts and pop_fronts", "[stage1]") {
    list l;
    l.push_front(123);
    l.push_front(132);
    l.push_front(213);
    l.pop_front();
    l.push_front(231);
    CHECK(l.size() == 3);
    CHECK_FALSE(l.empty());
    REQUIRE(l.front() == 231);

    l.push_front(312);
    REQUIRE(l.size() == 4);
    REQUIRE(l.front() == 312);

    l.pop_front();
    l.pop_front();

    l.push_front(321);
    REQUIRE(l.size() == 3);
    REQUIRE(l.front() == 321);
    REQUIRE_FALSE(l.empty());
}

TEST_CASE("More complex test cases", "[stage1]") {
    list l;
    SECTION("Series of push_back and pop_front") {
        for (int i = 0; i < 30; i += 4) {
            l.push_back(i);
            REQUIRE(l.back() == i);
            REQUIRE(l.front() == i);
            REQUIRE(l.size() == 1);
            l.pop_front();
            check_empty(l);
        }
    }
    SECTION("Series of push_front and pop_back") {
        for (int i = 0; i < 30; i += 4) {
            l.push_front(i);
            REQUIRE(l.back() == i);
            REQUIRE(l.front() == i);
            REQUIRE(l.size() == 1);
            l.pop_back();
            check_empty(l);
        }
    }
    SECTION("Complex usage examples") {
        check_direction direction = GENERATE(check_direction::forward, check_direction::backward);
        SECTION("#1") {
            for (int i : {0, 2, 4, 6, 8}) {
                l.push_back(i);
            }
            REQUIRE(l.size() == 5);
            l.pop_back();
            l.pop_front();
            REQUIRE(l.size() == 3);
            for (int i : {1, 3, 0}) {
                l.push_front(i);
            }
            check_list_equal(l, { 0, 3, 1, 2, 4, 6 }, direction);
        }
        SECTION("#2") {
            for (int i : {0, 2, 4}) {
                l.push_back(i);
                l.push_front(i);
            }
            l.pop_front();
            l.pop_back();
            l.pop_back();
            REQUIRE(l.size() == 3);
            REQUIRE(l.front() == 2);
            REQUIRE(l.back() == 0);
            for (int i : {1, 2, 3}) {
                l.push_front(i);
                l.push_back(i);
            }
            check_list_equal(l, { 3, 2, 1, 2, 0, 0, 1, 2, 3 }, direction);
        }
        SECTION("#3") {
            for (int i : {1, 2, 3, 4}) {
                l.push_back(i);
            }
            for (int i = 0; i < 2; ++i) {
                l.pop_back();
                l.pop_front();
            }
            check_empty(l);

            for (int i : {1, 2, 3, 4}) {
                l.push_back(i);
            }
            l.pop_back();
            l.pop_back();
            for (int i : {5, 6}) {
                l.push_back(i);
            }
            l.pop_front();
            l.pop_front();
            l.pop_front();
            REQUIRE(l.size() == 1);
            REQUIRE_FALSE(l.empty());
            for (int i : {7, 8}) {
                l.push_front(i);
            }
            check_list_equal(l, { 8, 7, 6 }, direction);
        }
    }
}
