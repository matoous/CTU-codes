// by Matous Dzivjak <dzivjak@matous.me>
#include <stdio.h>
#include <vector>
#include <limits>
#include <unistd.h>
#include <algorithm>
#include <stack>

#define PII pair<int,int>
#define VPII vector<PII>
#define COST second
#define TO first

using namespace std;

int N, K, x, y, c, subtreeWithStart = -1;
int first;
bool foundFirst = false;
vector<bool> key;
vector<VPII> graph;
vector<int> parent;
vector<bool> isInCycle;

#define CMASK 0x1FFFFFFF
#define VMASK 0x20000000
#define NMASK 0x40000000

inline bool is_visited(int x){
  return parent[x] & VMASK;
}

inline bool has_parent(int x){
  return !(parent[x] & NMASK);
}

inline int parent_of(int x){
  return parent[x] & CMASK;
}

inline void set_visited(int x){
  parent[x] |= VMASK;
}

inline void set_parent(int x, int y){
  bool visited = is_visited(x);
  parent[x] = y;
  if(visited) set_visited(x);
}

// Find the cycle in the graph
vector<int> findCycle(int F){
  parent = vector<int>(N, NMASK);
  vector<int> cnodes;
  stack<int> S;

  set_parent(F, F);
  S.push(F);

  while(!S.empty()){
    int current = S.top(); S.pop();

    for(auto edge : graph[current]) {
      if(edge.TO == parent_of(current)) continue;

      if(has_parent(edge.TO) && is_visited(edge.TO)) { // found cycle
        cnodes.push_back(edge.TO); isInCycle[edge.TO] = true;
        cnodes.push_back(current); isInCycle[current] = true;
        auto tmp = parent_of(current);
        while(tmp != edge.TO){       // traverse back whole cycle
          cnodes.push_back(tmp); isInCycle[tmp] = true;
          tmp = parent_of(tmp);
        }
        return cnodes;
      }

      set_parent(edge.TO, current);
      S.push(edge.TO);
    }
    set_visited(current);
  }
  return cnodes;
}

// Find cost of child and its childs
int childCost(int me, int parent){
  stack<int> S;
  if (me == first) foundFirst = true; // remember in which subtree we found the starting node
  int total = 0;
  for(auto edge : graph[me]){
    if(edge.TO != parent){
      auto result = childCost(edge.TO, me);
      if (result & NMASK)
        total += (result & CMASK) + 2*edge.COST;
    }
  }
  if (key[me] || total) return NMASK | total;
  else return 0; // we can forget this subtree since there is no keyserver
}

// Find total cost for given cycle subtree
int childCosts(int x){
  int total = 0;
  for(auto edge : graph[x])
    if(!isInCycle[edge.TO]){
      auto result = childCost(edge.TO, x);
      if (foundFirst){
        subtreeWithStart = x;
        foundFirst = false;
      }
      if (result & NMASK) {
        total += 2*edge.COST + (result & CMASK);
        key[x] = true;
      }
    }
  return total;
}

int main(){
  // LOAD DATA
  scanf("%d %d", &N, &K);
  key.resize(N, false);
  graph.resize(N, VPII(0));
  isInCycle.resize(N, false);
  first = numeric_limits<int>::max();
  for(int i = 0; i < N; i++){
    scanf("%d %d %d", &x, &y, &c);
    graph[x].push_back(make_pair(y,c));
    graph[y].push_back(make_pair(x,c));
  }
  for(int i = 0; i < K; i++){
    scanf("%d", &x);
    if(x < first) first = x;
    key[x] = true;
  }

  // find cycle
  auto cycleNodes = findCycle(first);

  // calculate total cost of subtrees, cost that we can't change
  int totalForKeys = 0;
  fill(parent.begin(), parent.end(), -1);
  for(auto node : cycleNodes)
    totalForKeys += childCosts(node);
  if(subtreeWithStart < 0)
    subtreeWithStart = first;

  // condensate graph on the cycle
  fill(parent.begin(), parent.end(), -1);
  int best = 0;
  vector<pair<pair<int,int>,int>> cycleEdges = vector<pair<pair<int,int>,int>>();

  auto firstNode = cycleNodes[0];
  int tmp = firstNode;
  for(auto edge : graph[firstNode]){
    if(isInCycle[edge.TO]){
      parent[edge.TO] = firstNode;
      best += edge.COST;
      cycleEdges.push_back(make_pair(make_pair(firstNode, edge.TO), edge.COST));
      tmp = edge.TO;
      break;
    }
  }
  while(tmp != subtreeWithStart){
    for(auto edge : graph[tmp]){
      if(isInCycle[edge.TO] && parent[tmp] != edge.TO){
        parent[edge.TO] = tmp;
        best += edge.COST;
        cycleEdges.push_back(make_pair(make_pair(tmp, edge.TO), edge.COST));
        tmp = edge.TO;
        break;
      }
    }
  }

  // common
  int wholeCycleCost = best;
  best += totalForKeys;
  int currentScore = wholeCycleCost*2 + totalForKeys;
  for(int i = 0; i < (int)cycleEdges.size(); i++){
    currentScore -= 2*cycleEdges[i].second;
    while(!(key[cycleEdges[i].first.second]) && i < (int)cycleEdges.size()-1){
      i++;
      if(i < (int)cycleEdges.size())
        currentScore -= 2*cycleEdges[i].second;
    }
    if(currentScore < best)
      best = currentScore;
    currentScore = wholeCycleCost*2 + totalForKeys;
  }
  printf("%d\n", best);
  return 0;
}
