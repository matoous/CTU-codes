#include <stdio.h>

#include "graph.h"

int main(int argc, char *argv[])
{
   int ret = 0;
   if (argc > 2) {
      graph_t *graph = allocate_graph();
      fprintf(stderr, "Load bin file '%s'\n", argv[1]);
      load_bin(argv[1], graph);
      fprintf(stderr, "Save txt file '%s'\n", argv[2]);
      save_txt(graph, argv[2]);
      free_graph(&graph);
   } else {
      fprintf(stderr, "Usage %s input_bin_file output_txt_file\n", argv[0]);
      ret = -1;
   }
   return ret;
}
