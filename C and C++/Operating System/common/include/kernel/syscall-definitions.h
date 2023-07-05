#pragma once

#define fd_stdin 0
#define fd_stdout 1
#define fd_stderr 2

#define sc_exit 1
#define sc_fork 2
#define sc_read 3
#define sc_write 4
#define sc_open 5
#define sc_close 6
#define sc_lseek 19
#define sc_pseudols 43
#define sc_outline 105
#define sc_sched_yield 158
#define sc_createprocess 191
#define sc_trace 252

//AG 2
#define sc_brk 2000
#define sc_sbrk 2001
#define sc_returnCurrentHeap 20011
#define sc_swapout 2002
#define sc_swapin 2003

#define sc_random_pra 2010
#define sc_mean_pra 2020
#define sc_stddeviation_pra 2021

#define sc_swap_out_in_test 2050

#define sc_swapped_out_cow_pages 2080
#define sc_swapped_in_cow_pages 2081



//AG 1
#define sc_pthreadcreate 1000
#define sc_pthreadexit 1001
#define sc_pthreadcancel 1002
#define sc_pthreadjoin 1003
#define sc_pthreaddetach 1004
#define sc_pthread_setcancelstate 1005
#define sc_pthread_setcanceltype 1006
#define sc_clock 1007
#define sc_sleep 1008
#define sc_usleep 1009
#define sc_waitpid 1010
#define sc_execv 1011





