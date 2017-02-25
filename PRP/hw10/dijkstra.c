#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "dijkstra.h"
#include "graph.h"
#include "pq_heap.h"


#ifndef SHIFT
#define SHIFT 48
#endif


typedef struct {
   int edge_start; // index to the first edge in the array of edges (-1 if does not exist)
   int edge_count; // number of edges (may be 0)
   int parent; // index to the parent node on the shortest path from the given node 
   int cost; // cost of the shortest path from the starting node to this node 
} node_t;

typedef struct {
   graph_t *graph;
   node_t *nodes;
   int num_nodes; // number of nodes;
   int start_node; //
} dijkstra_t;

// - function ----------------------------------------------------------------
void* dijkstra_init(void)
{
   dijkstra_t *dij = (dijkstra_t*)malloc(sizeof(dijkstra_t));
   if (dij) {
      dij->nodes = NULL;
      dij->num_nodes = 0;
      dij->start_node = -1;
      dij->graph = allocate_graph();
      if (!dij->graph) {
         free(dij); 
         dij = NULL;
      }
   }
   return dij;
}

// - function ----------------------------------------------------------------
_Bool dijkstra_load_graph(const char *filename, void *dijkstra)
{
   _Bool ret = false;
   dijkstra_t *dij = (dijkstra_t*)dijkstra;
   if (
         dij && dij->graph
      ) { // edges has not been loaded
      // dijkstra_t and graph has been allocated and edges have been loaded here
      // go through the edges and create array of nodes with indexing to edges
      // 1st get the maximal number of nodes
      load_txt(filename, dij->graph);
      int m = dij->graph->num_nodes;
      dij->nodes = (node_t*)malloc(sizeof(node_t) * m);

      if (dij->nodes) { // allocation does not fail
         dij->num_nodes = m;

         // 2nd initialization of the nodes
         for (int i = 0; i < m; ++i) {
            dij->nodes[i].edge_start = -1;
            dij->nodes[i].edge_count = 0;
            dij->nodes[i].parent = -1;
            dij->nodes[i].cost = -1;
         }

         // 3rd add edges to the nodes
         for (int i = 0; i < dij->graph->num_edges; ++i) {
            int cur = dij->graph->edges[i].from;
            if (dij->nodes[cur].edge_start == -1) { // first edge
               dij->nodes[cur].edge_start = i; // mark the first edge in the array of edges
            }
            dij->nodes[cur].edge_count += 1; // increase number of edges
         }
         ret = true;
      }
   }
   return ret;
}

// - function ----------------------------------------------------------------
_Bool dijkstra_solve(void *dijkstra, int label) 
{
   _Bool ret = false;
   dijkstra_t *dij = (dijkstra_t*)dijkstra;
   if (!dij || label < 0 || label >= dij->num_nodes) {
      return false;
   }
   dij->start_node = label;

   void *pq = pq_alloc(dij->num_nodes);

   dij->nodes[label].cost = 0; // initialize the starting node
   pq_push(pq, label, 0);

   int cur_label;
   while ( !pq_is_empty(pq) && pq_pop(pq, &cur_label) ) {
      node_t *cur = &(dij->nodes[cur_label]);
      for (int i = 0; i < cur->edge_count; ++i) { // relax all children 
         const edge_t *const edge = &(dij->graph->edges[cur->edge_start + i]); // avoid copying
         node_t *to = &(dij->nodes[edge->to]);
         const int cost = cur->cost + edge->cost;
         if (to->cost == -1) { // the node to has not been visited yet
            to->cost = cost;
            to->parent = cur_label;
            pq_push(pq, edge->to, cost); 
         } else if (cost < to->cost) { // already relaxed check if we can make a shortcut to child node via the current node
            to->cost = cost;
            to->parent = cur_label;
            pq_update(pq, edge->to, cost);
         }
      } // end all children of the cur node;
   } // end pq_is_empty

   pq_free(pq); // release the memory

   return ret;
}

// - function ----------------------------------------------------------------
_Bool dijkstra_save_path(const void *dijkstra, const char *filename)
{
   _Bool ret = false;
   const dijkstra_t *const dij = (dijkstra_t*)dijkstra;
   if (dij) {
      FILE *f = fopen(filename, "w");
      char st[9];
      short st_c = 0;
      char outs[100000];
      int outs_c = 0;
      int n;
      if (f) {
         for (int i = 0; i < dij->num_nodes; ++i) {
            const node_t *const node = &(dij->nodes[i]);
            n = i;
            if(!n){
                outs[outs_c++] = '0';
            } else{
                while(n){
                    st[st_c++] = (n % 10) + SHIFT;
                    n /= 10;
                }
                while(st_c){
                    outs[outs_c++] = st[--st_c];
                }
            }
            outs[outs_c++] = ' ';
            n = node->cost;
            if(!n){
                outs[outs_c++] = '0';
            } else{
                if(n < 0){
                    outs[outs_c++] = '-';
                    outs[outs_c++] = '1';
                } else{
                    while(n){
                        st[st_c++] = (n % 10) + SHIFT;
                        n /= 10;
                    }
                    while(st_c){
                       outs[outs_c++] = st[--st_c];
                    }
                }
            }
            outs[outs_c++] = ' ';
            n = node->parent;
            if(!n){
                outs[outs_c++] = '0';
            } else{
                if(n < 0){
                    outs[outs_c++] = '-';
                    outs[outs_c++] = '1';
                } else{
                    while(n){
                        st[st_c++] = (n % 10) + SHIFT;
                        n /= 10;
                    }
                    while(st_c){
                       outs[outs_c++] = st[--st_c];
                    }
                }
            }
            outs[outs_c++] = '\n';
            if(outs_c > 99950){
                outs[outs_c++] = '\0';
                fputs(outs,f);
                outs_c = 0;
            }
        } // end all nodes
        if(outs_c){
            outs[outs_c++] = '\0';
            fputs(outs,f);
        }
        ret = fclose(f) == 0;
      }
   }
   return ret;
}

// - function ----------------------------------------------------------------
void dijkstra_free(void *dijkstra)
{
   dijkstra_t *dij = (dijkstra_t*)dijkstra;
   if (dij) {
      if (dij->graph) {
         free_graph(&(dij->graph));
      }
      if (dij->nodes) {
         free(dij->nodes);
      }
      free(dij);
   }
}


_Bool dijkstra_get_solution(const void *dijkstra, int n, int solution[][3]){
    dijkstra_t *dij = (dijkstra_t*)dijkstra;
    node_t *nodes = dij->nodes;
    for(int i = 0; i < dij->num_nodes; ++i, ++nodes){
        solution[i][0] = i;
        solution[i][1] = nodes->cost;
        solution[i][2] = nodes->parent;
    }
    return true;
}

_Bool dijkstra_set_graph(int e, int edges[][3], void *dijkstra)
{
    dijkstra_t *dij = (dijkstra_t*)dijkstra;
    _Bool ret = false;
    if(dij && dij->graph){
        // SETTING GRAPH
        edge_t *g_e = (edge_t*)malloc((e + 1) * sizeof(edge_t));
        assert(g_e != NULL);
        free(dij->graph->edges);
        dij->graph->edges = g_e;
        dij->graph->capacity = (e + 1);
        edge_t *ge = dij->graph->edges;
        for(int i = 0; i < e; ++i, ++ge){
            ge->from = edges[i][0];
            ge->to = edges[i][1];
            ge->cost = edges[i][2];
            dij->graph->num_edges++;
        }
        //print_graph(dij->graph);


        int m = -1;
        for (int i = 0; i < dij->graph->num_edges; ++i) {
            const edge_t *const edg = &(dij->graph->edges[i]); // use pointer to avoid copying
            m = m < edg->from ? edg->from : m;
            m = m < edg->to ? edg->to : m;
        }
        m += 1; // m is the index therefore we need +1 for label 0
        dij->nodes = (node_t*)malloc(sizeof(node_t) * m);
        if (dij->nodes) { // allocation does not fail
            dij->num_nodes = m;
            // 2nd initialization of the nodes
            for (int i = 0; i < m; ++i) {
                dij->nodes[i].edge_start = -1;
                dij->nodes[i].edge_count = 0;
                dij->nodes[i].parent = -1;
                dij->nodes[i].cost = -1;
            }
            // 3rd add edges to the nodes
            for (int i = 0; i < dij->graph->num_edges; ++i) {
                int cur = dij->graph->edges[i].from;
                if (dij->nodes[cur].edge_start == -1) { // first edge
                    dij->nodes[cur].edge_start = i; // mark the first edge in the array of edges
                }
                dij->nodes[cur].edge_count += 1; // increase number of edges
            }
            ret = true;
        }
    }
    return ret;
}

void dijkstra_save_bin(void* dijkstra, const char* file){
    //printf("saving bin\n");
    dijkstra_t *dij = (dijkstra_t*)dijkstra;
    save_bin(dij->graph,file);
}

void dijkstra_load_bin(void* dijkstra, const char* file){
    //printf("loading bin\n");
    dijkstra_t *dij = (dijkstra_t*)dijkstra;
    load_bin(file, dij->graph);
}

/* end of dijkstra.c */