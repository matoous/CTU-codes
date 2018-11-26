#include "test-helpers.hpp"

#include <random>
#include <algorithm>

using pjc::autocorrect;

size_t lines_in_file(const std::string& filename){
    size_t count = 0;
    std::string line;

    std::fstream infile;
    infile.open(filename);
    while (getline(infile, line))
    count++;

    infile.close();
    return count;
}