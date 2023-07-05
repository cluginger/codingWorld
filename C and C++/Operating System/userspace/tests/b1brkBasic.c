#include "stdio.h"
#include "unistd.h"
#include "pthread.h"
#include "assert.h"
#include "syscall.h"

int main()
{
  void* startvalue = sbrk(0);
  printf("sbrk\n");
  getchar();
  sbrk(10000);
  int* test = startvalue + 5000;
  *test = 20;
  printf("brk\n");
//  getchar();
  brk(startvalue);
  return 0;

}