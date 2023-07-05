//
// Created by lifilius on 02.06.23.
//
#include "ustl/uvector.h"

#ifndef SWEB_PREALLOCATE_H
#define SWEB_PREALLOCATE_H


class PreAllocate
{
public:
      PreAllocate(size_t page_count);
      PreAllocate();
      ~PreAllocate();
    size_t getPage();
    void allocatePages(size_t page_count);
    void freeAllPreAllocatedPages();
    ustl::vector<size_t> preallocedPages;
};


#endif //SWEB_PREALLOCATE_H
