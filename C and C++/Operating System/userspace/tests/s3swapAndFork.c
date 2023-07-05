#include <stdio.h>
#include <assert.h>
#include "unistd.h"

#define SIZE ((500 * 1000)/sizeof(long))

long array[SIZE];
long array2[SIZE];

pthread_t thread;
pthread_t thread2;

void* function1()
{
    pid_t pid = fork();
    if(pid == 0)
    {
        printf("I am a child (%ld)\n", pid);
    }
    else
    {
        printf("I am a parent (%ld)\n", pid);
    }

    for(int i = 0; i < SIZE; i++)
    {
        array[i] = i;
    }
    for(int i = 0; i < SIZE; i++)
    {
        assert(array[i] == i);
    }

    return NULL;
}

void* function2()
{
    pid_t pid2 = fork();
    if(pid2 == 0)
    {
        printf("I am a child (%ld)\n", pid2);
    }
    else
    {
        printf("I am a parent (%ld)\n", pid2);
    }

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
    assert(!pthread_create(&thread, NULL, function1, NULL));
    assert(!pthread_create(&thread2, NULL, function2, NULL));
    

    pthread_join(thread, NULL);
    pthread_join(thread2, NULL);
        
    

    printf("SUCCESS\n");
    return 0;
}