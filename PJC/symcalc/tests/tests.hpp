#pragma once

#include "cmdline.hpp"
#include <vector>

struct TestData {
    const char *input;
    const char *expected;
};

using tests_cmd = std::vector<TestData>;
using tests_cmd_err = std::vector<const char *>; // input only

struct TestCmdList {
    std::vector<Commands::Command> commands;
    const tests_cmd &tests;
};

struct TestCmdErrList {
    std::vector<Commands::Command> commands;
    const tests_cmd_err &tests;
};


extern TestCmdList test_cmd_print_valid;
extern TestCmdErrList test_cmd_print_err;
extern std::vector<TestCmdList> test_cmd_print_bonus;
extern TestCmdList test_cmd_simplify;
extern std::vector<TestCmdList> test_cmd_derive;
extern std::vector<TestCmdList> test_cmd_evaluate_good;
extern std::vector<TestCmdErrList> test_cmd_evaluate_bad;

namespace {
    template <typename T, size_t N>
    constexpr size_t ARRAY_SIZE(const T (&)[N]) {
        return N;
    }
    template <typename T>
    struct fmt_range_impl {
        const T &range;
        const char *delimiter;
    };
    template <typename T>
    static inline std::ostream& operator<<(std::ostream &os, fmt_range_impl<T> const &r) {
        bool first = true;
        for (const auto &v : r.range) {
            if (!first)
                os << r.delimiter;
            else
                first = false;
            os << v;
        }
        return os;
    }

    template <typename T>
    static inline fmt_range_impl<T> fmt_range(const T &r, const char *delimiter) {
        return fmt_range_impl<T> {r, delimiter};
    }
}

// stringify Command
namespace Commands {
    std::ostream& operator<<(std::ostream &os, Print const &p);
    std::ostream& operator<<(std::ostream &os, Derive const &d);
    std::ostream& operator<<(std::ostream &os, Evaluate const &e);
    std::ostream& operator<<(std::ostream &os, Simplify const &);
    std::ostream& operator<<(std::ostream &os, Command const &);
}
