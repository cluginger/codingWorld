#include "SwapManager.h"
#include "new.h"
#include "debug.h"
#include "Scheduler.h"
#include "Thread.h"
#include "ArchMemory.h"
#include "PRA.h"


SwapManager swap_manager;
SwapManager* SwapManager::instance_ = 0;

SwapManager* SwapManager::instance()
{
  if(unlikely(!instance_))
    new(&swap_manager) SwapManager();
  return instance_;
}

SwapManager::SwapManager():
        swap_request_lock("SwapManager::swap_request_lock"),
        swap_cond_mutex("SwapManager::swap_cond_mutex"),
        nopod_lock("number_of_pages_on_disk_lock")
{
  assert(instance_ == 0);
  instance_ = this;
  disk = BDManager::getInstance()->getDeviceByNumber(3);
  disk->setBlockSize(PAGE_SIZE);
  bitmap = new Bitmap(disk->getNumBlocks());
}

size_t SwapManager::addSwapOutRequest()
{
  debug(SWAPOUT_REQUEST, "creating SWAP_OUT Request\n");
  Condition swap_cond(&swap_cond_mutex, "swap_cond");
  //Condition* swap_cond = new Condition(&swap_cond_mutex, "swap_cond");
  SwapRequest request{SWAP_OUT, nullptr,0,&swap_cond,0,0};
//    request.swap_type = SWAP_OUT;
//    request.arch_mem = nullptr;
//    request.vpn = 0;
//    request.swap_cond = &swap_cond;
  swap_request_lock.acquire();
  swap_requests.push_back(&request);

  debug(SWAPOUT_REQUEST, "next_ppn in addSwapOutRequest before wait: %p\n", (void*) request.next_ppn);
  swap_cond_mutex.acquire();
  swap_request_lock.release();


  debug(SWAPOUT_REQUEST, "now wait and release coming\n");
  request.swap_cond->waitAndRelease();
  debug(SWAPOUT_REQUEST, "next_ppn in addSwapOutRequest after wait:%zx\n", (void*)request.next_ppn);

  return request.next_ppn;
}

void SwapManager::addSwapInRequest(size_t vpn, ArchMemory* arch_mem)
{
  debug(SWAPIN_REQUEST, "SWAP IN vpn: %p\n", (void*)vpn);
  size_t ppn = PageManager::instance()->allocPPN();
  debug(SWAP, "ALLOCING in  %p\n", (void*)ppn);

  swap_request_lock.acquire();
  arch_mem->arch_memory_lock.acquire();
  for(auto swap_request: swap_requests)
  {
    if(swap_request->vpn == vpn && swap_request->arch_mem == arch_mem)
    {
      debug(SWAPIN_REQUEST, "SwapIN already swapped in vpn: %p\n", (void*)vpn);

      printArchmemMapping(swap_request->vpn, swap_request->arch_mem);

      PageManager::instance()->freePPN(ppn);
      //swap_cond_mutex.acquire();
      arch_mem->arch_memory_lock.release();
      swap_request_lock.release();
      Scheduler::instance()->yield();

      //swap_request->swap_cond->waitAndRelease();
      return;
    }
  }

  Condition swap_cond(&swap_cond_mutex, "swap_cond");


  SwapRequest request{SWAP_IN, (ArchMemory*)arch_mem, (size_t)vpn, &swap_cond,arch_mem->getOffset(vpn),ppn};
  if(request.offset == -1)
  {
    debug(SWAPIN_REQUEST, "offset is -1\n");
    debug(SWAPIN_REQUEST, "PPN with offset is -1: %p\n", ppn);
    debug(SWAPIN_REQUEST, "VPN with offset is -1: %p\n", vpn);
    printArchmemMapping(request.vpn, request.arch_mem);

    PageManager::instance()->freePPN(ppn);
    arch_mem->arch_memory_lock.release();
    swap_request_lock.release();
    return;
  }
  else
    debug(SWAPIN_REQUEST, "offset is NOT -1\n");
  arch_mem->arch_memory_lock.release();

  swap_requests.push_back(&request);
  debug(SWAPIN_REQUEST, "addSwapInRequest with ppn: %p\n", (void*)request.next_ppn);
  swap_cond_mutex.acquire();
  swap_request_lock.release();


  request.swap_cond->waitAndRelease();


  debug(SWAPIN_REQUEST, "next_ppn in addSwapInRequest after wait: %p\n", (void*)request.next_ppn);
}

void SwapManager::handleSwapRequests()
{

  while(!swap_requests.empty())
  {
    swap_request_lock.acquire();
    size_t num_of_req = swap_requests.size();
    auto request = swap_requests.back();
    swap_requests.pop_back();
    assert(swap_requests.size() == num_of_req - 1 && ustl::find(swap_requests.begin(),swap_requests.end(), request) == swap_requests.end());

    if(request->swap_type == SWAP_IN)
    {
      cancelSameSwapInRequests(request);
    }
    if(request->swap_type == SWAP_OUT && request->should_not_be_swapped == false)
    {
      debug(SWAP, "i am a swap out type\n");
      request->next_ppn = handleSwapOut();
      debug(SWAP, "Handle swaprequest: swapped out page with ppn: %p\n", request->next_ppn);
    }
    else if(request->swap_type == SWAP_IN && request->should_not_be_swapped == false)
    {
      handleSwapIn(request);
    }
    else
    {
      debug(SWAP, "No SwapIN because it is a duplicate request with vpn: %p \n", request->vpn);
    }
    swap_cond_mutex.acquire();
    request->swap_cond->broadcast();
    swap_cond_mutex.release();
    swap_request_lock.release();

  }

}

size_t SwapManager::handleSwapOut()
{
  IPT::instance()->ipt_lock.acquire();
  size_t ppn = PRA::instance()->startPRA();
  //IPT::instance()->printIPT();
  auto ipt = IPT::instance()->getIPTEntry(ppn);
  assert(!ipt->process_info.empty());
  for(auto [archmem, vpn] : ipt->process_info)
  {
    //assert(!info.first->arch_memory_lock.isHeldBy(currentThread));
    archmem->arch_memory_lock.acquire();
    debug(SWAPOUT_HANDLE, "VPN: %p -> ARCHMEM: %p", vpn, (void*)archmem);
  }
  debug(SWAPOUT_HANDLE, "ppn in handleSwapOut: %p\n", (void*)ppn);
  debug(SWAPOUT_HANDLE, "vpn in handleSwapOut: %p\n", (void*)ArchMemory::getIdentAddressOfPPN(ppn));

  size_t offset = getFreeBlock();
  spt_[offset] = new SPT_Entry;
  assert(ipt->swappable == true);
  ipt->swappable = false;
  assert(!ipt->process_info.empty());
  for(auto info : ipt->process_info)
  {
    ArchMemoryMapping m = info.first->resolveMapping(info.second);
    assert(!((m.pt[m.pti].present && m.pt[m.pti].swap) ||(m.pt[m.pti].cow && m.pt[m.pti].writeable)));
    //assert here
    debug(SWAPOUT_HANDLE, "SWAPOUT present: %u, swap: %u\n", m.pt[m.pti].present, m.pt[m.pti].swap);
    assert(m.pt);
    assert(m.pt[m.pti].present == 1);
    assert(m.pt[m.pti].swap == 0);
    m.pt[m.pti].present = 0;
    m.pt[m.pti].swap = 1;
    m.pt[m.pti].page_ppn = offset;

    if(m.pt[m.pti].cow == 1)
    {
      counter_swap_out_cow++;
    }
  }

  auto page = (uint32*)ArchMemory::getIdentAddressOfPPN(ppn);
  debug(SWAPOUT_HANDLE, "(Swapout)checksum on vpn: %p\n", page);
  spt_[offset]->checksum = checksum(page, PAGE_SIZE);
  debug(SWAPOUT_HANDLE, "SWAPOUT ppn: %p, swappable: %zu\n", ppn, ipt->swappable);
  assert(disk->writeData(offset * disk->getBlockSize(), disk->getBlockSize(), (char*) ArchMemory::getIdentAddressOfPPN(ppn)) != -1);
  nopod_lock.acquire();
  number_of_pages_on_disk++;
  nopod_lock.release();
  PRA::instance()->count_lock.acquire();
  PRA::instance()->outCount++;
  PRA::instance()->count_lock.release();

  //spt_.insert({offset, spt_entry});
  for(auto info : ipt->process_info)
  {
    //assert(!info.first->arch_memory_lock.isHeldBy(currentThread));
    info.first->arch_memory_lock.release();
  }
  assert(spt_.at(offset));
  assert(!ipt->process_info.empty());

  for(auto info : ipt->process_info)
  {
    if(swap_test == true)
    {
      archmem_test = info.first;
      vpn_test = info.second;
    }
    //assert(info.first->arch_memory_lock.isHeldBy(currentThread));
    //info.first->arch_memory_lock.release();
    spt_[offset]->process_info.insert(info);
    debug(SPT, "added spt entry -> ppn: %p vpn: %p\n", (void*)ppn, (void*)info.second);
  }
  ipt->process_info.clear();
  assert(spt_.at(offset));
  IPT::instance()->ipt_lock.release();
  return ppn;
}

void SwapManager::handleSwapIn(SwapRequest* request)
{
//    printArchmemMapping(request->vpn, request->arch_mem);
  IPT::instance()->ipt_lock.acquire();
  request->arch_mem->arch_memory_lock.acquire();
  assert(request->next_ppn);
  assert(request->vpn);

  size_t ppn = request->next_ppn;
  ssize_t offset = request->arch_mem->getOffset(request->vpn);
  //request->arch_mem->arch_memory_lock.release();
  if (offset == -1 || spt_.find(offset) == spt_.end())
  {
    debug(SWAPIN_HANDLE, "[SwapIn] no swapped out page in spt_ for this address found\n");
    //IPT::instance()->printIPT();
    //printSPT();
    debug(SWAPIN_HANDLE, "offset -1: ppn %p vpn: %p\n", (void*)ppn, (void*)request->vpn);
    //request->arch_mem->arch_memory_lock.acquire();
    printArchmemMapping(request->vpn, request->arch_mem);
    //request->arch_mem->arch_memory_lock.release();
    debug(SWAP, "ppn with offset -1: %p |vpn with offset -1: %p\n", (void*)ppn, (void*)request->vpn);
    PageManager::instance()->freePPN(ppn);
    request->arch_mem->arch_memory_lock.release();
    IPT::instance()->ipt_lock.release();
    return;
  }

  IPT::instance()->addIPTEntry(ppn, request->vpn, request->arch_mem, true);


  debug(SWAPIN_HANDLE, "spt size: %zu |offset %p |ppn: %p |vpn: %p |swappable %d\n", spt_.size(), (void*)offset, (void*)ppn, (void*)request->vpn, IPT::instance()->getIPTEntry(ppn)->swappable);
  assert(!IPT::instance()->getIPTEntry(ppn)->process_info.empty());

  debug(SWAPIN_HANDLE, "krass offset: %p\n", (void*)offset);
  assert(spt_.find(offset) != spt_.end());
  IPT::instance()->getIPTEntry(ppn)->process_info = spt_.at(offset)->process_info;

  //assert(!IPT::instance()->ipt_[ppn]->process_info.empty());
  //spt_.erase(offset);
  //assert(spt_.find(offset) == spt_.end());


  auto page = (uint32*)ArchMemory::getIdentAddressOfPPN(ppn);
  assert(disk->readData(offset * disk->getBlockSize(), disk->getBlockSize(), (char*) ArchMemory::getIdentAddressOfPPN(ppn)) !=-1);
  nopod_lock.acquire();
  number_of_pages_on_disk -= 1;
  nopod_lock.release();
  PRA::instance()->count_lock.acquire();
  PRA::instance()->inCount++;
  PRA::instance()->count_lock.release();
//    debug(SWAP, "Swapin checksum: %u, Swapout checksum: %ld\n",checksum(page, PAGE_SIZE) ,spt_.at(offset)->checksum);
//    debug(SWAPIN_HANDLE, "Swapin checksum: %u, Swapout checksum: %ld\n",checksum(page, PAGE_SIZE) ,spt_.at(offset)->checksum);


  assert(checksum(page, PAGE_SIZE) == spt_.at(offset)->checksum);
  assert(!IPT::instance()->getIPTEntry(ppn)->process_info.empty());
  request->arch_mem->arch_memory_lock.release();
  for(auto info : IPT::instance()->getIPTEntry(ppn)->process_info)
  {
    info.first->arch_memory_lock.acquire();
  }
  for(auto info : IPT::instance()->getIPTEntry(ppn)->process_info)
  {

    ArchMemoryMapping m = info.first->resolveMapping(info.second);
    assert(!((m.pt[m.pti].present && m.pt[m.pti].swap) ||(m.pt[m.pti].cow && m.pt[m.pti].writeable)));

    //assert(m.pt->page_ppn && m.pt[m.pti].page_ppn); //!!!!! keine Ahnung wieso des da rein kommt :)
//        debug(SWAPIN_HANDLE, "SWAPIN present: %zu, swap: %zu\n", m.pt[m.pti].present, m.pt[m.pti].swap);
    assert(m.pt);
    assert(m.pt[m.pti].swap);
    assert(!m.pt[m.pti].present);

    if(m.pt[m.pti].swap == 1)
    {
      m.pt[m.pti].page_ppn = ppn;
      m.pt[m.pti].swap = 0;
      m.pt[m.pti].present = 1;
    }
    if(m.pt[m.pti].cow == 1)
    {
      counter_swap_in_cow++;
    }
    debug(SPT, "deleted spt entry -> ppn: %p vpn: %p\n", (void*)ppn, (void*)info.second);
  }
  for(auto info : IPT::instance()->getIPTEntry(ppn)->process_info)
  {
    info.first->arch_memory_lock.release();
  }
  //info.first->arch_memory_lock.release();

  //bitmap->unsetBit(offset);

  /*for(auto spt_info : spt_.at(offset)->process_info)
  {
      if(spt_info.first == request->arch_mem)
      {
          spt_.at(offset)->process_info.erase(spt_info.first);
      }
  }*/


  /*if(spt_.at(offset)->process_info.empty())
  {
      delete spt_.at(offset);
  }*/

  delete spt_.at(offset);

  spt_.erase(offset);
  //debug(SPT, "deleted spt entry -> ppn: %p vpn: %p\n", (void*)ppn, (void*)spt_.at(offset)->process_info.second);
  //assert(spt_.find(offset) == spt_.end());

  //request->arch_mem->arch_memory_lock.release();
  IPT::instance()->ipt_lock.release();

}
size_t SwapManager::getFreeBlock()
{
  for(size_t i = 0; i < disk->getNumBlocks(); i++)
  {
    if(bitmap->getBit(i) == 0)
    {
      bitmap->setBit(i);
      return i;
    }
  }
  assert(false);
}

void SwapManager::swapOutAndSwapInRandomPage()
{
  swap_test = true;

  addSwapOutRequest();
  addSwapInRequest(vpn_test, archmem_test);

  archmem_test = nullptr;
  vpn_test = NULL;
  swap_test = false;


}

void SwapManager::deleteAndFreeSPTEntry(size_t offset, ArchMemory* arch_mem)
{
  assert(spt_.at(offset));
  for(auto spt_entry : spt_.at(offset)->process_info)
  {
    //debug(SPT, "spt_size in destructor archmem: %d\n", spt_.size());
    if(spt_entry.first == arch_mem){

      spt_.at(offset)->process_info.erase(arch_mem);
      debug(SPT, "deleted and freed spt entry -> ppn: %p vpn: %p arch: %p\n", (void*)offset, (void*)spt_entry.second, spt_entry.first);

    }
  }
  if(spt_.at(offset)->process_info.empty()){


    delete spt_.at(offset);
    spt_.erase(offset);

  }
}
void SwapManager::printSPT()
{
  if(spt_.empty())
  {
    debug(SPT_TABLE, "Nothing to show - SPT is empty\n");
  }
  for(auto spt_entry : spt_)
  {
    if(!spt_entry.second->process_info.empty())
    {
      for(auto info : spt_entry.second->process_info)
      {
        debug(SPT_TABLE, "offset: %p, vpn: %p, arch: %p\n", spt_entry.first, info.second, info.first);
      }
    }

    //debug(SPT, "SPT Process Info Size: %d\n", spt_entry.second->process_info.size());
  }
  //debug(SPT, "SPT Size: %d\n", spt_.size());
}

void SwapManager::clearSptEntriesByArchmem(ArchMemory *arch_mem)
{
  for(auto spt_entry : spt_)
  {
    if(!spt_entry.second->process_info.empty())
    {
      for(auto info : spt_entry.second->process_info)
      {
        if(info.first == arch_mem)
        {
          deleteAndFreeSPTEntry(spt_entry.first, arch_mem);
          //spt_.at(spt_entry.first)->process_info.erase(arch_mem);
        }


      }

    }

  }
}

void SwapManager::cancelSameSwapInRequests(SwapRequest *request)
{
  assert(swap_request_lock.isHeldBy(currentThread));

  for (size_t i = 0; i < swap_requests.size(); ++i)
  {
    assert(swap_requests.at(i) != nullptr);
    if(swap_requests.at(i)->vpn == request->vpn && swap_requests.at(i)->arch_mem == request->arch_mem)
    {
      debug(SWAP, "We found the dublicate with vpn: %p and mark it as should_not_be_swapped\n", (void*)swap_requests.at(i)->vpn);
      swap_requests.at(i)->should_not_be_swapped = true;
    }
  }
}

void SwapManager::printArchmemMapping(size_t vpage, ArchMemory *archmem)
{
  ArchMemoryMapping m = archmem->resolveMapping(vpage);
  size_t page = m.pt[m.pti].page_ppn;
  debug(ARCHMEM_MAPPING, "print ArchMemory Mapping for vpn: %p |"
                         "page: %p |"
                         "present: %d |"
                         "writable: %d |"
                         "swap %d |"
                         "cow %d |"
                         "dirty %d\n"
  ,(void*)vpage, (void*)page, m.pt[m.pti].present, m.pt[m.pti].writeable, m.pt[m.pti].swap, m.pt[m.pti].cow, m.pt[m.pti].dirty);
//  assert(false && "we should not have entered here\n");
}

size_t SwapManager::getCounterSwapOutCow()
{
  return counter_swap_out_cow;
}

size_t SwapManager::getCounterSwapInCow()
{
  return counter_swap_in_cow;
}

void SwapManager::printUsedBlocksOnDisk()
{
  bitmap->bmprint();
}