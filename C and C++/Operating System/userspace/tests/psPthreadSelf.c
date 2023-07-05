#include <stdio.h>
#include <pthread.h>
#include "assert.h"
#include "sched.h"

#define TESTSIZE 20

void hello()
{
  printf("Thread TID: %ld\n", pthread_self());
}


int main()
{
  {
    printf("press enter to end testcase (if there was no kpanic lol)\n");
    pthread_t thread[TESTSIZE];
    for(int i = 0; i < TESTSIZE; i++)
      assert(pthread_create(&thread[i], NULL, (void*)hello, NULL) == 0  && "thread creation failed\n");

    getchar();
  }

  return 0;
}