#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "graph.h"

#define BSWAP32(n) ((n) >> 24) | (((n) << 8) & 0x00FF0000L) | (((n) >> 8) & 0x0000FF00L) | ((n) << 24)
#define SHIFT 48
#define BASE_CAP 100
#define N1 0
#define N2 1
#define COST 2

#define IS_BIG_ENDIAN (*(uint16_t *)"\0\xff" < 0x100)

/* Allocate a new graph and return a reference to it. */
graph_t* allocate_graph(){
    graph_t *graph;
    graph = (graph_t*)malloc(sizeof(graph_t));
    graph->arr = (uint32_t**)malloc(BASE_CAP*sizeof(uint32_t*));
    for(int i = 0; i < BASE_CAP; i++){
        graph->arr[i] = (uint32_t*)malloc(3*sizeof(uint32_t));
    }
    graph->capacity = BASE_CAP;
    graph->edges_count = 0;
    return graph;
}

/* Free all allocated memory and set reference to the graph to NULL. */
void free_graph(graph_t **graph){
    for(int i = 0; i < (*graph)->capacity; i++){
        free((*graph)->arr[i]);
    }
    free((*graph)->arr);
    free(*graph);
}

/* Load a graph from the text file. */
void load_txt(const char *fname, graph_t *graph){
    FILE *f = fopen( fname, "r" );
    int c;
    if(f == NULL){
        fputs("Error in opening file!\n", stderr);
    }
    uint32_t n1, n2, cos;
    while(1){
        n1 = n2 = cos = 0;
        if((c = fgetc(f)) != EOF){
            n1 += (c - SHIFT);
        } else{
            break;
        }
        while((c = fgetc(f)) && c != ' '){
            n1 *= 10; n1 += (c - SHIFT);
        }
        while((c = fgetc(f)) && c != ' '){
            n2 *= 10; n2 += (c - SHIFT);
        }
        while((c = fgetc(f)) && c != '\n'){
            cos *= 10; cos += (c - SHIFT);
        }
        graph->arr[graph->edges_count][N1] = n1;
        graph->arr[graph->edges_count][N2] = n2;
        graph->arr[graph->edges_count][COST] = cos;
        graph->edges_count++;
        if(graph->edges_count == graph->capacity){
            int pre_cap = graph->capacity;
            (graph->capacity <= 10000) ? (graph->capacity *= 2) : (graph->capacity += 10000);
            graph->arr = (uint32_t**)realloc(graph->arr,graph->capacity*sizeof(uint32_t*));
            for(int i = pre_cap; i < graph->capacity; i++){
                graph->arr[i] = (uint32_t*)malloc(3*sizeof(uint32_t));
            }
        }
    }
    fclose(f);
}

/* Load a graph from the binary file. */
void load_bin(const char *fname, graph_t *graph){
    FILE *f = fopen( fname, "rb" );
    if(f == NULL){
        fputs("Error in opening file!", stderr);
    }
    uint32_t n1, n2, cost;
    while(fread(&n1, sizeof(uint32_t), 1, f)){
        if(!fread(&n2, sizeof(uint32_t), 1, f)) break;
        if(!fread(&cost, sizeof(uint32_t), 1, f)) break;
        if(IS_BIG_ENDIAN){
            graph->arr[graph->edges_count][N1] = n1;
            graph->arr[graph->edges_count][N2] = n2;
            graph->arr[graph->edges_count][COST] = cost;
        } else{
            graph->arr[graph->edges_count][N1] = BSWAP32(n1);
            graph->arr[graph->edges_count][N2] = BSWAP32(n2);
            graph->arr[graph->edges_count][COST] = BSWAP32(cost);
        }
        graph->edges_count++;
        if(graph->edges_count == graph->capacity){
            int pre_cap = graph->capacity;
            (graph->capacity <= 100000) ? (graph->capacity *= 2) : (graph->capacity += 50000);
            graph->arr = (uint32_t**)realloc(graph->arr,graph->capacity*sizeof(uint32_t*));
            for(int i = pre_cap; i < graph->capacity; i++){
                graph->arr[i] = (uint32_t*)malloc(3*sizeof(uint32_t));
            }
        }
    }
    fclose(f);
}

/* Save the graph to the text file. */
void save_txt(const graph_t * const graph, const char *fname){
    FILE *file = fopen(fname, "w");
    char st[9];
    short st_c;
    int n;
    for(int i = 0; i < graph->edges_count; i++){
        n = graph->arr[i][N1];
        st_c = 0;
        if(n == 0){
            st[st_c++] = '0';
        } else{
            while(n){
                st[st_c++] = (n % 10) + SHIFT;
                n /= 10;
            }
        }
        for(int u = st_c - 1; u >= 0; u--){
            fputc(st[u], file);
        }
        fputc(' ', file);
        n = graph->arr[i][N2];
        st_c = 0;
        if(n == 0){
            st[st_c++] = '0';
        } else{
            while(n){
                st[st_c++] = (n % 10) + SHIFT;
                n /= 10;
            }
        }
        for(int u = st_c - 1; u >= 0; u--){
            fputc(st[u], file);
        }
        fputc(' ', file);
        n = graph->arr[i][COST];
        st_c = 0;
        while(n){
            st[st_c++] = (n % 10) + SHIFT;
            n /= 10;
        }
        for(int u = st_c - 1; u >= 0; u--){
            fputc(st[u], file);
        }
        fputc('\n', file);
    }   
    fclose(file);
}

/* Save the graph to the binary file. */
void save_bin(const graph_t * const graph, const char *fname){
    FILE *f = fopen( fname, "wb" );
    if(f == NULL){
        fputs("Error in opening file!\n", stderr);
    }
    if(IS_BIG_ENDIAN){
        for(int i = 0; i < graph->edges_count; i++){
            fwrite(&graph->arr[i][N1], sizeof(uint32_t), 1, f);
            fwrite(&graph->arr[i][N2], sizeof(uint32_t), 1, f);
            fwrite(&graph->arr[i][COST], sizeof(uint32_t), 1, f);
        }
    } else{
        for(int i = 0; i < graph->edges_count; i++){
            graph->arr[i][N1] = BSWAP32(graph->arr[i][N1]);
            graph->arr[i][N2] = BSWAP32(graph->arr[i][N2]);
            graph->arr[i][COST] = BSWAP32(graph->arr[i][COST]);
            fwrite(&graph->arr[i][N1], sizeof(uint32_t), 1, f);
            fwrite(&graph->arr[i][N2], sizeof(uint32_t), 1, f);
            fwrite(&graph->arr[i][COST], sizeof(uint32_t), 1, f);
        }
    }
    fclose(f);
}
