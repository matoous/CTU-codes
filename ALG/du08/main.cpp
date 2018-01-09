#include <stdio.h>
#include <vector>
#include <limits.h>

#define arr std::vector
#define me x1][y1][x2][y2 // 😱

int N, M;
arr<arr<arr<arr<int>>>> bars;
arr<arr<int>> colors;

int split(int x1, int y1, int x2, int y2){
  if(bars[me])
    return bars[me];
  int color = 2;
  bool mono = true;
  for(int i = x1; i <= x2 && mono; i++){
    for(int u = y1; u <= y2; u++){
      if(color != 2 && colors[i][u] != 2 && colors[i][u] != color){
        mono = false;
        break;
      } else if (color == 2 && colors[i][u] != 2){
        color = colors[i][u];
      }
    }
  }
  if(mono)
    return (bars[me] = 1);
  int pieces = INT_MAX, val;
  for(int i = x1; i < x2; i++) // horizontal splits
    if((val = split(x1, y1, i, y2) + split(i+1, y1, x2, y2)) < pieces)
      pieces = val;
  for(int u = y1; u < y2; u++) // vertical splits
    if((val = split(x1, y1, x2, u) + split(x1, u+1, x2, y2)) < pieces)
      pieces = val;
  return (bars[me] = pieces);
}

int main(){
  scanf("%d %d", &M, &N);
  colors.resize(M, arr<int>(N));
  bars.resize(M, arr<arr<arr<int>>>(N, arr<arr<int>>(M, arr<int>(N, 0))));
  for(int i = 0; i < M; i++)
    for(int u = 0; u < N; u++)
      scanf("%d", &colors[i][u]);
  printf("%d\n", split(0, 0, M-1, N-1));
  return 0;
}
