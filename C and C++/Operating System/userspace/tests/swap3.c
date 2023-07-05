#include <stdio.h>
#include <assert.h>
#include "pthread.h"
#include "sched.h"

#define SIZE 520000

long array2[SIZE];
pthread_t thread;
pthread_t thread2;

void* function2()
{
    for(int i = 0; i < SIZE; i++)
    {
        array2[i] = i;
    }
    for(int i = 0; i < SIZE; i++)
    {
        assert(array2[i] == i);
    }
    return NULL;
}
int main()
{
 
    printf("create\n");
    assert(!pthread_create(&thread, NULL, function2, NULL));
    //assert(!pthread_create(&thread2, NULL, function2, NULL));

    //sched_yield();
    
  
    printf("join\n");
    pthread_join(thread, NULL);
    //pthread_join(thread2, NULL);
    
    printf("SUCCESS\n");
    return 0;
}