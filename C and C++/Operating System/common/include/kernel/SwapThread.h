#pragma once

#include "Thread.h"
#include "SwapManager.h"

class SwapThread : public Thread
{
  public:
    SwapThread();
    virtual ~SwapThread();
    virtual void kill();
    virtual void Run();
};
