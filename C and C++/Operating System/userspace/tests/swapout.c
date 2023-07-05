#include <stdio.h>
#include <assert.h>
#include <syscall.h>

#define SIZE 20000

int main()
{
    int array[SIZE];
    for(int i = 0; i < SIZE; i++)
    {
        array[i] = 1;
    }
    printf("array item: %d\n", array[5]);
    printf("SUCCESS\n");
    __syscall(sc_swapout, 0x0, 0x0, 0x0, 0x0, 0x0);
    return 0;
}