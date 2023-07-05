#include <stdio.h>
#include <pthread.h>
#include "assert.h"
#include "sched.h"
#include <unistd.h>

#define TESTSIZE 10

void* hello()
{
    char *args[] = {"endprocess.c", (char*)NULL};

    char *path = "/usr/endprocess.sweb"; 

    if(execv(path, args) != 0)
    {

    }
        
  

    
    return 0;
}


int main()
{
    pthread_t thread[TESTSIZE];
    for(int i = 0; i < TESTSIZE; i++)
    {
      int ret = pthread_create(&thread[i], NULL, &hello, NULL);
      printf("%d\n",i);
      ret++;
    }

  

  return 0;
}