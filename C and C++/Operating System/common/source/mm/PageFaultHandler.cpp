#include "PageFaultHandler.h"
#include "kprintf.h"
#include "Thread.h"
#include "ArchInterrupts.h"
#include "offsets.h"
#include "Scheduler.h"
#include "Loader.h"
#include "Syscall.h"
#include "ArchThreads.h"
#include "PageManager.h"
#include "UserThread.h"
#include "IPT.h"
#include "UserProcess.h"
#include "SwapManager.h"
extern "C" void arch_contextSwitch();

const size_t PageFaultHandler::null_reference_check_border_ = PAGE_SIZE;

inline bool PageFaultHandler::checkPageFaultIsValid(size_t address, bool user,
                                                    bool present, bool switch_to_us, bool writing)
{
  assert((user == switch_to_us) && "Thread is in user mode even though is should not be.");
  assert(!(address < USER_BREAK && currentThread->loader_ == 0) && "Thread accesses the user space, but has no loader.");
  assert(!(user && currentThread->user_registers_ == 0) && "Thread is in user mode, but has no valid registers.");

  if(address < null_reference_check_border_)
  {
    debug(PAGEFAULT, "Maybe you are dereferencing a null-pointer.\n");
  }
  else if(!user && address >= USER_BREAK)
  {
    debug(PAGEFAULT, "You are accessing an invalid kernel address.\n");
  }
  else if(user && address >= USER_BREAK)
  {
    debug(PAGEFAULT, "You are accessing a kernel address in user-mode.\n");
  }
  else if(present && writing)
  {
    //debug(PAGEFAULT, "You got a pagefault even though the address is mapped.\n");
    return true;
  }
  else
  {
    // everything seems to be okay
    return true;
  }
  return false;
}

inline void PageFaultHandler::handlePageFault(size_t address, bool user,
                                          bool present, bool writing,
                                          bool fetch, bool switch_to_us)
{
  if (PAGEFAULT & OUTPUT_ENABLED)
    kprintfd("\n");
  debug(PAGEFAULT, "Address: %18zx - Thread %zu: %s (%p)\n",
        address, currentThread->getTID(), currentThread->getName(), currentThread);
  debug(PAGEFAULT, "Flags: %spresent, %s-mode, %s, %s-fetch, switch to userspace: %1d\n",
        present ? "    " : "not ",
        user ? "  user" : "kernel",
        writing ? "writing" : "reading",
        fetch ? "instruction" : "    operand",
        switch_to_us);

  ArchThreads::printThreadRegisters(currentThread, false);
//  debug(FORK, "present: %d\n writing: %d\n address: %18zx\n", present,writing,address);
  //size_t ppn = PageManager::instance()->allocPPN();
  if (checkPageFaultIsValid(address, user, present, switch_to_us, writing))
  {
    switch(getPageFaultType(address, user, present, switch_to_us, writing))
    {
      case COW_PF:
        currentThread->loader_->arch_memory_.handleCow(address);
        break;
      case STACK_PF:
        handleGrowingStack(address);
        break;
      case HEAP_PF:
        currentThread->loader_->arch_memory_.handleHeap(address);
        break;
      case GUARD_PAGE_PF:
        debug(GROW, "guard page works");
        Syscall::exit(1);
        break;
      case SWAP_FAULT:
        debug(SWAP, "SWAPIN Pagefault\n");
        SwapManager::instance()->addSwapInRequest(address/PAGE_SIZE, &currentThread->loader_->arch_memory_);
        break;
      case OTHER_PF:
        debug(SWAP, "dafuq we get here %p\n", address);
        currentThread->loader_->loadPage(address);
        break;
    }
  }
  else
  {
    // the page-fault seems to be faulty, print out the thread stack traces
    ArchThreads::printThreadRegisters(currentThread, true);
    currentThread->printBacktrace(true);
    if (currentThread->loader_)
      Syscall::exit(9999);
    else
      currentThread->kill();
  }
  debug(PAGEFAULT, "Page fault handling finished for Address: %18zx.\n", address);
}

void PageFaultHandler::enterPageFault(size_t address, bool user,
                                      bool present, bool writing,
                                      bool fetch)
{
  assert(currentThread && "You have a pagefault, but no current thread");
  //save previous state on stack of currentThread
  uint32 saved_switch_to_userspace = currentThread->switch_to_userspace_;

  currentThread->switch_to_userspace_ = 0;
  currentThreadRegisters = currentThread->kernel_registers_;
  ArchInterrupts::enableInterrupts();

  handlePageFault(address, user, present, writing, fetch, saved_switch_to_userspace);

  ArchInterrupts::disableInterrupts();
  currentThread->switch_to_userspace_ = saved_switch_to_userspace;
  if (currentThread->switch_to_userspace_)
    currentThreadRegisters = currentThread->user_registers_;
}

PageFaultType PageFaultHandler::getPageFaultType(size_t address, size_t user, size_t present, size_t writing, size_t switch_to_us)
{
//  debug(FORK, "%zu, %zu, %zu, %zu, %zu\n", address, user, present, writing, switch_to_us);
  assert(address || user || present || writing || switch_to_us);

  auto process = ((UserThread*)currentThread)->parent_process_;
  if(present && writing)
    return COW_PF;
  //IPT::instance()->ipt_lock.acquire();
  currentThread->loader_->arch_memory_.arch_memory_lock.acquire();
  ArchMemoryMapping m  = currentThread->loader_->arch_memory_.resolveMapping(address/PAGE_SIZE);
//  debug(SWAP, "m.pt: %p\n", m.pt);
  if(m.pt)
  {
    debug(SWAP, "m.pt -> vpn %p", (void*)address);
    debug(SWAP, "m.pt.swap: %d\n", m.pt[m.pti].swap);
    debug(SWAP, "m.pt.present: %d\n", m.pt[m.pti].present);
  }
  else
  {
    debug(SWAP, "m.pt for vpn %p is NULL\n", (void*)address);
    debug(SWAP, "Is this an valid Adress? The answer is %zu\n",currentThread->loader_->arch_memory_.checkAddressValid(address));
  }

  if(m.pt && m.pt[m.pti].swap && !m.pt[m.pti].present)
  {
    currentThread->loader_->arch_memory_.arch_memory_lock.release();
    //IPT::instance()->ipt_lock.release();

    return SWAP_FAULT;
  }
  currentThread->loader_->arch_memory_.arch_memory_lock.release();
  //IPT::instance()->ipt_lock.release();
  process->threadMap_lock.acquire();
  for(auto &user_thread : process->threadMap)
  {
    auto stack_start = user_thread.second->start_of_stack;
    //debug(SWAP, "stack begin: %p\n", stack_start - (NUM_PAGES * PAGE_SIZE));
    //debug(SWAP, "stack end: %p\n", stack_start - PAGE_SIZE);

    //auto stack_end = user_thread.second->end_of_stack;
    if((address >= (stack_start - (NUM_PAGES * PAGE_SIZE))) && (address <= (stack_start - PAGE_SIZE)))
    {
      debug(SWAP, "STACK_PF");
      debug(HEAP, "GOING into STACK??\n");
      process->threadMap_lock.release();
      return STACK_PF;
    }
  }
  process->threadMap_lock.release();
  PROCESS->heap_lock.acquire();
  if(process->heapStart <= address && address <= process->heapEndCurrent)
  {
    debug(HEAP, "GOING into HEAP\n");
    PROCESS->heap_lock.release();
    return HEAP_PF;
  }
  else
  {
    debug(HEAP, "heapstart %p - address %p - heapend  %p\n", (void*)process->heapStart, (void*)address, (void*)process->heapEndCurrent);
    PROCESS->heap_lock.release();
  }

  debug(HEAP, "GOING into OTHER??\n");
  return OTHER_PF;
}


size_t PageFaultHandler::handleGrowingStack(size_t address)
{
  debug(SWAP, "ALLOCING in growing stack\n");
  size_t page = PageManager::instance()->allocPPN();
  debug(USED_PPNs, "Prealloc in handleGrowingStack\n");
  PreAllocate pages(3);
  IPT::instance()->ipt_lock.acquire();
  currentThread->loader_->arch_memory_.arch_memory_lock.acquire();

  if(!currentThread->loader_->arch_memory_.checkAddressValid(address))
  {

    bool vpn_mapped = currentThread->loader_->arch_memory_.mapPage(address / PAGE_SIZE, page, 1, &pages);
    if (!vpn_mapped)
    {
      debug(PAGEFAULT, "PFH::handlegrowingstack: The page has been mapped by someone else.\n");
      PageManager::instance()->freePPN(page);
    }
    debug(GROW, "growing stack works\n");
    currentThread->loader_->arch_memory_.arch_memory_lock.release();
    IPT::instance()->ipt_lock.release();
    return 0;
  }
  else
  {
    PageManager::instance()->freePPN(page);
  }

  currentThread->loader_->arch_memory_.arch_memory_lock.release();
  IPT::instance()->ipt_lock.release();
  return 1;
}

