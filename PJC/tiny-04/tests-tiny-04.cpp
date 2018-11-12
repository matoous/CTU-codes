#include "tiny-04.hpp"

#include "catch.hpp"

#include <vector>

namespace {
    void add(BST& b, std::vector<double> elems) {
        for (auto elem : elems) {
            b.add(elem);
        }
    }
    std::vector<double> vec(std::vector<double> elems) {
        return elems;
    }
}

TEST_CASE("Basic usage", "[all]") {
    BST b;
    REQUIRE_FALSE(b.contains(0));
    REQUIRE_FALSE(b.contains(1));
    REQUIRE_FALSE(b.contains(2));
    b.add(1);
    REQUIRE(b.contains(1));
    b.add(2);
    REQUIRE(b.contains(2));
    b.add(0);
    REQUIRE(b.contains(0));
}

TEST_CASE("Preorder traversal", "[all][traversal]") {
    BST b;
    add(b, {3, 1, 2, 4});
    REQUIRE(b.preorder_elements() == vec({3, 1, 2, 4}));

    BST b2;
    add(b2, {4, 2, 3, 1});
    REQUIRE(b2.preorder_elements() == vec({4, 2, 1, 3}));

    BST b3;
    add(b3, {4, 5, 7, 8, 6});
    REQUIRE(b3.preorder_elements() == vec({ 4, 5, 7, 6, 8 }));
}

TEST_CASE("Copies", "[all][copies][special]") {
    BST b1; add(b1, { 4, 2, 3, 1 });
    CHECK(b1.preorder_elements() == vec({ 4, 2, 1, 3 }));
    BST b2(b1);
    REQUIRE(b2.preorder_elements() == vec({ 4, 2, 1, 3 }));
    SECTION("Check that b2 and b1 are disconnected") {
        b2.add(5);
        REQUIRE(b2.preorder_elements() == vec({ 4, 2, 1, 3, 5 }));
        REQUIRE(b1.preorder_elements() == vec({ 4, 2, 1, 3 }));
    }
    BST b3; add(b3, { 1, 1, 1, 3, 3, 4, 4, 5 });
    b3 = b1;
    REQUIRE(b3.preorder_elements() == vec({ 4, 2, 1, 3 }));
    SECTION("Check that b3 and b1 are disconnected") {
        b3.add(0);
        REQUIRE(b3.preorder_elements() == vec({4, 2, 1, 0, 3}));
        REQUIRE(b1.preorder_elements() == vec({4, 2, 1, 3}));
    }
}

TEST_CASE("Moves", "[all][copies][special]") {
    BST b1; add(b1, {3, 0, 2, 3, 4});
    SECTION("Move constructor") {
        BST b2(std::move(b1));
        REQUIRE(b2.preorder_elements() == vec({ 3, 0, 2, 4 }));
        SECTION("b2 must be usable") {
            b2.add(33);
            CHECK(b2.contains(33));
            REQUIRE(b2.preorder_elements() == vec({ 3, 0, 2, 4, 33 }));
        }
    }
    SECTION("Move assignment") {
        BST b3; add(b3, { 1, 2, 3, 4, 5 });
        b3 = std::move(b1);
        REQUIRE(b3.preorder_elements() == vec({ 3, 0, 2, 4 }));
        SECTION("b3 must be usable") {
            b3.add(-2);
            CHECK(b3.contains(-2));
            REQUIRE(b3.preorder_elements() == vec({3, 0, -2, 2, 4}));
        }
    }
}

