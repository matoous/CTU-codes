// by Matous Dzivjak <dzivjak@matous.me>, 48 lines, OH GOD I AM SO GOOD, ALMOST AS SIMON MANOUR, ALMOST...
#include <stdio.h>
#include <vector>
#include <map>

using namespace std;

#define EDGE pair<char, int>
#define LETTER first
#define TO second

vector<vector<EDGE>> graph;
map<pair<int, int>, int> save;
char rege[21], C;
int M, I, F, T, now, best = 0;

int Find(int idx, int currentNode){
  if(save.find(make_pair(idx,currentNode)) != save.end())
    return save.at(make_pair(idx,currentNode));
  int bestScore = 0, score;
  for(auto edge : graph[currentNode]){
    int tmp = idx;
    while(rege[tmp] != '\0' && rege[tmp+1] == '*'){
      if(rege[tmp] == edge.LETTER)
        if((score = 1 + Find(tmp, edge.TO)) > bestScore)
          bestScore = score;
      tmp += 2;
    }
    if(rege[tmp] != '\0' && rege[tmp] == edge.LETTER)
      if((score = 1 + Find(tmp + 1, edge.TO)) > bestScore)
        bestScore = score;
  }
  save[make_pair(idx, currentNode)] = bestScore;
  return bestScore;
}

int main() {
  scanf("%s %d %d", rege, &M, &I);
  graph.resize(M);
  for(int i = 0; i < I; i++){
    scanf("%d %d %c", &F, &T, &C);
    graph[--F].push_back(EDGE(C, --T));
  }
  for(int i = 0; i < M; i++)
    if((now = Find(0, i)) > best)
        best = now;
  printf("%d\n", best);
  return 0;
}
