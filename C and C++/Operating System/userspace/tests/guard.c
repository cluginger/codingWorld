#include "pthread.h"
#include "stdio.h"
#include "assert.h"

#define SIZE 100000000

void* func()
{
    int array[SIZE];
    for(int i = 0; i < SIZE; i++)
    {
        array[i] = 10;
    }
    printf("%d\n", array[0]);

    return 0;
}

int main()
{
    pthread_t thread[50];
    for(int i = 0; i < 50; i++)
    {
        pthread_create(&thread[i], NULL, &func, NULL);
    }
    getchar();
    return 0;
}