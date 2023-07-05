#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include "assert.h"
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

void thread()
{
//  printf("thread calling mlock!\n");
  pthread_mutex_lock(&mut);
  printf("this should not be printed\n");
  while(1);
}


int main()
{

  pthread_t t1, t2;
  pthread_mutex_lock(&mut);
  assert(pthread_create(&t1, NULL, (void*)thread, NULL) == 0);
  assert(pthread_create(&t2, NULL, (void*)thread, NULL) == 0);
  getchar();
  pthread_mutex_unlock(&mut);
  pthread_mutex_unlock(&mut);
  getchar();
}