#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "assert.h"
#define TESTSIZE 1000000
#define SIZE ((1000 * 4096)/sizeof(long))
long array[SIZE];

int main()
{
    printf("write\n");
    for(int i = 0; i <= SIZE; i++)
    {
      if(i % 100000 == 0)
      {
        printf("%d\n", i);
        array[i] = i;
      }
    }
    printf("read\n");
    for(long i = SIZE; i >= 0; i--)
    {
      if(i % 100000 == 0)
      {
        printf("%ld\n", i);
        assert(array[i] == i);
      }
    }

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