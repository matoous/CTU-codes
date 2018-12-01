#include "dijkstra.h"

#include <stdio.h>

int main(int argc, char *argv[]) 
{
   int ret = 0;

   if (argc < 3) {
      fprintf(stderr, "Call as\n %s graph_file solution_file\n", argv[0]);
   } else {
      fprintf(stderr, "Load graph from %s\n", argv[1]);
      void *dij = dijkstra_init();
      dijkstra_load_graph(argv[1], dij);
      fprintf(stderr, "Find all shortest paths from the node 0\n");
      dijkstra_solve(dij, 0);
      fprintf(stderr, "Save solution to %s\n", argv[2]);
      dijkstra_save_path(dij, argv[2]);
      fprintf(stderr, "Free allocated memory\n");
      dijkstra_free(dij);
      ret = 0;
   }
   return ret;
}

/* end of tgraph_search.c */
