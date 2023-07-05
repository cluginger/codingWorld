//
// Created by lifilius on 02.06.23.
//

#include "../../include/kernel/PreAllocate.h"
#include "debug.h"

#include "PageManager.h"
#include "PreAllocate.h"

PreAllocate::PreAllocate(size_t page_count)
{
  for(size_t i = 0; i < page_count; i++)
  {
//    debug(USED_PPNs, "We alloc something in Preallocate\n");
    size_t ppn = PageManager::instance()->allocPPN();
    preallocedPages.push_back(ppn);
  }
}
PreAllocate::PreAllocate()
{
}

void PreAllocate::allocatePages(size_t page_count)
{
  for(size_t i = 0; i < page_count; i++)
  {
    size_t ppn = PageManager::instance()->allocPPN();
    debug(USED_PPNs, "PREALLOCATE We allocing %p\n", (void*)ppn);
    preallocedPages.push_back(ppn);
  }
}

size_t PreAllocate::getPage()
{
  if(preallocedPages.empty())
  {
    return -1;
  }
  size_t page = preallocedPages.front();
  preallocedPages.erase(preallocedPages.begin());
  return page;
}

void PreAllocate::freeAllPreAllocatedPages()
{
  size_t size = preallocedPages.size();
  for(size_t i = size; i > 0; i--)
  {
    size_t freethis = preallocedPages.back();
    debug(USED_PPNs, "PREALLOCATE We freeing %p\n", (void*)freethis);
    PageManager::instance()->freePPN(freethis);
    preallocedPages.pop_back();
  }
}
PreAllocate::~PreAllocate()
{
  freeAllPreAllocatedPages();
}