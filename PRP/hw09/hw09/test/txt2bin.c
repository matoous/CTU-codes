#include <stdio.h>

#include "graph.h"

int main(int argc, char *argv[])
{
   int ret = 0;
   if (argc > 2) {
      graph_t *graph = allocate_graph();
      fprintf(stderr, "Load txt file '%s'\n", argv[1]);
      load_txt(argv[1], graph);
      fprintf(stderr, "Save bin file '%s'\n", argv[2]);
      save_bin(graph, argv[2]);
      free_graph(&graph);
   } else {
      fprintf(stderr, "Usage %s input_txt_file output_bin_file\n", argv[0]);
      ret = -1;
   }
   return ret;
}
