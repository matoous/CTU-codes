#include "queue.h"

/* creates a new queue with a given size */
queue_t* create_queue(int capacity){
    queue_t *queue;
    queue = (queue_t*)malloc(sizeof(queue_t));
    queue->capacity = capacity;
    queue->first = -1;
    queue->last = -1;
    queue->counter = 0;
    queue->arr = (void**)malloc(capacity*sizeof(void*));
    return queue;
}

/* deletes the queue and all allocated memory */
void delete_queue(queue_t *queue){
    free(queue->arr);
    free(queue);
}
 
/* 
 * inserts a reference to the element into the queue
 * returns: true on success; false otherwise
 */
bool push_to_queue(queue_t *queue, void *data){
    if(queue->counter == queue->capacity){
        queue->capacity *= 2;
        void **temp_arr = (void**)realloc(queue->arr,queue->capacity*(sizeof(void*)));
        if(temp_arr == NULL){
            return false;
        }
        queue->arr = temp_arr;
        if(queue->last < queue->first){
            int index = 0;
            int placer = queue->capacity/2;
            while(index <= queue->last){
                queue->arr[placer++] = queue->arr[index];
                queue->arr[index] = NULL;
                index += 1;
            }
            queue->last = placer - 1;
        }
    }
    if(queue->first == -1){
        queue->arr[++queue->first] = data;
        queue->last++;
        queue->counter++;
    } else{
        if(queue->last + 1 == queue->capacity){
            queue->last = -1;
        }
        queue->arr[++queue->last] = data;
        queue->counter++;
    }
    return true;
}
 
/* 
 * gets the first element from the queue and removes it from the queue
 * returns: the first element on success; NULL otherwise
 */
void* pop_from_queue(queue_t *queue){
    if(queue->counter > 0){
        void *data = queue->arr[queue->first];
        queue->arr[queue->first] = NULL;
        queue->first++;
        if(queue->first == queue->capacity){
            queue->first = 0;
        }
        queue->counter--;
        if(queue->counter < queue->capacity/3){
            if(queue->first >= queue->capacity/2 || queue->last >= queue->capacity/2){
                int diff = queue->capacity - queue->first;
                for(int i = 0; i < queue->counter; i++){
                    int now = queue->last - i;
                    if(now < 0) now += queue->capacity;
                    int index = now+diff;
                    if(index >= queue->capacity) index -= queue->capacity;
                    queue->arr[index] = queue->arr[now];
                    queue->arr[now] = NULL;
                }
                queue->first = 0;
                queue->last = queue->counter -1;
            }
            queue->capacity /= 2;
            queue->arr = (void**)realloc(queue->arr,queue->capacity*(sizeof(void*)));
        }
        for(int i = 0; i < queue->capacity; i++){
            if(queue->arr[i] != NULL){
                printf("%d ",*((int*)queue->arr[i]));
            } else{
                printf("_ ");
            }
        }
        printf("\n");
        return data;
    } else{
        return NULL;
    }
}
 
/* 
 * gets idx-th element from the queue 
 * returns: the idx-th element on success; NULL otherwise
 */
void* get_from_queue(queue_t *queue, int idx){
    if(idx >= queue->counter || idx < 0){
        return NULL;
    }
    int index = queue->first + idx;
    if(index >= queue->capacity){
        index -= queue->capacity;
    }
    return queue->arr[index];
}
 
/* gets number of stored elements */
int get_queue_size(queue_t *q){
    return q->counter;
}
