#include "runner.hpp"
#include "../app.hpp" // declares handle_expr_line

#include <sstream>

std::pair<int, std::vector<std::string>>
process(std::vector<Commands::Command> const &cmds,
        std::vector<std::string> const &exprs)
{
    std::stringstream ss;
    for (const auto &e : exprs) {
        handle_expr_line(ss, e, cmds); // defined in app.cpp
    }
    auto lines = split(ss.str(), '\n');  // split is declared in cmdline.h
    if (!lines.empty() && lines.back().empty())
        lines.pop_back();
    return {0, std::move(lines)};
}
