#include "pthread.h"
#include "stdio.h"
#include "assert.h"


void function()
{
  printf("new function\n");
  sleep(5);
}


int main()
{
    pthread_t thread_1;
    assert(pthread_create(&thread_1, NULL, (void*)function, NULL) == 0);
    assert(pthread_join(thread_1, NULL) == 0);
    return 0;
}