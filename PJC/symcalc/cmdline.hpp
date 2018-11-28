#pragma once
#ifndef PJC_SYMCALC_CMDLINE_HPP
#define PJC_SYMCALC_CMDLINE_HPP

#include <mapbox/variant.hpp>
#include <string>
#include <vector>
#include <map>

namespace Commands {
    struct Print {
        enum class Format {
            Default, Prefix, Postfix, Infix
        };
        Format format = Format::Default;
        // Pokud neimplementujete bonus print, formát můžete ignorovat
    };

    struct Derive {
        std::string variable;
    };

    struct Evaluate {
        std::map<std::string, double> variables; // expr::variable_map_t, but we use this file in tests and ndo not want to depend on implementation
    };

    struct Simplify {

    };
    using Command = mapbox::util::variant<Print,Derive,Evaluate,Simplify>;
}

//! Rozdělí daný string podle oddělovače `delim` a vrátí části ve vektoru
std::vector<std::string> split(std::string const &line, char delim);

/**
 * Naparsuje příkaz. Při chybě vyhodí výjimku.
 */
Commands::Command parse_command(std::string const &str);

#endif // PJC_SYMCALC_CMDLINE_HPP
