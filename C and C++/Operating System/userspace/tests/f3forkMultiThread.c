#include "stdio.h"
#include "unistd.h"
#include "sched.h"

#define MULTIFORK 6
int processesPlanned = 1;
int processesResult = 0;

void forkit()
{

}


int main()
{
  forkit();
  sched_yield();
  return 0;
}