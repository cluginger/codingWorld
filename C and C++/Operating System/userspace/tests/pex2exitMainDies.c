#include <stdio.h>
#include <pthread.h>
#include "assert.h"
#include "sched.h"

#define TESTSIZE 10

void hello(void)
{
  //sched_yield();
  while(1);

}

int main()
{
  pthread_t thread[TESTSIZE];
  for(int i = 0; i < TESTSIZE; i++)
  {
    assert(pthread_create(&thread[i], NULL, (void*)hello, NULL) == 0  && "thread creation failed\n");
  }
  printf("Done creating - exiting\n");
  pthread_exit(0);
  return 0;
}