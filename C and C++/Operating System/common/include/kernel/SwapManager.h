#pragma once
#include "ArchMemory.h"
#include "Condition.h"
#include "ulist.h"
#include "uvector.h"
#include "IPT.h"
#include "BDVirtualDevice.h"
#include "Bitmap.h"
#include "BDManager.h"
#include "kstring.h"



enum SWAP_TYPE{
    SWAP_IN, SWAP_OUT
};
typedef struct
{
    SWAP_TYPE swap_type;
    ArchMemory *arch_mem;
    size_t vpn;
    //Mutex *swap_cond_lock;
    //Mutex *swap_cond_mutex;
    Condition *swap_cond;

    ssize_t offset;
    size_t next_ppn;
    bool should_not_be_swapped = false;
    
} SwapRequest;

typedef struct
{
    //size_t offset;
    ustl::map<ArchMemory*, size_t> process_info;
    size_t checksum;
}SPT_Entry;
//if implemented add shared/cow

class SwapManager
{
private:
    /* data */
    static SwapManager *instance_;
public:
    //we need something like a map to store the IPT entries
    static SwapManager *instance();
    SwapManager();
    
    
    Mutex swap_request_lock;
    Mutex swap_cond_mutex;

    //Condition* swap_condition;
    
    ustl::vector<SwapRequest*> swap_requests;
    ustl::map<size_t, SPT_Entry*> spt_;
    BDVirtualDevice* disk;
    Bitmap* bitmap;
    Mutex nopod_lock;
    size_t number_of_pages_on_disk = 0;
    size_t addSwapOutRequest(); 
    void deleteAndFreeSPTEntry(size_t offset, ArchMemory* arch_mem);
    void addSwapInRequest(size_t vpn, ArchMemory* arch_mem);
    void handleSwapRequests();
    size_t handleSwapOut();
    
    void handleSwapIn(SwapRequest* request);
    size_t getFreeBlock();
    void printSPT();
    void printUsedBlocksOnDisk();
    void clearSptEntriesByArchmem(ArchMemory *arch_mem);
    void cancelSameSwapInRequests(SwapRequest *request);

    void printArchmemMapping(size_t vpage, ArchMemory *archmem);
    void printSwapRequests();

    size_t counter_swap_out_cow = 0;
    size_t counter_swap_in_cow = 0;

    size_t getCounterSwapOutCow();
    size_t getCounterSwapInCow();



    //just for testing swap out and in a single page
    bool swap_test = false;
    ArchMemory* archmem_test = nullptr;
    size_t vpn_test = NULL;

    void swapOutAndSwapInRandomPage();
    

    
};



