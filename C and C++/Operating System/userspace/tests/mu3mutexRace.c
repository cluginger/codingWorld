#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include "assert.h"

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
#define THREADS 100
int letsgo = 0;
int number = 0;

void func()
{
  while(letsgo != 1){sched_yield();}
  pthread_mutex_lock(&mut);
  number += 1;
  printf("THREAD %ld number -> %d\n", pthread_self(), number);
  pthread_mutex_unlock(&mut);

}

int main()
{
  pthread_t t[THREADS];
  for(int i = 0; i < THREADS; i++)
  {
    assert(pthread_create(t + i, NULL, (void*)func, NULL) == 0  && "thread creation failed\n");
  }
  letsgo = 1;
  getchar();
}