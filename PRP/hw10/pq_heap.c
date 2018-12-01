/*
 * File name: pq_heap-no_update.c
 * Date:      2016/08/06 21:03
 * Author:    Jan Faigl
 */

#include "pq_heap.h"

#include <stdlib.h>
#include <stdbool.h>

/*
 * Simple implementation of the prioriry queue for the 
 * dijkstra algorithm based on array and with log(n) 
 * complexity of the pop function
 */
typedef struct {
   int size;    // the maximal number of entries in the array 
   int len;     // the current number of entries in the array
   int *cost;   // array with entries (costs)
   int *label;  // array with vertex labels 
   int *heapIDX;   // this is need for implementing pq_update()
                   // array with indexes of cost vertices, i.e.,where the particular 
                   // heapIDX[id] is the index where the cost of the vertex with the 
                   // label id is tored in cost. 
                   // E.g., cost of the vertex with the label id is at the heapIDX[id] 
                   // position in the cost array, thus, cost is cost[ heapIDX[ id ] ]
} pq_heap_s;

#define GET_PARENT(i) ((i-1) >> 1)
#define GET_LEFT(i) ((i<<1) + 1)

// - local function for implementing priority queue as heap ------------------
static void pq_down(pq_heap_s *pq);
static void pq_swap(pq_heap_s *pq, int i, int j);

// - function ----------------------------------------------------------------
void *pq_alloc(int size)
{
   pq_heap_s *pq = (pq_heap_s*)malloc(sizeof(pq_heap_s));
   if (pq) {
      pq->size = size;
      pq->len = 0;
      pq->cost = (int *)malloc(sizeof(int) * size);
      pq->label = (int *)malloc(sizeof(int) * size);
      pq->heapIDX = (int *)malloc(sizeof(int) * size);
      if (pq->cost && pq->label && pq->heapIDX) { // allocation fails
         for (int i = 0; i < pq->size; ++i) {
            pq->heapIDX[i] = -1; // set -1 to indicate that currently non of the graph vertices are placed in the heap
         }
      } else { // cleanup if something goes wrong
         if (pq->cost) { free(pq->cost); }
         if (pq->label) { free(pq->label); }
         if (pq->heapIDX) { free(pq->heapIDX); }
         free(pq);
         pq = NULL;
      }
   }
   return pq;
}   

// - function ----------------------------------------------------------------
void pq_free(void *_pq) 
{
   pq_heap_s *pq = (pq_heap_s*)_pq;
   if (pq) {
      free(pq->cost);
      free(pq->label);
      free(pq->heapIDX);
      free(pq);
   }
}

// - function ----------------------------------------------------------------
_Bool pq_is_empty(const void *_pq)
{
   pq_heap_s *pq = (pq_heap_s*)_pq;
   return pq && pq->len == 0 ? true : false;
}

// - function ----------------------------------------------------------------
_Bool pq_push(void *_pq, int label, int cost)
{
   _Bool ret = false;
   pq_heap_s *pq = (pq_heap_s*)_pq;
   if (
         pq 
         && pq->len < pq->size
         && label >= 0
         && label < pq->size
      ) {
      pq->cost[pq->len] = cost; //add the cost to the next slot in cost
      pq->label[pq->len] = label; //add vertex label
      pq->heapIDX[label] = pq->len; // remember position of the label cost 

      int cur = pq->len; // index of the entry added to the heap
      int parent = GET_PARENT(cur);
      while (cur >= 1 && pq->cost[parent] > pq->cost[cur]) {
         pq_swap(pq, parent, cur);
         cur = parent;
         parent = GET_PARENT(cur);
      }
      pq->len += 1;
      ret = true;
   }
   return ret;
}

// - function ----------------------------------------------------------------
_Bool pq_update(void *_pq, int label, int cost)
{
   _Bool ret = false;
   pq_heap_s *pq = (pq_heap_s*)_pq;
   if (
         pq 
         && pq->len < pq->size
         && label >= 0
         && label < pq->size
         && pq->heapIDX[label] != -1 //vertex with the label is in the pq 
      ) {
      int cur = pq->heapIDX[label]; // index of the entry added to the heap
      pq->cost[cur] = cost;
      int parent = GET_PARENT(cur);
      while (cur >= 1 && pq->cost[parent] > pq->cost[cur]) {
         pq_swap(pq, parent, cur);
         cur = parent;
         parent = GET_PARENT(cur);
      }
      ret = true;
   }
   return ret;
}

// - function ----------------------------------------------------------------
_Bool pq_pop(void *_pq, int *oLabel)
{
   _Bool ret = false;
   pq_heap_s *pq = (pq_heap_s*)_pq;
   if (pq && pq->len > 0) {
      *oLabel = pq->label[0];
      pq->heapIDX[*oLabel] = -1; //mark the vertex is not in the heap
      pq->len -= 1;
      pq->label[0] = pq->label[pq->len];
      pq->cost[0] = pq->cost[pq->len];
      pq->heapIDX[pq->label[0]] = 0;
      pq_down(pq);
      ret = true;
   }
   return ret;
}

// - function ----------------------------------------------------------------
_Bool pq_is_heap(void *heap, int n) 
{
   _Bool ret = true;
   pq_heap_s *pq = (pq_heap_s*)heap;
   int l = 2 * n + 1;
   int r = l + 1;

   if (l < pq->len) {
      ret = (pq->cost[l] < pq->cost[n]) ? false : pq_is_heap(heap, l);
   }
   if (r < pq->len) {
      ret = ret && ( (pq->cost[r] < pq->cost[n]) ? false : pq_is_heap(heap, r) );
   }
   return ret;
}

// - local function ----------------------------------------------------------
static void pq_down(pq_heap_s *pq)
{
   int cur;
   int hl, hr;
   int best ;

   cur = 0;
   hl = GET_LEFT(cur);
   if (hl >= pq->len) {
   } else {
      while (hl < pq->len) {
         hr = hl + 1;
         if (pq->cost[cur] > pq->cost[hl]) {
            best = hl; // left is the candite
         } else {
            best = cur;
         }
         if (hr < pq->len && pq->cost[best] > pq->cost[hr]) {
            best = hr; // right is the choice
         }
         if (best != cur) { // lower value found
            pq_swap(pq, cur, best);
            cur = best;
            hl = GET_LEFT(cur);
         } else {
            break;
         }
      }
   }
   //check_heap(0, heap, nodes);
}

// - local function ----------------------------------------------------------
static void pq_swap(pq_heap_s *pq, int i, int j)
{
   int t;
   pq->heapIDX[pq->label[i]] = j;
   pq->heapIDX[pq->label[j]] = i;
   t = pq->label[i];
   pq->label[i] = pq->label[j]; 
   pq->label[j] = t;
   t = pq->cost[i]; 
   pq->cost[i] = pq->cost[j];
   pq->cost[j] = t;
}

/* end of pq_heap-no_update.c */
