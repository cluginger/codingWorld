#include <stdio.h>
#include <pthread.h>
#include "assert.h"
#include "sched.h"


void hello()
{
  for(int i = 0; i < 5; i++)
  {
    sched_yield();
  }
}


int main()
{
  hello();
  printf("should be printed\n");
  exit(1234);

  printf("should not be printed\n");
  return 0;
}