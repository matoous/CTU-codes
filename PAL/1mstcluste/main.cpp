#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

struct Edge {
    int u, v, c;
};

bool operator<(Edge e, Edge f) {
    return e.c < f.c;
}

vector<int> rnk, root; //rank, parents

int parent(int u) {
    if (root[u] == u) {
        return root[u];
    } else {
        root[u] = parent(root[u]); //parent(ROOT[u], not only u)
        return root[u];
    }
}

bool find(int u, int v) {
    return parent(u) == parent(v);
}

void join(int u, int v) {
    int x = parent(u), y = parent(v);
    if (x != y) {
        if (rnk[x] < rnk[y]) root[x] = root[y];
        else if (rnk[x] > rnk[y]) root[y] = root[x];
        else {
            ++rnk[x];
            root[y] = root[x];
        }
    }
}

long long int kruskal(int num_nodes, vector<Edge> &edges) {
    sort(edges.begin(), edges.end());
    long long int mst_cost = 0;
    int added_edges = 0;
    for (const Edge &e : edges) {
        if (!find(e.u, e.v)) {
            mst_cost += e.c;
            join(e.u, e.v);
            ++added_edges;
            if (added_edges == num_nodes - 1) break;
        }
    }
    return mst_cost;
}

int main() {
    ios::sync_with_stdio(false);

    // Rows, Columns, Vertexes with potential, Extra edges
    int R, C, P, K;
    std::cin >> R >> C >> P >> K;

    // save best potentials and distances
    auto potentials = vector<int>(C * R, numeric_limits<int>::max());
    auto distances = vector<int>(C * R, numeric_limits<int>::max());

    // Load vertexes with potential
    auto vertexesWithPotential = vector<int>(P);
    for (int i = 0; i < P; ++i) {
        int r, c, p;
        std::cin >> r >> c >> p;
        // input is indexed from 1 so subtract 1
        auto idx = (r - 1) * C + (c - 1);
        potentials[idx] = p, distances[idx] = 0;
        vertexesWithPotential[i] = idx;
    }

    // Load extra edges
    auto extras = vector<int>(R * C, numeric_limits<int>::min());
    for (int i = 0; i < K; ++i) {
        int r1, c1, r2, c2;
        std::cin >> r1 >> c1 >> r2 >> c2;
        // again input is indexed from 1 so subtract 1
        int from = (r1 - 1) * C + (c1 - 1), to = (r2 - 1) * C + (c2 - 1);
        extras[from] = to, extras[to] = from;
    }

    // BFS from multiple nodes
    auto wave = vector<int>(0);
    for (auto &v : vertexesWithPotential) {
        wave.emplace_back(v);
    }

    int waveNum = 0;
    while (!wave.empty()) {
        ++waveNum;
        auto nextWave = vector<int>(0);
        for (const auto &curr : wave) {
            for (const auto &mod : vector<int>{C, -C, 1, -1, extras[curr] - curr}) {
                auto newIdx = curr + mod;
                // we ended outside the graph
                if (newIdx >= R * C || newIdx < 0) continue;
                // we overflowed the row with -1 or +1 index change
                if (newIdx / C != curr / C && (mod == 1 || mod == -1)) continue;
                // not yet visited
                if (distances[newIdx] > distances[curr] + 1) {
                    nextWave.push_back(newIdx);
                    distances[newIdx] = distances[curr] + 1;
                }
                if (distances[newIdx] == distances[curr] + 1 && potentials[newIdx] > potentials[curr]) {
                    potentials[newIdx] = potentials[curr];
                }
            }
        }
        wave = nextWave;
    }

    // it's Kruskal time
    vector<Edge> edges(0);
    rnk = vector<int>(R * C, 0);
    root = vector<int>(R * C, 0);
    for (int idx = 0; idx < R * C; ++idx) {
        rnk[idx] = 1, root[idx] = idx;
        // use only down / right edges so we don't have some edges twice
        if (idx % C < C - 1) {
            edges.push_back(
                    {idx, idx + 1, distances[idx] + distances[idx + 1] + abs(potentials[idx] - potentials[idx + 1])}
            );
        }
        if (idx / C < R - 1) {
            edges.push_back(
                    {idx, idx + C, distances[idx] + distances[idx + C] + abs(potentials[idx] - potentials[idx + C])}
            );
        }
        // add the extra edge (if exists)
        if (extras[idx] != numeric_limits<int>::min()) {
            int to = extras[idx];
            edges.push_back(
                    {idx, to, distances[idx] + distances[to] + abs(potentials[idx] - potentials[to])}
            );
        }
    }

    auto mst_cost = kruskal(R * C, edges);

    printf("%lld\n", mst_cost);

    return 0;
}
