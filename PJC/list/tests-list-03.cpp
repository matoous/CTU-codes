#include "catch.hpp"

#include "list.hpp"

#include "test-helpers.hpp"

/*
    Testy pro stage-03.

    Tento krok testuje
      * kopie
      * přesuny
      * operátory ==, !=
      * operátory <, <=, >, >=
      * kostruktor z vektoru ( list(const std::vector<double>& vec) )


    Testy v tomto kroku předpokládají, že metody testované v minulém
    kroku fungují správně, a tudíž se na nich mohou stavět testy pro další
    funkcionalitu.
*/


using pjc::list;


TEST_CASE("Operator==, !=", "[stage3]") {
    list l1, l2;
    SECTION("Empty lists are equal") {
        REQUIRE(l1 == l2);
        REQUIRE_FALSE(l1 != l2);
    }
    SECTION("Empty and non empty lists aren't equal") {
        l1.push_back(1.00);
        REQUIRE(l1 != l2);
        REQUIRE_FALSE(l1 == l2);
    }
    SECTION("Two lists with the same members") {
        append_to_list(l1, {1.22, 2.33, 3.44, 4.55});
        append_to_list(l2, {1.22, 2.33, 3.44, 4.55});
        REQUIRE(l1 == l2);
        REQUIRE_FALSE(l1 != l2);
    }
    SECTION("Two lists with different members") {
        append_to_list(l1, { 1.22, 3.33, 6.88 });
        append_to_list(l2, { 1.22, 3.33, 6.77 });
        REQUIRE(l1 != l2);
        REQUIRE_FALSE(l1 == l2);
    }
}

TEST_CASE("Operators <, <=, >, >=", "[stage3]") {
    SECTION("Empty lists are neither lesser nor greater") {
        list l1, l2;
        REQUIRE_FALSE(l1 < l2);
        REQUIRE_FALSE(l1 > l2);
        REQUIRE(l1 <= l2);
        REQUIRE(l1 >= l2);
    }
    SECTION("An empty list is smaller than any non-empty list") {
        list l1;
        list l2; append_to_list(l2, { 1, 2, 3 });
        REQUIRE(l1 < l2);
        REQUIRE(l1 <= l2);
        REQUIRE_FALSE(l1 >  l2);
        REQUIRE_FALSE(l1 >= l2);
    }
    SECTION("Equal lists are neither lesser nor greater") {
        list l1; append_to_list(l1, {1, 2, 3, 4, 5});
        list l2; append_to_list(l2, {1, 2, 3, 4, 5});
        REQUIRE_FALSE(l1 < l2);
        REQUIRE(l1 <= l2);
        REQUIRE_FALSE(l1 > l2);
        REQUIRE(l1 >= l2);
    }
    SECTION("List's prefix compares as smaller") {
        list l1; append_to_list(l1, { 1, 2, 3, 4 });
        list l2; append_to_list(l2, { 1, 2, 3, 4, 5 });
        REQUIRE(l1 <  l2);
        REQUIRE(l1 <= l2);
        REQUIRE_FALSE(l1 >  l2);
        REQUIRE_FALSE(l1 >= l2);
    }
    SECTION("Distinct lists") {
        list l1; append_to_list(l1, {1, 2, 3, 4, 5});
        list l2; append_to_list(l2, { 4, 3, 2, 1 });
        REQUIRE(l1 <  l2);
        REQUIRE(l1 <= l2);
        REQUIRE_FALSE(l1 >  l2);
        REQUIRE_FALSE(l1 >= l2);
    }
}

TEST_CASE("Copy constructor", "[stage3]") {
    list l1;
    SECTION("Copy of an empty list is empty, but usable") {
        CHECK(l1.empty());
        list l2(l1);
        REQUIRE(l2.empty());
        l2.push_back(1.44);
        REQUIRE_FALSE(l2.empty());
        REQUIRE(list_equal(l2, { 1.44 }));
        REQUIRE(l1.empty());
    }
    SECTION("Copy of a list is equal") {
        append_to_list(l1, {1.11, 2.22, 3.33, 4.44});
        list l2(l1);
        REQUIRE(l2 == l1);
    }
    SECTION("Modifying a copy doesn't modify the original") {
        append_to_list(l1, { 1.23, 4.56, 7.89 });
        list l2(l1);
        REQUIRE(l2 == l1);
        SECTION("Modify the old list") {
            l1.pop_back();
            CHECK(l1 != l2);
            REQUIRE(list_equal(l2, { 1.23, 4.56, 7.89 }));
        }
        SECTION("Modify the new list") {
            l2.push_back(1.11);
            CHECK(l1 != l2);
            REQUIRE(list_equal(l1, { 1.23, 4.56, 7.89 }));
        }
        SECTION("Modify both lists") {
            l1.push_back(1.23);
            l2.push_back(2.34);
            CHECK(l1 != l2);
            REQUIRE(list_equal(l1, { 1.23, 4.56, 7.89, 1.23 }));
            REQUIRE(list_equal(l2, { 1.23, 4.56, 7.89, 2.34 }));
        }
    }
}

TEST_CASE("Copy assignment", "[stage3]") {
    list l1; append_to_list(l1, { 1, 2, 3 });
    list l2; append_to_list(l2, { 4, 5, 6 });
    CHECK(l1 != l2);
    SECTION("Simple assignment") {
        l2 = l1;
        REQUIRE(l1 == l2);
        REQUIRE(list_equal(l2, { 1, 2, 3 }));
    }
    SECTION("Self assignment") {
        l2 = l2;
        REQUIRE(list_equal(l2, { 4, 5, 6 }));
    }
    SECTION("Copy-assigned lists are independent") {
        l2 = l1;
        l1.push_back(10);
        l2.pop_back();
        CHECK(l2 != l1);
        REQUIRE(list_equal(l1, { 1, 2, 3, 10 }));
        REQUIRE(list_equal(l2, { 1, 2 }));
    }
    SECTION("Repeated assignments") {
        l2 = l1;
        CHECK(l1 == l2);
        l1.push_back(4);
        l2 = l1;
        REQUIRE(l1 == l2);
        l1.push_back(5);
        l2 = l1;
        REQUIRE(l1 == l2);
        REQUIRE(list_equal(l2, { 1, 2, 3, 4, 5 }));
        l1.push_back(6);
        l1 = l2;
        REQUIRE(l1 == l2);
        REQUIRE(list_equal(l1, { 1, 2, 3, 4, 5 }));
    }
}

TEST_CASE("Move constructor", "[stage3]") {
    SECTION("Move constructor from empty list creates an empty list") {
        list l1;
        list l2(std::move(l1));
        REQUIRE(l2.empty());
        REQUIRE(l2.size() == 0);
    }
    SECTION("Move on a non-empty list") {
        list l1; append_to_list(l1, { 33, 22, 11, 44 });
        list l2(std::move(l1));
        REQUIRE(list_equal(l2, { 33, 22, 11, 44 }));
    }
    SECTION("List can be further used after being move-constructed") {
        list l1; append_to_list(l1, { 22, 33, 44, 11 });
        list l2(std::move(l1));
        l2.push_back(55);
        l2.push_front(11);
        REQUIRE(list_equal(l2, {11, 22, 33, 44, 11, 55}));
    }
}

TEST_CASE("Move assignment", "[stage3]") {
    SECTION("Move assignment from empty list creates an empty list") {
        list l1;
        list l2; append_to_list(l2, { 6, 7 ,8, 9 });
        l2 = std::move(l1);
        REQUIRE(l2.empty());
        REQUIRE(l2.size() == 0);
    }
    SECTION("Move assignment from a non-empty list moves the elements") {
        list l1; append_to_list(l1, { 1, 2, 3, 4 });
        list l2; append_to_list(l2, { 5, 6, 7, 8 });
        l2 = std::move(l1);
        REQUIRE(list_equal(l2, { 1, 2, 3, 4 }));
    }
    SECTION("List can be further used after being move-assigned-to") {
        list l1; append_to_list(l1, {8, 7, 6, 5});
        list l2; append_to_list(l2, {4, 3, 2, 1});
        l2 = std::move(l1);
        l2.push_back(22);
        l2.pop_front();
        REQUIRE(list_equal(l2, { 7, 6, 5, 22 }));
    }
}

TEST_CASE("vector constructor", "[stage3]") {
    SECTION("Empty vector leads to empty list") {
        list l(std::vector<double>{});
        REQUIRE(l.empty());
        REQUIRE(l.size() == 0);
    }
    SECTION("Empty list from empty vector can be used") {
        list l(std::vector<double>{});
        CHECK(l.empty());
        l.push_back(3);
        l.push_front(2);
        l.push_front(1);
        l.push_back(4);
        REQUIRE(list_equal(l, {1, 2, 3, 4}));
    }
    SECTION("Elements of non-empty vector are copied") {
        list l1(std::vector<double>{1, 2, 3, 4, 5});
        REQUIRE(list_equal(l1, {1, 2, 3, 4, 5}));
        SECTION("And the list can be used afterwards") {
            l1.pop_front();
            l1.push_back(2.3);
            REQUIRE(list_equal(l1, {2, 3, 4, 5, 2.3}));
        }
    }
}
