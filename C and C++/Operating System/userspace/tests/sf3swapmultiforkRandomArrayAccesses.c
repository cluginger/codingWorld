#include <stdio.h>
#include <assert.h>
#include "unistd.h"
#include "sys/syscall.h"
#include "../../../common/include/kernel/syscall-definitions.h"

//#define SIZE ((150 *4096)/sizeof(long))
#define SIZE ((1000 *4096)/sizeof(long))

long array[SIZE];
int main()
{
  for(int i = 0; i <= SIZE; i++)
  {
    array[i] = i;
  }
  pid_t pid = fork();
  pid_t pid_2 = fork();
  pid_t pid_3 = fork();


  if(pid == 0)
  {
    printf("I am a child (%ld)\n", pid);
    for(long i = 0; i <= SIZE; i++)
    {
      assert(array[i] == i);
    }
  }
  else
  {
    printf("I am a parent (%ld)\n", pid);
    for(long i = SIZE; i >= 0 ; i--)
    {
        assert(array[i] == i);
    }
  }
  if(pid_2 == 0)
  {
    printf("I am a child (%ld)\n", pid_2);
    for(long i = SIZE/2; i <= ((SIZE/2) + SIZE/4); i++)
    {
      assert(array[i] == i);
    }
  }
  else
  {
    printf("I am a parent (%ld)\n", pid_2);
    exit(90);
  }
  if(pid_3 == 0)
  {
    printf("I am a child (%ld)\n", pid_3);
    for(long i = 0; i <= SIZE/2; i++)
    {
      long temp = array[69420];
    }
  }
  else
  {
    printf("I am a parent (%ld)\n", pid_3);
    for(long i = SIZE; i >= SIZE - (SIZE/3); i--)
    {
      assert(array[i] == i);
    }
  }

  printf("There are %zu swapped out pages where cow == 1\n",__syscall(sc_swapped_out_cow_pages, 0x0, 0x0, 0x0, 0x0, 0x0)); 
  printf("There are %zu swapped in pages where cow == 1\n",__syscall(sc_swapped_in_cow_pages, 0x0, 0x0, 0x0, 0x0, 0x0)); 

  printf("SUCCESS\n");
  return 0;
}