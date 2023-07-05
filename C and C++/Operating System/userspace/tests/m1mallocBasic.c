#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "assert.h"
#define TESTSIZE 1000

int main()
{
  int* malloced = malloc(TESTSIZE * sizeof(int));
  assert(malloced != NULL);
  int iter = 0;
  while(iter != TESTSIZE)
  {
    malloced[iter] = iter;
    iter++;
  }
  printf("getchar to free all of it\n");
  getchar();
  free(malloced);
  printf("all gone\n");
  getchar();

  return 0;
}