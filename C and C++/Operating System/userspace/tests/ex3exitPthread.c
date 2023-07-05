#include <stdio.h>
#include <pthread.h>
#include <assert.h>


void* hello()
{
  printf("should be printed\n");
  exit(1234);
  printf("should not be printed\n");
  return 0;
}

int main()
{
    pthread_t t1;
    assert(pthread_create(&t1, NULL, &hello, NULL) == 0  && "thread creation failed\n");
    while(1);
    return 0;
}