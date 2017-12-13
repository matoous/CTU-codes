// by Matous Dzivjak <dzivjak@matous.me>, 52 lines, OH GOD I AM SO GOOD, ALMOST AS SIMON MANOUR, ALMOST...
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
int M, I, F, T;

int IllidanStormragePleaseDoWhatICanNot(int idx, int currentNode){
  if(rege[idx] == '\0')
    return 0;
  if(save.find(make_pair(idx,currentNode)) != save.end())
    return save.at(make_pair(idx,currentNode));
  int bestScore = 0, score;
  for(auto edge : graph[currentNode]){
    int tmp = idx;
    while(rege[tmp] != '\0' && rege[tmp+1] == '*'){
      if(rege[tmp] == edge.LETTER)
        if((score = 1 + IllidanStormragePleaseDoWhatICanNot(tmp, edge.TO)) > bestScore)
          bestScore = score;
      tmp += 2;
    }
    if(rege[tmp] != '\0' && rege[tmp] == edge.LETTER)
      if((score = 1 + IllidanStormragePleaseDoWhatICanNot(tmp + 1, edge.TO)) > bestScore)
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
  F = 0; // why init new variables when you have old ones :3
  for(int i = 0; i < M; i++)
    if((I = IllidanStormragePleaseDoWhatICanNot(0, i)) > F) // python style -> all on one line :3
        F = I;
  printf("%d\n", F);
  return 0;
}
