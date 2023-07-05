#include "stdio.h"
#include "time.h"
#include "assert.h"
#include "unistd.h"
#include "pthread.h"
#include "sched.h"


long double test;
void clocking()
{
  printf("starting\n");
  clock_t clock_1 = clock();
  for(int i = 0; i < 999999; i++)
  {
    test = test * test;
    test *= test;
  }
  clock_t clock_2 = clock();
  float passed_time = (float)(clock_2 - clock_1)/CLOCKS_PER_SEC;
  printf("Passed time: %f\n",passed_time );
}


int main()
{
  pthread_t threads[10];
  for(int i = 0; i < 10; i++)
  {
    pthread_create(&threads[i], NULL,(void*)clocking, NULL);
  }
  getchar();
  return 0;
}