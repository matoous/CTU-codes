#include <stdio.h>
#include <stdlib.h>

/* The main program by Matous Dzivjak */
int main(int argc, char *argv[])
{
  int num, index = 0;
  int *arr;
  arr = (int*)calloc(100000, sizeof(int));
  while(scanf("%d", &num)){
    arr[index++] = num;
  }

  for(int i = 0; i < index; i++){
    printf("%d, ", arr[i]);
  }
  return 0;
}

