#include <stdio.h>
#include <pthread.h>
#include "assert.h"
#include "sched.h"

void hello(void* n)
{
  if(n != NULL)
    printf("%s\n", (char*)n);
}

void create()
{
  pthread_t t1;
  char * arg = "hello world";
  assert(pthread_create(&t1, NULL, (void*)hello, arg) == 0 && "thread creation failed\n");
  sched_yield();
}


int main()
{
  create();
  printf("pcreate basic done\n");

  return 0;
}