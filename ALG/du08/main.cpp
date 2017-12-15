// by Matous Dzivjak <dzivjak@matous.me>
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
// some crazy shit here, fits on 48 lines for whole file
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
