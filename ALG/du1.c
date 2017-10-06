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
            // do it yourself :*
        }

    printf("%d\n", max);

    return 0;
}
