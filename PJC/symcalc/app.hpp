#pragma once

#include "cmdline.hpp"
#include <vector>

/**
 * Rozparsuje daný výraz a vykoná nad ním dané příkazy. Veškerý výstup (který by
 * měl jít na standardní výstup) vypisuje do `os`. Funkce by měla ošetřovat výjimky.
 */
void handle_expr_line(std::ostream &os, const std::string &e, std::vector<Commands::Command> const &cmds);
