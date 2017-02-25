#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "queue.h"

/* the tested queue */
queue_t *q;

/* allocate new integer with value a and add it to the queue */
void add(int a)
{
   int *p = (int*) malloc(sizeof(int));
   *p = a;
   bool ret = push_to_queue(q, (void*)p);
   if (!ret) {
      // free memory on failure
      free(p);
   }
}

/* print the int value on pointer p */
void print_int(void* p){
    if(p != NULL){
        printf("%d\n", *((int*)p));
    } else {
        printf("NULL\n");
    }
}

/* pop from the queue, print and free the element */
void pop(){
  void* p = pop_from_queue(q);
  print_int(p);
  free(p);
}

/* get i-th element and print it (do not remove them) */
void get(int idx){
  print_int(get_from_queue(q, idx));
}


/* 
 * TEST PROGRAM
 * - reads commands from stdin and executes them in the queue
*/
int main(int argc, char *argv[])
{
   int n;
   // read the size of the queue
   scanf("%d", &n);
   // create queue
   q = create_queue(n);

   while (true) {
      char s[2];
      // read one command
      int ret = scanf("%1s", s);
      if (ret != 1) {
         break;
      }

      // add command
      if (s[0] == 'a') {
         int a;
	 // read the argument of the command
         ret = scanf("%d", &a);
         if (ret != 1) {
            break;
         }
         add(a);
      // remove command	 
      } else if (s[0] == 'r') {
         pop();
      // get command	 
      } else if (s[0] == 'g') {
         int a;
	 // read the argument of the command
         ret = scanf("%d", &a);
         if (ret != 1) {
            break;
         }
         get(a);
      }
   }

   // remove rest of the elements in the queue
   while (get_queue_size(q)) {
      void* p = pop_from_queue(q);
      free(p);
   }

   // free memory
   delete_queue(q);
   q = NULL;

   // return 0 on succes
   return 0;
}
