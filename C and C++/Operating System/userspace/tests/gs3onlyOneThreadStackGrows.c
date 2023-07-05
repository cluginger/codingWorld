#include "pthread.h"
#include "stdio.h"
#include "assert.h"

#define SIZE 20000

void func()
{
    int array[SIZE];

    for(int i = 0; i < SIZE; i++)
    {
        array[i] = 10;
    }
    int* address1 = &array[SIZE-1];
    int* address2 = &array[SIZE];
    printf("address1: %p\n", address1);
    printf("address2: %p\n", address2);

    int diff = address2 - address1;
    printf("diff: %d\n", diff);
    assert(diff == 1);
}
void func2()
{
  while(1);
}

int main()
{
    pthread_t thread[5];

    for(int i = 0; i < 5; i++)
    {
      if(i == 3)
        assert(pthread_create(&thread[i], NULL, (void*)func, NULL) == 0);

      assert(pthread_create(&thread[i], NULL, (void*)func2, NULL) == 0);
    }
    sleep(2);
    return 0;
}