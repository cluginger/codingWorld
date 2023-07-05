#include "UserThread.h"
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
#include "Scheduler.h"
#include "Thread.h"
#include "../../../userspace/libc/include/defs.h"
#include "IPT.h"


//Constructor
UserThread::UserThread(ustl::string filename, FileSystemInfo *fs_info, UserProcess* parent_process , size_t entry_function, size_t start_routine, size_t arg):
Thread(fs_info, filename, Thread::USER_THREAD),
parent_process_(parent_process),
cancel_state_(PTHREAD_CANCEL_ENABLE),
cancel_type_(PTHREAD_CANCEL_DEFERRED),
join_state_(PTHREAD_JOINABLE),
thread_exit_status_lock("thread_exit_status_lock"),
joining_thread_lock("joining_thread_lock"),
cancel_type_lock("cancel_type_lock"),
cancel_state_lock("cancel_state_lock"),
join_state_lock("join_state_lock")
{
  loader_ = parent_process_->process_loader;

  phys_page_for_stack = PageManager::instance()->allocPPN();
  start_of_stack = USER_BREAK - (getTID() * NUM_PAGES * PAGE_SIZE);
  virt_page_for_stack = (USER_BREAK / PAGE_SIZE - 1) - (getTID() * NUM_PAGES);

//  ProcessRegistry::instance()->aslrModeLock.acquire();
//  if(ProcessRegistry::instance()->isAslrModeOn)
//  {
//    start_of_stack = USER_BREAK - (Scheduler::instance()->getRandomSeed() % PAGE_SIZE/4) * PAGE_SIZE;
//  }
//  ProcessRegistry::instance()->aslrModeLock.release();
//    debug(LOL, "STACKSTART: %p\n", (void*)start_of_stack);
//  end_of_stack = start_of_stack - (NUM_PAGES * PAGE_SIZE) + PAGE_SIZE;
  //make space for tid/mutex stuff on stack
//  size_t userSpacePlaceStart = loader_->arch_memory_.getIdentAddressOfPPN(phys_page_for_stack) + PAGE_SIZE;
//  size_t userSpaceTID  = userSpacePlaceStart - sizeof(size_t);
//  size_t userSpaceMutex  = userSpaceTID - sizeof(size_t);
//  size_t userSpaceCondVar  = userSpaceMutex - sizeof(size_t);
//  size_t userSpaceLocked  = userSpaceCondVar - sizeof(size_t);
//  _tid = (size_t*) userSpaceTID;
//  *_tid = getTID();
//  _mutex = (size_t*) userSpaceMutex;
//  *_mutex = 0;
//  _condvar = (size_t*) userSpaceCondVar;
//  *_condvar = 0;
//  _locked = (size_t*) userSpaceLocked;
  // TODOAG: why '5'?
//  *_locked = 0;

  debug(USED_PPNs, "Prealloc in UserThread\n");
  PreAllocate pages(3);
  IPT::instance()->ipt_lock.acquire();
  loader_->arch_memory_.arch_memory_lock.acquire();
  debug(USERTHREAD, "MAPI mapping %zx\n",virt_page_for_stack);
  bool vpn_mapped = loader_->arch_memory_.mapPage(virt_page_for_stack, phys_page_for_stack, 1, &pages);
  if (!vpn_mapped)
  {
    debug(LOADER, "Thread %ld:  The page has been mapped by someone else.\n", getTID());
    PageManager::instance()->freePPN(phys_page_for_stack);
    //pages.freeAllPreAllocatedPages();
  }
  //tell the stack we need the space
//  start_of_stack -= (sizeof(size_t) * 5);
  loader_->arch_memory_.arch_memory_lock.release();
  IPT::instance()->ipt_lock.release();

  ArchThreads::createUserRegisters(user_registers_, (void*)entry_function,
                                     (void*) start_of_stack,
                                     getKernelStackStartPointer());
  //adjusted and removed the - 1
  assert(start_of_stack / PAGE_SIZE  == user_registers_->rsp / PAGE_SIZE && "stack not correctly mapped\n");

  if(start_routine)
    user_registers_->rdi = start_routine;
  if(arg)
    user_registers_->rsi = arg;

  ArchThreads::setAddressSpace(this, loader_->arch_memory_);


  debug(USERPROCESS, "ctor: Done loading %s\n", filename.c_str());

  parent_process_->number_existing_threads_lock.acquire();
  parent_process_->number_existing_threads++;
  parent_process_->number_existing_threads_lock.release();

  parent_process->threadMap_lock.acquire();
  parent_process_->threadMap.insert(ustl::pair<size_t, UserThread*>(getTID(), this));
  parent_process->threadMap_lock.release();
  debug(USERTHREAD, "Added UserThread with TID %zu to ThreadMap of the Process\n",getTID());

  if (parent_process->getExitStatus() == false)
  {
    Scheduler::instance()->addNewThread(this);
    switch_to_userspace_ = 1;
  }
  else
  {
    //i think i have to unmap the page... i do it later
  }
  
}

//CopyConstructor
UserThread::UserThread(UserThread &src, UserProcess *parent_process):
Thread(parent_process->fs_info_, parent_process->filename_, Thread::USER_THREAD),
parent_process_(parent_process),
cancel_state_(PTHREAD_CANCEL_ENABLE),
cancel_type_(PTHREAD_CANCEL_DEFERRED),
join_state_(PTHREAD_JOINABLE),
thread_exit_status_lock("thread_exit_status_lock"),
joining_thread_lock("joining_thread_lock"),
cancel_type_lock("cancel_type_lock"),
cancel_state_lock("cancel_state_lock"),
join_state_lock("join_state_lock")
{
  //(DONE) TODOAG: wouldn't this always use the highest tid of the parent?
  // instead, we should copy the tid of the src-thread
  
  setTID(src.getTID());
  loader_ = parent_process_->process_loader;

  user_registers_ = new ArchThreadRegisters();

  memcpy(user_registers_, src.user_registers_, sizeof(ArchThreadRegisters));
  user_registers_->rsp0 = (size_t) getKernelStackStartPointer();
  user_registers_->rax = 0;

  ArchThreads::setAddressSpace(this, loader_->arch_memory_);

 



  parent_process_->number_existing_threads_lock.acquire();
  parent_process_->number_existing_threads++;
  parent_process_->number_existing_threads_lock.release();


  parent_process_->threadMap_lock.acquire();
  parent_process_->threadMap.insert(ustl::pair<size_t, UserThread*>(getTID(), this));
  parent_process_->threadMap_lock.release();
  debug(USERTHREAD, "Added UserThread with TID %zu to ThreadMap of the Process\n", getTID());
  
//  Scheduler::instance()->addNewThread(this);
  switch_to_userspace_ = 1;
}

UserThread::~UserThread()
{
  debug(USERTHREAD, "QWER UserThread Destructor gets called by Thread with ID %zu\n", getTID());


  debug(USERTHREAD, "number existing freds: %ld\n", parent_process_->number_existing_threads);

  if(ArchThreads::atomic_add(parent_process_->number_existing_threads, -1) == 1)
  {
    debug(USERTHREAD, "Deleting now the parent_process\n");
    delete parent_process_;
    return;
  }

}

/*
void UserThread::execvOverwriteUserThread(Loader* execv_loader)
{
  loader_ = execv_loader;
  parent_process_->process_loader = execv_loader;

  
//  setTID(parent_process_->getAndEnhanceTIDCounter());

  phys_page_for_stack = PageManager::instance()->allocPPN();
  parent_process_->process_loader->arch_memory_.arch_memory_lock.acquire();
  virt_page_for_stack = USER_BREAK / PAGE_SIZE - 1 - (getTID() * NUM_PAGES);

  bool vpn_mapped = parent_process_->process_loader->arch_memory_.mapPage(virt_page_for_stack, phys_page_for_stack, 1);

  assert(vpn_mapped && "Virtual page for stack was already mapped - this should never happen");
  
  ArchThreads::createUserRegisters(user_registers_, execv_loader->getEntryFunction(),
                                   (void*) (USER_BREAK - sizeof(pointer) - (getTID() * NUM_PAGES * PAGE_SIZE)),
                                   getKernelStackStartPointer());

  ArchThreads::setAddressSpace(this, execv_loader->arch_memory_);


  parent_process_->process_loader->arch_memory_.arch_memory_lock.release();

  parent_process_->threadMap_lock.acquire();
  parent_process_->threadMap.insert(ustl::pair<size_t, UserThread*>(getTID(), this));
  parent_process_->threadMap_lock.release();

  debug(USERTHREAD, "Added UserThread with TID %zu to ThreadMap of the Process\n", getTID());
  

  

}
*/
/////////////////////////GETTERS AND SETTERS//////////////////////////////////////////
void UserThread::setThreadExitStatus()
{
  thread_exit_status_lock.acquire();
  exit_is_called = true;
  thread_exit_status_lock.release();
  debug(USERTHREAD, "The exitflag of Thread with ID %zu and is now %d\n", getTID(), getThreadExitStatus());
}

bool UserThread::getThreadExitStatus()
{
  //thread_exit_status_lock.acquire();
  bool should_get_exited_flag = exit_is_called;
  //thread_exit_status_lock.release();
  return should_get_exited_flag;
}

void UserThread::setRetVal(void *retval)
{
  ret_val_pthreadexit = retval;
//  debug(USERTHREAD, "The ret_val_pthread_exit of Thread with ID %zu and is now %d\n", getTID(), getRetVal());
}
void *UserThread::getRetVal()
{

  return ret_val_pthreadexit;
}

bool UserThread::isUthreadlocked()
{

  if(_locked)
  {
//    debug(USERTHREAD, "LOC locked value: %ld\n" ,*_locked);
    // TODOAG: this can dereference memory after unmap
    return (*_locked == 1);
  }
  else
    return false;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UserThread::Run()
{
}