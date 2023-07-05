#include <stdio.h>
#include "sys/syscall.h"
#include "../../../common/include/kernel/syscall-definitions.h"
#include "sched.h"

#define SIZE 1500000
long array[SIZE];


int main()
{
    for(int i = 0; i < SIZE; i++)
    {
        array[i] = i;
    }

    __syscall(sc_random_pra, 0x0, 0x0, 0x0, 0x0, 0x0); 

    return 0;
}