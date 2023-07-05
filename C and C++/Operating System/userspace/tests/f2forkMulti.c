#include "stdio.h"
#include "unistd.h"
#include "sched.h"

#define MULTIFORK 6
int processesPlanned = 1;
int processesResult = 0;


void multifork()
{
  for(int z = 0; z < MULTIFORK; z++)
  {
    processesPlanned = processesPlanned * 2;
  }

  printf("Multifork: should end up with %d processes\n", processesPlanned);
  getchar();
  for(int i=0; i<MULTIFORK; ++i)
  {
    pid_t pid = fork();
    processesResult++;
    printf("process reval: %ld\n",pid);

  }
  sched_yield();

}


int main()
{
  multifork();
  sched_yield();
  return 0;
}