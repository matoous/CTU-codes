#include "tests.hpp"
#include <iostream>

namespace Commands {
    std::ostream& operator<<(std::ostream &os, Print const &p) {
        os << "print";
        if (p.format != Print::Format::Default)
            os << ':';

        switch (p.format) {
            case Print::Format::Prefix: os << "prefix"; break;
            case Print::Format::Postfix: os << "postfix"; break;
            case Print::Format::Infix: os << "infix"; break;
            case Print::Format::Default: break;
        }
        return os;
    };

    std::ostream& operator<<(std::ostream &os, Derive const &d) {
        return os << "derive:" << d.variable;
    }

    std::ostream& operator<<(std::ostream &os, Evaluate const &e) {
        os << "evaluate";
        for (const auto &pair : e.variables)
            os << ":" << pair.first << "=" << pair.second;
        return os;
    }

    std::ostream& operator<<(std::ostream &os, Simplify const &) {
        return os << "simplify";
    }

    std::ostream& operator<<(std::ostream &os, Command const &cmd) {
        cmd.match([&os](auto const &v) {os << v;});
        return os;
    }
}
