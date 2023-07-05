#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include "assert.h"

int main()
{
  pthread_mutex_t mut1;
  pthread_mutex_t mut2; //not init

  assert(pthread_mutex_init(NULL, NULL) == -1);
  assert(pthread_mutex_init(&mut1, NULL) == 0);
  assert(pthread_mutex_init(&mut1, NULL) == -1);

  assert(pthread_mutex_lock(NULL) == -1);
  assert(pthread_mutex_lock(&mut2) == -1);
  assert(pthread_mutex_lock(&mut1) == 0);

  pthread_mutex_t mut3 = PTHREAD_MUTEX_INITIALIZER;
  assert(pthread_mutex_trylock(&mut3) == 0);
  assert(pthread_mutex_trylock(NULL) == 0);
  assert(pthread_mutex_trylock(&mut2) == -1);
  assert(pthread_mutex_trylock(&mut3) == 1);

  pthread_mutex_t mut4 = PTHREAD_MUTEX_INITIALIZER;
  assert(pthread_mutex_unlock(NULL) == -1);
  assert(pthread_mutex_unlock(&mut2) == -1);
  assert(pthread_mutex_unlock(&mut4) == -1);
  assert(pthread_mutex_unlock(&mut1) == 1);
  assert(pthread_mutex_unlock(&mut1) == 0);

  assert(pthread_mutex_destroy(NULL) == -1);
  assert(pthread_mutex_destroy(&mut2) == -1);
  assert(pthread_mutex_destroy(&mut1) == 0);
}