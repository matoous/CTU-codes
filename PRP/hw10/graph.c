#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>


#include "graph.h"

#ifndef SHIFT
#define SHIFT 48
#endif

#ifndef INIT_SIZE 
#define INIT_SIZE 20
#endif

#ifndef BSWAP32
#define BSWAP32(n) ((n) >> 24) | (((n) << 8) & 0x00FF0000L) | (((n) >> 8) & 0x0000FF00L) | ((n) << 24)
#endif

void load_txt(const char *fname, graph_t *g) 
{
    long length;
    char *buffer;
    FILE *f = fopen ( fname , "rb" );
    if(f){
        fseek(f , 0L , SEEK_END);
        length = ftell(f);
        rewind(f);
    }
    buffer = calloc(1, length + 1);
    fread( buffer , length, 1 , f);
    fclose(f);
    if (buffer && length)
    {
    printf("%li fs\n", length);
        int n1, n2, cost;
        int m = -1;
        int i = 0;
        edge_t *e = g->edges;
        while(i < length){
            n1 = n2 = cost = 0;
            while(buffer[i] < 58 && buffer[i] > 47){
                n1 *= 10; n1 += (buffer[i++] - SHIFT);
            }
            i++;
            while(buffer[i] < 58 && buffer[i] > 47){
                n2 *= 10; n2 += (buffer[i++] - SHIFT);
            }
            i++;
            while(buffer[i] < 58 && buffer[i] > 47 && i < length){
                cost *= 10; cost += (buffer[i++] - SHIFT);
            }
            if(cost){
                e->from = n1;
                e->to = n2;
                e->cost = cost;
                m = m < e->from ? e->from : m;
                m = m < e->to ? e->to : m;
                g->num_edges++;
                e++;
                if (g->num_edges == g->capacity) {
                    enlarge_graph(g);
                }
            }
        }
        m += 1;
        g->num_nodes = m;
        free(buffer);
    }
   //print_graph(g);
}

/* Save the graph to the text file. */
void save_txt(const graph_t * const graph, const char *fname){
    FILE *f = fopen(fname, "w");
    char st[9];
    short st_c = 0;
    int n;
    for(int i = 0; i < graph->num_edges; ++i){
        const edge_t *const edge = &(graph->edges[i]);
        n = edge->from;
        if(n == 0){
            st[st_c++] = '0';
        } else{
            while(n){
                st[st_c++] = (n % 10) + SHIFT;
                n /= 10;
            }
        }
        while(st_c) fputc(st[--st_c], f);
        fputc(' ', f);
        n = edge->to;
        if(n == 0){
            st[st_c++] = '0';
        } else{
            while(n){
                st[st_c++] = (n % 10) + SHIFT;
                n /= 10;
            }
        }
        while(st_c) fputc(st[--st_c], f);
        fputc(' ', f);
        n = edge->cost;
        while(n){
            st[st_c++] = (n % 10) + SHIFT;
            n /= 10;
        }
        while(st_c) fputc(st[--st_c], f);
        fputc('\n', f);
    }
    fclose(f);
}

graph_t* allocate_graph(void) 
{
   graph_t *g = (graph_t*) malloc(sizeof(graph_t));
   assert(g != NULL);
   g->edges = NULL;
   g->num_edges = 0;
   g->capacity = 0;
   /* or we can call calloc */
   return g;
}

void free_graph(graph_t **g) 
{
   assert(g != NULL && *g != NULL);
   if ((*g)->capacity > 0) {
      free((*g)->edges);
   }
   free(*g);
   *g = NULL;
}

graph_t* enlarge_graph(graph_t *g) 
{
   assert(g != NULL);
   int n = g->capacity == 0 ? INIT_SIZE : g->capacity * 2; /* double the memory */

   edge_t *e = (edge_t*)malloc(n * sizeof(edge_t));
   assert(e != NULL);
   memcpy(e, g->edges, g->num_edges * sizeof(edge_t));
   free(g->edges);
   g->edges = e;
   g->capacity = n;
   return g;
}

void print_graph(graph_t *g) 
{
   assert(g != NULL);
   fprintf(stderr, "Graph has %d edges and %d edges are allocated\n", g->num_edges, g->capacity);
   edge_t *e = g->edges;
   for (int i = 0; i < g->num_edges; ++i, ++e) {
      printf("%d %d %d\n", e->from, e->to, e->cost);
   }
}

/* Load a graph from the binary file. */
void load_bin(const char *fname, graph_t *graph){
   FILE *f = fopen(fname, "rb");
   uint32_t n1, n2,cost;
   int m = -1;
   while (f && !feof(f)) {
      if (graph->num_edges == graph->capacity) {
         enlarge_graph(graph);
      }
      edge_t *e = graph->edges + graph->num_edges;
      if(!is_big_endian()){
        while (!feof(f) && graph->num_edges < graph->capacity) {
            if(!fread(&n1,sizeof(uint32_t),1,f)){
                break;
            }
            if(!fread(&n2,sizeof(uint32_t),1,f)){
                break;
            }
            if(!fread(&cost,sizeof(uint32_t),1,f)){
                break;
            }
            e->from = BSWAP32(n1);
            e->to = BSWAP32(n2);
            e->cost = BSWAP32(cost);
            m = m < e->from ? e->from : m;
            m = m < e->to ? e->to : m;
            graph->num_edges++;
            e++;
        }
      }
      while (!feof(f) && graph->num_edges < graph->capacity) {
          if(!fread(&n1,sizeof(uint32_t),1,f)){
              break;
          }
          if(!fread(&n2,sizeof(uint32_t),1,f)){
              break;
          }
          if(!fread(&cost,sizeof(uint32_t),1,f)){
              break;
          }
          e->from = n1;
          e->to = n2;
          e->cost = cost;
          m = m < e->from ? e->from : m;
          m = m < e->to ? e->to : m;
          graph->num_edges++;
          e++;
      }
   }
   m += 1;
   graph->num_nodes = m;
   if (f) {
      fclose(f);
   }
   //print_graph(graph);
}

/* Save the graph to the binary file. */
void save_bin(const graph_t * const graph, const char *fname){
    FILE *f = fopen(fname, "wb");
    uint32_t n1, n2,cost;
    edge_t *e = graph->edges;
   if(!is_big_endian()){
        for(int i = 0; i < graph->num_edges; ++i, ++e){
            n1 = BSWAP32(e->from);
            n2 = BSWAP32(e->to);
            cost = BSWAP32(e->cost);
            fwrite(&n1, sizeof(uint32_t), 1, f);
            fwrite(&n2, sizeof(uint32_t), 1, f);
            fwrite(&cost, sizeof(uint32_t), 1, f);
        }
   } else{
        for(int i = 0; i < graph->num_edges; ++i, ++e){
            fwrite(&(e->from), sizeof(uint32_t), 1, f);
            fwrite(&(e->to), sizeof(uint32_t), 1, f);
            fwrite(&(e->cost), sizeof(uint32_t), 1, f);
        }
   } 
    if (f) {
       fclose(f);
    }
}

int is_big_endian()
{
  static const int i = 1;
  return *((char *)&i) == 0;
}