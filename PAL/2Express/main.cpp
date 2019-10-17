#include <iostream>
#include <vector>
#include <stack>

using namespace std;

vector<int> in, out, labels, costs;
stack<int> s;
vector<vector<int>> graph;
int T, label;

void scc(int v) {
    in[v] = out[v] = ++T;
    s.push(v);
    for (auto u : graph[v]) {
        if (in[u] == -1) {
            scc(u);
            out[v] = min(out[v], out[u]);
        } else if (labels[u] == -1) {
            out[v] = min(out[v], in[u]);
        }
    }
    int t = -1;
    if (out[v] == in[v]) {
        while (t != v) {
            t = s.top();
            s.pop();
            labels[t] = label;
        }
        label++;
    }
}

pair<int, int> dfs(int n) {
    // price / length pair
    auto best = make_pair(0, 0);
    for (auto e : graph[n]) {
        // not in the same component and has non-decreasing cost
        if (labels[e] != labels[n] && costs[labels[e]] >= costs[labels[n]]) {
            auto curr = dfs(e);
            if (curr.first > best.first) {
                // we improved on price
                best = curr;
            } else if (curr.first == best.first && curr.second > best.second) {
                // we improved only on length
                best = curr;
            }
        }
    }
    // add current node to cost
    best.first += costs[labels[n]];
    // +1 to length of the path
    best.second++;
    return best;
}

void tarjan() {
    // in time
    in = vector<int>(graph.size(), -1);
    out = vector<int>(graph.size());
    labels = vector<int>(graph.size(), -1);

    T = 0, label = 0;
    for (int v = 0; v < graph.size(); v++) {
        if (in[v] == -1)
            scc(v);
    }
}

int main() {
    int N, M, x, y;
    cin >> N >> M;

    graph = vector<vector<int>>(N, vector<int>(0));
    for (int i = 0; i < M; i++) {
        cin >> x >> y;
        graph[x].push_back(y);
    }

    tarjan();

    // build new graph from strongly connected components
    costs = vector<int>(label, 0);
    for (int v = 0; v < graph.size(); v++) {
        ++costs[labels[v]];
        auto new_edges = vector<int>(0);
        // filter edges in the same component
        for (auto e : graph[v]) if (labels[e] != labels[v]) new_edges.push_back(e);
        graph[v] = new_edges;
    }

    // dfs from all nodes, try to find most expensive path
    auto best = make_pair(0, 0);
    for (int n = 0; n < N; n++) {
        auto res = dfs(n);
        // - one for length for the first node
        res.second--;
        // skip 0 length path
        if (res.second == 0) continue;
        if (res.first > best.first) {
            best = res; // better price
        } else if (res.first == best.first && res.second > best.second) {
            best = res; // same price, better length
        }
    }

    printf("%d %d\n", best.first, best.second);
    return 0;
}
