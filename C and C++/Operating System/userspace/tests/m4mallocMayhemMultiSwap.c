#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "assert.h"
#define TESTSIZE 1000000
#define THREADCOUNT 10
#define SIZE ((1000 * 4096)/sizeof(long))
pthread_t threads[THREADCOUNT];

void function()
{
  int* array = malloc(SIZE * sizeof(int));
  for(int i = 0; i <= SIZE; i++)
  {
    array[i] = i;
  }
  printf("read\n");
  for(long i = SIZE; i >= 0; i--)
  {
    if(i % 100000 == 0)
      printf("%ld\n", i);
    assert(array[i] == i);
  }

  printf("getchar to free all of it\n");
  getchar();
  free(array);
  printf("all gone\n");
}


int main()
{
  for(int i = 0; i < THREADCOUNT; i++)
  {
    pthread_create(&threads[i], 0, (void*)function,NULL);
  }
  getchar();
  return 0;
}