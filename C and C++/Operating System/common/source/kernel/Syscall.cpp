#include "offsets.h"
#include "Syscall.h"
#include "syscall-definitions.h"
#include "Terminal.h"
#include "debug_bochs.h"
#include "VfsSyscall.h"
#include "UserProcess.h"
#include "ProcessRegistry.h"
#include "File.h"
#include "UserThread.h"
#include "UserProcess.h"
#include "Scheduler.h"
#include "ArchThreads.h"
#include "ArchInterrupts.h"
#include "ArchMemory.h"
#include "Loader.h"
#include "SwapManager.h"
#include "PRA.h"


#define UTHREAD ((UserThread*)currentThread)
#define PROCESS UTHREAD->parent_process_

size_t Syscall::syscallException(size_t syscall_number, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5)
{
  size_t return_value = 0;
  if ((syscall_number != sc_sched_yield) && (syscall_number != sc_outline)) // no debug print because these might occur very often
  {
    debug(SYSCALL, "Syscall %zd called with arguments %zd(=%zx) %zd(=%zx) %zd(=%zx) %zd(=%zx) %zd(=%zx)\n",
          syscall_number, arg1, arg1, arg2, arg2, arg3, arg3, arg4, arg4, arg5, arg5);
  }

  if(UTHREAD->cancel_state_ == PTHREAD_CANCEL_ENABLE && UTHREAD->cancel_flag_)
  {
      debug(CANCEL, "DEFERRED CANCEL\n");
      pthread_exit(PTHREAD_CANCELED);
  }

  switch (syscall_number)
  {
    case sc_sched_yield:
      Scheduler::instance()->yield();
      break;
    case sc_createprocess:
      return_value = createprocess(arg1, arg2);
      break;
    case sc_exit:
      exit(arg1);
      break;
    case sc_write:
      return_value = write(arg1, arg2, arg3);
      break;
    case sc_read:
      return_value = read(arg1, arg2, arg3);
      break;
    case sc_open:
      return_value = open(arg1, arg2);
      break;
    case sc_close:
      return_value = close(arg1);
      break;
    case sc_outline:
      outline(arg1, arg2);
      break;
    case sc_trace:
      trace();
      break;
    case sc_pseudols:
      pseudols((const char*) arg1, (char*) arg2, arg3);
      break;
    case sc_pthreadcreate:
      return_value = pthread_create(arg1, arg2, arg3, arg4, arg5);
      break;
    case sc_pthreadexit:
      pthread_exit(arg1);
      break;
    case sc_clock:
      return_value = Scheduler::instance()->returnTime();
      break;
    case sc_sleep:
      Scheduler::instance()->sleep_(arg1);
      Scheduler::instance()->yield();
      break;
    case sc_usleep:
      Scheduler::instance()->usleep_(arg1);
      Scheduler::instance()->yield();
      break;
    case sc_fork:
      return_value = fork();
      break;
    case sc_pthread_setcancelstate:
      return_value = pthread_setcancelstate(arg1, (int*)arg2);
      break;
    case sc_pthread_setcanceltype:
      return_value = pthread_setcanceltype(arg1, (int*)arg2);
      break;
    case sc_pthreadcancel:
      return_value = pthread_cancel(arg1);
      break;
   /* case sc_execv:
      return_value = execv(arg1, arg2);
      break;
    */
    case sc_pthreadjoin:
      return_value = pthread_join(arg1, (void**)arg2);
      break;
    case sc_pthreaddetach:
      return_value = pthread_detach(arg1);
      break;
    case sc_waitpid:
      return_value = waitpid(arg1, (size_t*)arg2, arg3);
      break;
    case sc_random_pra:
      testRandom_PRA();
      break;

    case sc_swapout:
      return_value = SwapManager::instance()->handleSwapOut();
      break;
    case sc_brk:
      return_value = brk((void*)arg1,false);
      break;
    case sc_sbrk:
      return_value = (size_t) sbrk((intptr_t)arg1);
       break;
    case sc_returnCurrentHeap:
      PROCESS->heap_lock.acquire();
      return_value = PROCESS->heapEndCurrent;
      PROCESS->heap_lock.release();
      break;
    case sc_mean_pra:
      return_value = getMeanOfPRASample();
      break;
    case sc_swap_out_in_test:
      swapOnePageOutAndInTest();
      break;

    case sc_swapped_out_cow_pages:
      return_value = SwapManager::instance()->getCounterSwapOutCow();
      break;

    case sc_swapped_in_cow_pages:
      return_value = SwapManager::instance()->getCounterSwapInCow();
      break;

    /*case sc_stddeviation_pra:
      return_value = getSTDDeviationOfPRASample();
      break;*/
    default:
      return_value = -1;
      kprintf("Syscall::syscallException: Unimplemented Syscall Number %zd\n", syscall_number);
  }

    if(UTHREAD->cancel_type_ == PTHREAD_CANCEL_DEFERRED && UTHREAD->cancel_state_ == PTHREAD_CANCEL_ENABLE && UTHREAD->cancel_flag_)
    {
        debug(CANCEL, "DEFERRED CANCEL\n");
        pthread_exit(PTHREAD_CANCELED);
    }

    return return_value;
}

void Syscall::pseudols(const char *pathname, char *buffer, size_t size)
{
  if(buffer && ((size_t)buffer >= USER_BREAK || (size_t)buffer + size > USER_BREAK))
    return;
  if((size_t)pathname >= USER_BREAK)
    return;
  VfsSyscall::readdir(pathname, buffer, size);
}

void Syscall::exit(size_t exit_code)
{
  debug(SYSCALL, "Syscall::EXIT: called, exit_code: %zd\n", exit_code);
  PROCESS->exit_(exit_code);
}

size_t Syscall::write(size_t fd, pointer buffer, size_t size)
{
  //WARNING: this might fail if Kernel PageFaults are not handled
  if ((buffer >= USER_BREAK) || (buffer + size > USER_BREAK))
  {
    return -1U;
  }

  size_t num_written = 0;

  if (fd == fd_stdout) //stdout
  {
    debug(SYSCALL, "Syscall::write: %.*s\n", (int)size, (char*) buffer);
    kprintf("%.*s", (int)size, (char*) buffer);
    num_written = size;
  }
  else
  {
    num_written = VfsSyscall::write(fd, (char*) buffer, size);
  }
  return num_written;
}

size_t Syscall::read(size_t fd, pointer buffer, size_t count)
{
  if ((buffer >= USER_BREAK) || (buffer + count > USER_BREAK))
  {
    return -1U;
  }

  size_t num_read = 0;

  if (fd == fd_stdin)
  {
    //this doesn't! terminate a string with \0, gotta do that yourself
    num_read = currentThread->getTerminal()->readLine((char*) buffer, count);
    debug(SYSCALL, "Syscall::read: %.*s\n", (int)num_read, (char*) buffer);
  }
  else
  {
    num_read = VfsSyscall::read(fd, (char*) buffer, count);
  }
  return num_read;
}

size_t Syscall::close(size_t fd)
{
  return VfsSyscall::close(fd);
}

size_t Syscall::open(size_t path, size_t flags)
{
  if (path >= USER_BREAK)
  {
    return -1U;
  }
  return VfsSyscall::open((char*) path, flags);
}

void Syscall::outline(size_t port, pointer text)
{
  //WARNING: this might fail if Kernel PageFaults are not handled
  if (text >= USER_BREAK)
  {
    return;
  }
  if (port == 0xe9) // debug port
  {
    writeLine2Bochs((const char*) text);
  }
}

size_t Syscall::createprocess(size_t path, size_t sleep)
{
  // THIS METHOD IS FOR TESTING PURPOSES ONLY AND NOT MULTITHREADING SAFE!
  // AVOID USING IT AS SOON AS YOU HAVE AN ALTERNATIVE!

  // parameter check begin
  if (path >= USER_BREAK)
  {
    return -1U;
  }

  debug(SYSCALL, "Syscall::createprocess: path:%s sleep:%zd\n", (char*) path, sleep);
  ssize_t fd = VfsSyscall::open((const char*) path, O_RDONLY);
  if (fd == -1)
  {
    return -1U;
  }
  VfsSyscall::close(fd);
  // parameter check end

  size_t process_count = ProcessRegistry::instance()->processCount();
  ProcessRegistry::instance()->createProcess((const char*) path);
  if (sleep)
  {
    while (ProcessRegistry::instance()->processCount() > process_count) // please note that this will fail ;)
    {
      Scheduler::instance()->yield();
    }
  }
  return 0;
}

void Syscall::trace()
{
  currentThread->printBacktrace();
}

size_t Syscall::pthread_create(size_t thread, size_t attr, size_t start_routine, size_t arg, size_t wrapper)
{
  //check all Parameters
  if(thread == NULL || thread >= USER_BREAK || wrapper == NULL || wrapper >= USER_BREAK)
  {
    debug(SYSCALL, "invalid parameters\n");
    return -1;
  }

  
  debug(USERTHREAD, "Attribute:%ld\n", attr);

  //PROCESS->threadMap_lock.acquire();

  if(PROCESS->getExitStatus() == false)
  {
    auto new_thread = new UserThread(currentThread->getName(), currentThread->getWorkingDirInfo(), PROCESS,
                                   wrapper, start_routine, arg);
    *(size_t*)thread = new_thread->getTID();
    debug(SYSCALL, "New Thread with ID %zu is created\n", new_thread->getTID());
  }
  else
  {
    debug(SYSCALL, "EXIT is called so no other Thread should be created\n");  
  }
  
  //PROCESS->threadMap_lock.release();

  return 0;
}

void Syscall::pthread_exit(size_t retval)
{
  UserThread* uT = UTHREAD;
  UserProcess* uP = PROCESS;
  assert(currentThread->holding_lock_list_ == NULL);

  debug(SYSCALL, "pthread_exit is called by Thread with ID %zu\n", currentThread->getTID());
  uT->setRetVal((void*)retval);

  //TODO locking retvalmap und join state
  uT->join_state_lock.acquire();
  if(uT->join_state_ != PTHREAD_DETACHED)
  {
    uT->join_state_lock.release();
    uP->retvalMap_lock.acquire();
    uP->retvalMap.insert({uT->getTID(), (void*)retval});
    uP->retvalMap_lock.release();
  }
  else
  {
    uT->join_state_lock.release();
  }
    uT->setThreadExitStatus();
  uP->threadMap_lock.acquire();
  uP->eraseThreadFromMap(uT->getTID());
  uP->threadMap_lock.release();
    uT->joining_thread_lock.acquire();
    if (uT->joining_thread != NULL)
    {
//      debug(JOIN, "waking up");
      uP->wake_from_sleep_lock.acquire();
      uP->wake_from_sleep_cond.signal();
      uP->wake_from_sleep_lock.release();
    }
//    debug(JOIN, "calling kill");
    uT->joining_thread_lock.release();
    IPT::instance()->ipt_lock.acquire();
    uT->loader_->arch_memory_.arch_memory_lock.acquire();
    for(int i = 0; i < NUM_PAGES -1; i++)
    {
      size_t virtaddress = (uT->virt_page_for_stack + i) * PAGE_SIZE;
      debug(ARCHMEM, " umapping %zx\n",virtaddress);
      if(uT->loader_->arch_memory_.checkAddressValid((virtaddress)))
      {
        uP->process_loader->arch_memory_.unmapPage(uT->virt_page_for_stack + i);
      }
    }
    uT->loader_->arch_memory_.arch_memory_lock.release();
    IPT::instance()->ipt_lock.release();
    uT->kill();
}

size_t Syscall::fork()
{
  //size_t child_pid = -1;
  size_t child_pid = PROCESS->fork();
  return child_pid;
}
size_t Syscall::pthread_setcanceltype(int type,int *old_type)
{
    if(type != PTHREAD_CANCEL_ASYNCHRONOUS && type!=PTHREAD_CANCEL_DEFERRED)
    {
        debug(CANCEL, "Invalid type flag \n.");
        return -1;
    }

    if((size_t)old_type >=  USER_BREAK)
    {
        return -1;
    }

  UTHREAD->cancel_type_lock.acquire();
    if(old_type != NULL)
    {

        // (DONE)TODOAG: incorrect type, should be int
        *old_type = (int)UTHREAD->cancel_type_;
    }
    UTHREAD->cancel_type_= (size_t)type;
    UTHREAD->cancel_type_lock.release();
    return 0;
}

size_t Syscall::pthread_setcancelstate(int state, int *old_state)
{
    if(state != PTHREAD_CANCEL_ENABLE && state!=PTHREAD_CANCEL_DISABLE )
    {
        debug(CANCEL, "Invalid state flag \n.");
        return -1;
    }

    if((size_t)old_state >=  USER_BREAK)
    {
        return -1;
    }
    UTHREAD->cancel_state_lock.acquire();
    if(old_state != NULL)
    {
        // (DONE)TODOAG: incorrect type, should be int
        *old_state= UTHREAD->cancel_state_;
    }
    UTHREAD->cancel_state_ = (size_t)state;
    UTHREAD->cancel_state_lock.release();
    return 0;
}

size_t Syscall::pthread_cancel(size_t thread)
{
  return PROCESS->cancelThread(thread);
}

/*
size_t Syscall::execv(size_t path, size_t argv)
{
  if(PROCESS->getExitStatus())
  {
    debug(SYSCALL, "The process is currently terminating\n");
    return -1;
  }
  // TODOAG: what if the process exits now? You release the lock for exit status...
 

  if((char*)path == NULL || path >= USER_BREAK)
  {
    debug(SYSCALL, "Syscall::execv      ERROR:path >= USER_BREAK || path == NULL\n");
    return -1;
  }

  if((argv >= USER_BREAK))
  {
    debug(SYSCALL, "Syscall::execv      ERROR:arg >= USER_BREAK || path == NULL\n");
  
    return -1;
  }


  //check path length         laut posix max 256 Elemente
  auto path_check = (char*)path;
  size_t path_index = 0;
  bool allowed_path_range = false;

  while(path_index < 256)
  {
    if(path_check[path_index] == '\0')
    {
      allowed_path_range = true;
      break;
    }
    path_index++;
  }
  debug(EXECV,"The path has %zu letters\n", ++path_index);

  if(allowed_path_range == false)
  {
    debug(SYSCALL, "Syscall::execv      ERROR:Too Long Path\n");
    return -1;
  }

  if(argv != NULL)
  {
    //check argv
    auto argv_check = (char**)argv;
    size_t num_argv = 0;

    while(*argv_check)
    {
      num_argv++;
      argv_check++;
    }
    debug(EXECV,"There are %zu elements in argv\n", num_argv);

    // TODOAG: you have increased the argv-check ptr...
    if(argv_check[num_argv] != (char*)NULL)
    {
      debug(SYSCALL, "Syscall::execv      ERROR:Last Element of argv is not NULL\n");
      return -1;
    }
  }
  

  return PROCESS->execv((char*)path, (char**)argv);
}
*/
size_t Syscall::pthread_join(size_t thread, void**retval)
{
    debug(JOIN, "retval in Syscall join: %p\n", retval);
    return PROCESS->joinThread(thread, retval);
}

size_t Syscall::pthread_detach(size_t thread)
{
  PROCESS->threadMap_lock.acquire();
  auto thread_to_join_ =  PROCESS->threadMap.find(thread);
  if(thread_to_join_ == PROCESS->threadMap.end())
  {
    debug(JOIN,"thread not found\n");
    
    PROCESS->threadMap_lock.release();
    return -1;
  }
  else
    PROCESS->threadMap_lock.release();

  return PROCESS->detachThread(thread);
}

size_t Syscall::waitpid(size_t pid, size_t* status, size_t options)
{
  ProcessRegistry::instance()->processMap_lock.acquire();
  if(ProcessRegistry::instance()->processMap.find(pid) != ProcessRegistry::instance()->processMap.end())
  {
    ProcessRegistry::instance()->processMap_lock.release();
    return -1;
  }
  ProcessRegistry::instance()->processMap_lock.release();
  if(PROCESS->getPID() == pid)
  {
    return -1;
  }
  if((size_t)status > USER_BREAK)
  {
    return -1;
  }
  if(options)
  {}
  ProcessRegistry::instance()->deadProcessMap_lock.acquire();
  if(ProcessRegistry::instance()->deadProcessMap.find(pid) != ProcessRegistry::instance()->deadProcessMap.end())
  {
    ProcessRegistry::instance()->deadProcessMap_lock.release();
    return -1;
  }
  ProcessRegistry::instance()->deadProcessMap_lock.release();

  ProcessRegistry::instance()->waitpid_lock.acquire();
  ProcessRegistry::instance()->waitpid_cond.wait();
  ProcessRegistry::instance()->waitpid_lock.release();

  ProcessRegistry::instance()->deadProcessMap_lock.acquire();
  auto stat = ProcessRegistry::instance()->deadProcessMap.find(pid);
  ProcessRegistry::instance()->deadProcessMap_lock.release();

  if(status != NULL)
  {
    debug(WAIT, "status1: %zu", status);
    debug(WAIT, "stat_second: %zu", stat->second);
    *status = stat->second;
    debug(WAIT, "status2: %zu", status);
  }
  return pid;
}


void Syscall::swapOnePageOutAndInTest()
{
  return SwapManager::instance()->swapOutAndSwapInRandomPage();
}

void Syscall::testRandom_PRA()
{
  PRA::instance()->printRandomSample();
}

size_t Syscall::getMeanOfPRASample()
{
  return PRA::instance()->getMeanOfPRA();
}

/*double Syscall::getSTDDeviationOfPRASample()
{
  return PRA::instance()->getSTDDeviationOfPRA();
}
*/

void* Syscall::sbrk(intptr_t inc)
{
  PROCESS->heap_lock.acquire();
//  void* start = (void*) PROCESS->heapStart;
  void* curr = (void*) PROCESS->heapEndCurrent;
  void* ret = (void*) PROCESS->heapEndCurrent;
  size_t potential_heap = PROCESS->heapEndCurrent;
  int is_invalid = 0;
  if(inc == 0)
  {
    debug(HEAP, "SBRK inc 0\n");
    debug(HEAP, "SBRK curr %p - %p\n", curr,(void*) PROCESS->heapEndCurrent );
    is_invalid = brk(curr, true);
    PROCESS->heap_lock.release();
    return is_invalid == 0 ? ret : (void*) -1;
  }
  else if(inc > 0)
  {
    debug(HEAP, "SBRK inc > 0 (%ld)\n", inc);
    potential_heap += inc;
    is_invalid = brk((void*)potential_heap, true);
    if(is_invalid)
    {
      PROCESS->heap_lock.release();
      debug(HEAP, "SBRK the wished size exceeds the total possible heap size \n");
      return (void*) -1;
    }
    PROCESS->heapEndCurrent = potential_heap;
  }
  else
  {
    debug(HEAP, "BRK inc < 0 (%ld)\n",inc);
    potential_heap += inc;
    debug(HEAP, "SBRK inc < 0 (%p)\n",(void*)potential_heap);
    is_invalid = brk((void*)potential_heap, true);
    if(is_invalid)
    {
      PROCESS->heap_lock.release();
      debug(HEAP, "SBRK the wished size subceeds the total possible heap size \n");
      return (void*) -1;
    }
  }
  PROCESS->heap_lock.release();
  return ret;
}

int Syscall::brk(void *ds_end, bool called_by_sbrk)
{
  if(!called_by_sbrk)
    PROCESS->heap_lock.acquire();
  void* start = (void*) PROCESS->heapStart;
  void* end = (void*) PROCESS->heapEndTotal;
  void* curr = (void*) PROCESS->heapEndCurrent;
  debug(HEAP, "BRK ds_end %p, start -> %p, end -> %p\n", ds_end, start, end);
  if(start <= ds_end && ds_end <= end)
  {
    PROCESS->heapEndCurrent = (size_t)ds_end;
    debug(HEAP, "BRK new end of heap: %p\n", PROCESS->heapEndCurrent);
    if(ds_end <= end)
    {
      size_t vpn = (size_t) curr/PAGE_SIZE;
      size_t vpn_decremented = (size_t)ds_end/PAGE_SIZE;
      while(vpn_decremented < vpn)
      {
        UTHREAD->loader_->arch_memory_.handleBrk(vpn);
//        ArchMemoryMapping m = UTHREAD->loader_->arch_memory_.resolveMapping(vpn);
//        //maybe here assertions?
//        if(m.page_ppn != NULL)
//          UTHREAD->loader_->arch_memory_.unmapPage(vpn);
        vpn -= 1;
      }
    }
    if(!called_by_sbrk)
      PROCESS->heap_lock.release();
    return 0;
  }
  debug(HEAP, "BRK  new data segment not in bound\n");
  if(!called_by_sbrk)
    PROCESS->heap_lock.release();
  return -1;
}