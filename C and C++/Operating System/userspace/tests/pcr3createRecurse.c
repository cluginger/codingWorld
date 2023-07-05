
#include <stdio.h>
#include <pthread.h>
#include "assert.h"
#include "sched.h"


void create();

int main()
{
  pthread_t t1;
  assert(pthread_create(&t1, NULL, (void*)create, NULL) == 0 && "thread creation failed\n");
  pthread_exit(0);
  assert(pthread_join(t1, NULL) == 0);
  return 0;
}

void create()
{
  pthread_t t2;
  assert(pthread_create(&t2, NULL, (void*)create, NULL) == 0 && "thread creation failed\n");
  assert(pthread_join(t2, NULL) == 0);
  sched_yield();
}


