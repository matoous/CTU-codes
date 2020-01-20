#pragma once

#include <vector>

class Node {
public:
    int id, compId, tarLow, tarOrder;
    bool visited = false;
    bool open = false;
    std::vector<int> edges;
    bool safe = true;

    explicit Node(int id);
    // Comparing based on the node's id
    bool operator==(const Node &n1);
};