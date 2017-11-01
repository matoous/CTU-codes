// by Matous Dzivjak <dzivjak@matous.me>
#include <stdio.h>
#include <vector>
#include <queue>
#include <algorithm>

#define MIN(a,b) ((a < b) ? (a) : (b))

using namespace std;

int main(){
  vector<vector<int>> graph;
  int n, m, x, y;
  scanf("%d %d", &n, &m);
  graph.resize(n, vector<int>(0));
  for(int i = 0; i < m; i++){
    scanf("%d %d", &x, &y);
    x--, y--;
    graph[x].push_back(y);
    graph[y].push_back(x);
  }

  // Solve
  for(int i = 0; i < n; i++){
    // for each node, if its level 2, try bfs
    if(graph[i].size() == 2){
      vector<int> visited =  vector<int>(n, 0); // visits counter
      vector<int> parent = vector<int>(n, -1); // parent
      vector<int> depth = vector<int>(n, -1); // depth
      vector<int> result = vector<int>(0); // results

      bool bad = false;
      queue<int> Q;

      parent[i] = i;
      depth[i] = 0;
      Q.push(i);

      int curr_depth = 0;
      int in_curr_depth = 0;

      while(!Q.empty() && !bad){
        auto current = Q.front(); Q.pop();
        if(visited[current] == 2) continue; // we found element that's leaf or maybe not
        if(depth[current] != curr_depth){
          // we are changing depth, check that we found even number of nodes in this depth
          // fail otherwise
          if(in_curr_depth & 1 && curr_depth){
            bad = true;
            break;
          }
          curr_depth = depth[current];
          in_curr_depth = 0;
        }
        in_curr_depth++;
        for(auto next : graph[current]){
           if(visited[next] == 0){ // visiting for first time
              parent[next] = current;
              depth[next] = depth[current] + 1;
              visited[next]++;
              Q.push(next);
            } else if(visited[next] == 1 && depth[next] == depth[current] + 1){
              // visiting for second time and the depths match
              // we found a leaf
              visited[next]++;
              result.push_back(next);
            }
            else {
              // if we are visitng the node more times we know that the starting points is bad
              // also happens if we come to node with 2 different depths
              // would also happen if the tress contained cycles
              bad = true;
              break;
            }
        }
      }
      if(!bad) {
        sort(result.begin(), result.end());
        for(int i = 0; i < MIN(result.size(), 100); i++){
          if(i) printf(" %d", result[i]+1);
          else printf("%d", result[i]+1);
        }
        printf("\n");
        break;
      }
    }
  }
  return 0;
}
