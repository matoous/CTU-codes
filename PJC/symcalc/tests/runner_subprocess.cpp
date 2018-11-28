#include "runner.hpp"
#include "subprocess.hpp"
#include "tests.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <string.h>
#include <cstdlib> // std::getenv

std::string to_string(Commands::Command const &e) {
    std::stringstream ss;
    ss << e;
    return ss.str();
}

std::pair<int, std::vector<std::string>>
process(std::vector<Commands::Command> const &cmds,
        std::vector<std::string> const &exprs)
{
    std::vector<std::string> a;
    std::transform(begin(cmds), end(cmds), std::back_inserter(a), to_string);

    const char *test_cmd = std::getenv("TEST_CMD");
    if (!test_cmd)
        throw std::runtime_error("TEST_CMD env variable not defined");

    std::string cmd = test_cmd;
    for (const auto &s : a)
        (cmd += ' ') += s;

    subprocess::Process p(cmd.c_str());

    char line[1024];
    std::vector<std::string> res;

    for (const auto &expr : exprs) {
        fprintf(p.pstdin(), "%s\n", expr.c_str());
    }
    p.close_stdin();

    while (fgets(line, sizeof(line), p.pstdout())) {
        size_t len = strlen(line);
        if (len)
            --len; // do not include \n
        res.push_back(std::string(line, len));
    }
    p.close_stdout();
    int errcode = p.wait();

    return {errcode, res};
}
