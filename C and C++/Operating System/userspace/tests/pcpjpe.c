#include <stdio.h>
#include <pthread.h>
#include "assert.h"
#include "sched.h"

#define TESTSIZE 100

void hello()
{
  pthread_exit(0);
}


int main()
{
  {
    pthread_t thread[TESTSIZE];
    for(int i = 0; i < TESTSIZE; i++)
    {
      printf("%d\n", i);
      assert(pthread_create(thread + i, NULL, (void*)hello, NULL) == 0  && "thread creation failed\n");
      assert(pthread_join(thread[i], NULL) == 0  && "thread creation failed\n");
    }

  }

  return 0;
}
