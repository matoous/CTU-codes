//
// Created by matouus on 26/11/18.
//

#include "argh.h"
#include <iostream>
#include <set>

using namespace std;

int main(int argc, char* argv[]){
    argh::parser cmdl(argv);
    set<string> allowedFlags = {"p", "parallel"};

    for(const auto& par : cmdl.flags()){
        if(allowedFlags.find(par) == allowedFlags.end())
            return 0;
    }

    if (cmdl[{"-p", "--parallel"}])
        std::cout << "parallel!\n";

    return 0;
}