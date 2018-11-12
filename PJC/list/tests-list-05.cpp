#include "catch.hpp"

#include "list.hpp"

#include "test-helpers.hpp"

#include <iostream>
#include <algorithm>
#include <chrono>

/*
    Testy pro stage-05.

    Tento krok testuje hrubou algoritmickou složitost 3 metod:
      * merge
      * split
      * sort

    samplováním doby běhu těchto metod pro různě veliké vstupy.
    Měření samotné probíhá pouze vůči době běhů dané implementace,
    což eliminuje vliv low-level optimalizací na úspěšnost testů.

    Jako vždy, testy v tomto kroku předpokládají, že metody testované
    v minulém kroku fungují správně, a tudíž se na nich mohou stavět
    testy pro další funkcionalitu.
*/

using pjc::list;

TEST_CASE("merge - complexity", "[.long][merge][stage5]") {
    // In the past, some students actually implemented list::merge with
    // non-linear complexity. Since any sane implementation of merge
    // will have linear complexity, we check it here.
    double last_time = 0;
    for (int i = 256'000; i <= 5'000'000; i *= 4) {
        auto lhs_elems = generate_data(i);
        auto rhs_elems = generate_data(i);
        std::sort(begin(lhs_elems), end(lhs_elems));
        std::sort(begin(rhs_elems), end(rhs_elems));
        list lhs{ lhs_elems };
        list rhs{ rhs_elems };
        auto start_time = std::chrono::high_resolution_clock::now();
        lhs.merge(rhs);
        auto end_time = std::chrono::high_resolution_clock::now();
        auto time_diff = end_time - start_time;
        auto time_per_item = time_diff.count() / static_cast<double>( 2 * i);
        lhs_elems.insert(end(lhs_elems), begin(rhs_elems), end(rhs_elems));
        std::inplace_merge(begin(lhs_elems), begin(lhs_elems) + i, end(lhs_elems));
        REQUIRE(list_equal(lhs, lhs_elems));
        if (last_time != 0) {
            // Per element time should remain the same, more or less, allowing for jitter.
            // Even a very naive implementation can hit about 1.25 coefficient...
            REQUIRE(time_per_item / last_time <= 1.25);
        }
        std::cout << "merge: i = " << i << " time per item = " << time_per_item << '\n';
        last_time = time_per_item;
    }
}

TEST_CASE("split - complexity", "[.long][split][stage5]") {
    // Basically, split can either be implemented correctly,
    // taking linear time only in respect to size of left split part,
    // or incorrectly, taking linear time in respect to total size of
    // list, regardless of split position.
    // We want to detect the second case and fail the tests
    using namespace std::chrono_literals;
    double last_time = 0;
    for (int i = 64'000; i <= 20'000'000; i *= 4) {
        auto elems = generate_data(i);
        list l{ elems };
        auto it = l.cbegin();
        std::advance(it, 10);
        auto start_time = std::chrono::high_resolution_clock::now();
        auto parts = l.split(it);
        auto end_time = std::chrono::high_resolution_clock::now();
        // Add 0.5 ms to even out variability
        auto time_diff = end_time - start_time + 500us;
        auto total_time = static_cast<double>(time_diff.count());
        REQUIRE(list_equal(parts.first, { elems.begin(), elems.begin() + 10 }));
        REQUIRE(list_equal(parts.second, { elems.begin() + 10, elems.end() }));
        if (last_time != 0) {
            // The coefficient is a rough approximation.
            // A linear time coefficient should blow out, constant time
            // jitter should remain inside some reasonable coefficient
            REQUIRE(total_time / last_time <= 1.25);
        }
        last_time = total_time;
        std::cout << "split: i = " << i << " total time = " << (time_diff - 500us).count() << '\n';
    }
}

TEST_CASE("sort - complexity", "[.long][sort][stage5]") {
    // This is kinda complex...
    // We have to both test enough different sizes, and abort early in case of
    // quadratic (or worse) complexity.
    double last_time = 0;
    for (int i = 16'000; i <= 5'000'000; i *= 4) {
        auto elems = generate_data(i);
        list l{ elems };
        auto start_time = std::chrono::high_resolution_clock::now();
        l.sort();
        auto end_time = std::chrono::high_resolution_clock::now();
        auto time_diff = end_time - start_time;
        auto time_per_item = time_diff.count() / static_cast<double>(i);
        std::sort(begin(elems), end(elems));
        REQUIRE(list_equal(l, elems));
        if (last_time != 0) {
            // The coefficient is a rough approximation.
            // Even a very naive implementation can hit about 1.5 coefficient...
            REQUIRE(time_per_item / last_time <= 2);
        }
        std::cout << "sort: i = " << i << " time per item = " << time_per_item << '\n';
        last_time = time_per_item;
    }
}
