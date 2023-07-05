#include "PRA.h"
#include "Scheduler.h"
#include "debug.h"
#include "IPT.h"


PRA *PRA::instance_ = nullptr;
PRA *PRA::instance()
{
    if(instance_ == nullptr){
        instance_ = new PRA();
    }
    return instance_;
}
size_t PRA::startPRA()
{
  size_t page = 0;
  switch(active_pra)
  {
    case PRA_::RANDOM:
      debug(PRAs, "starting swap with RANDOM\n");
      page = randomPRA();
      count_lock.acquire();
      randomCount++;
      count_lock.release();
      break;
    case PRA_::AGING:
      debug(PRAs, "starting swap with AGING\n");
      page = agingPRA();
      count_lock.acquire();
      agingCount++;
      count_lock.release();
      break;
//    case PRA_::NFU:
//      debug(PRAs, "starting swap with NFU\n");
//      page = nfuPRA();
//      count_lock.acquire();
//      nfuCount++;
//      count_lock.release();
//      break;
    case PRA_::SECONDCHANCE:
      debug(PRAs, "starting swap with NFU\n");
      page = secondChancePRA();
      count_lock.acquire();
      secCount++;
      count_lock.release();
      break;
    default:
      break;
  }
  return page;
}

size_t PRA::agingPRA()
{
  size_t la_page = IPT::instance()->ipt_.begin()->first;
  IPTEntry* ipt_entry = IPT::instance()->getIPTEntry(la_page);
  while(ipt_entry->process_info.empty() || ipt_entry->swappable == false)
  {
    la_page = (la_page + 1);
    if(la_page >= max_Page_range -1)
      la_page -= max_Page_range -1;
    ipt_entry = IPT::instance()->getIPTEntry(la_page);
  }
  size_t lowest_age = ipt_entry->age;

  for(auto entry: IPT::instance()->ipt_)
  {
//    debug(PRAsAGING,"%ld %p(current) < %p(lowest)?\n" ,!entry.second->process_info.empty(), (void*)entry.second->age, (void*)lowest_age);
    debug(PRAsAGING, "age1 -> %ld age2-> %ld\n", entry.second->age , lowest_age);
    if(!entry.second->process_info.empty() && entry.second->swappable && entry.second->age < lowest_age)
    {
      debug(PRAsAGING,"-> we found a better page: from %p to %p\n" ,lowest_age, entry.second->age);
      lowest_age = entry.second->age;
      la_page = entry.first;
    }
  }

//  debug(PRAsAGING, "The aging number is %p \n", (void*)la_page);

  return la_page;
}

//size_t PRA::nfuPRA()
//{
//  size_t lac_page = IPT::instance()->ipt_.begin()->first;
//  IPTEntry* ipt_entry = IPT::instance()->getIPTEntry(lac_page);
//  while(ipt_entry->process_info.empty() || ipt_entry->swappable == false)
//  {
//    lac_page = (lac_page + 1);
//    if(lac_page >= max_Page_range -1)
//      lac_page -= max_Page_range -1;
//    ipt_entry = IPT::instance()->getIPTEntry(lac_page);
//  }
//  size_t lowest_access_count = ipt_entry->nfuHits;
//  for(auto entry: IPT::instance()->ipt_)
//  {
//    if(!entry.second->process_info.empty() && entry.second->swappable && (entry.second->nfuHits == 0))
//    {
//      lowest_access_count = entry.second->nfuHits;
//      lac_page = entry.first;
//      break;
//    }
////    debug(PRAsNFU,"%ld %ld < %ld?\n" ,!entry.second->process_info.empty(), lowest_access_count, entry.second->nfuHits);
//    if(!entry.second->process_info.empty() && entry.second->swappable && (entry.second->nfuHits < lowest_access_count))
//    {
//        debug(PRAsNFU,"%ld %ld < %ld?\n" ,!entry.second->process_info.empty(), lowest_access_count, entry.second->nfuHits);
//        debug(PRAsNFU,"-> we found a worse page: from %ld to %ld\n" ,lowest_access_count, entry.second->nfuHits);
//        debug(PRAsNFU, "-> The nfu number is %p \n", (void*)lac_page);
//        lowest_access_count = entry.second->nfuHits;
//        lac_page = entry.first;
//    }
//  }
//
//  return lac_page;
//}

size_t PRA::secondChancePRA()
{
//  size_t sec_page = IPT::instance()->ipt_.begin()->first;
//  IPTEntry* ipt_entry = IPT::instance()->getIPTEntry(sec_page);y
  debug(PRAsSEC, "LOL\n");
  for(auto entry : IPT::instance()->ipt_)
  {
    if(!entry.second->process_info.empty()  && entry.second->swappable)
    {
      if(entry.second->secondChance)
      {
        entry.second->secondChance = 0;
        debug(PRAsSEC, "secondChance 1\n");
      }
      else
      {
        debug(PRAsSEC, "secondChance 0\n");
        return entry.first;
      }
    }
  }
  debug(PRAsSEC, "WTF\n");
  return randomPRA();

}

size_t PRA::randomPRA()
{
    //IPT::instance()->ipt_lock.acquire();
    // source: https://en.wikipedia.org/wiki/Xorshift [5th June 2023 12:31]

    size_t random_seed = Scheduler::instance()->getRandomSeed();
    debug(PRAs, "The random seed is %zu \n", random_seed);

    size_t random_ppn = random_seed % max_Page_range;

    debug(PRAsRANDOM, "The random PPN is %p \n", (void*)random_ppn);

    IPTEntry* ipt_entry = IPT::instance()->getIPTEntry(random_ppn);
//    debug(SWAP,"ADAMA2\n");
    while(ipt_entry->process_info.empty() || ipt_entry->swappable == false)
    {
        random_ppn = (random_ppn + 1);
        if(random_ppn >= max_Page_range -1)
        {
            random_ppn -= max_Page_range -1;
        }

        ipt_entry = IPT::instance()->getIPTEntry(random_ppn);
    }
  debug(PRAsRANDOM, "The NEW random PPN is %p \n", (void*)random_ppn);
    //IPT::instance()->ipt_lock.release();

    if(pra_random_sample[random_ppn] == NULL)
    {
      pra_random_sample[random_ppn] = 1;
    }
    else
    {
      pra_random_sample[random_ppn] += 1;
    }
        debug(PRAsRANDOM, "The Random number is %p and the max_Page_range is %zu\n", (void*)random_ppn, max_Page_range);
    return random_ppn;
}

void PRA::printRandomSample()
{
  for(size_t i = 0; i < max_Page_range; i++)
  {
    kprintf("pIPT::instance()->ipt_lockpn %zu got swapped out %zu\n", i, pra_random_sample[i]);
    debug(PRAs, "ppn %zu got swapped out %zu\n", i, pra_random_sample[i]);
  }
}

size_t PRA::getMeanOfPRA()
{
    size_t mean = 0;
    for(size_t element: pra_random_sample)
    {
        mean += element;
    }
    //mean = mean/pra_random_sample.size();
    debug(PRAs,"The mean is %zu\n", mean);

    randomPRA_mean = mean;

    return mean;

}

void PRA::switchCurrentPRA()
{
  size_t next_pra = ++active_pra > PRA_::SECONDCHANCE ? 0 : active_pra;
  count_lock.acquire();
  switch(next_pra)
  {
    case PRA_::RANDOM:
      kprintf("Switched to RANDOM PRA - pages swapped so far: %ld\nSWEB: />", randomCount);
      active_pra = PRA_::RANDOM;
      break;
//    case PRA_::NFU:
//      kprintf("Switched to NFU PRA -    pages swapped so far: %ld\nSWEB: />", nfuCount);
//      active_pra = PRA_::NFU;
//      break;
    case PRA_::AGING:
      kprintf("Switched to AGING PRA -  pages swapped so far: %ld\nSWEB: />", agingCount);
      active_pra = PRA_::AGING;
      break;
    case PRA_::SECONDCHANCE:
      kprintf("Switched to SC PRA -     pages swapped so far: %ld\nSWEB: />", secCount);
      active_pra = PRA_::SECONDCHANCE;
      break;
    default:
      break;
  }
  count_lock.release();
}

/*double PRA::getSTDDeviationOfPRA()
{
    double squared_diff = 0.0;
    for(size_t element: pra_random_sample)
    {
        double diff = element - randomPRA_mean;
        squared_diff += diff * diff;
    }
    //double var = std::sqrt
    return squared_diff;
}
*/

void PRA::printPRAinfo()
{
  SwapManager::instance()->nopod_lock.acquire();
  count_lock.acquire();
  kprintf("PRA INFO\n"
          "Pages Swapped - RANDOM: %ld\n"
          /*"Pages Swapped -    NFU: %ld\n"*/
          "Pages Swapped -     SC: %ld\n"
          "Pages Swapped -  AGING: %ld\n"
          "Pages Swapped - OUT: %ld\n"
          "Pages Swapped - IN : %ld\n"
          "Pages on Disk: %ld\n"
          "SWEB: />", randomCount,secCount,agingCount, outCount, inCount, SwapManager::instance()->number_of_pages_on_disk);
  count_lock.release();
  SwapManager::instance()->nopod_lock.release();
}

PRA::PRA() : count_lock("count_lock")
{

}
PRA::~PRA()
{

}