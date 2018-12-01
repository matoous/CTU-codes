// by Matous Dzivjak <dzivjmat@fel.cvut.cz>
#include <stdio.h>
#include <vector>
#include <stdlib.h>

using namespace std;

#define len(x) (int)x.size()

typedef struct movie_t {
  int premovies;
  int walks;
  int start;
  int* nexts;
} movie_t;

int movie_len, cinemas;
int x, y;
vector<vector<int>> distances;
vector<vector<movie_t>> movies;
vector<vector<int>> idex;

inline void setIfBetter(movie_t * x, movie_t * y, int dist){
  y->premovies++;
  y->walks += dist;
  if(x->premovies < y->premovies || (x->premovies == y->premovies && y->walks <= x->walks)){
    x->premovies = y->premovies;
    x->walks = y->walks;
  }
}

movie_t bestPath(int cinema, int movie){
  // we are at the end
  if(movie == len(movies[cinema]))
    return movie_t{0,0,0};

  // we already solved this movie
  if(movies[cinema][movie].premovies != 0)
    return movies[cinema][movie];

  // stay in same cinema
  auto x = bestPath(cinema, movie+1);
  setIfBetter(&movies[cinema][movie], &x, 0);

  // try other cinemas
  for(int k = 0; k < cinemas; k++){
    // skip me
    if(k == cinema) continue;

    // get first possible movie in other cinema
    auto idx =  movies[cinema][movie].nexts[k] ;
    // skip if we can't catch any movie in the other cinema
    if(idx == len(movies[k])) continue;

    x = bestPath(k, idx);
    setIfBetter(&movies[cinema][movie], &x, distances[k][cinema]);
  }
  return movies[cinema][movie];
}

int main() {
  // init
  scanf("%d %d", &cinemas, &movie_len);
  distances.resize(cinemas, vector<int>(cinemas));
  movies.resize(cinemas, vector<movie_t>(0));
  idex.resize(cinemas, vector<int>(cinemas, 0));

  // scan distances
  for(int i = 0; i < cinemas; i++)
    for(int u = 0; u < cinemas; u++)
      scanf("%d", &distances[i][u]);

  // scan movie times
  for(int i = 0; i < cinemas; i++){
    scanf("%d", &x);
    movies[i].resize(x);
    for(int u = 0; u < x; u++){
      scanf("%d", &y);
      movies[i][u] = movie_t { 0, 0, y, (int*)malloc(cinemas*sizeof(int)) };
    }
  }

  // for each movie precompute which first movie in other cinema can be visited
  for(int i = 0; i < cinemas; i++){
    for(int u = 0; u < len(movies[i]);u++){ // for each movie in cinema
      for(int k = 0; k < cinemas; k++){ // for each other cinema
        if(k == i) continue;
        while(movies[k][idex[i][k]].start < movies[i][u].start + movie_len + distances[i][k] && idex[i][k] < len(movies[k]))
          idex[i][k]++;
        movies[i][u].nexts[k] = idex[i][k];
      }
    }
  }

  // solve
  movie_t best = { 0, 0, 0 };
  for(int i = 0; i < cinemas; i++){
    auto curr = bestPath(i, 0);
    if(curr.premovies > best.premovies || (curr.premovies == best.premovies && curr.walks < best.walks))
      best = curr;
  }
  printf("%d %d\n", best.premovies, best.walks);
  return 0;
}
