#include "cmdline.hpp" // parse_command
#include "app.hpp" // handle_expr_line
#include "expr.hpp"

#include <vector>
#include <iostream>

int main(int argc, char *argv[])
{
    std::vector<Commands::Command> commands(argc-1);
    for(int i = 0; i < argc-1; i++){
        commands[i] = parse_command(argv[i+1]);
        std::cout << argv[i+1] << std::endl;
    }

    auto x = create_expression_tree("(1) + 3-5");
    std::cout << x << std::endl;

    x = create_expression_tree("(1) + 3-5 + 5*(4/2)");
    std::cout << x << std::endl;

    x = create_expression_tree("(1) + 3-5 + 5*(4/2) +4 -log((2+2)^2) * 2^2 - sin(x+1)");
    std::cout << x << std::endl;

    x = create_expression_tree("(1+0) + (0+1) + (0+0) + (0*x) + (log(1))");
    std::cout << x << std::endl;
    std::cout << x->simplify() << std::endl;

}
