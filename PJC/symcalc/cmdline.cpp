#include "cmdline.hpp"
#include "expr.hpp"

#include <stdexcept>
#include <sstream>

using namespace std;

vector<string> split(string const &line, char delim) {
    vector<string> res;
    if (line.empty())
        return res;
    size_t first = 0;
    size_t last;
    while ((last = line.find(delim, first)) != string::npos) {
        res.push_back(line.substr(first, last-first));
        first = last+1;
    }
    res.push_back(line.substr(first));
    return res;
}

double str_to_double(std::string const &str)
{
    std::size_t pos;
    double res = std::stod(str, &pos);
    if (pos != str.size())
        throw std::invalid_argument("numeric string ends with garbage");
    // and still, std::stod (and its underlying C implementation, strtod)
    // does one more extra thing nobody asked it to: skip (optional) leading
    // whitespace ... damn the stupid library functions
    return res;
}

Commands::Command parse_command(string const &str) {
    auto args = split(str, ':');
    auto cmd = args[0];
    args.erase(args.begin());

    auto check_nargs = [&args](std::string name, unsigned count) {
        if (args.size() != count) {
            std::stringstream ss;
            ss << name << " expects " << count << " arguments";
            throw std::runtime_error(ss.str());
        }
    };

    if (cmd == "derive") {
        check_nargs("derive", 1);
        return Commands::Derive{std::move(args[0])};
    } else if (cmd == "evaluate") {
        expr::variable_map_t variables;
        for (const auto &assignment : args) {
            auto eq = assignment.find('=');
            if (eq == string::npos)
                throw std::runtime_error("bad variable assignment in evaluate");
            string key = assignment.substr(0, eq);
            double value = str_to_double(assignment.substr(eq+1));
            variables[key] = value;
        }
        return Commands::Evaluate {std::move(variables)};
    } else if (cmd == "print") {
        check_nargs("print", 0);
        // TODO: bonus print
        return Commands::Print {};
    } else if (cmd == "simplify") {
        check_nargs("simplify", 0);
        return Commands::Simplify {};
    } else {
        throw std::runtime_error("invalid command");
    }
}
