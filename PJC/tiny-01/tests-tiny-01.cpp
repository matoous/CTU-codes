#include "catch.hpp"

#include "tiny-01.hpp"

std::vector<std::string> make_vec(std::initializer_list<std::string> items) {
    return items;
}

TEST_CASE("Single word overload", "[pluralize][serial]") {
    SECTION("Regular plurals") {
        REQUIRE(pluralize("car") == "cars");
        REQUIRE(pluralize("dog") == "dogs");
        REQUIRE(pluralize("cot") == "cots");
    }

    SECTION("Some less than regular words") {
        REQUIRE(pluralize("mouse") == "mice");
        REQUIRE(pluralize("radius") == "radii");
        REQUIRE(pluralize("firefly") == "fireflies");
    }

    SECTION("Respects capitalization") {
        REQUIRE(pluralize("Car") == "Cars");
        REQUIRE(pluralize("Mouse") == "Mice");
        REQUIRE(pluralize("German") == "Germans");
    }

    SECTION("Uncountables") {
        REQUIRE(pluralize("equipment") == "equipment");
        REQUIRE(pluralize("species") == "species");
        REQUIRE(pluralize("series") == "series");
        REQUIRE(pluralize("swine") == "swine");
    }
}

TEST_CASE("Multi word overload", "[pluralize][batch]") {
    REQUIRE(
        pluralize({"car", "dog", "cot"}) == make_vec({"cars", "dogs", "cots"})
    );
    REQUIRE(
        pluralize({"mouse", "radius", "firefly"}) == make_vec({"mice", "radii", "fireflies"})
    );
    REQUIRE(
        pluralize({"Car", "Mouse", "German"}) == make_vec({"Cars", "Mice", "Germans"})
    );
    REQUIRE(
        pluralize({"equipment", "species", "series", "swine"}) == make_vec({"equipment", "species", "series", "swine"})
    );
}
