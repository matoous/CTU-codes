#include "tiny-06.hpp"
#include "catch.hpp"

#include <random>
#include <vector>

namespace {
    std::uniform_real_distribution<double> double_dist { 0, 1 };
    std::bernoulli_distribution bool_dist { 0.5 };
    std::uniform_int_distribution<int> int_dist(int lower, int upper) {
        return std::uniform_int_distribution<int>(lower, upper);
    }
}

// Nejméně záruk, neportabilní -- musí se kontrolovat pouze zaručené intervaly
TEST_CASE("Check random generator", "[random][all]") {
    auto rand_gen = numbers_generator::create_by_kind(generator_kind::random, 1234);
    CHECK(rand_gen->bools_generated() == 0);
    CHECK(rand_gen->doubles_generated() == 0);
    CHECK(rand_gen->ints_generated() == 0);
    SECTION("Generate bools until we get 10 falses") {
        size_t num_false = 0;
        while (num_false < 10) {
            auto res = rand_gen->random_bool();
            if (!res) { ++num_false; }
        }
        REQUIRE(rand_gen->bools_generated() >= 10);
        REQUIRE(rand_gen->falses_generated() == 10);
        REQUIRE(rand_gen->trues_generated() == rand_gen->bools_generated() - 10);
    }
    SECTION("Generate doubles until we get 10 small ones") {
        size_t num_small = 0;
        size_t num_big = 0;
        while (num_small < 10) {
            auto res = rand_gen->random_double();
            if (res < 0.1) { ++num_small; }
            else { ++num_big; }
        }
        REQUIRE(rand_gen->doubles_generated() == num_small + num_big);
        REQUIRE(rand_gen->min_generated_double() <  0.1);
        REQUIRE(rand_gen->min_generated_double() >= 0.0);
        REQUIRE(rand_gen->max_generated_double() >= 0.1);
        REQUIRE(rand_gen->max_generated_double() <  1.0);
    }
    SECTION("Generate ints from significantly different ranges") {
        for (int i = 0; i < 16; i += 4) {
            auto res = rand_gen->random_int(-200 + i * 100, -100 + i * 100);
            CHECK(res >= -200 + i * 100);
            CHECK(res <= -100 + i * 100);
        }
        CHECK(rand_gen->ints_generated() == 4);
        REQUIRE(rand_gen->min_generated_int() >= -200);
        REQUIRE(rand_gen->min_generated_int() <= -100);
        REQUIRE(rand_gen->max_generated_int() >= 1000);
        REQUIRE(rand_gen->max_generated_int() <= 1100);
    }
}

/* The PRNGs themselves are well-defined and consistent across implementations,
 * but unfortunately that can't be said about the distribution objects, which
 * vary.
 */

TEST_CASE("mt generator", "[twister][all]") {
    auto mt_gen = numbers_generator::create_by_kind(generator_kind::mersenne, 1234);
    std::mt19937_64 mt(1234);
    CHECK(mt_gen->bools_generated() == 0);
    CHECK(mt_gen->doubles_generated() == 0);
    CHECK(mt_gen->ints_generated() == 0);
    SECTION("Run coinflips for a while") {
        auto coinflips = mt_gen->random_int(40, 80);
        CHECK(coinflips == int_dist(40, 80)(mt));
        int ntrues = 0;
        for (int i = 0; i < coinflips; ++i) {
            static_cast<void>(mt_gen->random_bool());
            ntrues += bool_dist(mt);
        }
        CHECK(mt_gen->bools_generated() == coinflips);
        REQUIRE(mt_gen->trues_generated() == ntrues);
        REQUIRE(mt_gen->falses_generated() == coinflips-ntrues);
    }
    SECTION("Generate some doubles") {
        auto num_doubles = mt_gen->random_int(50, 100);
        CHECK(num_doubles == int_dist(50, 100)(mt));
        std::vector<double> values;
        for (int i = 0; i < num_doubles; ++i) {
            static_cast<void>(mt_gen->random_double());
            values.push_back(double_dist(mt));
        }
        std::sort(begin(values), end(values));
        CHECK(mt_gen->doubles_generated() == num_doubles);
        REQUIRE(mt_gen->min_generated_double() == values.front());
        REQUIRE(mt_gen->max_generated_double() == values.back());
    }
    SECTION("Generate some ints") {
        auto num_ints = mt_gen->random_int(60, 120);
        CHECK(num_ints == int_dist(60, 120)(mt));
        std::vector<int> values;
        for (int i = 0; i < num_ints; ++i) {
            static_cast<void>(mt_gen->random_int(i, 200));
            values.push_back(int_dist(i, 200)(mt));
        }
        values.push_back(num_ints);
        std::sort(begin(values), end(values));
        CHECK(mt_gen->ints_generated() == values.size());
        REQUIRE(mt_gen->min_generated_int() == values.front());
        REQUIRE(mt_gen->max_generated_int() == values.back());
    }
}

TEST_CASE("minstd generator", "[minstd][all]") {
    auto minstd_gen = numbers_generator::create_by_kind(generator_kind::minstd, 1234);
    std::minstd_rand minstd(1234);
    CHECK(minstd_gen->bools_generated() == 0);
    CHECK(minstd_gen->doubles_generated() == 0);
    CHECK(minstd_gen->ints_generated() == 0);
    SECTION("Run coinflips for a while") {
        auto coinflips = minstd_gen->random_int(40, 80);
        CHECK(coinflips == int_dist(40, 80)(minstd));
        int ntrues = 0;
        for (int i = 0; i < coinflips; ++i) {
            static_cast<void>(minstd_gen->random_bool());
            ntrues += bool_dist(minstd);
        }
        CHECK(minstd_gen->bools_generated() == coinflips);
        REQUIRE(minstd_gen->trues_generated() == ntrues);
        REQUIRE(minstd_gen->falses_generated() == coinflips-ntrues);
    }
    SECTION("Generate some doubles") {
        auto num_doubles = minstd_gen->random_int(50, 100);
        CHECK(num_doubles == int_dist(50, 100)(minstd));
        std::vector<double> values;
        for (int i = 0; i < num_doubles; ++i) {
            static_cast<void>(minstd_gen->random_double());
            values.push_back(double_dist(minstd));
        }
        std::sort(begin(values), end(values));
        CHECK(minstd_gen->doubles_generated() == num_doubles);
        REQUIRE(minstd_gen->min_generated_double() == values.front());
        REQUIRE(minstd_gen->max_generated_double() == values.back());
    }
    SECTION("Generate some ints") {
        auto num_ints = minstd_gen->random_int(60, 120);
        CHECK(num_ints == int_dist(60, 120)(minstd));
        std::vector<int> values;
        for (int i = 0; i < num_ints; ++i) {
            static_cast<void>(minstd_gen->random_int(i, 200));
            values.push_back(int_dist(i, 200)(minstd));
        }
        values.push_back(num_ints);
        std::sort(begin(values), end(values));
        CHECK(minstd_gen->ints_generated() == values.size());
        REQUIRE(minstd_gen->min_generated_int() == values.front());
        REQUIRE(minstd_gen->max_generated_int() == values.back());
    }
}
