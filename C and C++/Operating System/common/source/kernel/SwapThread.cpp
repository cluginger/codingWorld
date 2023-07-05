#include "SwapThread.h"
#include "Scheduler.h"

SwapThread::SwapThread() : Thread(NULL, "SwapThread", Thread::KERNEL_THREAD)
{
}

SwapThread::~SwapThread()
{
  assert(false && "SwapThread destruction means that you probably have accessed an invalid pointer somewhere.");
}

void SwapThread::kill()
{
  assert(false && "SwapThread destruction means that you probably have accessed an invalid pointer somewhere.");
}

void SwapThread::Run()
{
  while (1)
  {
    SwapManager::instance()->handleSwapRequests();
    //here comes a function where we go through a swapping request list
    //Scheduler::instance()->();
    Scheduler::instance()->yield();
  }
}

