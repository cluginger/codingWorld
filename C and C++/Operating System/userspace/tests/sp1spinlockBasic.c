#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>

int number = 0;

void* countup()
{
  while(1)
  {
    if(number > 50)
      break;
    printf("- %d\n", number++);
  }


  return 0;
}

int main()
{
  pthread_t thread;
  pthread_create(&thread, NULL, countup, NULL);
  while(1)
  {

    if(number > 50)
      break;
    printf("%d\n", number++);
  }

  getchar();
}