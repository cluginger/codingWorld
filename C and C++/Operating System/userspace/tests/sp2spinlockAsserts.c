#include "stdio.h"
#include "pthread.h"
#include "assert.h"

pthread_spinlock_t lock_;

void* unlockit()
{
  int check2;
  printf("LOL pthread_spin_unlock(&lock_)\n");
  check2 = pthread_spin_unlock(&lock_);
  assert(check2 == 0);
  return 0;
}

int main()
{
  pthread_t thread;

  int check;
  printf("1 pthread_spin_init(&lock_, 3)\n");
  check = pthread_spin_init(&lock_, 3);
  assert(check == -1);

  printf("2 pthread_spin_init(&lock_, NULL)\n");
  check = pthread_spin_init(&lock_, NULL);
  assert(check == 0);

  printf("3 pthread_spin_unlock(&lock_)\n");
  check = pthread_spin_unlock(&lock_);
  assert(check == -1);

  printf("4 pthread_spin_lock(0)\n");
  check = pthread_spin_lock(0);
  assert(check == -1);

  printf("5 pthread_spin_trylock(0)\n");
  check = pthread_spin_trylock(0);
  assert(check == -1);

  printf("6 pthread_spin_unlock(0)\n");
  check = pthread_spin_unlock(0);
  assert(check == -1);

  printf("7 pthread_spin_lock(&lock_)\n");
  check = pthread_spin_lock(&lock_);
  assert(check == 0);

  printf("8 pthread_spin_trylock(&lock_)\n");
  check = pthread_spin_trylock(&lock_);
  assert(check == -1);


  pthread_create(&thread, NULL, unlockit, NULL);
  printf("2nd thread unlocking\n");
  getchar();

  printf("10 pthread_spin_unlock(&lock_)\n");
  check = pthread_spin_unlock(&lock_);
  assert(check == -1);

  printf("11 pthread_spin_trylock(&lock_)\n");
  check = pthread_spin_trylock(&lock_);
  assert(check == 0);

  printf("12 pthread_spin_unlock(&lock_)\n");
  check = pthread_spin_unlock(&lock_);
  assert(check == 0);

}
