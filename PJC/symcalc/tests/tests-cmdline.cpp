#include "catch.hpp"
#include "../cmdline.hpp"

namespace Commands {
    bool operator==(Simplify const &, Simplify const &) {return true;}
    bool operator==(Print const &, Print const &) {return true;}
    bool operator==(Derive const &a, Derive const &b) {return a.variable == b.variable;}
    bool operator==(Evaluate const &a, Evaluate const &b) {return a.variables == b.variables;}
}

TEST_CASE("split") {
    using vec = std::vector<std::string>;
    CHECK(split("", ':') == vec {});
    CHECK(split("aa", ':') == vec {"aa"});
    CHECK(split("aa:", ':') == vec {"aa", ""});
    CHECK(split(":aa", ':') == vec {"", "aa"});
    CHECK(split("aa:bbb:a", ':') == vec {"aa", "bbb", "a"});
    CHECK(split(":aa::c:", ':') == vec {"", "aa", "", "c", ""});
}

TEST_CASE("parse_command: print, simplify") {
    SECTION("valid") {
        CHECK(parse_command("print") == Commands::Print{});
        CHECK(parse_command("simplify") == Commands::Simplify{});
    }
    SECTION("invalid") {
        CHECK_THROWS(parse_command("print:asd"));
        CHECK_THROWS(parse_command("simplify:asd"));
    }
}
TEST_CASE("parse_command: evaluate") {
    CHECK(parse_command("evaluate") == Commands::Evaluate{});
    CHECK(parse_command("evaluate:x=42") == Commands::Evaluate{{{"x", 42}}});
    CHECK(parse_command("evaluate:x=42:yyy=123.45e7") ==
          Commands::Evaluate{{{"x", 42}, {"yyy", 123.45e7}}});
    CHECK_THROWS(parse_command("evaluate:x=asd"));
    CHECK_THROWS(parse_command("evaluate:x=42asd"));
    CHECK_THROWS(parse_command("evaluate:x"));
    CHECK_THROWS(parse_command("evaluate:"));
    CHECK_THROWS(parse_command("evaluate:="));
    // do not check that variable names are valid identifiers
}

TEST_CASE("parse_command: derive") {
    CHECK(parse_command("derive:x") == Commands::Derive{"x"});
    CHECK(parse_command("derive:logvariablename") == Commands::Derive{"logvariablename"});
    CHECK_THROWS(parse_command("derive"));
    CHECK_THROWS(parse_command("derive:var1:var2"));
    // do not check whether the variable name is a valid identifier
}

TEST_CASE("parse_command: invalid command") {
    CHECK_THROWS(parse_command("asdasd"));
    CHECK_THROWS(parse_command("evaluateaaa"));
    CHECK_THROWS(parse_command("evaluate :a=1"));
}
