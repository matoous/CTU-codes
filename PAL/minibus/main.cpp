#include <iostream>
#include <vector>
#include "Node.h"
#include "Component.h"
#include <stack>
#include <cstring>

using namespace std;

int tarOrder = 0;
int stopsNo;
int edgesNo;
int largestCost = 0;

vector<Node> nodes;
vector<Node*> open;
vector<vector<Node*>> tarComps;
Component *comps;
bool *visited;

// he protecc, he attacc but most importantly he prevent overflow on stacc
void topologicalSort(int v, stack<int>& stacc)
{
    visited[v] = true;
    auto edges = comps[v].edges.begin();
    for (int i = 0; i < comps[v].edges.size(); ++i) {
        if (!visited[i]) topologicalSort(*edges, stacc);
        edges++;
    }
    stacc.push(v);
}

void tarjan(Node *node) {
    node->tarOrder = tarOrder;
    node->tarLow = tarOrder;
    open.push_back(node);
    node->open = true;
    node->visited = true;
    tarOrder++;

    for (int neighId : node->edges) {
        Node *neigh = &nodes[neighId];

        if (!neigh->visited) {
            tarjan(neigh);
            node->tarLow = std::min(node->tarLow, neigh->tarLow);

        } else if (neigh->open) {
            node->tarLow = std::min(node->tarLow, neigh->tarOrder);
        }
    }

    if (node->tarLow == node->tarOrder) {
        std::vector<Node*> comp;
        int compId = tarComps.size();
        Node *curr = nullptr;

        do {
            curr = open.back();
            open.pop_back();
            curr->open = false;
            curr->compId = compId;
            comp.push_back(curr);
        } while (curr->id != node->id);

        tarComps.push_back(comp);
    }
}

int main() {
    std::cin >> stopsNo >> edgesNo;

    for (int i = 0; i < stopsNo; ++i) {
        nodes.emplace_back(i);
    }

    for (int i = 0; i < edgesNo; ++i) {
        int from, to;
        std::cin >> from >> to;
        nodes[from].edges.push_back(to);
    }

    for (Node &node : nodes) {
        if (!node.visited) {
            tarjan(&node);
        }
    }

    // If an outgoing or inbound edge to/from another component is present, mark as unsafe 
    for (Node &node : nodes) {
        for (int edge : node.edges) {
            if (nodes.at(edge).compId != node.compId) {
                node.safe = false;
                nodes.at(edge).safe = false;
            }
        }
    }

    // Create graph from components
    comps = new Component[tarComps.size()];
    for (int j = 0; j < tarComps.size(); ++j) {
        for (Node *node : tarComps.at(j)) {
            if (node->safe) {
                comps[j].cost++;
            } else {
                // Find components accessible from this node
                for (int edge : node->edges) {
                    if (nodes.at(edge).compId != j) comps[j].edges.insert(nodes.at(edge).compId);
                }
            }
        }
    }

    int *costs = (int*) calloc(tarComps.size(), sizeof(int));
    visited = new bool[tarComps.size()];
    memset(visited, false, sizeof(bool) * tarComps.size());

    // Dyn. prog. approach solving the max. cost to enter a node
    stack<int> stacc;

    for (int i = 0; i < tarComps.size(); ++i) {
        if(!visited[i]) {
            topologicalSort(i, stacc);
        }
    }

    while (!stacc.empty()) {
        int curr = stacc.top();
        stacc.pop();
        int cost = costs[curr] + comps[curr].cost;

        for (int edge : comps[curr].edges) {
            if (costs[edge] < cost) {
                costs[edge] = cost;
            }
        }

    }

    // Looking for the largest cost
    for (int k = 0; k < tarComps.size(); ++k) {
        int curr = costs[k] + comps[k].cost;
        if (curr > largestCost) largestCost = curr;
    }

    std::cout << largestCost << std::endl;

    return 0;
}