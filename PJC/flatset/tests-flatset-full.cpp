#include "flatset.hpp"
#include "tests-helpers.hpp"
#include "catch.hpp"

#include <forward_list>
#include <type_traits>
#include <algorithm>
#include <vector>
#include <iterator>
#include <random>
#include <climits>
#include <type_traits>
#include <cmath>
#include <functional>
#include <numeric>

namespace {
    template <typename T>
    bool reports_as_empty(flat_set<T> const& fs) {
        return fs.empty() && fs.size() == 0;
    }

    class MyKey {
        friend struct MyKeyComparator;
        int i;
        static unsigned s_nconstructs;
        operator int() const {return i;}
    public:
        static unsigned nconstructs() {return s_nconstructs;}

        explicit MyKey(int i = 0) : i(i) {s_nconstructs++;}
        MyKey(const MyKey &o) : i(o.i) {s_nconstructs++;}
        MyKey& operator=(const MyKey &o) {i = o.i; s_nconstructs++; return *this;}
        MyKey(MyKey &&o) = default;
        MyKey& operator=(MyKey &&o) = default;
    };
    struct MyKeyComparator {
        bool operator()(MyKey const &a, MyKey const &b) const {
            return a.i < b.i;
        }
    };
    template <typename C, typename T, typename U>
    bool equals(T &&a, U &&b, C &&cmp = C()) {
        return !cmp(a, b) && !cmp(b, a);
    }

    unsigned MyKey::s_nconstructs = 0;
}

/**
 * All of the tests for basic functionality start here
 */

TEST_CASE("Basic functionality -- constructors, insert, erase, size, empty, etc", "[basics]") {
    SECTION("Range constructor") {
        SECTION("Empty range -> empty set") {
            std::vector<int> v;
            REQUIRE(reports_as_empty(flat_set<int>(v.begin(), v.end())));
        }
        SECTION("Singular range -> singular set") {
            auto fs2 = make_flat_set<int>({1});
            REQUIRE(!fs2.empty());
            REQUIRE(fs2.size() == 1);
            REQUIRE(*fs2.begin() == 1);
        }
        SECTION("Multiple elements in order") {
            std::vector<int> elems({ 1, 2, 3, 56, 78, 86 });
            flat_set<int> s3(elems.begin(), elems.end());
            REQUIRE(!s3.empty());
            REQUIRE(s3.size() == 6);
            REQUIRE(set_equal(s3, elems));
        }
        SECTION("Multiple elements out of order") {
            std::vector<int> elems({ 1, 86, 2, 56, 78, 3 });
            flat_set<int> s3(elems.begin(), elems.end());
            REQUIRE(!s3.empty());
            REQUIRE(s3.size() == 6);
            REQUIRE(set_equal(s3, {1,2,3,56,78,86}));
        }
        SECTION("Multiple elements with duplicates") {
            std::vector<int> elems({ 1, 2, 3, 56, 2, 78, 78, 86, 1 });
            flat_set<int> s3(elems.begin(), elems.end());
            REQUIRE(!s3.empty());
            REQUIRE(s3.size() == 6);
            REQUIRE(set_equal(s3, {1,2,3,56,78,86}));
        }
    }
    SECTION("Series of inserts in order") {
        flat_set<int> fs;
        for (int i : {-9, 0, 2000, 543210123}) {
            fs.insert(i);
            REQUIRE(*(--fs.end()) == i);
        }
        REQUIRE(fs.size() == 4);
        REQUIRE(set_equal(fs, { -9, 0, 2000, 543210123 }));
    }
    SECTION("Series of inserts out of order") {
        flat_set<int> fs;
        std::vector<int> elems = {5, 0, 1, 2, 9};
        for (int i : elems) {
            fs.insert(i);
            REQUIRE(std::is_sorted(fs.begin(), fs.end()));
        }
        REQUIRE(fs.size() == 5);
        REQUIRE(set_equal(fs, { 0, 1, 2, 5, 9 }));
    }
    SECTION("Range insert") {
        flat_set<int> fs;
        std::vector<int> elems = {5, 0, 0, 1, 9, 2};
        fs.insert(elems.begin(), elems.end());
        REQUIRE(fs.size() == 5);
        REQUIRE(set_equal(fs, { 0, 1, 2, 5, 9 }));
    }
    SECTION("insert return value") {
        flat_set<int> fs;
        std::pair<flat_set<int>::iterator, bool> ret;
        ret = fs.insert(1);
        REQUIRE(ret.second == true);
        REQUIRE(ret.first == fs.begin());
        ret = fs.insert(2);
        REQUIRE(ret.second == true);
        REQUIRE(ret.first == ++fs.begin());
        ret = fs.insert(1);
        REQUIRE(ret.second == false);
        REQUIRE(ret.first == fs.begin());
    }
    SECTION("erase") {
        flat_set<int> fs;
        std::vector<int> elems = {0,1,2,5,9};
        flat_set<int>::iterator i;
        flat_set<int>::size_type ret;
        fs.insert(elems.begin(), elems.end());
        REQUIRE(set_equal(fs, {0,1,2,5,9}));

        i = fs.erase(fs.begin());
        REQUIRE(i == fs.begin());
        REQUIRE(set_equal(fs, {1,2,5,9}));

        i = fs.find(5);
        i = fs.erase(i);
        REQUIRE(*i == 9);
        REQUIRE(set_equal(fs, {1,2,9}));

        ret = fs.erase(123);
        REQUIRE(ret == 0);
        ret = fs.erase(2);
        REQUIRE(ret == 1);
        REQUIRE(set_equal(fs, { 1, 9 }));

        i = fs.erase(fs.begin(), fs.end());
        REQUIRE(reports_as_empty(fs));
        REQUIRE(i == fs.end());
    }
    SECTION("find") {
        auto fs2 = make_flat_set<int>({5, 0, 0, 1, 9, 2});
        REQUIRE(fs2.find(0) == fs2.begin()+0);
        REQUIRE(fs2.find(9) == fs2.begin()+4);
        REQUIRE(fs2.find(42) == fs2.end()); // greater than max
        REQUIRE(fs2.find(-1) == fs2.end()); // lesser than min
        REQUIRE(fs2.find(3) == fs2.end());  // in range, but not present
    }
    SECTION("find in empty set") {
        flat_set<int> fs2;
        REQUIRE(fs2.find(42) == fs2.end()); // should not crash on *0
    }
    SECTION("lower_bound") {
        auto fs2 = make_flat_set<int>({5, 0, 0, 1, 9, 2});
        REQUIRE(fs2.lower_bound(0) == fs2.begin());
        REQUIRE(fs2.lower_bound(9) == fs2.begin()+4);
        REQUIRE(fs2.lower_bound(42) == fs2.end()); // greater than max
        REQUIRE(fs2.lower_bound(-1) == fs2.begin()); // lesser than min
        REQUIRE(fs2.lower_bound(3) == fs2.begin()+3);  // in range, but not present
    }
    SECTION("upper_bound") {
        auto fs2 = make_flat_set<int>({ 5, 0, 0, 1, 9, 2 });
        REQUIRE(fs2.upper_bound(0) == fs2.begin() + 1);
        REQUIRE(fs2.upper_bound(9) == fs2.end());
        REQUIRE(fs2.lower_bound(42) == fs2.end()); // greater than max
        REQUIRE(fs2.upper_bound(-1) == fs2.begin()); // lesser than min
        REQUIRE(fs2.upper_bound(3) == fs2.begin() + 3);  // in range, but not present
    }
    SECTION("size, clear, capacity, reserve") {
        flat_set<int> fs2;
        REQUIRE(fs2.size() == 0);

        fs2.insert(42);
        REQUIRE(fs2.size() == 1);
        REQUIRE(fs2.capacity() >= fs2.size());

        fs2.clear();
        REQUIRE(fs2.size() == 0);

        fs2.reserve(1024);
        REQUIRE(fs2.capacity() >= 1024);
    }
}

TEST_CASE("Iterators", "[iterators]") {
    SECTION("Empty flat_set's iterators should be equal.") {
        flat_set<int> l;
        REQUIRE(l.begin() == l.end());
    }
    SECTION("Non empty flat_set") {
        auto l1 = make_flat_set<int>({ 1, 2, 3 });
        SECTION("Moving through the flat_set") {
            auto sit = l1.begin();
            auto eit = l1.end();
            REQUIRE(sit != eit);
            SECTION("Forward") {
                ++sit;
                REQUIRE(sit != eit);
                ++sit;
                REQUIRE(sit != eit);
                ++sit;
                REQUIRE(sit == eit);
            }
            SECTION("Backward") {
                --eit;
                REQUIRE(sit != eit);
                --eit;
                REQUIRE(sit != eit);
                --eit;
                REQUIRE(sit == eit);
            }
            SECTION("Forward - postincr") {
                REQUIRE(sit++ != eit);
                REQUIRE(sit++ != eit);
                REQUIRE(sit++ != eit);
                REQUIRE(sit   == eit);
            }
            SECTION("Backward - postdecr") {
                REQUIRE(sit != eit--);
                REQUIRE(sit != eit--);
                REQUIRE(sit != eit--);
                REQUIRE(sit == eit);
            }
        }
        SECTION("Dereferencing iterators") {
            auto sit = l1.begin();
            for (int i = 1; i <= 3; ++i) {
                REQUIRE(*sit == i);
                ++sit;
            }
            REQUIRE(sit == l1.end());
        }
    }
    SECTION("Constructors") {
        SECTION("Default inited iterators should be equal") {
            flat_set<int>::iterator it, it2;
            REQUIRE(it == it2);
        }
        SECTION("Iterator's copy is equal") {
            flat_set<int> l = make_flat_set<int>({ 1, 2, 3 });
            auto it = l.begin();
            auto it2(it);
            REQUIRE(it2 == it);
            REQUIRE(*it2 == *it);
            SECTION("And can be used to iterate") {
                for (int i = 1; i <= 3; ++i) {
                    REQUIRE(*it2 == i);
                    ++it2;
                }
                REQUIRE(it2 == l.end());
            }
            SECTION("And so is copy assigned") {
                it2 = it;
                REQUIRE(it2 == it);
                for (int i = 1; i <= 3; ++i) {
                    REQUIRE(*it2 == i);
                    ++it2;
                }
                REQUIRE(it2 == l.end());
            }
        }
    }
}

TEST_CASE("Constant Iterators", "[iterators]") {
    SECTION("Empty flat_set's iterators should be equal.") {
        flat_set<int> l;
        REQUIRE(l.cbegin() == l.cend());
    }
    SECTION("Non empty flat_set") {
        flat_set<int> l1 = make_flat_set<int>({ 1, 2, 3 });
        SECTION("Moving through the flat_set") {
            auto sit = l1.cbegin();
            auto eit = l1.cend();
            REQUIRE(sit != eit);
            SECTION("Forward") {
                ++sit;
                REQUIRE(sit != eit);
                ++sit;
                REQUIRE(sit != eit);
                ++sit;
                REQUIRE(sit == eit);
            }
            SECTION("Backward") {
                --eit;
                REQUIRE(sit != eit);
                --eit;
                REQUIRE(sit != eit);
                --eit;
                REQUIRE(sit == eit);
            }
            SECTION("Forward - postincr") {
                REQUIRE(sit++ != eit);
                REQUIRE(sit++ != eit);
                REQUIRE(sit++ != eit);
                REQUIRE(sit == eit);
            }
            SECTION("Backward - postdecr") {
                REQUIRE(sit != eit--);
                REQUIRE(sit != eit--);
                REQUIRE(sit != eit--);
                REQUIRE(sit == eit);
            }
        }
        SECTION("Dereferencing iterators") {
            auto sit = l1.cbegin();
            for (int i = 1; i <= 3; ++i) {
                REQUIRE(*sit == i);
                ++sit;
            }
            REQUIRE(sit == l1.cend());
        }
    }
    SECTION("Constructors") {
        SECTION("Default inited iterators should be equal") {
            flat_set<int>::const_iterator it, it2;
            REQUIRE(it == it2);
        }
        SECTION("Iterator's copy is equal") {
            flat_set<int> l = make_flat_set<int>({ 1, 2, 3 });
            auto it = l.cbegin();
            auto it2(it);
            REQUIRE(it2 == it);
            REQUIRE(*it2 == *it);
            SECTION("And can be used to iterate") {
                for (int i = 1; i <= 3; ++i) {
                    REQUIRE(*it2 == i);
                    ++it2;
                }
                REQUIRE(it2 == l.cend());
            }
            SECTION("And so is copy assigned") {
                it2 = it;
                REQUIRE(it2 == it);
                for (int i = 1; i <= 3; ++i) {
                    REQUIRE(*it2 == i);
                    ++it2;
                }
                REQUIRE(it2 == l.cend());
            }
        }
    }
}

TEST_CASE("Explicit comparator", "[comparator]") {
    SECTION("Function object") {
        flat_set<int, std::greater<int>> fs;
        std::vector<int> elements{ 1,2,3,4,5 };
        fs.insert(elements.begin(), elements.end());
        REQUIRE(std::equal(fs.begin(), fs.end(), elements.rbegin(), elements.rend()));
    }
    SECTION("Function") {
        flat_set<int, bool(*)(int, int)> fs([] (int a, int b) {return a > b; });
        std::vector<int> elements{ 1,2,3,4,5 };
        fs.insert(elements.begin(), elements.end());
        REQUIRE(std::equal(fs.begin(), fs.end(), elements.rbegin(), elements.rend()));
    }
    SECTION("Swapping swaps comparators") {
        auto my_lesser = [] (int a, int b) {return a < b; };
        auto my_greater = [] (int a, int b) {return a > b; };
        std::vector<int> e{ 0,1,2,3 };
        flat_set<int, bool(*)(int, int)> l1(e.begin(), e.end(), my_lesser);
        flat_set<int, bool(*)(int, int)> l2(e.begin(), e.end(), my_greater);
        l1.swap(l2);
        l1.insert(100);
        REQUIRE(l1.find(100) == l1.begin());
        l2.insert(-100);
        REQUIRE(l2.find(-100) == l2.begin());
    }
    SECTION("Custom key") {
        std::vector<MyKey> e{ MyKey(1), MyKey(2), MyKey(3), MyKey(4), MyKey(100) };

        SECTION("Compare using same type") {
            using flat_set_k_t = flat_set<MyKey, MyKeyComparator>;
            flat_set_k_t fs(e.begin(), e.end());

            auto off = MyKey::nconstructs();
            REQUIRE(equals<MyKeyComparator>(*fs.find(MyKey{ 3 }), e[2]));
            REQUIRE(MyKey::nconstructs() - off == 1); // implicit conversion from 3 to MyKey(3)

            REQUIRE(fs.find(MyKey{ 42 }) == fs.end());
        }
    }
}

TEST_CASE("operator==, !=", "[eq]") {
    SECTION("Empty flat_sets are equal") {
        REQUIRE(flat_set<int>{} == flat_set<int>{});
        REQUIRE(flat_set<int>{} == flat_set<int>{});
    }
    SECTION("empty and non empty flat_sets") {
        flat_set<int> l1 = make_flat_set<int>({ 1, 2, 3 });
        REQUIRE(l1 != flat_set<int>{});
        REQUIRE(flat_set<int>{} != l1);
    }
    SECTION("Two non-equal non-empty flat_sets") {
        flat_set<int> l1 = make_flat_set<int>({ 1, 2, 3 });
        flat_set<int> l2 = make_flat_set<int>({ 4, 5, 6 });
        REQUIRE(l1 != l2);
    }
    SECTION("Two equal non-empty flat_sets") {
        flat_set<int> l1 = make_flat_set<int>({ 4, 5, 6});
        flat_set<int> l2 = make_flat_set<int>({ 4, 5, 6 });
        REQUIRE(l1 == l2);
    }
    SECTION("A flat_set is equal to itself") {
        flat_set<int> l1 = make_flat_set<int>({12, 32, 43, 54, 765, 123, 43, 1, 1, 1});
        REQUIRE(l1 == l1);
    }
}

TEST_CASE("Relational operators (<, <=, >, >=)", "[ord]"){
    SECTION("One empty flat_set is not smaller than another") {
        REQUIRE_FALSE(flat_set<int>{} < flat_set<int>{});
    }
    SECTION("Empty flat_set should be smaller than non-empty flat_set") {
        REQUIRE(flat_set<int>{} < make_flat_set<int>({ 1, 2, 3 }));
    }
    SECTION("lhs is a subset") {
        auto l1 = make_flat_set<int>({1, 2, 3});
        auto l2 = make_flat_set<int>({ 1, 2, 3, 4 });
        REQUIRE(l1 < l2);
    }
    SECTION("rhs is a subset") {
        auto l1 = make_flat_set<int>({ 1, 2, 3 });
        auto l2 = make_flat_set<int>({ 1, 2 });
        REQUIRE(l1 > l2);
    }
    SECTION("flat_sets are equal") {
        auto l1 = make_flat_set<int>({ 3, 2, 1 });
        flat_set<int> l2(l1);
        REQUIRE(l1 <= l2);
        REQUIRE(l1 >= l2);
    }
    SECTION("Just diff flat_sets") {
        auto l1 = make_flat_set<int>({ 1, 3, 5 });
        auto l2 = make_flat_set<int>({ 1, 4, 5 });
        REQUIRE(l1 < l2);
    }
    SECTION("Just a mismatch 2") {
        auto l1 = make_flat_set<int>({ 1, 2, 3, 4, 5, 6 });
        auto l2 = make_flat_set<int>({ 1, 2, 3, 4, 5 });
        REQUIRE(l1 > l2);
    }
    SECTION("Different types") {
        SECTION("Same length") {
            auto l1 = make_flat_set<std::string>({ "abc", "def", "ghi" });
            auto l2 = make_flat_set<std::string>({ "abc", "def", "ghj" });
            REQUIRE(l1 < l2);
        }
        SECTION("Shorter is later") {
            auto l1 = make_flat_set<std::string>({ "qqq" });
            auto l2 = make_flat_set<std::string>({ "a", "b", "c" });
            REQUIRE(l2 < l1);
        }
    }
}

TEST_CASE("Copy operations", "[copy]") {
    SECTION("Move insert moves") {
        flat_set<tracker> fs;
        tracker t(1);
        auto start = tracker::cnt;
        fs.insert(std::move(t));
        auto t1 = tracker::cnt;
        t1.move_constructors = start.move_constructors;
        t1.move_assignments = start.move_assignments;
        REQUIRE(start == t1);
    }
    SECTION("Value insert performs 1 copy") {
        flat_set<tracker> fs;
        tracker t(1);
        auto start = tracker::cnt;
        fs.insert(t);
        auto t1 = tracker::cnt;
        t1.move_constructors = start.move_constructors;
        t1.move_assignments = start.move_assignments;
        t1.copy_constructors -= 1;
        REQUIRE(start == t1);
    }
    SECTION("Range constructor only performs n copies") {
        std::vector<tracker> elems{ 1, 9, 2, 4, 3, 8 };
        auto start = tracker::cnt;
        flat_set<tracker> fs(elems.begin(), elems.end());
        auto t1 = tracker::cnt;
        t1.destructors -= t1.move_constructors - start.move_constructors;
        t1.move_constructors = start.move_constructors;
        t1.move_assignments = start.move_assignments;
        t1.copy_constructors -= 6;
        REQUIRE(start == t1);
    }

    SECTION("Copy constructor") {
        SECTION("Make sure flat_sets are disconnected") {
            auto l1 = make_flat_set<int>({ 1, 2, 3 });
            flat_set<int> l2(l1);
            SECTION("Simple copy") {
                REQUIRE(set_equal(l2, { 1, 2, 3 }));
            }
            SECTION("Add to old flat_set") {
                l1.insert(4);
                REQUIRE(set_equal(l1, { 1, 2, 3, 4 }));
                REQUIRE(set_equal(l2, { 1, 2, 3 }));
            }
            SECTION("Add to new flat_set") {
                l2.insert(4);
                REQUIRE(set_equal(l1, { 1, 2, 3 }));
                REQUIRE(set_equal(l2, { 1, 2, 3, 4 }));
            }
            SECTION("Add to both flat_sets") {
                l1.insert(4);
                l2.insert(5);
                REQUIRE(set_equal(l1, { 1, 2, 3, 4 }));
                REQUIRE(set_equal(l2, { 1, 2, 3, 5 }));
            }
        }
        SECTION("Copying an empty flat_set") {
            flat_set<int> l1;
            flat_set<int> l2(l1);
            REQUIRE(reports_as_empty(l1));
            REQUIRE(reports_as_empty(l2));
        }
    }
    SECTION("Copy assignment") {
        auto l1 = make_flat_set<int>({ 1, 2, 3 });
        auto l2 = make_flat_set<int>({ 4, 5, 6, 7 });
        SECTION("Simple assignment") {
            l2 = l1;
            REQUIRE(set_equal(l1, { 1, 2, 3 }));
            REQUIRE(set_equal(l2, { 1, 2, 3 }));
        }
        SECTION("Self assignment") {
            l2 = l2;
            REQUIRE(!l2.empty());
            REQUIRE(set_equal(l2, { 4, 5, 6, 7 }));
        }
        SECTION("Repeated assignments") {
            l2 = l1;
            REQUIRE(set_equal(l2, { 1, 2, 3 }));
            l1.insert(4);
            l2 = l1;
            REQUIRE(set_equal(l2, { 1, 2, 3, 4 }));
            l1.insert(5);
            l2 = l1;
            REQUIRE(set_equal(l2, { 1, 2, 3, 4, 5 }));
            l1.insert(6);
            l2 = l1;
            REQUIRE(set_equal(l2, { 1, 2, 3, 4, 5, 6 }));
        }
    }
}

TEST_CASE("Move operations", "[move]") {
    SECTION("Move constructor") {
        SECTION("From empty flat_set") {
            flat_set<int> l1;
            flat_set<int> l2(std::move(l1));
            REQUIRE(reports_as_empty(l2));
        }
        SECTION("From full flat_set") {
            auto elems = std::vector<int>({ 1, 2, 3, 4, 5, 6, 7, 8 });
            flat_set<int> l1(elems.begin(), elems.end());
            flat_set<int> l2(std::move(l1));
            REQUIRE(set_equal(l2, elems));
        }
    }
    SECTION("Move assignment") {
        SECTION("From empty flat_set") {
            flat_set<int> l1;
            SECTION("To empty flat_set") {
                flat_set<int> l2;
                l2 = std::move(l1);
                REQUIRE(reports_as_empty(l2));
            }
            SECTION("To full flat_set") {
                auto l2 = make_flat_set<int>({ 1, 2, 3, 4, 5, 6 });
                l2 = std::move(l1);
                REQUIRE(reports_as_empty(l2));
            }
        }
        SECTION("From full flat_set") {
            auto l1 = make_flat_set<int>({ 5, 4, 3, 2, 1 });
            SECTION("to empty flat_set") {
                flat_set<int> l2;
                l2 = std::move(l1);
                REQUIRE(set_equal(l2, { 5, 4, 3, 2, 1 }));
            }
            SECTION("To full flat_set") {
                auto l2 = make_flat_set<int>({ 4, 2, 3 });
                l2 = std::move(l1);
                REQUIRE(set_equal(l2, { 5, 4, 3, 2, 1 }));
            }
        }
        SECTION("Back and forth") {
            auto l1 = make_flat_set<int>({ 44, 2, 4 });
            auto l2 = make_flat_set<int>({ 1, 2, 3, 4, 5, 6 });
            l1 = std::move(l2);
            REQUIRE(set_equal(l1, { 1, 2, 3, 4, 5, 6 }));
            l2 = std::move(l1);
            REQUIRE(set_equal(l2, { 1, 2, 3, 4, 5, 6 }));

            l2.clear();
            SECTION("Sets can be used after being move assigned") {
                l2.insert(7);
                l2.erase(l2.begin());
                l2.insert(0);
                REQUIRE(set_equal(l2, { 0 }));
            }
        }
    }
}

TEST_CASE("member swap", "[swap]") {
    SECTION("Swapping empty flat_sets leaves both empty") {
        flat_set<int> l1, l2;
        l1.swap(l2);
        REQUIRE(reports_as_empty(l1));
        REQUIRE(reports_as_empty(l2));
    }
    SECTION("Swapping elements actually swaps them") {
        auto l1 = make_flat_set<int>({ 1, 2, 3 });
        SECTION("empty and full") {
            flat_set<int> l2;
            l1.swap(l2);
            REQUIRE(reports_as_empty(l1));
            REQUIRE(set_equal(l2, { 1, 2, 3 }));
            SECTION("Roundtrip") {
                l1.swap(l2);
                REQUIRE(set_equal(l1, { 1, 2, 3 }));
                REQUIRE(reports_as_empty(l2));
            }
        }
        SECTION("full and full") {
            auto l2 = make_flat_set<int>({ 4, 5, 6, 7 });
            l1.swap(l2);
            REQUIRE(set_equal(l1, { 4, 5, 6, 7 }));
            REQUIRE(set_equal(l2, { 1, 2, 3 }));
            SECTION("Roundtrip") {
                l1.swap(l2);
                REQUIRE(set_equal(l1, { 1, 2, 3 }));
                REQUIRE(set_equal(l2, { 4, 5, 6, 7 }));
            }
        }
    }
}

TEST_CASE("swap overload", "[swap]") {
    SECTION("Swapping empty flat_sets leaves both empty") {
        flat_set<int> l1, l2;
        swap(l1, l2);
        REQUIRE(reports_as_empty(l1));
        REQUIRE(reports_as_empty(l2));
    }
    SECTION("Swapping elements actually swaps them") {
        auto l1 = make_flat_set<int>({ 1, 2, 3 });
        SECTION("empty and full") {
            flat_set<int> l2;
            swap(l1, l2);
            REQUIRE(reports_as_empty(l1));
            REQUIRE(set_equal(l2, { 1, 2, 3 }));
            SECTION("Roundtrip") {
                swap(l1, l2);
                REQUIRE(set_equal(l1, { 1, 2, 3 }));
                REQUIRE(reports_as_empty(l2));
            }
        }
        SECTION("full and full") {
            auto l2 = make_flat_set<int>({ 4, 5, 6, 7 });
            swap(l1, l2);
            REQUIRE(set_equal(l1, { 4, 5, 6, 7 }));
            REQUIRE(set_equal(l2, { 1, 2, 3 }));
            SECTION("Roundtrip") {
                swap(l1, l2);
                REQUIRE(set_equal(l1, { 1, 2, 3 }));
                REQUIRE(set_equal(l2, { 4, 5, 6, 7 }));
            }
        }
    }
}

/**
 * Advanced functionality tests start here
 */

TEST_CASE("Reserve should not construct any elements", "[reference-handling]") {
    flat_set<tracker> set;
    auto old = tracker::cnt;
    set.reserve(100);
    REQUIRE(old == tracker::cnt);
}

TEST_CASE("Proper reference handling in inserts", "[reference-handling]") {
    flat_set<tracker> set;
    set.reserve(100);
    SECTION("Insert by copy") {
        tracker t;
        auto oldt = tracker::cnt;
        set.insert(t);
        auto newt = tracker::cnt;
        REQUIRE(oldt.copy_constructors == newt.copy_constructors - 1);
    }
    SECTION("Insert by move") {
        auto oldt = tracker::cnt;
        set.insert(tracker{});
        auto newt = tracker::cnt;
        REQUIRE(oldt.move_constructors == newt.move_constructors - 1);
    }
    SECTION("Range insert copies") {
        std::vector<tracker> vec{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        auto oldt = tracker::cnt;
        set.insert(begin(vec), end(vec));
        auto newt = tracker::cnt;
        REQUIRE(oldt.copy_constructors == newt.copy_constructors - 9);
    }
}

TEST_CASE("Proper iterator tag handling in range insert", "[iterator_category][insert]") {
    using input_iter = fake_input_iterator<std::vector<tracker>::iterator, false>;
    const size_t num_elems = 1000;
    std::vector<tracker> elems; elems.reserve(num_elems);
    std::generate_n(std::back_inserter(elems), num_elems, [] () {
        static int i = 0;
        return i++;
    });
    SECTION("RandomAccessIterator range constructor") {
        auto oldt = tracker::cnt;
        flat_set<tracker> fs(begin(elems), end(elems));
        auto newt = tracker::cnt;
        REQUIRE(oldt.copy_constructors == newt.copy_constructors - num_elems);
        REQUIRE(fs == make_flat_set(elems));
    }
    SECTION("ForwardIterator range constructor") {
        std::forward_list<tracker> slist(begin(elems), end(elems));
        auto oldt = tracker::cnt;
        flat_set<tracker> fs(begin(slist), end(slist));
        auto newt = tracker::cnt;
        REQUIRE(oldt.copy_constructors == newt.copy_constructors - num_elems);
        REQUIRE(fs == make_flat_set(elems));
    }
    SECTION("InputIterator range constructor") {
        auto oldt = tracker::cnt;
        flat_set<tracker> fs(input_iter(begin(elems)),
                             input_iter(end(elems)));
        auto newt = tracker::cnt;
        REQUIRE(oldt.copy_constructors < newt.copy_constructors - num_elems);
        REQUIRE(fs == make_flat_set(elems));
    }
    SECTION("RandomAccessIterator range insert") {
        flat_set<tracker> fs;
        auto oldt = tracker::cnt;
        fs.insert(begin(elems), end(elems));
        auto newt = tracker::cnt;
        REQUIRE(oldt.copy_constructors == newt.copy_constructors - num_elems);
        REQUIRE(fs == make_flat_set(elems));
    }
    SECTION("ForwardIterator range insert") {
        std::forward_list<tracker> slist(begin(elems), end(elems));
        flat_set<tracker> fs;
        auto oldt = tracker::cnt;
        fs.insert(begin(slist), end(slist));
        auto newt = tracker::cnt;
        REQUIRE(oldt.copy_constructors == newt.copy_constructors - num_elems);
        REQUIRE(fs == make_flat_set(elems));
    }
    SECTION("InputIterator range insert") {
        flat_set<tracker> fs;
        auto oldt = tracker::cnt;
        fs.insert(input_iter(begin(elems)),
                  input_iter(end(elems)));
        auto newt = tracker::cnt;
        REQUIRE(oldt.copy_constructors < newt.copy_constructors - num_elems);
        REQUIRE(fs == make_flat_set(elems));
    }
}

TEST_CASE("Proper iterator tag handling in range constructor", "[iterator_category][constructor]") {
    using input_iter = fake_input_iterator<std::vector<tracker>::iterator, false>;
    const size_t num_elems = 1000;
    std::vector<tracker> elems; elems.reserve(num_elems);
    std::generate_n(std::back_inserter(elems), num_elems, [] () {
        static int i = 0;
        return i++;
    });
    SECTION("RandomAccessIterator range insert") {
        flat_set<tracker> fs;
        auto oldt = tracker::cnt;
        fs.insert(begin(elems), end(elems));
        auto newt = tracker::cnt;
        REQUIRE(oldt.copy_constructors == newt.copy_constructors - num_elems);
        REQUIRE(fs == make_flat_set(elems));
    }
    SECTION("ForwardIterator range insert") {
        std::forward_list<tracker> slist(begin(elems), end(elems));
        flat_set<tracker> fs;
        auto oldt = tracker::cnt;
        fs.insert(begin(slist), end(slist));
        auto newt = tracker::cnt;
        REQUIRE(oldt.copy_constructors == newt.copy_constructors - num_elems);
        REQUIRE(fs == make_flat_set(elems));
    }
    SECTION("InputIterator range insert") {
        flat_set<tracker> fs;
        auto oldt = tracker::cnt;
        fs.insert(input_iter(begin(elems)),
                  input_iter(end(elems)));
        auto newt = tracker::cnt;
        REQUIRE(oldt.copy_constructors < newt.copy_constructors - num_elems);
        REQUIRE(fs == make_flat_set(elems));
    }

}

/**
 * Stuff the compilation tests down here, nobody is likely to care about them
 */

TEST_CASE("Compilation tests", "[basics][compile-time]") {
    auto l = make_flat_set<int>({ 1, 2 });
    const auto& const_l = l;
    REQUIRE(!const_l.empty());
    REQUIRE(const_l.size() == 2);
    flat_set<int>::iterator it = l.begin();
    flat_set<int>::const_iterator cit = it;
    static_cast<void>(cit); // silence unused variable warning
}

TEST_CASE("compile-time tests", "[basics][compile-time]") {
    using namespace std;

    // nice idea, but ...
    static_assert(!is_convertible<flat_set<int>*, std::set<int>*>::value, "do not derive from std::set :)");

    using my_set = flat_set<int>;
    my_set fs;
    my_set::iterator i = fs.begin();
    my_set::const_iterator ci = const_cast<const my_set&>(fs).begin();
    unused(i, ci); // silence compiler warnings

    static_assert(is_convertible<my_set::iterator, my_set::const_iterator>::value, "An iterator has to be convertible to const iterator");
    // either const_iterator is not convertible to iterator or even iterator is const, so that it does not matter
    static_assert(!is_convertible<my_set::const_iterator, my_set::iterator>::value || is_const<remove_reference_t<iterator_traits<my_set::iterator>::reference>>::value, "");
    // iterators are random access; this also prevents using std::set
    static_assert(is_same<iterator_traits<my_set::iterator>::iterator_category, random_access_iterator_tag>::value, "");
}

TEST_CASE("Anticheat tests", "[basics][workaround-check]") {
    // Don't implement flat_set via set ... not even by composition
    SECTION("The underlying storage is contiguous") {
        auto fs = make_flat_set<int>({ 1,2,3,4,5 });
        int *expected = &*fs.begin();

        for (auto i = fs.begin(), e = fs.end(); i != e; ++i) {
            REQUIRE(expected == &*i);
            expected++;
        }
    }
}
