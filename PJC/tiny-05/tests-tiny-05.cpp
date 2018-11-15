#include "tiny-05.hpp"

#include "catch.hpp"

#include <type_traits>
#include <complex>

// Check basic cheating -- plain type alias
static_assert(!std::is_same<std::complex<float>, pjc::complex>::value, "");
static_assert(!std::is_same<std::complex<double>, pjc::complex>::value, "");
static_assert(!std::is_same<std::complex<long double>, pjc::complex>::value, "");

// Check basic cheating -- deriving from std::complex
static_assert(!std::is_base_of<std::complex<float>, pjc::complex>::value, "");
static_assert(!std::is_base_of<std::complex<double>, pjc::complex>::value, "");
static_assert(!std::is_base_of<std::complex<long double>, pjc::complex>::value, "");

// Get some helpers into the right namespace for ADL
namespace pjc {

bool operator==(pjc::complex lhs, pjc::complex rhs) {
    return lhs.real() == Approx(rhs.real()) &&
           lhs.imag() == Approx(rhs.imag());
}

std::ostream& operator<<(std::ostream& os, pjc::complex lhs) {
    os << '(' << lhs.real() << ", " << lhs.imag() << ')';
    return os;
}

}

using pjc::complex;


TEST_CASE("Basic usage", "[all]") {
    complex c(1, 2);
    REQUIRE(c.real() == 1);
    REQUIRE(c.imag() == 2);
    CHECK(c == complex{ 1, 2 });

    c.real(4.55);
    REQUIRE(c.real() == 4.55);
    c.imag(3.33);
    REQUIRE(c.imag() == 3.33);
    CHECK(c == complex{ 4.55, 3.33 });
}

TEST_CASE("binary +", "[all]") {
    SECTION("complex + complex") {
        SECTION("#1") {
            complex c1(1.2, 3.4);
            complex c2(3.1, 0.9);
            auto c3 = c1 + c2;
            REQUIRE(c3 == complex(4.3, 4.3));
        }
        SECTION("#2") {
            complex c1(0.001, 0.001);
            complex c2(0, 0.0);
            auto c3 = c1 + c2;
            REQUIRE(c3 == complex(0.001, 0.001));
        }
        SECTION("#3") {
            complex c1(1.2, -33);
            complex c2(-13.1, 10.9);
            auto c3 = c1 + c2;
            REQUIRE(c3 == complex(-11.9, -22.1));
        }
    }
    SECTION("complex + double") {
        SECTION("#1") {
            complex c1(1.22, 3.44);
            auto sum = c1 + 2.22;
            REQUIRE(sum == complex(3.44, 3.44));
        }
        SECTION("#2") {
            complex c1(-22, 22);
            auto sum = c1 + 11;
            REQUIRE(sum == complex(-11, 22));
        }
        SECTION("#3") {
            complex c1(-7.44, 12);
            auto sum = c1 + 7.44;
            REQUIRE(sum == complex(0, 12));
        }
    }
    SECTION("double + complex") {
        SECTION("#1") {
            complex c1(1.22, 3.44);
            auto sum = 2.22 + c1;
            REQUIRE(sum == complex(3.44, 3.44));
        }
        SECTION("#2") {
            complex c1(-22, 22);
            auto sum = 11 + c1;
            REQUIRE(sum == complex(-11, 22));
        }
        SECTION("#3") {
            complex c1(-7.44, 12);
            auto sum = 7.44 + c1;
            REQUIRE(sum == complex(0, 12));
        }
    }
}

TEST_CASE("binary -", "[all]") {
    SECTION("complex - complex") {
        SECTION("#1") {
            complex c1(1.2, 3.4);
            complex c2(3.1, 0.9);
            auto c3 = c1 - c2;
            REQUIRE(c3 == complex(-1.9, 2.5));
        }
        SECTION("#2") {
            complex c1(0.001, 0.001);
            complex c2(0, 0.0);
            auto c3 = c1 - c2;
            REQUIRE(c3 == complex(0.001, 0.001));
        }
        SECTION("#3") {
            complex c1(1.2, -33);
            complex c2(-13.1, 10.9);
            auto c3 = c1 - c2;
            REQUIRE(c3 == complex(14.3, -43.9));
        }
    }
    SECTION("complex - double") {
        SECTION("#1") {
            complex c1(1.22, 3.44);
            auto res = c1 - 2.22;
            REQUIRE(res == complex(-1, 3.44));
        }
        SECTION("#2") {
            complex c1(-22, 22);
            auto res = c1 - 11;
            REQUIRE(res == complex(-33, 22));
        }
        SECTION("#3") {
            complex c1(-7.44, 12);
            auto res = c1 - 7.44;
            REQUIRE(res == complex(-14.88, 12));
        }
    }
    SECTION("double - complex") {
        SECTION("#1") {
            complex c1(1.22, 3.44);
            auto res = 2.22 - c1;
            REQUIRE(res == complex(1, -3.44));
        }
        SECTION("#2") {
            complex c1(-22, 22);
            auto res = 11 - c1;
            REQUIRE(res == complex(33, -22));
        }
        SECTION("#3") {
            complex c1(-7.44, 12);
            auto res = 7.44 - c1;
            REQUIRE(res == complex(14.88, -12));
        }
    }
}

TEST_CASE("binary *", "[all]") {
    // complex, complex
    SECTION("complex * complex") {
        SECTION("#1") {
            complex c1(1.2, 3.4);
            complex c2(3.1, 0.9);
            auto c3 = c1 * c2;
            REQUIRE(c3 == complex(0.66, 11.62));
        }
        SECTION("#2") {
            complex c1(0.001, 0.001);
            complex c2(0, 0.0);
            auto c3 = c1 * c2;
            REQUIRE(c3 == complex(0, 0));
        }
        SECTION("#3") {
            complex c1(1.2, -33);
            complex c2(-13.1, 10.9);
            auto c3 = c1 * c2;
            REQUIRE(c3 == complex(343.98, 445.38));
        }
    }
    SECTION("complex * double") {
        SECTION("#1") {
            complex c1(1.22, 3.44);
            auto res = c1 * 2.22;
            REQUIRE(res == complex(2.7084, 7.6368));
        }
        SECTION("#2") {
            complex c1(-22, 22);
            auto res = c1 * 11;
            REQUIRE(res == complex(-242, 242));
        }
        SECTION("#3") {
            complex c1(-7.44, 12);
            auto res = c1 * 7.44;
            REQUIRE(res == complex(-55.3536, 89.28));
        }
    }
    SECTION("double * complex") {
        SECTION("#1") {
            complex c1(1.22, 3.44);
            auto res = 2.22 * c1;
            REQUIRE(res == complex(2.7084, 7.6368));
        }
        SECTION("#2") {
            complex c1(-22, 22);
            auto res = 11 * c1;
            REQUIRE(res == complex(-242, 242));
        }
        SECTION("#3") {
            complex c1(-7.44, 12);
            auto res = 7.44 * c1;
            REQUIRE(res == complex(-55.3536, 89.28));
        }
    }
}
