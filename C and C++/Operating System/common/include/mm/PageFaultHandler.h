#pragma once

#include "types.h"
#include "PreAllocate.h"
#define UTHREAD ((UserThread*)currentThread)
#define PROCESS UTHREAD->parent_process_

  enum PageFaultType {
    COW_PF, STACK_PF, HEAP_PF, GUARD_PAGE_PF, OTHER_PF, SWAP_FAULT
  };
class PageFaultHandler
{
private:
  /**
   * The border address at which it is assumed that
   * a pagefault happened by dereferencing a null pointer.
   */
  static const size_t null_reference_check_border_;

  /**
   * Print out the pagefault information. Check if the pagefault is valid, or the thread state is corrupt.
   * Afterwards, load a the if necessary.
   * @param address The address on which the fault happened
   * @param user true if the fault occurred in user mode, else from kernel mode
   * @param present true if the fault happened on a already mapped page
   * @param switch_to_us the switch to userspace flag of the current thread
   */
  static inline bool checkPageFaultIsValid(size_t address, bool user, bool present, bool switch_to_us, bool writing);

  /**
   * Print out the pagefault information. Check if the pagefault is valid, or the thread state is corrupt.
   * Afterwards, load a the if necessary.
   * @param address The address on which the fault happened
   * @param user true if the fault occurred in user mode, else from kernel mode
   * @param present true if the fault happened on a already mapped page
   * @param writing true if the fault happened by writing to an address, else reading
   * @param fetch true in case the fault happened by an instruction fetch, else by an operand fetch
   * @param switch_to_us the switch to userspace flag of the current thread
   */
  static inline void handlePageFault(size_t address, bool user,
                                     bool present, bool writing,
                                     bool fetch, bool switch_to_us);

public:
  /**
   * Enter a new pagefault. The pagefault is processed.
   * Afterwards, a context switch is performed (if needed).
   * @param address The address on which the fault happened
   * @param user true if the fault occurred in user mode, else from kernel mode
   * @param present true if the fault happened on a already mapped page
   * @param writing true if the fault happened by writing to an address, else reading
   * @param fetch true in case the fault happened by an instruction fetch, else by an operand fetch
   */
  static void enterPageFault(size_t address, bool user,
                             bool present, bool writing,
                             bool fetch);


  static size_t handleGrowingStack(size_t address);
  static PageFaultType getPageFaultType(size_t address, size_t user, size_t present, size_t switch_to_us, size_t writing);
};
