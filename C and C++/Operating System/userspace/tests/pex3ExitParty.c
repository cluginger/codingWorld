#include <stdio.h>
#include <pthread.h>
#include "assert.h"
#include "sched.h"

pthread_spinlock_t spin = PTHREAD_SPINLOCK_INITIALIZER;

#define TESTSIZE 200

void hello(void)
{
  pthread_spin_lock(&spin);
  printf("exit\n");
//  pthread_exit(0);
  pthread_spin_unlock(&spin);
}

int main()
{
  pthread_t thread[TESTSIZE];
  pthread_spin_lock(&spin);
  for(int i = 0; i < TESTSIZE; i++)
  {
    assert(pthread_create(&thread[i], NULL, (void*)hello, NULL) == 0  && "thread creation failed\n");
    printf("%d\n",i);
    if(i == TESTSIZE - 1)
        printf("press getchar() to exit all\n");
  }
  getchar();
  pthread_spin_unlock(&spin);


  return 0;
}