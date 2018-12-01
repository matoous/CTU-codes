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
    }

    std::string line;
    while (getline(std::cin, line)) {
        try {
            handle_expr_line(std::cout, line, commands);
        }
        catch(...){
            std::cout << "! " << std::endl;
        }
    }

    return 0;
}
