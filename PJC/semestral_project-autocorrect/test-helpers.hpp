#pragma once

#include <vector>
#include <string>
#include <fstream>

size_t lines_in_file(const std::string&);
bool vector_contains(const std::vector<std::string>&, std::string);
std::vector<std::string> generateWrongWords(std::string filename);