#include <stdio.h>
#include <pthread.h>
#include "assert.h"

#define TESTSIZE 10

void* hello()
{
    printf("Hello World\n");
    return 0;
}

int main()
{
    pthread_t t[TESTSIZE];
    for(int i = 0; i < TESTSIZE; i++)
    {
      assert(pthread_create(&t[i], NULL, &hello, NULL) == 0);
    }

    printf("Sleep 5 sec\n");
    sleep(5);

    printf("The main Thread says bye\n");

    return 0;
}