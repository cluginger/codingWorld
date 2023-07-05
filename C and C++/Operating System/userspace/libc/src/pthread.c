#include "pthread.h"
#include "sys/syscall.h"
#include "../../../common/include/kernel/syscall-definitions.h"
#include "sched.h"
#include <stdlib.h>



/**
 * function stub
 * posix compatible signature - do not change the signature!
 */
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine)(void *), void *arg)
{  

  return __syscall(sc_pthreadcreate, (size_t)thread, (size_t)attr, (size_t)start_routine, (size_t)arg, (size_t) wrapper);

}

void wrapper(void *(*start_routine)(void *), void *arg)
{
  pthread_exit(start_routine(arg));
}

pthread_t pthread_self(void)
{
//  return __syscall(sc_self, 0x0, 0x0, 0x0, 0x0, 0x0);
  return (pthread_t) pthreadTidRetrieve();
}

/**
 * function stub
 * posix compatible signature - do not change the signature!
 */
void pthread_exit(void *value_ptr)
{
  __syscall(sc_pthreadexit, (size_t)value_ptr, 0x0, 0x0, 0x0, 0x0);
}

/**
 * function stub
 * posix compatible signature - do not change the signature!
 */
int pthread_cancel(pthread_t thread)
{
    return __syscall(sc_pthreadcancel, (size_t)thread, 0x0, 0x0, 0x0, 0x0);
}
/**
 * function stub
 * posix compatible signature - do not change the signature!
 */
int pthread_join(pthread_t thread, void **value_ptr)
{
    return __syscall(sc_pthreadjoin, (size_t)thread, (size_t) value_ptr, 0x0, 0x0, 0x0);
}

/**
 * function stub
 * posix compatible signature - do not change the signature!
 */
int pthread_detach(pthread_t thread)
{
    return __syscall(sc_pthreaddetach, (size_t)thread, 0x0, 0x0, 0x0, 0x0);
}


/**
 * function stub
 * posix compatible signature - do not change the signature!
 */
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
{
  if(mutex == NULL || mutex->is_init)
    return -1;

  mutex->is_init = 1;
  mutex->lock_on = 0;
  return 0;
}

/**
 * function stub
 * posix compatible signature - do not change the signature!
 */
int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
  if(mutex == NULL || mutex->is_init == NULL)
    return -1;

  mutex->lock_on = 0;
  mutex->is_init = 0;
  pthread_spin_destroy(&mutex->lock_list);

  return 0;
}


int pthread_mutex_trylock(pthread_mutex_t *mutex)
{
//  printf("entering trylock\n");
  if(mutex == NULL || mutex->is_init == NULL)
    return -1;

  int old_val = 1;
  asm("xchg %0,%1"
          : "=r" (old_val)
          : "m" (mutex->lock_on), "0" (old_val)
          : "memory");
  //return 1 -> already locked
  //return 0 -> now locked
//  printf("trylock returning %d\n", old_val);
  return old_val;
}

/**
 * function stub
 * posix compatible signature - do not change the signature!
 */
int pthread_mutex_lock(pthread_mutex_t *mutex)
{
  pthread_spin_init(&mutex->lock_list, 0);
  if(mutex == NULL || mutex->is_init == NULL)
    return -1;

  size_t* mutex_stack = returnuserSpacePlaceStuff(MUTEX);

  int is_locked = pthread_mutex_trylock(mutex);
  if(!is_locked)
  {
    mutex->first_in_slist = mutex_stack;
  }
  else
  {
    pthread_spin_lock(&mutex->lock_list);
    size_t* current = mutex->first_in_slist;
    while(*current != 0)
    {
     current = (size_t*)*current;
    }
    *current = (size_t)mutex_stack;
    pthread_spin_unlock(&mutex->lock_list);
    //the current is the last!
    size_t* locked = returnuserSpacePlaceStuff(LOCKED);
    *locked = 1;
    sched_yield();
  }


  return 0;
}

/**
 * function stub
 * posix compatible signature - do not change the signature!
 */
int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
//  printf("calling unlock\n");
  int old_val = 0;
  if(mutex == NULL || mutex->is_init == NULL || mutex->lock_on == 0)
  {
    return -1;
  }
  pthread_spin_lock(&mutex->lock_list);
  size_t* firstEntryInSleepList = mutex->first_in_slist;
  size_t* next_entry = (size_t*)*firstEntryInSleepList;
  pthread_spin_unlock(&mutex->lock_list);
  if(*firstEntryInSleepList == 0)
  {
//    printf("first entry is 0");
    asm("xchg %0,%1"
            : "=r" (old_val)
            : "m" (mutex->lock_on), "0" (old_val)
            : "memory");
  }
  else
  {
    pthread_spin_lock(&mutex->lock_list);
    mutex->first_in_slist = next_entry;
    pthread_spin_unlock(&mutex->lock_list);
    size_t locked_addr = (size_t)next_entry - (sizeof(size_t) * 2);
    size_t *lockedUS = (size_t*)locked_addr;
    *lockedUS = 0;
//    printf("lockedU addr: %p\n", (void*)locked_addr);
//    printf("lockedUS to 0!\n");
  }
//  printf("leaving unlock\n");
  return 0;
}

/**
 * function stub
 * posix compatible signature - do not change the signature!
 */
int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr)
{
  return -1;
}

/**
 * function stub
 * posix compatible signature - do not change the signature!
 */
int pthread_cond_destroy(pthread_cond_t *cond)
{
  return -1;
}

/**
 * function stub
 * posix compatible signature - do not change the signature!
 */
int pthread_cond_signal(pthread_cond_t *cond)
{
  return -1;
}

/**
 * function stub
 * posix compatible signature - do not change the signature!
 */
int pthread_cond_broadcast(pthread_cond_t *cond)
{
  return -1;
}

/**
 * function stub
 * posix compatible signature - do not change the signature!
 */
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
  return -1;
}

/**
 * function stub
 * posix compatible signature - do not change the signature!
 */
int pthread_spin_destroy(pthread_spinlock_t *lock)
{

  if(!lock)
    return -1;

  if(lock->lock_on)
    return -1;
  else
    lock->lock_on = 0;

  if(lock->is_init == 0)
    return -1;
  else
    lock->is_init = 0;

  return 0;
}

/**
 * function stub
 * posix compatible signature - do not change the signature!
 */
int pthread_spin_init(pthread_spinlock_t *lock, int pshared)
{

  if(lock == NULL)
    return -1;

//with __atomic_load_n(&lock->is_init, __ATOMIC_SEQ_CST)?

  if(lock->is_init|| (!(pshared == PTHREAD_PROCESS_PRIVATE || pshared == PTHREAD_PROCESS_SHARED)))
    return -1;
  else
    lock->is_init = 1;

   lock->pshared = pshared;

  lock->lock_on = 0;


  return 0;
}

/**
 * function stub
 * posix compatible signature - do not change the signature!
 */
int pthread_spin_lock(pthread_spinlock_t *lock)
{

  if(lock == NULL || lock->is_init == 0)
    return -1;

  size_t old_val = 1;
  do
  {
    asm("xchg %0,%1"
            : "=r" (old_val)
            : "m" (*lock), "0" (old_val)
            : "memory");
  } while(old_val && !sched_yield());
  return old_val;

}

/**
 * function stub
 * posix compatible signature - do not change the signature!
 */
int pthread_spin_trylock(pthread_spinlock_t *lock)
{

  if(lock == NULL || lock->is_init == 0 || lock->lock_on)
    return -1;

  size_t old_val = 1;
  do
  {
    asm("xchg %0,%1"
            : "=r" (old_val)
            : "m" (*lock), "0" (old_val)
            : "memory");
  } while(old_val && !sched_yield());
  return old_val;
}

/**
 * function stub
 * posix compatible signature - do not change the signature!
 */
int pthread_spin_unlock(pthread_spinlock_t *lock)
{
  if(lock == NULL)
    return -1;

  if(lock->is_init == 0 ||lock->lock_on == 0 )
    return -1;

  pthread_spinlock_t* spinlock_ = lock;
  spinlock_->lock_on = 0;

  return 0;

}

/**
 * function stub
 * posix compatible signature - do not change the signature!
 */
int pthread_setcancelstate(int state, int *oldstate)
{
    return __syscall(sc_pthread_setcanceltype, (size_t)state, (size_t)oldstate, 0x0, 0x0, 0x0);
}

/**
 * function stub
 * posix compatible signature - do not change the signature!
 */
int pthread_setcanceltype(int type, int *oldtype)
{
    return __syscall(sc_pthread_setcanceltype, (size_t)type, (size_t)oldtype, 0x0, 0x0, 0x0);
}


size_t* returnuserSpacePlaceStuff(size_t stackPosition)
{
  size_t stuff = 0;
  size_t* address = &stuff; //get address of local var
  size_t hack = (size_t) address / (size_t)(__STACKSIZETOTAL);
  size_t stackStart = (size_t) hack * (size_t)(__STACKSIZETOTAL) + (__STACKSIZETOTAL);

  size_t* start = (size_t*) stackStart;

  size_t stackStart1 = stackStart - (sizeof(size_t));
  size_t* tid = (size_t* )stackStart1;

  size_t stackStart2 = stackStart - (sizeof(size_t) * 2);
  size_t* mutex = (size_t* )stackStart2;

  size_t stackStart3 = stackStart - (sizeof(size_t) * 3);
  size_t* condvar = (size_t* )stackStart3;

  size_t stackStart4 = stackStart - (sizeof(size_t) * 4);
  size_t*  locked= (size_t* )stackStart4;

//  printf("PS TID address:    %p\n", tid);
//  printf("PS TID value:      %ld\n", *tid);
//  printf("PS mutex address:  %p\n", mutex);
//  printf("PS mutex value:    %ld\n", *mutex);
//  printf("PS end address:    %p\n", end);
//  printf("PS end value:      %ld\n", *end);
  switch(stackPosition)
  {
    case CONDVAR:
//      printf("%p\n", mutex);
      return condvar;
    case MUTEX:
//      printf("%p\n", mutex);
      return mutex;
    case TID:
      return tid;
    case LOCKED:
//      printf("%p\n", locked);
      return locked;
    case START:
      return start;
    default:
      return NULL;
  }
}
size_t pthreadTidRetrieve()
{
  size_t* test = returnuserSpacePlaceStuff(TID);
//  printf("PS adress: %p\n", test);
//  printf("PS value:  %ld\n", *test);
  return *test;
}

