#include <stdio.h>
#include <assert.h>
#include "pthread.h"
#include "sched.h"

#define SIZE 520000
#define THREADCOUNT 3
long array[SIZE];
long array2[SIZE];
pthread_t thread[THREADCOUNT];
pthread_t thread2[THREADCOUNT];
void* function()
{
    for(int i = 0; i < 10; i++)
    {
        array[i] = i;
    }
    for(int i = 0; i < 10; i++)
    {
        assert(array[i] == i);
    }
    return NULL;
}
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
    for(int i = 0; i < THREADCOUNT; i++)
    {
        printf("%d create\n", i);
        assert(!pthread_create(&thread[i], NULL, function, NULL));
        assert(!pthread_create(&thread2[i], NULL, function2, NULL));
    }
    //sched_yield();
    
    for(int i = 0; i < THREADCOUNT; i++)
    {
        printf("%d join\n", i);
        pthread_join(thread[i], NULL);
        pthread_join(thread2[i], NULL);
        
    }
    
    printf("SUCCESS\n");
    return 0;
}