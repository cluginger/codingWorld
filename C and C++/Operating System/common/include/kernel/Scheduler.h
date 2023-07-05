#pragma once

#include "types.h"
#include <ulist.h>
#include "IdleThread.h"
#include "CleanupThread.h"
#include "SwapThread.h"
#define US_TICKS 54925


class Thread;
class Mutex;
class SpinLock;
class Lock;

class Scheduler
{
  public:
    static Scheduler *instance();

    void addNewThread(Thread *thread);
    void sleep();
    void wake(Thread *thread_to_wake);
    void yield();
    void printThreadList();
    void printStackTraces();
    void printLockingInformation();
    bool isSchedulingEnabled();
    bool isCurrentlyCleaningUp();
    void incTicks();
    size_t getTicks();
    uint64 getRdtscTicks();
    void checkAcessedBits();
    void shiftAge();
    size_t getRandomSeed();

    void setTime();
    uint64 returnTime();
    void sleep_(uint64 seconds);
    void usleep_(uint64 useconds);
    uint64 processTimeSTART;
    uint64 rdtscNew, rdtscOld, rdtscDiff, rdtscDiffNS, rdtscDiffUS;
    uint64 rdtscDEV, rdtscDEVBound, rdtscAVGDiff, rdtscAVGCount;
    bool start_time = true;
    bool clockDone = false;
    uint64 rdtsc_us, rdtsc_sec;


    /**
     * NEVER EVER EVER CALL THIS METHOD OUTSIDE OF AN INTERRUPT CONTEXT
     * this is the method that decides which threads will be scheduled next
     * it is called by either the timer interrupt handler or the yield interrupt handler
     * and changes the global variables currentThread and currentThreadRegisters
     */
    void schedule();

  protected:
    friend class IdleThread;
    friend class CleanupThread;
    friend class SwapThread;

    void cleanupDeadThreads();
    

  private:
    Scheduler();

    /**
     * Scheduler internal lock abstraction method
     * locks the thread-list against concurrent access by prohibiting a thread switch
     * don't call this from an Interrupt-Handler, since Atomicity won't be guaranteed
     */
    void lockScheduling();

    /**
     * Scheduler internal lock abstraction method
     * unlocks the thread-list
     */
    void unlockScheduling();

    static Scheduler *instance_;

    typedef ustl::list<Thread*> ThreadList;
    ThreadList threads_;

    size_t block_scheduling_;

    size_t ticks_;

    IdleThread idle_thread_;
    CleanupThread cleanup_thread_;
    SwapThread swap_thread_;
};