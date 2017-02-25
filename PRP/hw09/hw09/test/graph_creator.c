#include <stdio.h>
#include <stdlib.h>

#define EDGE_H (3+2)
#define EDGE_L 1
#define COST 10

int main(int argc, char *argv[])
{
   if (argc < 2) {
      fprintf(stderr, "Usage %s number_nodes [seed] [filename]\n", argv[0]);
      return -1;
   }
   int n = atoi(argv[1]);
   int fi = 2;
   unsigned int seed = argc > 3 ? atoi(argv[fi++]) : rand();

   int max_node = n - 1;
   FILE *out = argc > 2 ? fopen(argv[fi], "w") : stdout;

   srandom(seed);
   int cost = rand() % COST + 1; // cost always > 0
   fprintf(out, "%i %i %i\n", 0, 1, cost);  // always connect 0 with the 1 node


   const int max_edges = n * EDGE_H;
   int *edge_to = (int*)malloc(max_edges*sizeof(int)); //keep track edges forms a single connected component
   for (int i = 0; i < max_edges; ++i) {
      edge_to[i] = -1; //clear edge 
   }
   int edge_buffer[EDGE_L + EDGE_H];

   for (int i = 0; i < n; ++i) {
      const int e = EDGE_L + rand() % EDGE_H;
      edge_to[i] = 1;
      for (int j = 0; j < e; ++j) {
	 int to = 1 + rand() % max_node; // prefer forward edges not connected to the 1st (0) node
	 while (edge_to[to] != -1) { //prefer connect not connecting nodes
	    to = 1 + rand() % max_node; 
	 }
	 edge_to[i] = 1;
	 edge_buffer[j] = to; // remember nodes that are connected
	 int cost = rand() % COST + 1; // cost always > 0
	 fprintf(out, "%i %i %i\n", i, to, cost);
      } //end edges from i
      edge_to[i] = -1;
      for (int j = 0;  j < e; ++j) { //clear buffer of edges
	 edge_to[edge_buffer[j]] = -1;
      }
   }
   free(edge_to);
   if (out != stdout) {
      fclose(out);
   }
   return EXIT_SUCCESS;
}
