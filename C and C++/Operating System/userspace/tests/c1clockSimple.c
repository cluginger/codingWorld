#include "stdio.h"
#include "time.h"
#include "assert.h"
#include "unistd.h"

void sleepy()
{
  printf("Sleep Test about 10sec\n");
  sleep(10);
  printf("done sleepin -> now usleeping 5,000,000 usec (~5sec)\n");
  usleep(5000000);
  printf("done usleepin\n");
}

void clocksleep()
{
  printf("Testing 2 clock with 5sec sleep inbetween\n");
  size_t before = clock();
  sleep(5);
  size_t after = clock();
  float passed_time = (float)(after - before)/CLOCKS_PER_SEC;
  printf("\nPassed time: %f\n",passed_time);

}


int main()
{
  long double test;
  clock_t clocks[10];
  printf("Check of 10 clocks: ");
  for(int i = 0; i < 10; i++)
  {
    clocks[i] = clock();
    if(i > 0)
      assert(clocks[i] > clocks[i-1] && "time decreased??\n");
  }
  printf("all good\n\n");

  printf("Clock test 10 Sec wait period\nStart with getchar");
  getchar();
  clock_t clock_1 = clock();
  for(int i = 0; i < 99999999; i++)
  {
    test = test * test;
    test *= test;
  }
  clock_t clock_2 = clock();
  float passed_time = (float)(clock_2 - clock_1)/CLOCKS_PER_SEC;
  printf("\nPassed time: %f\n",passed_time );
  sleepy();
  clocksleep();
  return 0;
}