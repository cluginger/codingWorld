#include <stdio.h>
#include <assert.h>
#include "unistd.h"
#include "sched.h"
#include "pthread.h"

//#define SIZE ((150 *4096)/sizeof(long))
#define SIZE ((1000 *4096)/sizeof(long))
#define THREADCOUNT 5

long array[SIZE];

void forkit()
{
  for(int i = 0; i < SIZE; i++)
  {
    array[i] = i;
  }
  for(int i = 0; i < SIZE; i++)
  {
    assert(array[i] == i);
  }

  pid_t pid = fork();
  if(pid == 0)
  {
    printf("I am a child (%ld)\n", pid);
  }
  else
  {
    printf("I am a parent (%ld)\n", pid);
  }

}

int main()
{
  pthread_t threads[THREADCOUNT];
  for(int i = 0; i < THREADCOUNT; i++)
  {
    pthread_create(&threads[i], NULL, (void*)forkit, NULL);
  }
  sched_yield();
  sched_yield();
  sched_yield();
  sched_yield();
  getchar();
  printf("SUCCESS\n");
  return 0;
}