#include "IPT.h"
#include "PageManager.h"
#include "new.h"
#include "offsets.h"
#include "paging-definitions.h"
#include "ArchCommon.h"
#include "ArchMemory.h"
#include "kprintf.h"
#include "Scheduler.h"
#include "KernelMemoryManager.h"
#include "assert.h"
#include "Bitmap.h"

IPT *IPT::instance_ = nullptr;

IPT *IPT::instance()
{
    if(instance_ == nullptr){
        instance_ = new IPT();
    }
    return instance_;
}

IPT::IPT(): ipt_lock("PageManager::ipt_lock")
{
    for(size_t i = 0; i < PageManager::instance()->getTotalNumPages(); i++){
        debug(IPT_DEBUG, "we fill IPT constructor : %p\n", (void*)i);
        ipt_[i] = new IPTEntry;
        
    }

    debug(FORK, "PT Entries got initialized");
}
IPT::~IPT()
{
}

bool IPT::addIPTEntry(size_t ppn, size_t vpn, ArchMemory* arch_mem, bool swappable)
{
  assert(ipt_lock.isHeldBy(currentThread));
  assert(ppn <= PageManager::instance()->getTotalNumPages());
  assert(ipt_[ppn]);

  for(auto pt_entry : ipt_[ppn]->process_info)
  {
    if(pt_entry.first == arch_mem && pt_entry.second == vpn)
    {
      debug(IPT_DEBUG, "ipt entry already exists: ppn: %p, vpn: %p\n", ppn, pt_entry.second);
      return false;
    }
  }
  
  ipt_[ppn]->process_info.insert(ustl::pair<ArchMemory*,size_t>(arch_mem, vpn));
  debug(IPT_DEBUG, "added ipt entry -> ppn: %p vpn: %p\n", (void*)ppn, (void*)vpn);
  ipt_[ppn]->swappable = swappable;
  ipt_[ppn]->secondChance = 1;
//  ipt_[ppn]->nfuHits = 0;
  ipt_[ppn]->age = 0x000000000000;

  return true; 
}

void IPT::deleteIPTEntry(size_t ppn, ArchMemory* arch_mem)
{

  assert(ipt_lock.isHeldBy(currentThread));
  assert(ppn <= PageManager::instance()->getTotalNumPages());
  assert(ipt_[ppn]);
  for(auto pt_entry : ipt_[ppn]->process_info)
  {
    if(pt_entry.first == arch_mem){

      ipt_[ppn]->process_info.erase(arch_mem);
      debug(IPT_DEBUG, "deleted ipt entry -> ppn: %p vpn: %p\n", (void*)ppn, (void*)pt_entry.second);
      //&& ipt_[ppn]->swappable
    }
  }
}
  
  
  
  


size_t IPT::getNumberOfProcessAccess(size_t ppn)
{
  assert(ipt_lock.isHeldBy(currentThread));
  return ipt_[ppn]->process_info.size();
}

IPTEntry *IPT::getIPTEntry(size_t ppn)
{
  assert(ipt_lock.isHeldBy(currentThread));
  assert(ppn < PageManager::instance()->getTotalNumPages());
  assert(ipt_[ppn]);
  


  return ipt_[ppn];
}

void IPT::setAllIPTEntriesToSwappable()
{
  ipt_lock.acquire();
  for(auto pt_entry : ipt_)
  {
    pt_entry.second->swappable = true;
  }


  ipt_lock.release();
}

void IPT::deleteAndFreeIPTEntry(size_t ppn, ArchMemory *arch_mem){

  assert(ipt_lock.isHeldBy(currentThread));
  assert(arch_mem->arch_memory_lock.isHeldBy(currentThread));
  assert(ppn <= PageManager::instance()->getTotalNumPages());
  assert(ipt_[ppn]);
  for(auto pt_entry : ipt_[ppn]->process_info)
  {
    if(pt_entry.first == arch_mem){

      ipt_[ppn]->process_info.erase(arch_mem);
      debug(IPT_DEBUG, "deleted and freed ipt entry -> ppn: %p vpn: %p arch: %p\n", (void*)ppn, (void*)pt_entry.second, pt_entry.first);
      //&& ipt_[ppn]->swappable
      //PageManager::instance()->freePPN(ppn);
      
      
    }
  }
  if(ipt_[ppn]->process_info.empty()){
        //ipt_[ppn]->swappable = false;
        PageManager::instance()->freePPN(ppn);
        //deleteAndFreeIPTEntry(ppn);
      }
}

void IPT::printIPT()
{
  if(ipt_.empty())
  {
    debug(IPT_TABLE, "Nothing to show - SPT is empty\n");
  }
  for(auto ipt_entry : ipt_)
  {
    if(!ipt_entry.second->process_info.empty())
    {
      for(auto info : ipt_entry.second->process_info)
      {
        debug(IPT_TABLE, "ppn: %p, swappable: %zu, vpn: %p, arch: %p\n", (void*)ipt_entry.first, ipt_entry.second->swappable, (void*)info.second, (void*)info.first);
      }
      
    }
    //debug(IPT_DEBUG, "IPT Process Info Size: %d\n", ipt_entry.second->process_info.size());
  }
  //debug(IPT_DEBUG, "IPT Size: %d\n", ipt_.size());
  
}

void IPT::clearIptEntriesByArchmem(ArchMemory *arch_mem)
{
   for(auto ipt_entry : ipt_)
    {      
        for(auto info : ipt_entry.second->process_info)
        {
            if(info.first == arch_mem)
            {
                ipt_entry.second->process_info.erase(info.first);
            }
        

        }
    
    }
    
}

