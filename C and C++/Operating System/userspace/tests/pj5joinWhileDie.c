#include "pthread.h"
#include "stdio.h"
#include "assert.h"

pthread_t t[3];

void endless()
{
  while(1);
}

void cancel()
{
  assert(pthread_cancel(t[0]) == 0);
  printf("canceled\n");
}

void join()
{
  assert(pthread_join(t[0], NULL) == 0);
  printf("join\n");
}

int main()
{
  assert(pthread_create(&t[0], NULL, (void*)endless, NULL) == 0);
  printf("created endless thread\n");
  assert(pthread_create(&t[1], NULL, (void*)cancel, NULL) == 0);
  printf("created cancel thread\n");
  assert(pthread_create(&t[2], NULL, (void*)join, NULL) == 0);
  printf("created join thread\n");
  getchar();
  return 0;
}