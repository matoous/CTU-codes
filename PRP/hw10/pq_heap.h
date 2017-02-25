/*
 * File name: pq_heap.h
 * Date:      2016/08/06 20:41
 * Author:    Jan Faigl
 */

#ifndef __PQ_HEAP_H__
#define __PQ_HEAP_H__

/*
 * Function: pq_alloc
 * ----------------------------
 *   Return address of the newly created priority queue for storing up to the given size
 *   entries. The return value is pointer to void and the particular implementation
 *   of the priority queue can be hidden in the pq.c 
 *   It is supposed the pq_push() and pq_update() function will provide cost of the
 *   vertex with the 0 <= label <= (size-1)
 *
 *   size:  maximal number of the entries in priority queue 
 *
 *   returns: address to the newly allocated priority queue on success; NULL otherwise
 */
void *pq_alloc(int size);

/*
 * Function: pq_free
 * ----------------------------
 *   Deallocate all the associated memory of the given priority queue
 *   been push to the priority queue.
 *
 *   _pq:  pointer to the priority queue allocated by the pq_alloc()
 *
 *   returns:
 */
void pq_free(void *_pq);

/*
 * Function: pq_is_empty
 * ----------------------------
 *   Return true if the given priority queue is empty
 * 
 *   _pq:  pointer to the priority queue allocated by the pq_alloc()
 *
 *   returns: true if priority queue is empty; false otherwise
 */
_Bool pq_is_empty(const void *_pq);

/*
 * Function: pq_push
 * ----------------------------
 *   Return true if the given label of the vertex with the associated cost has 
 *   been push to the priority queue. The given vertex label is supposed 
 *   to be within the range the 0 <= label <= (size-1), where
 *   the size is the maximal number of entries in the priority queue 
 *   provided to the pq_alloc()
 *
 *   _pq:  pointer to the priority queue allocated by the pq_alloc()
 *   label: a vertex label with associated cost
 *   cost:  current cost of the vertex with the label 
 *
 *   returns: true on success; false otherwise
 */
_Bool pq_push(void *_pq, int label, int cost);

/*
 * Function: pq_update
 * ----------------------------
 *   Return true if a vertex with the given label is already in the priority queue
 *   and its cost has been updated  The given vertex label is supposed 
 *   to be within the range the 0 <= label <= (size-1), where
 *   the size is the maximal number of entries in the priority queue
 *   provided to the pq_alloc()
 *
 *   _pq:  pointer to the priority queue allocated by the pq_alloc()
 *   label: a vertex label with associated cost
 *   cost:  new cost of the vertex with the given label 
 *
 *   returns: true on success; false otherwise
 */
_Bool pq_update(void *_pq, int label, int cost);


/*
 * Function: pq_pop
 * ----------------------------
 *   Return true if the label with the lowest cost has been 
 *   removed from the priority queue
 *
 *   _pq:  pointer to the priority queue allocated by the pq_alloc()
 *   oLabel: pointer to int, where the label of the
 *           vertex with the lowest cost in the priority queue is stored
 *           on success
 *
 *   returns: true on success; false otherwise
 */
_Bool pq_pop(void *_pq, int *oLabel);

/*
 * Function: pq_is_heap
 * ----------------------------
 *   Return true if the given priority queue is heap, i.e., 
 *   the heap property is satified
 *
 *   heap:  pointer to the heap allocated by the pq_alloc()
 *
 *   n: index of the item in the heap
 *
 *   returns: true on success; false otherwise
 */
_Bool pq_is_heap(void *heap, int n);

#endif
