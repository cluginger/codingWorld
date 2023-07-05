#include <stdio.h>
#include <syscall.h>


int main()
{
    __syscall(sc_swap_out_in_test, 0x0, 0x0, 0x0, 0x0, 0x0);
    
    return 0;
}