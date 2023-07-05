#include "pthread.h"
#include "stdio.h"
#include "assert.h"
#include "defs.h"
#include "sched.h"

void function()
{
  int old;
    
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old);
    while(1){
    
    }

}

void create_and_cancel()
{
  pthread_t t1;
  assert(pthread_create(&t1, NULL, (void*)&function, NULL) == 0 && "thread creation failed\n");
  assert(pthread_cancel(t1) == 0 && "thread cancellation failed\n");
}

void cancel_multi()
{
  pthread_t thread[10];
  for(int i = 0; i < 10; i++)
  {
    assert(pthread_create(&thread[i], NULL, (void*)&function, (void*)1)  == 0 && "thread creation failed\n");
  }
  for(int j = 0; j < 10; j++)
  {
    assert(pthread_cancel(thread[j]) == 0 && "thread cancellation failed\n");
    printf(" %ld cancelled\n", thread[j]);
  }
}

int main()
{
  create_and_cancel();
  printf("pcancel basic done\n");
  cancel_multi();
  printf("pcancel multi done\n");
  return 0;
}