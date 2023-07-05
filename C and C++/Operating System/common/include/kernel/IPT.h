#pragma once

#include "Mutex.h"
#include "umap.h"

class ArchMemory;

typedef struct IPTEntry
{
  ustl::map<ArchMemory*, size_t> process_info;// ArchMem* to vpn
  bool swappable = false;
  size_t age = 0x000000000000;
  size_t secondChance = 1;
  bool isdirty = false;
} IPTEntry;

class IPT
{
    public:

    virtual ~IPT();
    static IPT *instance();
    ustl::map<size_t, IPTEntry*> ipt_; //ppn to IPT

    bool addIPTEntry(size_t ppn, size_t vpn, ArchMemory* arch_mem, bool swappable);
    void deleteIPTEntry(size_t ppn, ArchMemory* arch_mem);
    void deleteAndFreeIPTEntry(size_t ppn, ArchMemory *arch_mem);
    size_t getNumberOfProcessAccess(size_t ppn);
    IPTEntry* getIPTEntry(size_t ppn);
    void printIPT();

    void clearIptEntriesByArchmem(ArchMemory *arch_mem);


    //just for randomPRA test!!!
    void setAllIPTEntriesToSwappable();

    Mutex ipt_lock;

    private:

    IPT();
    static IPT *instance_;
};