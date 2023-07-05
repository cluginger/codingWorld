#pragma once

#include <types.h>
#include "Thread.h"
#include "Scheduler.h"
#include "kprintf.h"
#include "uvector.h"

class Syscall
{
  public:
    static size_t syscallException(size_t syscall_number, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5);

    static void exit(size_t exit_code);
    static void outline(size_t port, pointer text);
    static size_t write(size_t fd, pointer buffer, size_t size);
    static size_t read(size_t fd, pointer buffer, size_t count);
    static size_t close(size_t fd);
    static size_t open(size_t path, size_t flags);
    static size_t createprocess(size_t path, size_t sleep);
    static void pseudols(const char *pathname, char *buffer, size_t size);
    static void trace();

    //AG2
    static void* sbrk(intptr_t inc);
    static int brk(void *end, bool called_by_sbrk);




    //AG1
    static size_t pthread_create(size_t thread, size_t attr, size_t start_routine, size_t arg, size_t wrapper);
    static void pthread_exit(size_t retval);
    static size_t pthread_cancel(size_t thread);
    static size_t pthread_join(size_t thread, void **retval);
    static size_t fork();
    static size_t pthread_setcanceltype(int type, int *old_type);
    static size_t pthread_setcancelstate(int state, int *old_state);
    static size_t pthread_detach(size_t thread);
    static size_t waitpid(size_t pid, size_t* status, size_t options);

    static void swapOnePageOutAndInTest();
    static void testRandom_PRA();
    static size_t getMeanOfPRASample();
    //static double getSTDDeviationOfPRASample();
    //static size_t execv(size_t path, size_t argv);
};

