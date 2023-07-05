#include "stdio.h"
#include "unistd.h"
#include "pthread.h"
#include "assert.h"
#include "syscall.h"

int main()
{
  int thething = 10;
  void* heap1 = sbrk(0);
  sbrk(1000);
  size_t* heap2 = sbrk(0);

  printf("start %p | enhanced %p\n", heap1, heap2);
  assert(heap2 == heap1 + 1000);

  sbrk(-1000);
  heap2 = sbrk(0);
  *heap2 = 3;
  printf("1 -> %p | 2 -> %p\n", heap1, heap2);
  assert(heap1 == heap2);
  sbrk(2000);
  int* pointer = sbrk(0) - 1000;
  *pointer = 3;
  sbrk(-2000);
  assert(sbrk(0) == heap1);

  void* oldval = sbrk(4000);
  void* brk_value = sbrk(0);
  sbrk(-4000);
  assert(oldval == sbrk(0));
//  size_t currentHeap = __syscall(sc_returnCurrentHeap,0x0,0x0,0x0,0x0,0x0);
  brk(brk_value);
  assert(sbrk(0) ==  brk_value);



//  printf("0  %p\n", sbrk(0));
//  sbrk(100);
//  printf("4096 %p\n", sbrk(0));
//  sbrk(1000);
//  printf("4096 %p\n", sbrk(0));
//  sbrk(-100);
//  printf("-4095 %p\n", sbrk(0));
  return 0;

}