#include "catch.hpp"

#include "tiny-03.hpp"

#include <sstream>

void verify_result(statistics const& stats, size_t sz, int min, int max, int median) {
    REQUIRE(stats.total_datapoints == sz);
    REQUIRE(stats.min == min);
    REQUIRE(stats.max == max);
    REQUIRE(stats.median == median);
}

TEST_CASE("Some basic checks", "[all]") {
    std::stringstream sstr;
    SECTION("Trivial positive example") {
        sstr << "3 17 32 29\n";
        auto summary = summarize_data(sstr);
        REQUIRE(summary.size() == 1);
        verify_result(summary[0], 3, 17, 32, 29);
    }
    SECTION("Multiline example") {
        sstr << "1 1\n"
             << "2 2 2\n"
             << "9 84 24 82 24 80 24 55 73 29";
        auto summary = summarize_data(sstr);
        REQUIRE(summary.size() == 3);
        verify_result(summary[0], 1, 1, 1, 1);
        verify_result(summary[1], 2, 2, 2, 2);
        verify_result(summary[2], 9, 24, 84, 55);
    }
    SECTION("Input with some invalid values") {
        sstr << "3 22 23 24\n"
             << "4 44 55 266 22\n"
             << "3 11 22 33\n"
             << "1 11\n";
        auto summary = summarize_data(sstr);
        REQUIRE(summary.size() == 4);
        verify_result(summary[0], 3, 22, 24, 23);
        REQUIRE(summary[1].total_datapoints == static_cast<size_t>(-1));
        verify_result(summary[2], 3, 11, 33, 22);
        verify_result(summary[3], 1, 11, 11, 11);
    }
}
