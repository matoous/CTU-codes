#ifndef __DIJKSTRA_H__
#define __DIJKSTRA_H__

/*
 * Initialize structure for storing graph, solution, and eventual
 * variables for dijkstra algorithm
 *
 * return: point to the allocated structure; NULL on an error
 */   
void* dijkstra_init(void);
 
 
/*
 * Load input graph (in text format) from the given file and store it 
 * to the dijkstra structure previously created by dijkstra_init()
 *
 * return: true on success; false otherwise
 */   
_Bool dijkstra_load_graph(const char *filename, void *dijkstra);


/*
 * Set the graph to the dijkstra structure instead of direct 
 * loading the file as in dijkstra_load_graph() function.
 * The given array edges should not be directly used for the
 * internal representation of the graph in the dijkstra algorithm
 * 
 * return: true on success; false on an error, e.g., memory allocation
 */ 
_Bool dijkstra_set_graph(int e, int edges[][3], void *dijkstra);

 
/*
 * Solve the dijsktra algorithm on the graph previously 
 * loaded by the dijkstra_load_graph() set by dijkstra_set_graph()
 * The solution is stored in some internal structure of the dijkstra
 * type passed to the function
 *
 * return: true on success; false otherwise
 */
_Bool dijkstra_solve(void *dijkstra, int label);
 
 
/*
 * Retrived the solution found by the function dijkstra_solve()
 * It is assumed the passed argument solution[][3] is properly allocated, 
 * and thus the internal solution of the dijkstra can be used to fill the 
 * solution[][3].
 * 
 * return: true on success; false otherwise 
 */
_Bool dijkstra_get_solution(const void *dijkstra, int n, int solution[][3]);
 
 
/*
 * Directly solve the solution found by the dijkstra_solve() in to the
 * file (in the text format) with the given filename. 
 *
 * return: true on success; false otherwise
 */
_Bool dijkstra_save_path(const void *dijkstra, const char *filename);
 
 
/*
 * Release on allocated memory of the passed structure for
 * the dijkstra algorithm, e.g., graph as an array of pointers to struct edge
 * and solution as an array of pointers to struct node.
 * All previosly allocated memory related to solution of the shortest
 * paths is freed
 */
void dijkstra_free(void *dijkstra);

void dijkstra_save_bin(void* dijkstra, const char* file);

void dijkstra_load_bin(void* dijkstra, const char* file);

#endif

/* end of dijkstra.h */
