#include <stdio.h>
#include <pthread.h>
#include "assert.h"
#include "sched.h"

#define THREADS 300
#define THREADS_2 200,

pthread_t t[THREADS];

void stayalive()
{
  while(1);
}

void stayaliveshort()
{
  sched_yield();
}

int main()
{

  for(int i = 0; i < (THREADS - 100); i++)
  {
    assert(pthread_create(t + i, NULL, (void*)stayalive, NULL) == 0 && "thread creation failed\n");
    printf("%d\n", i);
    if(i == THREADS - 101)
      printf("200T created.\n");
  }
  printf("wait until all created then getchar\n");
  getchar();

  for(int j = THREADS -100; j < THREADS; j++)
  {
    assert(pthread_create(t + j-100, NULL, (void*)stayaliveshort, NULL) == 0 && "thread creation failed\n");
    assert(pthread_cancel(t[j-100]) == 0 && "thread creation failed\n");
    printf("%d\n", j);
  }

  return 0;
}