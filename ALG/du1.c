#include <stdio.h>
#include <stdlib.h>

#define MINERALS first
#define TOXIC second
#define IUCELL grid[i][u]
#define MIN(x,y) ((x < y) ? (x):(y))

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
    int m, n;
    scanf("%d %d", &m, &n);
    node** grid = (node**)malloc(m*sizeof(node*));
    for(int i = 0; i < m; i++) {
        grid[i] = (node*)calloc(n, sizeof(node));
        for(int u = 0; u < n; u++){
            scanf("%d", &IUCELL.self);
            IUCELL.lefts = make_pair();
            IUCELL.ups = make_pair();
            if (u) {
                IUCELL.lefts = grid[i][u-1].lefts;
                if (grid[i][u-1].self == 2) IUCELL.lefts.TOXIC++;
                else if (grid[i][u-1].self == 1) IUCELL.lefts.MINERALS++;
            }
            if (i) {
                IUCELL.ups = grid[i-1][u].ups;
                if (grid[i-1][u].self == 2) IUCELL.ups.TOXIC++;
                else if (grid[i-1][u].self == 1) IUCELL.ups.MINERALS++;
            }
        }
    }

    int max = 0, lower_bound = 1;
    for(int i = 0; i < m; i++) for(int u = 0; u < n; u++){
            node current = grid[i][u];
            int ct = current.lefts.TOXIC + current.ups.TOXIC, cm = current.lefts.MINERALS + current.ups.MINERALS;
            for(int j = MIN(m-i, n-u)-1; j >= lower_bound; j--){
                int minerals = grid[i+j][u].ups.MINERALS - grid[i+j][u].lefts.MINERALS + grid[i][u+j].lefts.MINERALS - grid[i][u+j].ups.MINERALS + (current.self == 1 ? -1 : 0) + grid[i+j][u+j].ups.MINERALS + grid[i+j][u+j].lefts.MINERALS + (grid[i+j][u+j].self == 1 ? 1 : 0) - cm;
                int toxic = grid[i+j][u].ups.TOXIC - grid[i+j][u].lefts.TOXIC + grid[i][u+j].lefts.TOXIC - grid[i][u+j].ups.TOXIC + (current.self == 2 ? -1 : 0) + grid[i+j][u+j].ups.TOXIC + grid[i+j][u+j].lefts.TOXIC + (grid[i+j][u+j].self == 2 ? 1 : 0) - ct;
                if (toxic*2 > minerals) continue;
                if (minerals > max){
                    max = minerals;
                    lower_bound = max/4;
                }
            }
        }

    printf("%d\n", max);

    return 0;
}