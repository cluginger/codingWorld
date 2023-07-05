#include <stdio.h>
#include <pthread.h>
#include "stdlib.h"
#include "assert.h"
#include "sched.h"

#define TESTSIZE 10

void hello(void)
{
  while(1)
    sched_yield();
}

int main()
{
  pthread_t thread;
  assert(pthread_create(&thread, NULL, (void*)hello, NULL) == 0  && "thread creation failed\n");
  printf("exiting:\n");
  exit(42069);
  printf("should not be printed\n");
  return 0;
}