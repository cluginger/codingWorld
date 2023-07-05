#include "ArchMemory.h"
#include "ArchInterrupts.h"
#include "kprintf.h"
#include "PageManager.h"
#include "kstring.h"
#include "ArchThreads.h"
#include "Thread.h"
#include "IPT.h"
#include "SwapManager.h"
#include "debug.h"

PageMapLevel4Entry kernel_page_map_level_4[PAGE_MAP_LEVEL_4_ENTRIES] __attribute__((aligned(PAGE_SIZE)));
PageDirPointerTableEntry kernel_page_directory_pointer_table[2 * PAGE_DIR_POINTER_TABLE_ENTRIES] __attribute__((aligned(PAGE_SIZE)));
PageDirEntry kernel_page_directory[2 * PAGE_DIR_ENTRIES] __attribute__((aligned(PAGE_SIZE)));
PageTableEntry kernel_page_table[8 * PAGE_TABLE_ENTRIES] __attribute__((aligned(PAGE_SIZE)));

ArchMemory::ArchMemory() : arch_memory_lock("arch_memory_lock")
{
  debug(SWAP, "ALLOCPPN COMING FROM ARCHMEMORY\n");
  page_map_level_4_ = PageManager::instance()->allocPPN();
  PageMapLevel4Entry* new_pml4 = (PageMapLevel4Entry*) getIdentAddressOfPPN(page_map_level_4_);
  memcpy((void*) new_pml4, (void*) kernel_page_map_level_4, PAGE_SIZE);
  memset(new_pml4, 0, PAGE_SIZE / 2); // should be zero, this is just for safety
}

template<typename T>
bool ArchMemory::checkAndRemove(pointer map_ptr, uint64 index)
{
  T* map = (T*) map_ptr;
  debug(A_MEMORY, "%s: page %p index %zx\n", __PRETTY_FUNCTION__, map, index);
  ((uint64*) map)[index] = 0;
  for (uint64 i = 0; i < PAGE_DIR_ENTRIES; i++)
  {
    if (map[i].present != 0 || map[i].swap)
      return false;
  }
  return true;
}

bool ArchMemory::unmapPage(uint64 virtual_page)
{
  //IPT::instance()->ipt_lock.acquire();
  //arch_memory_lock.acquire();

  debug(ARCHMEM, "unmapping vpn: %p/n", virtual_page);

  ArchMemoryMapping m = resolveMapping(virtual_page);

  if(m.page_ppn == 0)
  {
    debug(ARCHMEM, "m.page_ppn = %ld\n", m.page_ppn);
    assert(m.page_ppn != 0);
  }
  if(m.page_size != PAGE_SIZE)
  {
    debug(ARCHMEM, "m.page_size: %ld, PAGESIZE: %ld\n", m.page_size, PAGE_SIZE);
    assert(m.page_size == PAGE_SIZE);
  }
  if(!m.pt[m.pti].present)
  {
    debug(ARCHMEM, "m.pt[m.pti] NOT PRESENT\n");
    assert(m.pt[m.pti].present);
  }

  m.pt[m.pti].present = 0;
  //if swap bit 1 then delete SPT Entry and set to 0
  if(!((m.pt[m.pti].present && m.pt[m.pti].swap) ||(m.pt[m.pti].cow && m.pt[m.pti].writeable)))
  {
    if(m.pt[m.pti].swap)
    {
      SwapManager::instance()->deleteAndFreeSPTEntry(m.pt[m.pti].page_ppn, this);
      m.pt[m.pti].swap = 0;
    }
    else
    {
      IPT::instance()->deleteAndFreeIPTEntry(m.pt[m.pti].page_ppn, this);
    }
  }

  //else:
  //IPT::instance()->ipt_lock.acquire();

  //IPT::instance()->ipt_lock.release();

//  debug(FORK, "free %ld\n", m.page_ppn);

  ((uint64*)m.pt)[m.pti] = 0; // for easier debugging
  bool empty = checkAndRemove<PageTableEntry>(getIdentAddressOfPPN(m.pt_ppn), m.pti);
  if (empty)
  {
    empty = checkAndRemove<PageDirPageTableEntry>(getIdentAddressOfPPN(m.pd_ppn), m.pdi);
    PageManager::instance()->freePPN(m.pt_ppn);
    //IPT::instance()->deleteAndFreeIPTEntry(m.pt_ppn, this);
  }
  if (empty)
  {
    empty = checkAndRemove<PageDirPointerTablePageDirEntry>(getIdentAddressOfPPN(m.pdpt_ppn), m.pdpti);
    PageManager::instance()->freePPN(m.pd_ppn);
    //IPT::instance()->deleteAndFreeIPTEntry(m.pd_ppn, this);
  }
  if (empty)
  {
    checkAndRemove<PageMapLevel4Entry>(getIdentAddressOfPPN(m.pml4_ppn), m.pml4i);
    PageManager::instance()->freePPN(m.pdpt_ppn);
    //IPT::instance()->deleteAndFreeIPTEntry(m.pdpt_ppn, this);
  }
//  debug(FORK, "Freed everything");
  //arch_memory_lock.release();
  //IPT::instance()->ipt_lock.release();
  //IPT::instance()->printIPT();
  //SwapManager::instance()->printSPT();
  return true;
}

template<typename T>
void ArchMemory::insert(pointer map_ptr, uint64 index, uint64 ppn, uint64 bzero, uint64 size, uint64 user_access,
                        uint64 writeable)
{
  assert(map_ptr & ~0xFFFFF00000000000ULL);
  T* map = (T*) map_ptr;
  debug(A_MEMORY, "%s: page %p index %zx ppn %zx user_access %zx size %zx\n", __PRETTY_FUNCTION__, map, index, ppn,
        user_access, size);
  if (bzero)
  {
    memset((void*) getIdentAddressOfPPN(ppn), 0, PAGE_SIZE);
    assert(((uint64* )map)[index] == 0);
  }
  map[index].size = size;
  map[index].writeable = writeable;
  map[index].page_ppn = ppn;
  map[index].user_access = user_access;
  map[index].present = 1;
  assert(map[index].swap != 1);
}

bool ArchMemory::mapPage(uint64 virtual_page, uint64 physical_page, uint64 user_access, PreAllocate *ppns)
{
  debug(ARCHMEM, "mapPage with vpn: %p and ppn %p\n", (void*)virtual_page, (void*)physical_page);
  assert(arch_memory_lock.isHeldBy(currentThread) && IPT::instance()->ipt_lock.isHeldBy(currentThread));
  debug(A_MEMORY, "pml4 %zx vpn %zx ppn %zx access %zx\n", page_map_level_4_, virtual_page, physical_page, user_access);
  ArchMemoryMapping m = resolveMapping(page_map_level_4_, virtual_page);
  //if swap bit set 1 then return true
  if(m.pt && m.pt[m.pti].swap){
    debug(SWAP, "ppn in mapPage: %p\n", m.pt[m.pti].page_ppn);
    return false;
  }

  assert((m.page_size == 0) || (m.page_size == PAGE_SIZE));

  if (m.pdpt_ppn == 0)
  {
//    debug(SWAP, "dafuq MAPPING 1\n");
    m.pdpt_ppn = ppns->getPage();
    insert<PageMapLevel4Entry>((pointer) m.pml4, m.pml4i, m.pdpt_ppn, 1, 0, 1, 1);
  }

  if (m.pd_ppn == 0)
  {
//    debug(SWAP, "dafuq MAPPING 2\n");
    m.pd_ppn = ppns->getPage();
    insert<PageDirPointerTablePageDirEntry>(getIdentAddressOfPPN(m.pdpt_ppn), m.pdpti, m.pd_ppn, 1, 0, 1, 1);
  }

  if (m.pt_ppn == 0)
  {
//    debug(SWAP, "dafuq MAPPING 3\n");
    m.pt_ppn = ppns->getPage();
    insert<PageDirPageTableEntry>(getIdentAddressOfPPN(m.pd_ppn), m.pdi, m.pt_ppn, 1, 0, 1, 1);
  }

  if (m.page_ppn == 0)
  {
//    debug(SWAP, "dafuq MAPPING 4\n");
    //IPT::instance()->ipt_lock.acquire();
//    debug(FORK, "add in mapping");
    debug(SWAP, "map page: %p\n", physical_page);
    IPT::instance()->addIPTEntry(physical_page, virtual_page, this, true);
    //IPT::instance()->ipt_lock.release();
    insert<PageTableEntry>(getIdentAddressOfPPN(m.pt_ppn), m.pti, physical_page, 0, 0, user_access, 1);

    return true;
  }
  return false;
}

ArchMemory::~ArchMemory()
{
  IPT::instance()->ipt_lock.acquire();
  arch_memory_lock.acquire();
  debug(SWAP, "number of free pages at start: %zu\n", PageManager::instance()->getNumFreePages());
  assert(currentThread->kernel_registers_->cr3 != page_map_level_4_ * PAGE_SIZE && "thread deletes its own arch memory");
  PageMapLevel4Entry* pml4 = (PageMapLevel4Entry*) getIdentAddressOfPPN(page_map_level_4_);
  for (uint64 pml4i = 0; pml4i < PAGE_MAP_LEVEL_4_ENTRIES / 2; pml4i++) // free only lower half
  {
    if (pml4[pml4i].present)
    {
      PageDirPointerTableEntry* pdpt = (PageDirPointerTableEntry*) getIdentAddressOfPPN(pml4[pml4i].page_ppn);
      for (uint64 pdpti = 0; pdpti < PAGE_DIR_POINTER_TABLE_ENTRIES; pdpti++)
      {
        if (pdpt[pdpti].pd.present)
        {
          assert(pdpt[pdpti].pd.size == 0);
          PageDirEntry* pd = (PageDirEntry*) getIdentAddressOfPPN(pdpt[pdpti].pd.page_ppn);
          for (uint64 pdi = 0; pdi < PAGE_DIR_ENTRIES; pdi++)
          {
            if (pd[pdi].pt.present)
            {
              assert(pd[pdi].pt.size == 0);
              PageTableEntry* pt = (PageTableEntry*) getIdentAddressOfPPN(pd[pdi].pt.page_ppn);
              for (uint64 pti = 0; pti < PAGE_TABLE_ENTRIES; pti++)
              {
                if (pt[pti].present)
                {
                  IPT::instance()->deleteAndFreeIPTEntry(pt[pti].page_ppn, this);

                }
                else if(pt[pti].swap)
                {
                  SwapManager::instance()->deleteAndFreeSPTEntry(pt[pti].page_ppn, this);
                  //pt[pti].swap = 0;
                }
                //else if swap bit set 1 delete SPT Entry
                //debug(IPT_DEBUG, "IPT size after delete: %d\n",IPT::instance()->ipt_[pt[pti].page_ppn]->process_info.size());
                //debug(SPT, "SPT size after delete: %d\n",SwapManager::instance()->spt_[pt[pti].page_ppn]->process_info.size());
              }

              pd[pdi].pt.present = 0;
              PageManager::instance()->freePPN(pd[pdi].pt.page_ppn);
            }
          }
          pdpt[pdpti].pd.present = 0;
          PageManager::instance()->freePPN(pdpt[pdpti].pd.page_ppn);
        }
      }
      pml4[pml4i].present = 0;
      PageManager::instance()->freePPN(pml4[pml4i].page_ppn);
    }
  }
  PageManager::instance()->freePPN(page_map_level_4_);
  debug(SWAP, "number of free pages after clearing: %zu\n", PageManager::instance()->getNumFreePages());

  //!!!!! we have to clear the spt/ipt
  SwapManager::instance()->clearSptEntriesByArchmem(this);

  IPT::instance()->clearIptEntriesByArchmem(this);

  debug(SWAP, "ENDE ARCHMEM");
  arch_memory_lock.release();
  IPT::instance()->ipt_lock.release();
}

pointer ArchMemory::checkAddressValid(uint64 vaddress_to_check)
{
  ArchMemoryMapping m = resolveMapping(page_map_level_4_, vaddress_to_check / PAGE_SIZE);
  if (m.page != 0)
  {
    debug(ARCHMEM, "checkAddressValid %zx and %zx -> true\n", page_map_level_4_, vaddress_to_check);
    return m.page | (vaddress_to_check % m.page_size);
  }
  else
  {
    debug(ARCHMEM, "checkAddressValid %zx and %zx -> false\n", page_map_level_4_, vaddress_to_check);
    return 0;
  }
}

const ArchMemoryMapping ArchMemory::resolveMapping(uint64 vpage)
{
  assert(arch_memory_lock.isHeldBy(currentThread) && "archmemory lock is not held when calling resolveMapping\n");
  return resolveMapping(page_map_level_4_, vpage);
}

const ArchMemoryMapping ArchMemory::resolveMapping(uint64 pml4, uint64 vpage)
{
  assert((vpage * PAGE_SIZE < USER_BREAK || vpage * PAGE_SIZE >= KERNEL_START) &&
         "This is not a valid vpn! Did you pass an address to resolveMapping?");
  ArchMemoryMapping m;

  m.pti = vpage;
  m.pdi = m.pti / PAGE_TABLE_ENTRIES;
  m.pdpti = m.pdi / PAGE_DIR_ENTRIES;
  m.pml4i = m.pdpti / PAGE_DIR_POINTER_TABLE_ENTRIES;

  m.pti %= PAGE_TABLE_ENTRIES;
  m.pdi %= PAGE_DIR_ENTRIES;
  m.pdpti %= PAGE_DIR_POINTER_TABLE_ENTRIES;
  m.pml4i %= PAGE_MAP_LEVEL_4_ENTRIES;

  assert(pml4 < PageManager::instance()->getTotalNumPages());
  m.pml4 = (PageMapLevel4Entry*) getIdentAddressOfPPN(pml4);
  m.pdpt = 0;
  m.pd = 0;
  m.pt = 0;
  m.page = 0;
  m.pml4_ppn = pml4;
  m.pdpt_ppn = 0;
  m.pd_ppn = 0;
  m.pt_ppn = 0;
  m.page_ppn = 0;
  m.page_size = 0;
  if (m.pml4[m.pml4i].present)
  {
    m.pdpt_ppn = m.pml4[m.pml4i].page_ppn;
    m.pdpt = (PageDirPointerTableEntry*) getIdentAddressOfPPN(m.pml4[m.pml4i].page_ppn);
    if (m.pdpt[m.pdpti].pd.present && !m.pdpt[m.pdpti].pd.size) // 1gb page ?
    {
      m.pd_ppn = m.pdpt[m.pdpti].pd.page_ppn;
      if (m.pd_ppn > PageManager::instance()->getTotalNumPages())
      {
        debug(A_MEMORY, "%zx\n", m.pd_ppn);
      }
      assert(m.pd_ppn < PageManager::instance()->getTotalNumPages());
      m.pd = (PageDirEntry*) getIdentAddressOfPPN(m.pdpt[m.pdpti].pd.page_ppn);
      if (m.pd[m.pdi].pt.present && !m.pd[m.pdi].pt.size) // 2mb page ?
      {
        m.pt_ppn = m.pd[m.pdi].pt.page_ppn;
        assert(m.pt_ppn < PageManager::instance()->getTotalNumPages());
        m.pt = (PageTableEntry*) getIdentAddressOfPPN(m.pd[m.pdi].pt.page_ppn);
        if (m.pt[m.pti].present)
        {
          m.page = getIdentAddressOfPPN(m.pt[m.pti].page_ppn);
          m.page_ppn = m.pt[m.pti].page_ppn;
          assert(m.page_ppn < PageManager::instance()->getTotalNumPages());
          m.page_size = PAGE_SIZE;
        }
      }
      else if (m.pd[m.pdi].page.present)
      {
        m.page_size = PAGE_SIZE * PAGE_TABLE_ENTRIES;
        m.page_ppn = m.pd[m.pdi].page.page_ppn;
        m.page = getIdentAddressOfPPN(m.pd[m.pdi].page.page_ppn);
      }
    }
    else if (m.pdpt[m.pdpti].page.present)
    {
      m.page_size = PAGE_SIZE * PAGE_TABLE_ENTRIES * PAGE_DIR_ENTRIES;
      m.page_ppn = m.pdpt[m.pdpti].page.page_ppn;
      assert(m.page_ppn < PageManager::instance()->getTotalNumPages());
      m.page = getIdentAddressOfPPN(m.pdpt[m.pdpti].page.page_ppn);
    }
  }
  return m;
}

uint64 ArchMemory::get_PPN_Of_VPN_In_KernelMapping(size_t virtual_page, size_t *physical_page,
                                                   size_t *physical_pte_page)
{
  ArchMemoryMapping m = resolveMapping(((uint64) VIRTUAL_TO_PHYSICAL_BOOT(kernel_page_map_level_4) / PAGE_SIZE),
                                       virtual_page);
  if (physical_page)
    *physical_page = m.page_ppn;
  if (physical_pte_page)
    *physical_pte_page = m.pt_ppn;
  return m.page_size;
}

void ArchMemory::mapKernelPage(size_t virtual_page, size_t physical_page)
{
  ArchMemoryMapping mapping = resolveMapping(((uint64) VIRTUAL_TO_PHYSICAL_BOOT(kernel_page_map_level_4) / PAGE_SIZE),
                                             virtual_page);
  PageMapLevel4Entry* pml4 = kernel_page_map_level_4;
  assert(pml4[mapping.pml4i].present);
  PageDirPointerTableEntry *pdpt = (PageDirPointerTableEntry*) getIdentAddressOfPPN(pml4[mapping.pml4i].page_ppn);
  assert(pdpt[mapping.pdpti].pd.present);
  PageDirEntry *pd = (PageDirEntry*) getIdentAddressOfPPN(pdpt[mapping.pdpti].pd.page_ppn);
  assert(pd[mapping.pdi].pt.present);
  PageTableEntry *pt = (PageTableEntry*) getIdentAddressOfPPN(pd[mapping.pdi].pt.page_ppn);
  assert(!pt[mapping.pti].present);
  pt[mapping.pti].writeable = 1;
  pt[mapping.pti].page_ppn = physical_page;
  pt[mapping.pti].present = 1;
  asm volatile ("movq %%cr3, %%rax; movq %%rax, %%cr3;" ::: "%rax");
}

void ArchMemory::unmapKernelPage(size_t virtual_page)
{
  ArchMemoryMapping mapping = resolveMapping(((uint64) VIRTUAL_TO_PHYSICAL_BOOT(kernel_page_map_level_4) / PAGE_SIZE),
                                             virtual_page);
  PageMapLevel4Entry* pml4 = kernel_page_map_level_4;
  assert(pml4[mapping.pml4i].present);
  PageDirPointerTableEntry *pdpt = (PageDirPointerTableEntry*) getIdentAddressOfPPN(pml4[mapping.pml4i].page_ppn);
  assert(pdpt[mapping.pdpti].pd.present);
  PageDirEntry *pd = (PageDirEntry*) getIdentAddressOfPPN(pdpt[mapping.pdpti].pd.page_ppn);
  assert(pd[mapping.pdi].pt.present);
  PageTableEntry *pt = (PageTableEntry*) getIdentAddressOfPPN(pd[mapping.pdi].pt.page_ppn);
  assert(pt[mapping.pti].present);
  pt[mapping.pti].present = 0;
  pt[mapping.pti].writeable = 0;
  PageManager::instance()->freePPN(pt[mapping.pti].page_ppn);
  asm volatile ("movq %%cr3, %%rax; movq %%rax, %%cr3;" ::: "%rax");
}

PageMapLevel4Entry* ArchMemory::getRootOfKernelPagingStructure()
{
  return kernel_page_map_level_4;
}

size_t ArchMemory::countPresentBits(PageMapLevel4Entry* pml4)
{
  size_t count = 0;
  count++;
  for (uint64 pml4i = 0; pml4i < PAGE_MAP_LEVEL_4_ENTRIES / 2; pml4i++) // free only lower half
  {
    if (pml4[pml4i].present)
    {
      count++;
      auto pdpt = (PageDirPointerTableEntry*) getIdentAddressOfPPN(pml4[pml4i].page_ppn);
      for (uint64 pdpti = 0; pdpti < PAGE_DIR_POINTER_TABLE_ENTRIES; pdpti++)
      {
        if (pdpt[pdpti].pd.present)
        {
          count++;
          PageDirEntry* pd = (PageDirEntry*) getIdentAddressOfPPN(pdpt[pdpti].pd.page_ppn);
          for (uint64 pdi = 0; pdi < PAGE_DIR_ENTRIES; pdi++)
          {
            if (pd[pdi].pt.present)
            {
              count++;
            }
          }
        }
      }
    }
  }
  return count;
}

ArchMemory::ArchMemory(ArchMemory const &src) : arch_memory_lock("arch_memory_lock")
{
  debug(USED_PPNs, "preallocate in ArchMem CC\n");
  PreAllocate pages;
  size_t needed_page_count = 0, needed_page_count_check = 0;
  arch_memory_lock.acquire();
  auto pml4 = (PageMapLevel4Entry*) getIdentAddressOfPPN(src.page_map_level_4_);
  while(1)
  {
    needed_page_count = countPresentBits(pml4);
    arch_memory_lock.release();
    pages.allocatePages(needed_page_count);
    IPT::instance()->ipt_lock.acquire();
    arch_memory_lock.acquire();
    needed_page_count_check = countPresentBits(pml4);
    debug(LOL, "count: %ld - checkcount: %ld\n", needed_page_count, needed_page_count_check);
    if(needed_page_count_check != needed_page_count)
    {
      pages.freeAllPreAllocatedPages();
      debug(LOL, "more... MOOORE!\n");
      IPT::instance()->ipt_lock.release();
      continue;
    }
    else
      break;
  }

  size_t actual_needed_page_count = 0;
  page_map_level_4_ = pages.getPage();
  actual_needed_page_count++;
  PageMapLevel4Entry* pml4_clone = (PageMapLevel4Entry*) getIdentAddressOfPPN(page_map_level_4_);
  memcpy((void*)pml4_clone, (void*) pml4, PAGE_SIZE);
  for (uint64 pml4i = 0; pml4i < PAGE_MAP_LEVEL_4_ENTRIES / 2; pml4i++) // free only lower half
  {
    if (pml4_clone[pml4i].present)
    {
      uint64 pdpt_level = pages.getPage();
      actual_needed_page_count++;
      PageDirPointerTableEntry* pdpt_clone = (PageDirPointerTableEntry*) getIdentAddressOfPPN(pdpt_level);
      PageDirPointerTableEntry* pdpt = (PageDirPointerTableEntry*) getIdentAddressOfPPN(pml4_clone[pml4i].page_ppn);
      memcpy((void*)pdpt_clone, (void*) pdpt, PAGE_SIZE);
      pml4_clone[pml4i].page_ppn = pdpt_level;
      for (uint64 pdpti = 0; pdpti < PAGE_DIR_POINTER_TABLE_ENTRIES; pdpti++)
      {
        if (pdpt_clone[pdpti].pd.present)
        {
          assert(pdpt_clone[pdpti].pd.size == 0);
          uint64 pd_level = pages.getPage();
          actual_needed_page_count++;
          PageDirEntry* pd_clone = (PageDirEntry*) getIdentAddressOfPPN(pd_level);
          PageDirEntry* pd = (PageDirEntry*) getIdentAddressOfPPN(pdpt_clone[pdpti].pd.page_ppn);
          memcpy((void*)pd_clone, (void*) pd, PAGE_SIZE);
          pdpt_clone[pdpti].pd.page_ppn = pd_level;
          for (uint64 pdi = 0; pdi < PAGE_DIR_ENTRIES; pdi++)
          {
            if (pd_clone[pdi].pt.present)
            {
              assert(pd_clone[pdi].pt.size == 0);
              uint64 pt_level = pages.getPage();
              actual_needed_page_count++;
              PageTableEntry* pt_clone = (PageTableEntry*) getIdentAddressOfPPN(pt_level);
              PageTableEntry* pt = (PageTableEntry*) getIdentAddressOfPPN(pd_clone[pdi].pt.page_ppn);
              memcpy((void*)pt_clone, (void*) pt, PAGE_SIZE);
              pd_clone[pdi].pt.page_ppn = pt_level;
              for (uint64 pti = 0; pti < PAGE_TABLE_ENTRIES; pti++)
              {
                size_t vpn = pti + (pdi << 9) + (pdpti << 18) + (pml4i << 27);
                if (pt[pti].present)
                {
                  pt[pti].cow = 1;
                  pt_clone[pti].cow = 1;
                  pt[pti].writeable = 0;
                  pt_clone[pti].writeable = 0;
                  //assert(!IPT::instance()->ipt_lock.isHeldBy(currentThread) && "failed lock in CC");
                  //IPT::instance()->ipt_lock.acquire();
                  IPT::instance()->addIPTEntry(pt[pti].page_ppn, vpn, this, true);
                  //IPT::instance()->ipt_lock.release();

                }
                else if(pt[pti].swap)
                {
                  pt[pti].cow = 1;
                  pt_clone[pti].cow = 1;
                  pt[pti].writeable = 0;
                  pt_clone[pti].writeable = 0;

                  SwapManager::instance()->spt_.at(pt[pti].page_ppn)->process_info.insert({this, vpn});
                }
                //else swap bit set 1 add SPT Entry and do cow annd writeable setting
              }

            }
          }

        }
      }

    }

  }
  //IPT::instance()->printIPT();
  //SwapManager::instance()->printSPT();
  //assert(IPT::instance()->ipt_.size() == 0);
  //assert(SwapManager::instance()->spt_.size() == 0);
  assert(actual_needed_page_count == needed_page_count);
  arch_memory_lock.release();
  IPT::instance()->ipt_lock.release();
}

void ArchMemory::handleCow(size_t address)
{
  size_t newpage = PageManager::instance()->allocPPN();
  debug(SWAP, "ALLOCPPN COMING FROM COW %p\n", (void*)newpage);
  IPT::instance()->ipt_lock.acquire();
  arch_memory_lock.acquire();
  size_t vpn = address / PAGE_SIZE;
  auto m = resolveMapping(vpn);
  if (!m.pt || m.pt[m.pti].swap == 1)
  {
    PageManager::instance()->freePPN(newpage);
    arch_memory_lock.release();
    IPT::instance()->ipt_lock.release();
    return;
  }

  //assert(!IPT::instance()->ipt_lock.isHeldBy(currentThread) && "PFH already acquired");
  //IPT::instance()->ipt_lock.acquire();
  auto process_access = IPT::instance()->getNumberOfProcessAccess(m.pt[m.pti].page_ppn);
  // (DONE) TODOAG: check if pt exists, otherwise it's just a nullptr 
  if(m.pt[m.pti].cow && m.pt[m.pti].present && process_access > 1)
  {
    assert(!((m.pt[m.pti].cow && m.pt[m.pti].writeable)) || m.pt[m.pti].present || !(m.pt[m.pti].swap));
    auto current_page = m.pt[m.pti].page_ppn;
    IPT::instance()->addIPTEntry(newpage, vpn, this, true);
    IPT::instance()->deleteAndFreeIPTEntry(current_page, this);
    //IPT::instance()->deleteIPTEntry(current_page, this);

    memcpy((void*)ArchMemory::getIdentAddressOfPPN(newpage), (void*)ArchMemory::getIdentAddressOfPPN(m.pt[m.pti].page_ppn), PAGE_SIZE);
    m.pt[m.pti].page_ppn = newpage;
    m.pt[m.pti].writeable = 1;
    m.pt[m.pti].cow = 0;

    //IPT::instance()->ipt_lock.release();
  }

  else if(m.pt[m.pti].cow && m.pt[m.pti].present && process_access == 1)
  {
    //IPT::instance()->ipt_lock.release();
    assert(!((m.pt[m.pti].cow && m.pt[m.pti].writeable) || !(m.pt[m.pti].present)));
    debug(FORK, "Exactly one process");
    m.pt[m.pti].writeable = 1;
    m.pt[m.pti].cow = 0;
    debug(FORK, "free %ld\n", newpage);
    PageManager::instance()->freePPN(newpage);
  }
  else
  {
    //IPT::instance()->ipt_lock.release();
    m.pt[m.pti].writeable = 1;
    m.pt[m.pti].cow = 0;
    debug(FORK, "free %ld\n", newpage);
    PageManager::instance()->freePPN(newpage);
  }
  arch_memory_lock.release();
  IPT::instance()->ipt_lock.release();
}

bool ArchMemory::isDirty(size_t vpn, size_t ppn){
  arch_memory_lock.acquire();
  ArchMemoryMapping m = resolveMapping(vpn);
  m.pt[m.pti].present = 0;
  if(m.pt[m.pti].dirty)
  {
    m.pt[m.pti].swap = 1;
    m.pt[m.pti].page_ppn = ppn;
    arch_memory_lock.release();

    return true;
  }
  m.pt[m.pti].swap = 0;
  m.pt[m.pti].page_ppn = 0;
  arch_memory_lock.release();
  return false;

}

ssize_t ArchMemory::getOffset(size_t vpn)
{
  assert(arch_memory_lock.isHeldBy(currentThread));
  ArchMemoryMapping m = resolveMapping(vpn);
  assert(!((m.pt[m.pti].present && m.pt[m.pti].swap) ||(m.pt[m.pti].cow && m.pt[m.pti].writeable)));

  assert(m.pt);
  if(m.pt && m.pt[m.pti].swap && !m.pt[m.pti].present)
    return m.pt[m.pti].page_ppn;
  else
    return -1;

}


void ArchMemory::handleHeap(size_t address)
{
  debug(USED_PPNs, "PreAlloc in handleHeap\n");
  PreAllocate pages(3);
  size_t new_ppn = PageManager::instance()->allocPPN();
  debug(SWAP, "ALLOC COMING FROM HEAP %p\n", (void*)new_ppn);
  IPT::instance()->ipt_lock.acquire();
  arch_memory_lock.acquire();
  debug(HEAP,"heap handled?\n");
  size_t vpn = (address & ~(PAGE_SIZE - 1))/PAGE_SIZE;
  if(mapPage(vpn, new_ppn, 1, &pages))
  {
    ArchMemoryMapping m = resolveMapping(vpn);
    m.pt[m.pti].page_ppn = new_ppn;
  }
  else
  {
    PageManager::instance()->freePPN(new_ppn);
  }
  arch_memory_lock.release();
  IPT::instance()->ipt_lock.release();
}

void ArchMemory::handleAccessedBits(size_t pml4_)
{

  auto pml4 = (PageMapLevel4Entry*) getIdentAddressOfPPN(pml4_);
  if(pml4)
  {
    for (uint64 pml4i = 0; pml4i < PAGE_MAP_LEVEL_4_ENTRIES / 2; pml4i++) //only lower half
    {
      if (pml4[pml4i].present)
      {
        auto pdpt = (PageDirPointerTableEntry*) getIdentAddressOfPPN(pml4[pml4i].page_ppn);
        for (uint64 pdpti = 0; pdpti < PAGE_DIR_POINTER_TABLE_ENTRIES; pdpti++)
        {
          if (pdpt[pdpti].pd.present)
          {
            auto pd = (PageDirEntry*) getIdentAddressOfPPN(pdpt[pdpti].pd.page_ppn);
            for (uint64 pdi = 0; pdi < PAGE_DIR_ENTRIES; pdi++)
            {
              if (pd[pdi].pt.present)
              {
                auto pt = (PageTableEntry*) getIdentAddressOfPPN(pd[pdi].pt.page_ppn);
                for (uint64 pti = 0; pti < PAGE_TABLE_ENTRIES; pti++)
                {
                  if (pt[pti].present)
                  {
                    if(pt[pti].accessed)
                    {
//                      IPT::instance()->ipt_[pt[pti].page_ppn]->nfuHits++;
                      IPT::instance()->ipt_[pt[pti].page_ppn]->age |= 0x100000000000;
//                      debug(PRAsAGING, "AGE BITCH\n");
//                      debug(LOL, "page: %p -> ID :%ld\n", pt[pti].page_ppn, currentThread->getTID());
//                      size_t vpn = pti + (pdi << 9) + (pdpti << 18) + (pml4i << 27);
//                      debug(PRAsNFU, "NFU %ld hits PPN %p VPN %p\n", IPT::instance()->ipt_[pt[pti].page_ppn]->nfuHits, pt[pti].page_ppn,vpn);
//                      debug(LOL, "AGING %p hits\n", (void*)IPT::instance()->ipt_[pt[pti].page_ppn]->agingHits);
                      pt[pti].accessed = 0;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

void ArchMemory::handleBrk(size_t vpn)
{
  IPT::instance()->ipt_lock.acquire();
  arch_memory_lock.acquire();
  ArchMemoryMapping m = resolveMapping(vpn);
  if(m.page_ppn)
  {
    debug(HEAP, "unmapping\n");
    unmapPage(vpn);
  }
  arch_memory_lock.release();
  IPT::instance()->ipt_lock.release();
}