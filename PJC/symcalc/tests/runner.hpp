#pragma once

#include "cmdline.hpp"

#include <vector>
#include <string>
#include <utility>

// Return: program error code, lines of output
std::pair<int, std::vector<std::string>>
process(std::vector<Commands::Command> const &cmds,
        std::vector<std::string> const &exprs);
