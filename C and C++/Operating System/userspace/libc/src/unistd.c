#include "unistd.h"
#include "../../../common/include/kernel/syscall-definitions.h"
#include "sys/syscall.h"


/**
 * function stub
 * posix compatible signature - do not change the signature!
 */
int brk(void *end_data_segment)
{
  return __syscall(sc_brk, (size_t) end_data_segment, 0x0, 0x0, 0x0, 0x0);
}

/**
 * function stub
 * posix compatible signature - do not change the signature!
 */
void* sbrk(intptr_t increment)
{
  return (void*) __syscall(sc_sbrk, (size_t) increment, 0x0, 0x0, 0x0, 0x0);
}


/**
 * function stub
 * posix compatible signature - do not change the signature!
 */
unsigned int sleep(unsigned int seconds)
{
  return (unsigned int)__syscall(sc_sleep, seconds, 0x0, 0x0, 0x0, 0x0);
}

int usleep(useconds_t usec)
{
  return (int)__syscall(sc_usleep, usec, 0x0, 0x0, 0x0, 0x0);
}


/**
 * function stub
 * posix compatible signature - do not change the signature!
 */
int ftruncate(int fildes, off_t length)
{
    return -1;
}
