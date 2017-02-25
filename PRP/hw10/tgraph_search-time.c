#include "dijkstra.h"

#include <stdio.h>

#include <math.h>
#include <time.h>

long getTimeInMillis(void) 
{
   struct timespec spec;
   clock_gettime(CLOCK_REALTIME, &spec);
   return spec.tv_sec * 1000 + round(spec.tv_nsec / 1.0e6);
}

int main(int argc, char *argv[]) 
{
   int ret = 0;

   if (argc < 3) {
      fprintf(stderr, "Call as\n %s graph_file solution_file\n", argv[0]);
   } else {
      fprintf(stderr, "Load graph from %s\n", argv[1]);
      long t1 = getTimeInMillis();
      void *dij = dijkstra_init();
      dijkstra_load_graph(argv[1], dij);
      long t2 = getTimeInMillis();
      fprintf(stdout, "Load time ....%lims\n", (t2-t1));
      fprintf(stderr, "Find all shortest paths from the node 0\n");
      dijkstra_solve(dij, 0);
      long t3 = getTimeInMillis();
      fprintf(stderr, "Save solution to %s\n", argv[2]);
      fprintf(stdout, "Solve time ...%li ms\n", (t3-t2));
      dijkstra_save_path(dij, argv[2]);
      long t4 = getTimeInMillis();
      fprintf(stdout, "Save time ....%li ms\n", (t4-t3));
      fprintf(stderr, "Free allocated memory\n");
      fprintf(stdout, "Total time ...%lims\n", (t4-t1));
      dijkstra_free(dij);
      ret = 0;
   }
   return ret;
}

/* end of tgraph_search-time.c */
