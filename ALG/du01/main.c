#include <stdio.h>
#include <stdlib.h>

#define MINERALS first
#define TOXIC second
#define MIN(x,y) ((x < y) ? (x):(y))
#define IS_TOXIC(x) (x.self==2)
#define IS_MINERAL(x) (x.self==1)

typedef struct pair {
    int first, second;
} pair;

pair make_pair(){
    return (pair){.first = 0, .second = 0};
}

typedef struct node {
    int self;
    pair ups, lefts;
} node;

int main(){
    // variables
    int m, n;
    scanf("%d %d", &m, &n);
    node** grid = (node**)malloc(m*sizeof(node*));

    for(int i = 0; i < m; i++) {
        // alloc this row
        grid[i] = (node*)calloc(n, sizeof(node));
        for(int u = 0; u < n; u++){
            scanf("%d", &grid[i][u].self);
            grid[i][u].lefts = make_pair();
            grid[i][u].ups = make_pair();
            if (u) {
                grid[i][u].lefts = grid[i][u-1].lefts;
                if IS_TOXIC(grid[i][u-1]) grid[i][u].lefts.TOXIC++;
                else if IS_MINERAL(grid[i][u-1]) grid[i][u].lefts.MINERALS++;
            }
            if (i) {
                grid[i][u].ups = grid[i-1][u].ups;
                if IS_TOXIC(grid[i-1][u]) grid[i][u].ups.TOXIC++;
                else if IS_MINERAL(grid[i-1][u]) grid[i][u].ups.MINERALS++;
            }
        }
    }

    int max = 0, lower_bound = 1;
    for(int i = 0; i < m; i++)
        for(int u = 0; u < n; u++){
            node current = grid[i][u];
            int current_toxics = current.lefts.TOXIC + current.ups.TOXIC;
            int current_minerals = current.lefts.MINERALS + current.ups.MINERALS;
            for(int j = MIN(m-i, n-u)-1; j >= lower_bound; j--){
                int minerals = grid[i+j][u].ups.MINERALS - grid[i+j][u].lefts.MINERALS
                              + grid[i][u+j].lefts.MINERALS - grid[i][u+j].ups.MINERALS
                              + (IS_MINERAL(current) ? -1 : 0)
                              + grid[i+j][u+j].ups.MINERALS + grid[i+j][u+j].lefts.MINERALS
                              + (IS_MINERAL(grid[i+j][u+j]) ? 1 : 0)
                              - current_minerals;
                int toxic = grid[i+j][u].ups.TOXIC - grid[i+j][u].lefts.TOXIC
                              + grid[i][u+j].lefts.TOXIC - grid[i][u+j].ups.TOXIC
                              + (IS_TOXIC(current) ? -1 : 0)
                              + grid[i+j][u+j].ups.TOXIC + grid[i+j][u+j].lefts.TOXIC
                              + (IS_TOXIC(grid[i+j][u+j]) ? 1 : 0)
                              - current_toxics;
                if (toxic*2 > minerals) continue;
                if (minerals > max){
                    max = minerals;
                    // we don't have to try 4x4 squares if we already have 500 as best value
                    lower_bound = max/4;
                }
            }
        }

    printf("%d\n", max);

    return 0;
}
