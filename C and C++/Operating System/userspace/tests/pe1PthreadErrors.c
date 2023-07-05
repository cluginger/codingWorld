#include <stdio.h>
#include <pthread.h>
#include "assert.h"
#include "sched.h"

pthread_t threads[10];

void func()
{
  sched_yield();
}

void func2()
{
  while(1);
}

void selfjoin()
{
  assert(pthread_join(threads[5],NULL) == -1);
}

int main()
{
  //pthread_attr_t attribute;
  
  printf("pthread_create: ");
  assert(pthread_create(NULL, NULL, (void*)&func, NULL) == -1); //thread is NULL
  assert(pthread_create(&threads[0], NULL, NULL, NULL) == -1); // startroutine is NULL (doesnt has to be error - but we have it, why not)
//  assert(pthread_create(&threads[1], &attribute, (void*)&func, NULL) == -1); //attr is >= USERBREAK
  assert(pthread_create(&threads[2], NULL, (void*)&func2, NULL) == 0); //normal pcreate

  printf("passed\npthread_cancel: ");
  assert(pthread_cancel(threads[4]) == -1); //Thread doesnt exist
  assert(pthread_cancel(threads[2]) == 0);

  printf("passed\npthread_join: ");
  assert(pthread_detach(threads[7]) == -1);
  assert(pthread_join(threads[4],NULL) == -1);
//  assert(pthread_join(threads[2],NULL) == 0);

  assert(pthread_create(&threads[5], NULL, (void*)&selfjoin, NULL) == 0);
  //TODO: tests for: thread not joinable/
  printf("passed\n");

}
