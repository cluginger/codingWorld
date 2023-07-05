#include <stdio.h>
#include <assert.h>
#include "unistd.h"

//#define SIZE ((150 *4096)/sizeof(long))
#define SIZE ((150 *4096)/sizeof(long))

long array[SIZE];
int main()
{

    pid_t pid = fork();
    pid_t pid_2 = fork();
    pid_t pid_3 = fork();

    if(pid == 0)
    {
        printf("I am a child (%ld)\n", pid);
    }
    else
    {
        printf("I am a parent (%ld)\n", pid);
    }
    if(pid_2 == 0)
    {
        printf("I am a child (%ld)\n", pid_2);
    }
    else
    {
        printf("I am a parent (%ld)\n", pid_2);
    }
    if(pid_3 == 0)
    {
        printf("I am a child (%ld)\n", pid_3);
    }
    else
    {
        printf("I am a parent (%ld)\n", pid_3);
    }
    

    for(int i = 0; i < SIZE; i++)
    {
        array[i] = i;
    }
    for(int i = 0; i < SIZE; i++)
    {
        assert(array[i] == i);
    }


    printf("SUCCESS\n");
    return 0;
}