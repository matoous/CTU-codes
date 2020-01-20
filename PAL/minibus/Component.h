#pragma once

#include <set>

class Component {
public:
    int cost = 0;
    std::set<int> edges;
};