#include "Scheduler.h"
#include "Thread.h"
#include "panic.h"
#include "ArchThreads.h"
#include "ArchCommon.h"
#include "kprintf.h"
#include "ArchInterrupts.h"
#include "KernelMemoryManager.h"
#include <ulist.h>
#include "backtrace.h"
#include "ArchThreads.h"
#include "Mutex.h"
#include "umap.h"
#include "ustring.h"
#include "Lock.h"
#include "UserThread.h"
#include "Syscall.h"
#include "Loader.h"
#include "ArchMemory.h"

ArchThreadRegisters *currentThreadRegisters;
Thread *currentThread;

Scheduler *Scheduler::instance_ = nullptr;

Scheduler *Scheduler::instance()
{
  if (unlikely(!instance_))
    instance_ = new Scheduler();
  return instance_;
}

Scheduler::Scheduler()
{
  block_scheduling_ = 0;
  ticks_ = 0;
  addNewThread(&cleanup_thread_);
  addNewThread(&idle_thread_);
  addNewThread(&swap_thread_);
}

void Scheduler::schedule()
{

  assert(!ArchInterrupts::testIFSet() && "Tried to schedule with Interrupts enabled");
  if (block_scheduling_)
  {
    debug(SCHEDULER, "schedule: currently blocked\n");
    return;
  }
  auto it = threads_.begin();
  for(; it != threads_.end(); ++it)
  {
    if(start_time)
      start_time = false;
    else if(!start_time && currentThread && currentThread->loader_)
    {
      auto uT = (UserThread*)currentThread;
      uT->parent_process_->processTime = uT->parent_process_->processTime + (getRdtscTicks() - processTimeSTART)/rdtsc_us;
    }
    if((*it)->schedulable())
    {
//      if((*it)->loader_)
//      {
//        auto uThread = (UserThread *)(*it);
//        if (uThread->isUthreadlocked())
//          continue;
//      }
      currentThread = *it;
      auto uThread = (UserThread*)currentThread;
      if(uThread->cancel_flag_ && uThread->cancel_type_ == PTHREAD_CANCEL_ASYNCHRONOUS
      && uThread->cancel_state_ == PTHREAD_CANCEL_ENABLE && uThread->switch_to_userspace_ == 1)
      {
        uThread->kernel_registers_->rip = (uint64_t)Syscall::pthread_exit;
        uThread->kernel_registers_->rdi = PTHREAD_CANCELED;
        uThread->switch_to_userspace_ = 0;
      }
      checkAcessedBits();
      processTimeSTART = getRdtscTicks();
      break;
    }
  }

  assert(it != threads_.end() && "No schedulable thread found");
  ustl::rotate(threads_.begin(), it + 1, threads_.end()); // no new/delete here - important because interrupts are disabled
  //debug(SCHEDULER, "Scheduler::schedule: new currentThread is %p %s, switch_to_userspace: %d\n", currentThread, currentThread->getName(), currentThread->switch_to_userspace_);

  currentThreadRegisters = currentThread->switch_to_userspace_ ? currentThread->user_registers_ : currentThread->kernel_registers_;

}

void Scheduler::addNewThread(Thread *thread)
{
  assert(thread);
  debug(SCHEDULER, "addNewThread: %p  %zd:%s\n", thread, thread->getTID(), thread->getName());
  if (currentThread)
  {
    ArchThreads::debugCheckNewThread(thread);
  }

  KernelMemoryManager::instance()->getKMMLock().acquire();
  lockScheduling();
  KernelMemoryManager::instance()->getKMMLock().release();
  threads_.push_back(thread);
  unlockScheduling();
}

void Scheduler::sleep()
{
  currentThread->setState(Sleeping);
  assert(block_scheduling_ == 0);
  yield();
}

void Scheduler::wake(Thread* thread_to_wake)
{
  // wait until the thread is sleeping
  while(thread_to_wake->getState() != Sleeping)
    yield();
  thread_to_wake->setState(Running);
}

void Scheduler::yield()
{
  assert(this);
  if (!ArchInterrupts::testIFSet())
  {
    assert(currentThread);
    kprintfd("Scheduler::yield: WARNING Interrupts disabled, do you really want to yield ? (currentThread %p %s)\n",
             currentThread, currentThread->name_.c_str());
    currentThread->printBacktrace();
  }
  ArchThreads::yield();
}

void Scheduler::cleanupDeadThreads()
{
  /* Before adding new functionality to this function, consider if that
     functionality could be implemented more cleanly in another place.
     (e.g. Thread/Process destructor) */

  assert(currentThread == &cleanup_thread_);

  lockScheduling();
  uint32 thread_count_max = sizeof(cleanup_thread_.kernel_stack_) / (2 * sizeof(Thread*));
  thread_count_max = ustl::min(thread_count_max, threads_.size());
  Thread* destroy_list[thread_count_max];
  uint32 thread_count = 0;
  for (uint32 i = 0; i < threads_.size() && thread_count < thread_count_max; ++i)
  {
    Thread* tmp = threads_[i];
    if (tmp->getState() == ToBeDestroyed)
    {
      destroy_list[thread_count++] = tmp;
      threads_.erase(threads_.begin() + i); // Note: erase will not realloc!
      --i;
    }
  }
  unlockScheduling();
  if (thread_count > 0)
  {
    for (uint32 i = 0; i < thread_count; ++i)
    {
      delete destroy_list[i];
    }
    debug(SCHEDULER, "cleanupDeadThreads: done\n");
  }
}

void Scheduler::printThreadList()
{
  lockScheduling();
  debug(SCHEDULER, "Scheduler::printThreadList: %zd Threads in List\n", threads_.size());
  for (size_t c = 0; c < threads_.size(); ++c)
    debug(SCHEDULER, "Scheduler::printThreadList: threads_[%zd]: %p  %zd:%25s     [%s]\n", c, threads_[c],
          threads_[c]->getTID(), threads_[c]->getName(), Thread::threadStatePrintable[threads_[c]->state_]);
  unlockScheduling();
}

void Scheduler::lockScheduling() //not as severe as stopping Interrupts
{
  if (unlikely(ArchThreads::testSetLock(block_scheduling_, 1)))
    kpanict("FATAL ERROR: Scheduler::*: block_scheduling_ was set !! How the Hell did the program flow get here then ?\n");
}

void Scheduler::unlockScheduling()
{
  block_scheduling_ = 0;
}

bool Scheduler::isSchedulingEnabled()
{
  return this && block_scheduling_ == 0;
}

bool Scheduler::isCurrentlyCleaningUp()
{
  return currentThread == &cleanup_thread_;
}

size_t Scheduler::getTicks()
{
  return ticks_;
}

void Scheduler::incTicks()
{
  ++ticks_;
}

void Scheduler::printStackTraces()
{
  lockScheduling();
  debug(BACKTRACE, "printing the backtraces of <%zd> threads:\n", threads_.size());

  for (const auto& thread : threads_)
  {
    thread->printBacktrace();
    debug(BACKTRACE, "\n");
    debug(BACKTRACE, "\n");
  }

  unlockScheduling();
}

void Scheduler::printLockingInformation()
{
  lockScheduling();
  kprintfd("\n");
  debug(LOCK, "Scheduler::printLockingInformation:\n");

  for(Thread* t : threads_)
  {
    if(t->holding_lock_list_)
      Lock::printHoldingList(t);
  }
  for(Thread* t : threads_)
  {
    if(t->lock_waiting_on_)
      debug(LOCK, "Thread %s (%p) is waiting on lock: %s (%p).\n",
            t->getName(), t, t->lock_waiting_on_ ->getName(), t->lock_waiting_on_ );
  }
  debug(LOCK, "Scheduler::printLockingInformation finished\n");
  unlockScheduling();
}

uint64 Scheduler::getRdtscTicks()
{
  unsigned int lo, hi;
  asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
  return ((unsigned long long)hi << 32 ) | lo;
}

void Scheduler::setTime()
{
//  debug(SCHEDULER, "AVERAGES: %ld\n", rdtscAVGCount);
  rdtscNew = getRdtscTicks();
  rdtscDiff = rdtscNew - rdtscOld;

  if(!rdtscAVGDiff)
  {
    rdtscDiffUS = rdtscDiff / US_TICKS;
    rdtscAVGDiff = rdtscAVGDiff + rdtscDiffUS;
    rdtscAVGCount++;
    rdtsc_us = (rdtscAVGDiff / rdtscAVGCount);
  }
  else
  {
    rdtscDiffUS = rdtscDiff / US_TICKS;
    rdtscDEVBound = (rdtscAVGDiff * 5/rdtscAVGCount)/100;

  rdtscDEV =  rdtscAVGDiff/rdtscAVGCount > rdtscDiffUS ? (rdtscAVGDiff/rdtscAVGCount) - rdtscDiffUS : rdtscDiffUS - (rdtscAVGDiff/rdtscAVGCount);

  if(rdtscDEV < rdtscDEVBound)
  {
    rdtscAVGDiff = rdtscAVGDiff + rdtscDiffUS;
    rdtscAVGCount++;
  }

  if (!clockDone && rdtscAVGCount >= 10)
    clockDone = true;

  rdtsc_us = (rdtscAVGDiff / rdtscAVGCount);
  }
  rdtscOld = rdtscNew;

}

uint64 Scheduler::returnTime()
{
  auto uT = (UserThread*)currentThread;
  return (getRdtscTicks() - processTimeSTART)/rdtsc_us + uT->parent_process_->processTime;
}

void Scheduler::sleep_(uint64 seconds)
{
  uint64 currentTime = getRdtscTicks();
  currentThread->asleep = currentTime + (seconds * rdtsc_us * 1000000);
}

void Scheduler::usleep_(uint64 useconds)
{
  uint64 currentTime = getRdtscTicks();
  currentThread->asleep = currentTime + (useconds * rdtsc_us);
}



void Scheduler::checkAcessedBits()
{
  //TODO: locking lol
  if(currentThread && currentThread->loader_)
  {
    auto uT = (UserThread*)currentThread;
    uT->loader_->arch_memory_.ArchMemory::handleAccessedBits(uT->loader_->arch_memory_.page_map_level_4_);
  }
}

void Scheduler::shiftAge()
{
//  debug(LOL, "%ld\n", ticks_);
  for (auto ipt_entry: IPT::instance()->ipt_)
  {
    ipt_entry.second->age = ipt_entry.second->age >> 1;
//    if(ipt_entry.second->age != 0)
//    debug(PRAsAGING, "ueu  -> %p\n", (void*)ipt_entry.second->age);
  }
}

size_t Scheduler::getRandomSeed()
{
  size_t seed = Scheduler::instance()->getRdtscTicks();
  seed ^= seed << 13;
  seed ^= seed >> 7;
  seed ^= seed << 17;

  return seed;
}

