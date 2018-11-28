#include "tests.hpp"
#include "runner.hpp"
#include "cmdline.hpp"

#include "catch.hpp"

#if !defined(RUNNER)
# error Compile this only in runner configurations.
#endif

namespace {
    struct Result {
        std::vector<std::string> inputs;
        std::vector<std::string> expected;
        std::vector<std::string> lines;
        int code;
    };
    Result run_cmd(TestCmdList const &cmd) {
        std::vector<std::string> inputs;
        std::vector<std::string> expected;
        for (const auto &ie : cmd.tests) {
            inputs.push_back(ie.input);
            expected.push_back(ie.expected);
        }
        auto code_lines = process(cmd.commands, inputs);
        int code = code_lines.first;
        const auto &lines = code_lines.second;
        return {std::move(inputs), std::move(expected), std::move(lines), code};
    }
    Result run_cmd_err(TestCmdErrList const &cmd) {
        std::vector<std::string> inputs;
        for (const auto &ie : cmd.tests) {
            inputs.push_back(ie);
        }
        auto code_lines = process(cmd.commands, inputs);
        int code = code_lines.first;
        const auto &lines = code_lines.second;
        return {std::move(inputs), {}, std::move(lines), code};
    }
}

/* We assign assertions to sections based on line number, so unfortunately the checking
 * code must be duplicated to have distinct line numbers on the CHECKs.
 */

TEST_CASE("Complex: print", "[.long][parse_write][valid]") {
    const auto &cmd = test_cmd_print_valid;
    auto r = run_cmd(cmd);

    CHECK(r.code == 0);
    INFO("Commands: " << fmt_range(cmd.commands, " "));
    {
        //INFO("Output: " << fmt_range(res, "\n"));
        CHECK(r.lines.size() == r.expected.size());
    }
    if (r.lines.size() == r.expected.size()) {
        for (size_t i=0; i<r.lines.size(); ++i) {
            INFO("Expression: " << r.inputs[i]);
            CHECK(r.lines[i] == r.expected[i]);
        }
    }
}

TEST_CASE("Complex: bonus print", "[.long][bonus_print]") {
    for (const auto &cmd : test_cmd_print_bonus) {
        auto r = run_cmd(cmd);
        CHECK(r.code == 0);
        INFO("Commands: " << fmt_range(cmd.commands, " "));
        {
            //INFO("Output: " << fmt_range(res, "\n"));
            CHECK(r.lines.size() == r.expected.size());
        }
        if (r.lines.size() == r.expected.size()) {
            for (size_t i=0; i<r.lines.size(); ++i) {
                INFO("Expression: " << r.inputs[i]);
                CHECK(r.lines[i] == r.expected[i]);
            }
        }
    }
}

TEST_CASE("Complex: error handling", "[.long][errors]") {
    run_cmd_err(test_cmd_print_err);
    for (const auto &cmd : test_cmd_evaluate_bad) {
        auto r = run_cmd_err(cmd);
        CHECK(r.code == 0);
        INFO("Commands: " << fmt_range(cmd.commands, " "));
        CHECK(r.lines.size() == r.inputs.size());
        if (r.lines.size() == r.inputs.size()) {
            for (size_t i=0; i<r.lines.size(); ++i) {
                INFO("Expression: " << r.inputs[i]);
                INFO("Expected error, got " << r.lines[i]);
                bool error_detected = !r.lines[i].empty() && r.lines[i][0] == '!';
                CHECK(error_detected);
            }
        }
    }
}

TEST_CASE("Complex: evaluate (valid expressions)", "[.long][evaluate][valid]") {
    for (const auto &cmd : test_cmd_evaluate_good) {
        auto r = run_cmd(cmd);
        CHECK(r.code == 0);
        INFO("Commands: " << fmt_range(cmd.commands, " "));
        {
            //INFO("Output: " << fmt_range(res, "\n"));
            CHECK(r.lines.size() == r.expected.size());
        }
        if (r.lines.size() == r.expected.size()) {
            for (size_t i=0; i<r.lines.size(); ++i) {
                INFO("Expression: " << r.inputs[i]);
                CHECK(r.lines[i] == r.expected[i]);
            }
        }
    }
}

TEST_CASE("Complex: derive", "[.long][derive][valid]") {
    for (const auto &cmd : test_cmd_derive) {
        auto r = run_cmd(cmd);
        CHECK(r.code == 0);
        INFO("Commands: " << fmt_range(cmd.commands, " "));
        {
            //INFO("Output: " << fmt_range(res, "\n"));
            CHECK(r.lines.size() == r.expected.size());
        }
        if (r.lines.size() == r.expected.size()) {
            for (size_t i=0; i<r.lines.size(); ++i) {
                INFO("Expression: " << r.inputs[i]);
                CHECK(r.lines[i] == r.expected[i]);
            }
        }
    }
}

TEST_CASE("Complex: simplify", "[.long][simplify][valid]") {
    const auto &cmd = test_cmd_simplify;
    auto r = run_cmd(cmd);
    CHECK(r.code == 0);
    INFO("Commands: " << fmt_range(cmd.commands, " "));
    {
        //INFO("Output: " << fmt_range(res, "\n"));
        CHECK(r.lines.size() == r.expected.size());
    }
    if (r.lines.size() == r.expected.size()) {
        for (size_t i=0; i<r.lines.size(); ++i) {
            INFO("Expression: " << r.inputs[i]);
            CHECK(r.lines[i] == r.expected[i]);
        }
    }
}
