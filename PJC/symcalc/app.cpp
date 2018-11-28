#include "expr.hpp"
#include "cmdline.hpp"

#include <iostream>

using namespace std;

void process_expr(std::ostream &os, expr initial_expr, vector<Commands::Command> const &commands) {
    expr e = std::move(initial_expr);
    for (const auto &cmd : commands) {
        using namespace Commands;
        cmd.match(
            [&](Commands::Derive const &derive) {
                // TODO
            },
            [&](Commands::Simplify const &) {
                // TODO
            },
            [&](Commands::Evaluate const &evaluate) {
                // TODO
            },
            [&](Commands::Print const &p) {
                // TODO
            }
        );
    }
}

void handle_expr_line(std::ostream &os, std::string const &line, vector<Commands::Command> const &commands) {
    // TODO
}
