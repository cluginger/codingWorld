#include <stdio.h>
#include <assert.h>

#define SIZE ((1000 * 4096)/sizeof(long))
long array[SIZE];
int main()
{
    printf("write\n");
    for(int i = 0; i <= SIZE; i++)
    {
      if(i % 100000 == 0)
        printf("%d\n", i);
      array[i] = i;
    }
    printf("read\n");
    for(long i = SIZE; i >= 0; i--)
    {
      if(i % 100000 == 0)
        printf("%ld\n", i);
      assert(array[i] == i);
    }
    printf("SUCCESS\n");
    return 0;
}