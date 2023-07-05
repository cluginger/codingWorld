#include <stdio.h>
#include <assert.h>
#include "sched.h"
#define lol 2000

#define SIZE (4096 * 991) //pagesize times number of pages
//Size 4096000
#define THREADS 2
char array[SIZE];
void access()
{
  for(int i = 0; i < SIZE; i++)
  {
      array[i] = 1;
  }
  sched_yield();
  size_t value = SIZE - (SIZE/8);
  for(int j = 0; j < 10; j++)
  {
    for(int i = SIZE; i > value; i--)
    {
      if(i % 10000 == 0)
      {
        array[i] = 10;
      }
    }
    sched_yield();
  }

}
int main()
{
  pthread_t threads[THREADS];
  for(int i = 0; i < 10; i++)
    access();

  printf("done - getchar to continue\n");
  getchar();
  return 0;
}