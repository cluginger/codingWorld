#include "stdio.h"
#include "unistd.h"
#include "sched.h"

void fork_test()
{
  pid_t pid = fork();
  if(pid == 0)
  {
    printf("I am a child (%ld)\n", pid);
  }
  else
  {
    printf("I am a parent (%ld)\n", pid);
  }
  printf("fork done (printed twice)\n");

}

int main()
{
    fork_test();
    sched_yield();
    return 0;
}