#ifdef RUNNER
# include "runner.hpp"
#else
# include "expr.hpp"
#endif

#include "tests.hpp"
#include "cmdline.hpp"

#include "catch.hpp"
#include <sstream>


namespace {
#ifdef RUNNER
// The exception type is lost, so we do not check it
# define CHECK_THROWS_MAYBEAS(expr, exc_type) CHECK_THROWS(expr)

    void check_lines_one(std::pair<int, std::vector<std::string>> const &code_lines) {
        int errcode = code_lines.first;
        const auto &lines = code_lines.second;

        INFO("Program output lines: {\"" << fmt_range(lines, "\", \"") << "\"}");
        CHECK(errcode == 0);
        CHECK(lines.size() == 1);
        if (lines.empty())
            return;
        const auto &line = lines[0];

        // throw an exception if the output line starts with "!"
        if (!line.empty() && line[0] == '!')
            throw std::runtime_error(line);
    }
    using namespace Commands;
    std::string parse_and_write(std::string input, Commands::Print::Format fmt = Commands::Print::Format::Default) {
        auto code_lines = process({Print{fmt}}, {std::move(input)});
        check_lines_one(code_lines);
        return code_lines.second.at(0);
    }
    std::string test_simplify(std::string input) {
        auto code_lines = process({Simplify{}, Print{}}, {std::move(input)});
        check_lines_one(code_lines);
        return code_lines.second.at(0);
    }
    std::string test_derive(std::string input, const char *var) {
        auto code_lines = process({Derive{var}, Print{}}, {std::move(input)});
        check_lines_one(code_lines);
        return code_lines.second.at(0);
    }
    std::string test_derive_simplify(std::string input, const char *var) {
        auto code_lines = process({Derive{var}, Simplify{}, Print{}}, {std::move(input)});
        check_lines_one(code_lines);
        return code_lines.second.at(0);
    }
    double test_evaluate(std::string input, std::map<std::string, double> const &vm) {
        auto code_lines = process({Evaluate{vm}}, {std::move(input)});
        check_lines_one(code_lines);
        return std::stod(code_lines.second.at(0));
    }
#else
# define CHECK_THROWS_MAYBEAS(expr, exc_type) CHECK_THROWS_AS(expr, exc_type)
    std::string to_string(expr e) {
        std::stringstream ss;
        ss << e;
        return ss.str();
    }
    std::string parse_and_write(std::string input) {
        expr e = create_expression_tree(input);
        return to_string(e);
    }
    std::string test_simplify(std::string input) {
        expr e = create_expression_tree(input);
        e = e->simplify();
        return to_string(e);
    }
    std::string test_derive(std::string input, const char *var) {
        expr e = create_expression_tree(input);
        e = e->derive(var);
        return to_string(e);
    }
    std::string test_derive_simplify(std::string input, const char *var) {
        expr e = create_expression_tree(input);
        e = e->derive(var);
        e = e->simplify();
        return to_string(e);
    }
    double test_evaluate(std::string input, expr::variable_map_t const &vm) {
        expr e = create_expression_tree(input);
        return e->evaluate(vm);
    }
#endif
}

struct TestDeriveData {
    const char *input;
    const char *variable;
    const char *expected;
};

struct TestEvaluateData {
    const char *input;
    std::map<std::string, double> variables;
    double expected;
};


TEST_CASE("Parsing and writing", "[parse_write][valid]") {
    static const TestData s_test_data[] = {
        { "1",                "1" },
        { "12.34",            "12.34" },
        { "1.26e6",           "1.26e+06" },
        { "1.26e-6",          "1.26e-06" },
        { "variable",         "variable" },
        { "sin(x)",           "(sin x)" },
        { "cos(x)",           "(cos x)" },
        { "log(x)",           "(log x)" },
        { "1+1",              "(+ 1 1)" },
        { "1-1",              "(- 1 1)" },
        { "1*1",              "(* 1 1)" },
        { "1/1",              "(/ 1 1)" },
        { "1^1",              "(^ 1 1)" },

        // should matter only for evaluate, not here
        { "log(0)",           "(log 0)" },

        // whitespace
        { "sin ( x ) ",       "(sin x)" },
        { " 1   \t + 1  ",    "(+ 1 1)" },

        // brackets
        { "(1)", "1" },
        { "(1+(((sin(x)))))", "(+ 1 (sin x))" },

        // precedence
        { "1+2*3",            "(+ 1 (* 2 3))" },
        { "1+2*3*4",          "(+ 1 (* (* 2 3) 4))" },
        { "1*2^3",            "(* 1 (^ 2 3))" },
        { "1*2^3^4",          "(* 1 (^ 2 (^ 3 4)))" },

        { "(1+2)*3*4",        "(* (* (+ 1 2) 3) 4)" },
        { "(1*2)^3^4",        "(^ (* 1 2) (^ 3 4))" },

        { "2^sin(1+1)",       "(^ 2 (sin (+ 1 1)))" },
    };
    for (const auto &test : s_test_data) {
        SECTION(test.input) {
            INFO("For input: " << test.input);
            REQUIRE(parse_and_write(test.input) == test.expected);
        }
    }
}

TEST_CASE("Simplify", "[simplify][valid]") {
    static const TestData s_test_data[] = {
        // rules
        { "0+x",           "x" },
        { "x+0",           "x" },
        { "x-0",           "x" },
        { "0*x",           "0" },
        { "x*0",           "0" },
        { "1*x",           "x" },
        { "x*1",           "x" },
        { "0/0",           "(/ 0 0)" },
        { "0/x",           "0" },
        { "x/1",           "x" },
        { "x^1",           "x" },
        { "0^0",           "1" },
        { "x^0",           "1" },
        { "0^x",           "0" },
        { "log(1)",        "0" },

        // depth
        { "0*x + x*0",     "0" },
        { "0*x - x*0",     "0" },
        { "(0*x) * (x*0)", "0" },
        { "(0*x) / (1+0)", "0" },
        { "(0*x) ^ (x*0)", "1" },
        { "log(1*1)",      "0" },
        { "sin(1*1)",      "(sin 1)" },
        { "cos(1*1)",      "(cos 1)" },

        // indempotence
        { "42",            "42" },
        { "xxx",           "xxx" },
        { "1+x",           "(+ 1 x)" },
        { "1-x",           "(- 1 x)" },
        { "2*x",           "(* 2 x)" },
        { "2/x",           "(/ 2 x)" },
        { "2^x",           "(^ 2 x)" },
        { "sin(1)",        "(sin 1)" },
        { "cos(1)",        "(cos 1)" },
        { "log(2)",        "(log 2)" },

        { "2*x + x*2",     "(+ (* 2 x) (* x 2))" },
        { "2*x - x*2",     "(- (* 2 x) (* x 2))" },
        { "(2*x) * (x*2)", "(* (* 2 x) (* x 2))" },
        { "(2*x) / (x*2)", "(/ (* 2 x) (* x 2))" },
        { "(2*x) ^ (x*2)", "(^ (* 2 x) (* x 2))" },

        { "2*1 + 2*1",     "(+ 2 2)" },
        { "2*1 - 2*1",     "(- 2 2)" },
        { "(2*1) * (2*1)", "(* 2 2)" },
        { "(2*1) / (2*1)", "(/ 2 2)" },
        { "(2*1) ^ (2*1)", "(^ 2 2)" },
    };
    for (const auto &test : s_test_data) {
        SECTION(test.input) {
            INFO("For input: " << test.input);
            REQUIRE(test_simplify(test.input) == test.expected);
        }
    }
}

TEST_CASE("Derive", "[derive][valid]") {
    static const TestDeriveData s_test_data[] = {
        // input, variable, expected
        { "42",   "x",       "0" },
        { "x",    "x",       "1" },
        { "x",    "y",       "0" },
        { "abra", "cadabra", "0" },

        { "x+1",    "x", "(+ 1 0)" },
        { "x-1",    "x", "(- 1 0)" },
        { "x*2",    "x", "(+ (* 1 2) (* x 0))" },
        { "x/2",    "x", "(/ (- (* 1 2) (* x 0)) (^ 2 2))" },
        { "x^2",    "x", "(* (^ x 2) (+ (/ (* 1 2) x) (* (log x) 0)))" },
        { "sin(x)", "x", "(* (cos x) 1)" },
        { "cos(x)", "x", "(* (- 0 (sin x)) 1)" },
        { "log(x)", "x", "(/ 1 x)" },
    };

    for (const auto &test : s_test_data) {
        SECTION(test.input) {
            INFO("For input: " << test.input);
            REQUIRE(test_derive(test.input, test.variable) == test.expected);
        }
    }
}

TEST_CASE("Derive and simplify", "[simplify]") {
    REQUIRE(test_derive_simplify("log(sin(2*x))", "x") == "(/ (* (cos (* 2 x)) 2) (sin (* 2 x)))");
}

TEST_CASE("Evaluate", "[evaluate][valid]") {
    static const TestEvaluateData s_test_data[] = {
        { "x+x*var",    {{"x", 3}, {"var", 13}}, 3+3*13 },
        { "x+x*var",    {{"x", 2}, {"var", 9}, {"asdasd", 42}}, 2+2*9 },

        { "42",         {},          42 },
        { "x",          {{"x", 38}}, 38 },
        { "sin(x)",     {{"x", 0}},   0 },
        { "cos(x)",     {{"x", 0}},   1 },
        { "log(x)",     {{"x", 1}},   0 },
        { "1+2",        {},           3 },
        { "1-2",        {},          -1 },
        { "2*3",        {},           6 },
        { "3/2",        {},           1.5 },
        { "3^2",        {},           9 },
        { "25^(0-1/2)", {},           0.2 },
    };

    for (const auto &test : s_test_data) {
        SECTION(test.input) {
            INFO("For input: " << test.input);
            REQUIRE(test_evaluate(test.input, test.variables) == test.expected);
        }
    }
}

#if !defined(RUNNER)
TEST_CASE("Equals", "[equals]") {
    auto E = create_expression_tree;

    SECTION("Same type") {
        CHECK(E("42") == E("42"));
        CHECK(E("42") != E("88"));

        CHECK(E("qqq") == E("qqq"));
        CHECK(E("qqq") != E("x"));

        CHECK(E("sin(1)") == E("sin(1)"));
        CHECK(E("sin(1)") != E("sin(2)"));

        CHECK(E("cos(x)") == E("cos(x)"));
        CHECK(E("cos(x)") != E("cos(y)"));

        CHECK(E("log(x)") == E("log(x)"));
        CHECK(E("log(x)") != E("log(y)"));
    }

    SECTION("Different types") {
        expr exprs[] = {
            E("1"), E("x"), E("sin(1)"), E("cos(1)"), E("log(1)"),
            E("1+1"),
            E("1+2"),
            E("2+1"),

            E("1-1"),
            E("1-2"),
            E("2-1"),

            E("1*1"),
            E("1*2"),
            E("2*1"),

            E("1/1"),
            E("1/2"),
            E("2/1"),

            E("1^1"),
            E("1^2"),
            E("2^1"),
        };
        for (size_t i=0; i<ARRAY_SIZE(exprs); ++i) {
            CHECK(exprs[i] == exprs[i]);
            for (size_t j=i+1; j<ARRAY_SIZE(exprs); ++j) {
                CHECK(exprs[i] != exprs[j]);
            }
        }
    }
}
#endif // !RUNNER


TEST_CASE("Error handling", "[errors]") {
#ifdef RUNNER
    #define E parse_and_write
#else
    #define E create_expression_tree
#endif

    CHECK_THROWS_MAYBEAS(E("1+(1"), parse_error);
    CHECK_THROWS_MAYBEAS(E("1+(1+2)+3)*1"), parse_error);
    CHECK_THROWS_MAYBEAS(E("1+"), parse_error);
    CHECK_THROWS_MAYBEAS(E("+"), parse_error);
    CHECK_THROWS_MAYBEAS(E(")"), parse_error);
    CHECK_THROWS_MAYBEAS(E("("), parse_error);
    CHECK_THROWS_MAYBEAS(E("()"), parse_error);
    CHECK_THROWS_MAYBEAS(E("sin(1 2)"), parse_error);
    CHECK_THROWS_MAYBEAS(E("sin()"), parse_error);
    CHECK_THROWS_MAYBEAS(E("(1+1)asd"), parse_error);
    CHECK_THROWS_MAYBEAS(E("(1+1)#"), tokenize_error);

    CHECK_THROWS_MAYBEAS(test_evaluate("log(0-1)", {}), domain_exception);
    CHECK_THROWS_MAYBEAS(test_evaluate("log(x)", {{"x", 0}}), domain_exception);
    CHECK_THROWS_MAYBEAS(test_evaluate("asd", {}), unbound_variable_exception);
    CHECK_THROWS_MAYBEAS(test_evaluate("sin", {}), unbound_variable_exception);

    CHECK_THROWS_MAYBEAS(parse_and_write("unknownfunc(1)"), unknown_function_exception);
    CHECK_THROWS_MAYBEAS(parse_and_write("sin 0"), parse_error);
}

#ifdef RUNNER
TEST_CASE("Bonus: Different print formats", "[bonus_print]") {
    static const struct {
        const char *input;
        const char *prefix;
        const char *infix;
        const char *postfix;
    } s_test_data[] = {
        { "1",                "1",        "1",        "1" },
        { "variable",         "variable", "variable", "variable" },
        { "sin(x)",           "(sin x)",  "sin(x)",     "x sin" },
        { "cos(x)",           "(cos x)",  "cos(x)",     "x cos" },
        { "log(x)",           "(log x)",  "log(x)",     "x log" },
        { "1+1",              "(+ 1 1)",  "(1 + 1)",  "1 1 +" },
        { "1-1",              "(- 1 1)",  "(1 - 1)",  "1 1 -" },
        { "1*1",              "(* 1 1)",  "(1 * 1)",  "1 1 *" },
        { "1/1",              "(/ 1 1)",  "(1 / 1)",  "1 1 /" },
        { "1^1",              "(^ 1 1)",  "(1 ^ 1)",  "1 1 ^" },

        // complex
        { "1+2*3",            "(+ 1 (* 2 3))",       "(1 + (2 * 3))",       "1 2 3 * +" },
        { "1+2*3*4",          "(+ 1 (* (* 2 3) 4))", "(1 + ((2 * 3) * 4))", "1 2 3 * 4 * +" },
        { "1*2^3",            "(* 1 (^ 2 3))",       "(1 * (2 ^ 3))",       "1 2 3 ^ *" },
        { "1*2^3^4",          "(* 1 (^ 2 (^ 3 4)))", "(1 * (2 ^ (3 ^ 4)))", "1 2 3 4 ^ ^ *" },

        { "(1+2)*3*4",        "(* (* (+ 1 2) 3) 4)", "(((1 + 2) * 3) * 4)", "1 2 + 3 * 4 *" },
        { "(1*2)^3^4",        "(^ (* 1 2) (^ 3 4))", "((1 * 2) ^ (3 ^ 4))", "1 2 * 3 4 ^ ^" },

        { "2^sin(1+1)",       "(^ 2 (sin (+ 1 1)))", "(2 ^ sin((1 + 1)))", "2 1 1 + sin ^" },
    };
    for (const auto &test : s_test_data) {
        INFO("For input: " << test.input);
        CHECK(parse_and_write(test.input, Commands::Print::Format::Prefix) == test.prefix);
        CHECK(parse_and_write(test.input, Commands::Print::Format::Infix) == test.infix);
        CHECK(parse_and_write(test.input, Commands::Print::Format::Postfix) == test.postfix);
    }
}
#endif // RUNNER
