#include <stdio.h>
#include <assert.h>
#include "sched.h"
#include "time.h"
#include "sys/syscall.h"
#include "../../../common/include/kernel/syscall-definitions.h"


#define SIZE (4096 * 991) //pagesize times number of pages
char array[SIZE];
#define TESTCOUNT 10

void access()
{
  for(int i = 0; i < SIZE; i++)
  {
    array[i] = 1;
  }
  sched_yield();
  size_t value = SIZE - (SIZE/8);
  for(int j = 0; j < 10; j++)
  {
    for(int i = SIZE; i > value; i--)
    {
      if(i % 10000 == 0)
      {
        array[i] = 10;
      }
    }
    sched_yield();
  }
}
void prepare()
{
  for(int i = 0; i < TESTCOUNT; i++)
    access();
}

int main()
{
  printf("change PRA with F8 to test different pra performances\n after testcase press F7 to show stats - getchar to proceed\n");
  getchar();
  prepare();
//  __syscall(sc_switch_pra, 1, 0x0, 0x0, 0x0, 0x0);
//  printf("start with AGING PRA\n");

//  __syscall(sc_switch_pra, 0, 0x0, 0x0, 0x0, 0x0);
//  printf("start with RANDOM PRA\n");
//  prepare();

//  __syscall(sc_switch_pra, 2, 0x0, 0x0, 0x0, 0x0);
//  printf("start with SECONDCHANCE PRA\n");
//  prepare();
  printf("test done.\n");
  return 0;
}