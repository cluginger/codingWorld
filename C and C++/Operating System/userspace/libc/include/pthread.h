#pragma once

#include "types.h"
#include "stdio.h"

#define __NUM_PAGES 32
#define __PAGE_SIZE 4096
#define __STACKSIZETOTAL (__NUM_PAGES*__PAGE_SIZE)

#ifdef __cplusplus
extern "C" {
#endif

//pthread typedefs
typedef size_t pthread_t;
typedef unsigned int pthread_attr_t;



//pthread spinlock typedefs
typedef struct{
    int lock_on;
    int pshared;
    int is_init;
}pthread_spinlock_t;

//pthread mutex typedefs

typedef struct{
    int lock_on;
    int is_init;
    size_t*  first_in_slist;
    pthread_spinlock_t  lock_list;
}pthread_mutex_t;



typedef unsigned int pthread_mutexattr_t;
#define PTHREAD_MUTEX_INITIALIZER {.is_init = 1};
#define PTHREAD_SPINLOCK_INITIALIZER {.lock_on = 0, .pshared = 0, .is_init = 1};


enum pshared{PTHREAD_PROCESS_PRIVATE, PTHREAD_PROCESS_SHARED};

//pthread cond typedefs
typedef unsigned int pthread_cond_t;
typedef unsigned int pthread_condattr_t;

extern int pthread_create(pthread_t *thread,
         const pthread_attr_t *attr, void *(*start_routine)(void *),
         void *arg);

void wrapper(void *(*start_routine)(void *), void *arg);

extern void pthread_exit(void *value_ptr);

extern int pthread_cancel(pthread_t thread);

extern int pthread_join(pthread_t thread, void **value_ptr);

extern int pthread_detach(pthread_t thread);

extern int pthread_mutex_init(pthread_mutex_t *mutex,
                              const pthread_mutexattr_t *attr);

extern int pthread_mutex_destroy(pthread_mutex_t *mutex);
extern int pthread_mutex_lock(pthread_mutex_t *mutex);
extern int pthread_mutex_trylock(pthread_mutex_t *mutex);
extern int pthread_mutex_unlock(pthread_mutex_t *mutex);

extern int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);

extern int pthread_cond_destroy(pthread_cond_t *cond);

extern int pthread_cond_signal(pthread_cond_t *cond);

extern int pthread_cond_broadcast(pthread_cond_t *cond);

extern int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);

extern int pthread_setcancelstate(int state, int *oldstate);

extern int pthread_setcanceltype(int type, int *oldtype);

extern int pthread_spin_init(pthread_spinlock_t *lock, int pshared);
extern int pthread_spin_lock(pthread_spinlock_t *lock);
extern int pthread_spin_destroy(pthread_spinlock_t *lock);
extern int pthread_spin_trylock(pthread_spinlock_t *lock);
extern int pthread_spin_unlock(pthread_spinlock_t *lock);

extern pthread_t pthread_self();
enum stackPosition{CONDVAR, MUTEX, TID, LOCKED, START};
extern size_t pthreadTidRetrieve();
extern size_t* returnuserSpacePlaceStuff(size_t case_);

#ifdef __cplusplus
}
#endif


