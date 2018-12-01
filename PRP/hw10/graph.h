#ifndef __GRAPH_H__
#define __GRAPH_H__

typedef struct {
   int from;
   int to;
   int cost;
} edge_t;

typedef struct {
   edge_t *edges;
   int num_edges;
   int num_nodes;
   int capacity;
} graph_t;

graph_t* enlarge_graph(graph_t *g);

void print_graph(graph_t *g);

int is_big_endian();
 
/* Allocate a new graph and return a reference to it. */
graph_t* allocate_graph();

/* Free all allocated memory and set reference to the graph to NULL. */
void free_graph(graph_t **graph);
 
/* Load a graph from the text file. */
void load_txt(const char *fname, graph_t *graph);

/* Load a graph from the binary file. */
void load_bin(const char *fname, graph_t *graph);
 
/* Save the graph to the text file. */
void save_txt(const graph_t * const graph, const char *fname);

/* Save the graph to the binary file. */
void save_bin(const graph_t * const graph, const char *fname);

/**/
void parse_graph(const int e, const int edges[][3], graph_t *graph);

#endif