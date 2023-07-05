#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "assert.h"
#include "sched.h"
#define TESTSIZE 1000
#define ALLOCATE 2000
#define THREADCOUNT 10
pthread_t threads[THREADCOUNT];
int go = 0;
void* array[THREADCOUNT];
int index = 0;

void allocateAndFree()
{
  int *malloc_ = malloc(ALLOCATE * sizeof(int));
  printf("Current heap: %p\n", sbrk(0));
  array[index++] = sbrk(0);
  *malloc_ = 3;
  while(1)
  {
    if(go)
      break;
  }
  printf("now freeing all of it\n");
  free(malloc_);
  printf("leaving\n");
}

int main()
{
  for(int i = 0; i < THREADCOUNT; i++)
  {
    pthread_create(&threads[i], 0, (void*)allocateAndFree,NULL);
  }
  getchar();
  go = 1;
  printf("getchar um die Abstände zwischen jedem Malloc zu sehen!\n");
  getchar();
  void* space;
  for(int i = 0; i < THREADCOUNT; i++)
  {
    if(i == 0)
      continue;
    else if(i == 1)
      space = (void*)(array[i] - array[i-1]);
    else
      assert((void*)(array[i] - array[i-1]) == space);
  }
}