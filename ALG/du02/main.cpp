#include <iostream>
#include <vector>
#include <map>
#include <limits>

#define VB vector<bool>
#define VI vector<int>
#define PIVI pair<int, VI>
#define VN vector<node_t>
#define NODE(x) (node_t){.as_node=false, .num=(x)}
#define NODES second
#define WEIGHT first

using namespace std;

typedef struct node_t {
    bool as_node;
    int num;
} node_t;

VB visited;
vector<PIVI> graph;
vector<VI> cost;
map<pair<VN, VI>, int> saver;

bool operator <(const node_t& x, const node_t& y) {
    return x.num < y.num;
}

int c;

// DFS for cleaning cycles in graph
int dfs(int a){
    visited[a] = true;
    VI real_children;
    for (auto node : graph[a].NODES){
        if (!visited[node]) {
            real_children.push_back(node);
            graph[a].WEIGHT += dfs(node);
        }
    }
    graph[a].NODES = real_children;
    return graph[a].WEIGHT;
}

// removes all cycles from graph
void clean_graph(){
    visited.resize(c, false);
    dfs(0);
}

int solve(vector<node_t> nodes, vector<int> positions){
    // return value if we already solved this one
    auto hv = saver.find(make_pair(nodes, positions));
    if(hv != saver.end())
        return hv->second;

    // one position and one number, nothing we can do
    if(nodes.size() == 1 && positions.size() == 1)
        return cost[positions[0]][nodes[0].num];

    // we ended up on one node with children
    if(nodes.size() == 1){
        vector<node_t> nn;
        // add current node but WITHOUT children
        nodes[0].as_node = true;
        nn.push_back(nodes[0]);
        // add all children as separate nodes
        for (int i : graph[nodes[0].num].NODES)
            nn.push_back(NODE(i));
        // solve and save
        auto val = solve(nn, positions);
        saver[make_pair(nodes, positions)] = val;
        return val;
    }

    // select one as first
    int min = numeric_limits<int>::max();
    for(int i = 0; i < nodes.size(); ++i){
        VI first_part;
        VI remaining_part;
        for(int u = 0; u < positions.size(); u++){
            if(u < (nodes[i].as_node ? 1 : graph[nodes[i].num].WEIGHT))
                first_part.push_back(positions[u]);
            else
                remaining_part.push_back(positions[u]);
        }
        VN remaining_nodes;
        for(int u = 0; u < nodes.size(); u++)
            if (u != i)
                remaining_nodes.push_back(nodes[u]);
        int tmp = solve(VN(1, nodes[i]), first_part) + solve(remaining_nodes, remaining_part);
        if (tmp < min)
            min = tmp;
    }

    // save value and return
    saver[make_pair(nodes, positions)] = min;
    return min;
}

int main() {
    cin >> c;

    // Scan installation costs
    cost.resize(c, VI(c,0));
    for(int i = 0; i < c; i++)
        for (int j = 0; j < c; ++j)
            cin >> cost[i][j];

    // Scan graph
    graph.resize(c, PIVI(1, VI(0)));
    int from, to;
    for(int i = 0; i < c-1; i++){
        cin >> from >> to;
        graph[from].NODES.push_back(to);
        graph[to].NODES.push_back(from);
    }

    // Create tree
    clean_graph();

    // For each possible starting position on the circle
    int min = numeric_limits<int>::max();
    for(int i = 0; i < c; i++){
        VI positions(c);
        for (int z = 0; z < c; ++z)
            positions[z] = (z + i) % c;
        int u = solve(VN(1, NODE(0)), positions);
        if (u < min)
            min = u;
    }

    // Print result
    cout << min << endl;

    return 0;
}