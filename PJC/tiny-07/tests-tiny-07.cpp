#include "tiny-07.hpp"

#include "catch.hpp"

#include <memory>


TEST_CASE("Check mersenne twister generator", "[mersenne][all]") {
    auto check_mt_gen = [] (numbers_generator& mtgen) {
        CHECK_FALSE(mtgen.random_bool());
        CHECK(mtgen.random_bool());
        CHECK_FALSE(mtgen.random_bool());
        CHECK(mtgen.random_double() == Approx(0.9457483679));
        CHECK(mtgen.random_double() == Approx(0.1856478315));
        CHECK(mtgen.random_double() == Approx(0.9487333737));
        CHECK(mtgen.random_int(20, 200) == 25);
        CHECK(mtgen.random_int(20, 200) == 177);
        CHECK(mtgen.random_int(20, 200) == 103);
    };
    auto mtgen = std::make_unique<mersenne_twister_generator>(1234);
    SECTION("Verify expected results") {
        check_mt_gen(*mtgen);
    }
    SECTION("Clone from derived pointer") {
        std::unique_ptr<mersenne_twister_generator> mtgen2 = mtgen->clone();
        check_mt_gen(*mtgen);
        check_mt_gen(*mtgen2);
    }
    SECTION("Clone from base pointer") {
        std::unique_ptr<numbers_generator> bgen(std::move(mtgen));
        auto mtgen2 = bgen->clone();
        // Check that the clone is indeed mt generator
        REQUIRE_NOTHROW(dynamic_cast<mersenne_twister_generator&>(*mtgen2));
        check_mt_gen(*bgen);
        check_mt_gen(*mtgen2);
    }
}

TEST_CASE("Check minstd generator", "[minstd][all]") {
    auto check_std_gen = [] (numbers_generator& mtgen) {
        CHECK_FALSE(mtgen.random_bool());
        CHECK(mtgen.random_bool());
        CHECK(mtgen.random_bool());
        CHECK(mtgen.random_double() == Approx(0.4801670224));
        CHECK(mtgen.random_double() == Approx(0.1423510221));
        CHECK(mtgen.random_double() == Approx(0.4262081654));
        CHECK(mtgen.random_int(20, 200) == 109);
        CHECK(mtgen.random_int(20, 200) == 110);
        CHECK(mtgen.random_int(20, 200) == 114);
    };
    auto stdgen = std::make_unique<minstd_generator>(1234);
    SECTION("Verify expected results") {
        check_std_gen(*stdgen);
    }
    SECTION("Clone from derived pointer") {
        std::unique_ptr<minstd_generator> stdgen2 = stdgen->clone();
        check_std_gen(*stdgen);
        check_std_gen(*stdgen2);
    }
    SECTION("Clone from base pointer") {
        std::unique_ptr<numbers_generator> bgen(std::move(stdgen));
        auto stdgen2 = bgen->clone();
        // Check that the clone is indeed minstd generator
        REQUIRE_NOTHROW(dynamic_cast<minstd_generator&>(*stdgen2));
        check_std_gen(*bgen);
        check_std_gen(*stdgen2);
    }
}

TEST_CASE("Check fixed generator", "[mersenne][all]") {
    auto check_fixed_gen = [] (numbers_generator& fixgen) {
        for (int i = 0; i < 10; ++i) {
            REQUIRE(fixgen.random_bool());
            REQUIRE(fixgen.random_double() == 0);
            REQUIRE(fixgen.random_int(10, 100) == 55);
        }
    };
    auto fixgen = std::make_unique<fixed_generator>();
    SECTION("Verify expected results") {
        check_fixed_gen(*fixgen);
    }
    SECTION("Clone from derived pointer") {
        std::unique_ptr<fixed_generator> fixgen2 = fixgen->clone();
        check_fixed_gen(*fixgen);
        check_fixed_gen(*fixgen2);
    }
    SECTION("Clone from base pointer") {
        std::unique_ptr<numbers_generator> bgen(std::move(fixgen));
        auto fixgen2 = bgen->clone();
        // Check that the clone is indeed fixed generator
        REQUIRE_NOTHROW(dynamic_cast<fixed_generator&>(*fixgen2));
        check_fixed_gen(*bgen);
        check_fixed_gen(*fixgen2);
    }
}
