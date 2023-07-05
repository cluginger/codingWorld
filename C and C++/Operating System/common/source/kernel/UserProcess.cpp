#include "ProcessRegistry.h"
#include "UserProcess.h"
#include "kprintf.h"
#include "Console.h"
#include "Loader.h"
#include "VfsSyscall.h"
#include "File.h"
#include "ArchMemory.h"
#include "PageManager.h"
#include "ArchThreads.h"
#include "offsets.h"
#include "ArchInterrupts.h"
#include "UserThread.h"
#include "Scheduler.h"


//Constructor
UserProcess::UserProcess(ustl::string filename, FileSystemInfo *fs_info, uint32 terminal_number) :
filename_(filename),
fs_info_(fs_info),
terminal_number_(terminal_number),
heap_lock("heap_lock"),
exit_condition_lock("exit condition lock"),
join_condition_lock("join condition lock"),
wake_from_sleep_lock("wake_from_sleep_lock"),
TID_lock("TID_lock"),
threadMap_lock("threadMap_lock"),
deadThreadMap_lock("deadThreadMap_lock"),
retval_lock("retval_lock"),
number_existing_threads_lock("number_existing_threads_lock"),
exit_status_lock("exit_status_lock"),
retvalMap_lock("retvalMap_lock"),
exit_condition(&exit_condition_lock,"exit_condition"),
join_condition(&join_condition_lock, "join_condition"),
wake_from_sleep_cond(&wake_from_sleep_lock, "wake_from_sleep"),
fd_(VfsSyscall::open(filename, O_RDONLY)),
pid_(ProcessRegistry::instance()->getAndEnhancePIDCounter())

{

  ProcessRegistry::instance()->processStart(); //should also be called if you fork a process

  if (fd_ >= 0)
    process_loader = new Loader(fd_);

  if (!process_loader || !process_loader->loadExecutableAndInitProcess())
  {
    debug(USERPROCESS, "Error: loading %s failed!\n", filename_.c_str());
    return;
  }

  auto first_thread = new UserThread(filename_, fs_info_, this, (size_t)process_loader->getEntryFunction(),NULL, NULL);

  if (main_console->getTerminal(terminal_number_))
    first_thread->setTerminal(main_console->getTerminal(terminal_number_));

  heap_lock.acquire();
  heapStart = process_loader->getHeapStart();
  ProcessRegistry::instance()->aslrModeLock.acquire();
  if(ProcessRegistry::instance()->isAslrModeOn)
    heapStart +=  Scheduler::instance()->getRandomSeed() % PAGE_SIZE/8 * PAGE_SIZE;
//  heapStart -= Scheduler::instance()->getRandomSeed() % PAGE_SIZE/2; //dont ask why % PAGE_SIZE/2 -> its a design decision
  ProcessRegistry::instance()->aslrModeLock.release();
  debug(HEAP, "heapStart: %p\n", (void*)heapStart);
  if(heapStart != 0)
  {
    heapEndTotal = heapStart + HEAP_END;
    heapEndCurrent = heapStart;
//    debug(USERPROCESS, "This UserProcess HEAP %p to %p\n", heapStart, heapEndTotal);
  }
  heap_lock.release();
  debug(USERPROCESS, "added thread\n");

}

//CopyConstructor
UserProcess::UserProcess(const UserProcess &src, UserThread &thread):
heapStart(src.heapStart),
heapEndCurrent(src.heapEndCurrent),
heapEndTotal(src.heapEndTotal),
filename_(src.filename_),
fs_info_(src.fs_info_),
terminal_number_(src.terminal_number_),
heap_lock("heap_lock"),
exit_condition_lock("exit condition lock"),
join_condition_lock("join condition lock"),
wake_from_sleep_lock("wake_from_sleep_lock"),
TID_lock("TID_lock"),
threadMap_lock("threadMap_lock"),
deadThreadMap_lock("deadThreadMap_lock"),
retval_lock("retval_lock"),
number_existing_threads_lock("number_existing_threads_lock"),
exit_status_lock("exit_status_lock"),
retvalMap_lock("retvalMap_lock"),
exit_condition(&exit_condition_lock,"exit_condition"),
join_condition(&join_condition_lock, "join_condition"),
wake_from_sleep_cond(&wake_from_sleep_lock, "wake_from_sleep"),
fd_(VfsSyscall::open(filename_, O_RDONLY)),
pid_(ProcessRegistry::instance()->getAndEnhancePIDCounter())

{

  ProcessRegistry::instance()->processStart(); //should also be called if you fork a process

  if (fd_ >= 0)
    process_loader = new Loader(*src.process_loader, fd_); //we need a Loader copy here

  if (!process_loader || !process_loader->loadExecutableAndInitProcess())
  {
    debug(USERPROCESS, "Error: loading %s failed!\n", filename_.c_str());
    return;
  }
  //we need a UserThread copy here
  auto first_thread = new UserThread(thread, this);

  assert(first_thread != NULL && "First Thread creation didnt work\n");

  debug(USERPROCESS, "added thread\n");

}


void UserProcess::pushDeadThreadToDeadMap(UserThread *dead_thread_to_add)
{
  deadThreadMap_lock.acquire();
  deadThreadMap.insert(ustl::pair<size_t, UserThread*>(dead_thread_to_add->getTID(), dead_thread_to_add));
  deadThreadMap_lock.release();
  debug(USERPROCESS, "Added DEAD UserThread with TID %zu to DeadThreadMap of the Process\n", dead_thread_to_add->getTID());
}

UserProcess::~UserProcess()
{
  assert(Scheduler::instance()->isCurrentlyCleaningUp());
  delete process_loader;
  process_loader = 0;
  if (fd_ > 0)
    VfsSyscall::close(fd_);

  ProcessRegistry::instance()->deadProcessMap.insert(ustl::pair<size_t, size_t>(this->getPID(), this->exit_value));
  ProcessRegistry::instance()->waitpid_lock.acquire();
  ProcessRegistry::instance()->waitpid_cond.broadcast();
  ProcessRegistry::instance()->waitpid_lock.release();


  ProcessRegistry::instance()->processExit();
}

size_t UserProcess::getAndEnhanceTIDCounter()
{
  return ArchThreads::atomic_add(tid_counter, 1);
}

bool UserProcess::checkIfAllThreadsAreDead()
{
  bool check = false;

  deadThreadMap_lock.acquire();
  if(deadThreadMap.size() == threadMap.size())
  {
    check = true;
  }
  deadThreadMap_lock.release();
  
  return check;
}

void UserProcess::exit_(size_t exit_code)
{
  debug(USERPROCESS, "exit_ gets called by Thread with ID %zu\n", currentThread->getTID());

  //for testcases
//  Scheduler::instance()->yield();
//  Scheduler::instance()->yield();
//  Scheduler::instance()->yield();
//  Scheduler::instance()->yield();
//  Scheduler::instance()->yield();
  //we wanna terminate the current thread as the last one
  if(getExitStatus() == false)
  {
    debug(WAIT, "exit code: %zu\n", exit_code);
    this->exit_value = exit_code;
    debug(WAIT, "exit value: %zu\n", exit_value);
    threadMap_lock.acquire();
    setExitStatus(true);
    for (const auto &user_thread : threadMap)
    {
      if(user_thread.second->getTID() != currentThread->getTID() && user_thread.second->getThreadExitStatus() == false)
      {
        //TODONE lock state/type
        user_thread.second->cancel_state_lock.acquire();
        user_thread.second->cancel_state_ = PTHREAD_CANCEL_ENABLE;
        user_thread.second->cancel_state_lock.release();
        user_thread.second->cancel_type_lock.acquire();
        user_thread.second->cancel_type_ = PTHREAD_CANCEL_ASYNCHRONOUS;
        user_thread.second->cancel_type_lock.release();
        user_thread.second->cancel_flag_ = true;
      }
    }
    threadMap_lock.release();

    deadThreadMap_lock.acquire();
    deadThreadMap.clear();
    deadThreadMap_lock.release();

    // TODOAG: what if a thread is currently in pcreate? Will it just run forever?
    
    //threadMap.clear();
    //retvalMap.clear();
  }
  else
  {
    debug(USERPROCESS, "killAllThreads gets called from execv\n");
  }

  ((UserThread*)currentThread)->kill();

}

void UserProcess::setExitStatus(bool status)
{
  exit_status_lock.acquire();
  exit_status = status;
  exit_status_lock.release();
}

bool UserProcess::getExitStatus()
{
  exit_status_lock.acquire();
  auto current_exit_status = exit_status;
  exit_status_lock.release();
  return current_exit_status;
}

size_t UserProcess::getNumOfExistingThreads()
{
  
  number_existing_threads_lock.acquire();
  auto number_of_existing_threads = number_existing_threads;
  number_existing_threads_lock.release();
  return number_of_existing_threads;
}

void UserProcess::eraseThreadFromMap(size_t tid)
{
  //put threadmap lock out of it
  debug(USERPROCESS, "eraseThreadFromMap gets Called\n");
  if(threadMap.find(tid) != threadMap.end())
  {
    threadMap.erase(tid);
    debug(USERPROCESS, "Erased Thread with TID %zu from ThreadMap of the Process\n", tid);
  }
}

size_t UserProcess::getPID()
{
  return pid_;
}

void UserProcess::Run()
{
  debug(USERPROCESS, "Run: Fail-safe kernel panic - you probably have forgotten to set switch_to_userspace_ = 1\n");
  assert(false);
}

size_t UserProcess::fork()
{
  UserThread* user_thread = (UserThread*)currentThread;
  debug(FORK, "in function fork()\n");
  UserProcess* child = new UserProcess(*user_thread->parent_process_, *user_thread);
  size_t childPid = child->getPID();
  Scheduler::instance()->addNewThread(child->threadMap.back().second);
  return childPid;

}

size_t UserProcess::cancelThread(size_t tid)
{
  //TODONE locking
  debug(CANCEL, "Cancel called in UserProcess\n");
  threadMap_lock.acquire();
  auto thread_to_cancel = threadMap.find(tid);
  if(thread_to_cancel == threadMap.end())
  {
    debug(CANCEL, "Thread not found in map\n");
    threadMap_lock.release();
    return -1;
  }

  UserThread* thread = thread_to_cancel->second;
  debug(CANCEL, "thread to cancel %zu\n", thread->getTID());
  thread->cancel_state_lock.acquire();
  // TODOAG: not posix-behavior, cancellation request should still be sent
  if(thread->cancel_state_ != PTHREAD_CANCEL_ENABLE)
  {
    thread->cancel_state_lock.release();
    threadMap_lock.release();
    return 0;
  }
  else
    thread->cancel_state_lock.release();

  //TODONE locking state/type
  thread->cancel_type_lock.acquire();
  if(thread->cancel_type_ == 0 || thread->cancel_type_ == 1)
  {
    thread->cancel_flag_ = true;
    thread->cancel_type_lock.release();
    threadMap_lock.release();
    return 0;
  }
  else
    thread->cancel_type_lock.release();
  threadMap_lock.release();
  return -1;

}


void* UserProcess::getRetvalByID(size_t tid)
{
  ScopeLock lock (retvalMap_lock);
  if(retvalMap.find(tid) != retvalMap.end()){
    auto value = retvalMap.at(tid);
    retvalMap.erase(tid);
//    debug(JOIN, "retvalue from map: %zu\n", value);
    return value;
  }
  else
    return NULL;
}

/*
size_t UserProcess::execv(char *path, char *argv[])
{
  // TODOAG: path, argv are userspace-ptrs, if they are invalid you loose fd
  auto fd_execv = VfsSyscall::open(path, O_RDONLY);
  auto current_thread = (UserThread*)currentThread;
  debug(EXECV,"Thread with ID %zu calls execv\n", current_thread->getTID());
  Loader *execv_loader;
  auto old_fd = getFD();

  if(argv != NULL)
  {
    // TODOAG: argv[0] does not have to be path
    debug(EXECV, "The filename is %s\n", argv[0]);
  }

  if(fd_execv >= 0)
  {
    debug(EXECV, "We have now a new open file descriptor\n");
    execv_loader = new Loader(fd_execv);

  }
  else
  {
    VfsSyscall::close(fd_execv);
    debug(EXECV, "file could not open\n");
   

    return -1;
  }

  if (!execv_loader || !execv_loader->loadExecutableAndInitProcess())
  {
    debug(EXECV, "Error: loading for execv failed!\n");
   
    return -1;
  }

  if(getExitStatus())
  { 
    debug(USERPROCESS, "Exit is already called, so execv wont get executed\n");
    // TODOAG: we close the old fd? It's still in use by other threads...
    if(VfsSyscall::close(old_fd) != 0)
    {
      debug(EXECV, "old filedescriptor didnt close\n");
    }
    delete execv_loader;
    execv_loader = 0;

    return -1;
  }

  else
  {
    setExitStatus(true);

    threadMap_lock.acquire();

    for (auto &user_thread : threadMap)
    {
      if(currentThread->getTID() != user_thread.second->getTID() && user_thread.second->getThreadExitStatus() == false)
      {
        user_thread.second->cancel_state_lock.acquire();
        user_thread.second->cancel_state_ = PTHREAD_CANCEL_ENABLE;
        user_thread.second->cancel_state_lock.release();
        user_thread.second->cancel_type_lock.acquire();
        user_thread.second->cancel_type_ = PTHREAD_CANCEL_ASYNCHRONOUS;
        user_thread.second->cancel_type_lock.release();
        user_thread.second->cancel_flag_ = true;
      }
    }

    threadMap_lock.release();
    
    if(getNumOfExistingThreads() > 1)
    {
      debug(USERPROCESS, "There are now %zu Threads left so we have to wait\n", getNumOfExistingThreads());
      exit_condition_lock.acquire();
      exit_condition.wait();
      exit_condition_lock.release();
    }
    

    while(getNumOfExistingThreads() > 1)
    {
      current_thread->cancel_type_lock.acquire();
      // TODOAG: we don't have to cancel the current thread just because we can cancel it
      // it should only be cancelled if someone requests it to be cancelled
      if (current_thread->cancel_type_ == PTHREAD_CANCEL_ASYNCHRONOUS)
      {
        current_thread->cancel_type_lock.release();
        if(VfsSyscall::close(old_fd) != 0)
        {
          debug(EXECV, "old filedescriptor didnt close\n");
        }
        delete execv_loader;
        execv_loader = 0;
        return -1;
      }
      else
        current_thread->cancel_type_lock.release();

      Scheduler::instance()->yield();
    }

  }
  
    
  assert(getNumOfExistingThreads() == 1 && "There are more than 1 active Threads left\n");

  setExitStatus(false);
  setFD(fd_execv);
  currentThread->loader_ = execv_loader;
  old_loader = process_loader;
  ((UserThread*)currentThread)->execvOverwriteUserThread(execv_loader);
  
  ArchThreads::createUserRegisters(currentThread->user_registers_, execv_loader->getEntryFunction(),
                                   (void*) (USER_BREAK - sizeof(pointer) - (currentThread->getTID() * NUM_PAGES * PAGE_SIZE)),
                                   currentThread->getKernelStackStartPointer());


  ArchThreads::setAddressSpace(current_thread, execv_loader->arch_memory_);


  if(VfsSyscall::close(old_fd) != 0)
  {
    debug(EXECV, "old filedescriptor didnt close\n");
  }  

  delete old_loader;
  old_loader = 0;

  process_loader = execv_loader;

  return 0;
}
*/


size_t UserProcess::joinThread(size_t tid, void **ret_val)
{
    debug(JOIN, "Join called in UserProcess\n");
    debug(JOIN, "ret_val: %p\n", ret_val);
    threadMap_lock.acquire();
    auto thread_to_join_ = threadMap.find(tid);
    if(thread_to_join_ == threadMap.end())
    {
      debug(JOIN,"thread not found\n");
      threadMap_lock.release();
      return -1;
    }

    auto thread = thread_to_join_->second;
    auto value = getRetvalByID(tid);
    auto caller = (UserThread*)currentThread;

    //JOINING ITSELF
    debug(JOIN, "there is no problem with join\n");
    if((size_t)caller->getTID() == tid)
    {
      debug(JOIN, "Thread trying to join itself\n");
      threadMap_lock.release();
      return -1;
    }

    if(value == NULL && thread == NULL)
    {
      debug(JOIN, "Thread died, already joined\n");
      threadMap_lock.release();
      return 0;
      //TODO: look over it whether -1 or 0 - cos its fine if he ded
    }

    //DETACHED
    //TODONE state locking
    thread->join_state_lock.acquire();
    if(thread->join_state_ == PTHREAD_DETACHED)
    {
      thread->join_state_lock.release();
      debug(JOIN, "Thread DETACHED, not joinable\n");
      threadMap_lock.release();
      return -1;
    }
    else
      thread->join_state_lock.release();

    if(value != NULL)
    {
      if(ret_val != NULL)
      {
        debug(JOIN, "Thread is already dead\n");
        *ret_val = value;
        threadMap_lock.release();
        return 0;
      }
    }
    thread->joining_thread_lock.acquire();

//    debug(JOIN, " thread to join: %zu\n", thread);
//    debug(JOIN, " caller joining thread: %zu\n", caller->joining_thread);
//    debug(JOIN, " caller: %zu\n", caller);
//    debug(JOIN, " thread to join joining thread: %zu\n", thread->joining_thread);
    
    if(thread->joining_thread != NULL && caller->joining_thread == thread && caller == thread->joining_thread)
    {
      debug(JOIN, "deadlock\n");
      thread->joining_thread_lock.release();
      threadMap_lock.release();
      return -1;
    }
    if(thread->joining_thread != NULL)
    {
      debug(JOIN, "already waiting on thread to join\n");
      thread->joining_thread_lock.release();
      threadMap_lock.release();
      return -1;
    }

//    debug(JOIN, "join survived");
    thread->joining_thread = currentThread;
    caller->joining_thread = thread;
    debug(JOIN, " thread to join joining thread: %p\n", thread->joining_thread);
    thread->joining_thread_lock.release();

//    threadMap_lock.release();//gone wenn mit cv

//    assert(currentThread->holding_lock_list_ == 0 && "still holding locks");
    debug(JOIN, "setting to sleep\n");
    //TODONE condition variable
//    Scheduler::instance()->sleep();
//     STATT DEM SLEEP
  threadMap_lock.release();
    wake_from_sleep_lock.acquire();
//     while(threadMap.find(tid) != threadMap.end())
//     {
       wake_from_sleep_cond.wait();
//     }
    wake_from_sleep_lock.release();


    debug(JOIN, "woke up");
    auto retvalue = getRetvalByID(tid);
    debug(JOIN, "retvalue: %zu\n", retvalue);
    debug(JOIN, "ret_value: %zu\n", ret_val);
    if(ret_val != NULL)
    {
      debug(JOIN, "join successfully with retval %ld\n", (ssize_t) retvalue);
      *ret_val = retvalue;
      debug(JOIN, "ret_value2:");
    }
//    thread->joining_thread = NULL;
//    caller->joining_thread = NULL;
    return 0;


}


size_t UserProcess::detachThread(size_t tid)
{
    debug(DETACH, "Detach called in UserProcess\n");
    threadMap_lock.acquire();
    auto target = threadMap.find(tid);
    if(target == threadMap.end())
    {
      debug(JOIN, "detach -> thread not found\n");
      threadMap_lock.release();
      return 0;
    }
    auto thread = target->second;
    //TODONE locking
    thread->join_state_lock.acquire();
    if(thread->join_state_ == PTHREAD_DETACHED)
    {
      thread->join_state_lock.release();
      debug(DETACH, "Thread already detached\n");
      threadMap_lock.release();
      return 0;
    }
    else
    {
      //TODONE lock
      thread->join_state_ = PTHREAD_DETACHED;
      thread->join_state_lock.release();
      debug(DETACH, "Thread is set to detached\n");
      threadMap_lock.release();
      return 0;
    }
}

int32_t UserProcess::getFD()
{
  return fd_;
}

void UserProcess::setFD(int32_t fd)
{
  fd_ = fd;
}