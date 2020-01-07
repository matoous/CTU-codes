#include <stdio.h>
#define FREE_SPACE 0x0
#define HEADER_SIZE 5
#define FLAGS_OFFSET 4

extern "C" void *my_malloc(unsigned int);
extern "C" int my_free(void*);

typedef unsigned long mword;
typedef unsigned char byte;

void *brk(void *address){
  unsigned w0 = 3;
  asm volatile (
      "   mov %%esp, %%ecx    ;"
      "   mov $1f, %%edx      ;"
      "   sysenter            ;"
      "1:                     ;"
      : "+a" (w0) : "S" ((unsigned)address) : "ecx", "edx", "memory");
    return (void*)w0;
}

typedef struct mem_manager_t {
  byte *brkStart, *brkMax;
  byte *first, *last;
  byte *last_allocated;
} mem_manager_t;

mem_manager_t memam;

void *my_malloc(unsigned int size){
  // if it's first call to my_malloc
  if(memam.brkStart == NULL){
    memam.brkStart = (byte*)brk(NULL);
    memam.brkMax = (byte*)brk(NULL);
  }
  // find empty block with enough space
  byte *current = memam.brkStart;
  while(current < memam.brkMax){
    mword blockSize = *((mword*)current);
    byte flags = *(current + FLAGS_OFFSET);
    if(flags == 0 && (blockSize + HEADER_SIZE > size || blockSize == 0))
      break;
    current += HEADER_SIZE + blockSize;
  }
  // run out of space, alloc more
  if(current >= memam.brkMax || *((mword*)current) < size + HEADER_SIZE){
    if(brk(memam.brkMax + 2*(size + HEADER_SIZE)) == NULL)
      return NULL;
    else
      memam.brkMax = (byte*)brk(NULL);
  }
  // found empty block, split the block, use one, let the other one empty
  if(*((mword*)current) != 0){
    mword blockSize = *((mword*)current);
    byte* newblockStart = current + size + HEADER_SIZE;
    *((mword*)(newblockStart)) = blockSize - size - 2*HEADER_SIZE;
    *(newblockStart + FLAGS_OFFSET) = (byte)0;
  }
  *((mword*)current) = size;
  *(current + FLAGS_OFFSET) = (byte)1;
  return current + HEADER_SIZE;
}

int my_free(void *address){
  byte *x = (byte*)address;
  if(x < memam.brkStart || x > memam.brkMax)
    return -1;
  if(*(x - HEADER_SIZE + FLAGS_OFFSET) == 0)
    return -1; // trying to free free block
  *(x - HEADER_SIZE + FLAGS_OFFSET) = 0;
  return 0;
}
