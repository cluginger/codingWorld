#include "pthread.h"
#include "stdio.h"
#include "assert.h"
#include "defs.h"
#include "sched.h"
#include "sys/syscall.h"

void function()
{
    while(1)
    {
        sched_yield();
    }
  
}

void function2()
{
    int old;
    
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old);
    while(1){
    
    }

}

int main()
{
  pthread_t thread;
  assert(!pthread_create(&thread, NULL, (void*)&function, NULL) && "thread creation failed\n");
  assert(!pthread_cancel(thread) && "thread cancellation failed\n");
  printf(" %ld cancelled deferred\n", thread);
  assert(pthread_cancel(-1) == -1 && "thread cancellation failed\n");
  assert(pthread_cancel(50) == -1 && "thread cancellation failed\n");

  assert(!pthread_create(&thread, NULL, (void*)&function2, NULL) && "thread creation failed\n");
  assert(!pthread_cancel(thread) && "thread cancellation failed\n");
  printf(" %ld cancelled async\n", thread);
  printf("Now setting the state\n");

  int old_state;
  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old_state);
  assert(old_state == PTHREAD_CANCEL_ENABLE);
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old_state);
  assert(old_state == PTHREAD_CANCEL_DISABLE);

  printf("state works\n");

  printf("Now setting the type\n");
  int old_type;
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old_type);
  assert(old_type == PTHREAD_CANCEL_DEFERRED);
  pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &old_type);
  assert(old_type == PTHREAD_CANCEL_ASYNCHRONOUS);

  printf("type works\n");

  return 0;
}