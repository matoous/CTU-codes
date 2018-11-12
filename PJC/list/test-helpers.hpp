#pragma once

#include "list.hpp"
#include <vector>
#include <iterator>

void append_to_list(pjc::list& l, std::vector<double> elems);

auto rbegin(const pjc::list& l) -> std::reverse_iterator<pjc::list::const_iterator>;

auto rend(const pjc::list& l) -> std::reverse_iterator<pjc::list::const_iterator>;

bool list_equal(const pjc::list& l, const std::vector<double>& numbers);
int generate_int();
std::vector<double> generate_data(size_t sz);
