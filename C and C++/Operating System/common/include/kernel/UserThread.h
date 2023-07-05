#pragma once

#include "Thread.h"
#include "UserProcess.h"
#include "../../../userspace/libc/include/defs.h"
#include "Mutex.h"

#define NUM_PAGES 32

class UserProcess;

class UserThread : public Thread
{
  public:
    
    UserThread(ustl::string filename, FileSystemInfo *fs_info, UserProcess* parent_process , size_t entry_function, size_t start_routine, size_t arg);
    UserThread(UserThread &user_thread, UserProcess* parent_process);
    virtual ~UserThread();

    virtual void Run(); // not used

    UserProcess *parent_process_;

    //should get locked but somehow i cant make Mutexes in this class
    void setThreadExitStatus();
    bool getThreadExitStatus();

    void setRetVal(void* retval);
    void* getRetVal();

    bool isUthreadlocked();
    //void execvOverwriteUserThread(Loader* execv_loader);

    size_t cancel_state_, cancel_type_, join_state_;
    bool cancel_flag_ = false;

    Mutex thread_exit_status_lock;
    Mutex joining_thread_lock;
    Mutex cancel_type_lock, cancel_state_lock, join_state_lock;
    size_t phys_page_for_stack;
    size_t virt_page_for_stack;

    size_t start_of_stack;
    size_t end_of_stack;
    Thread* joining_thread = 0;


    size_t* _tid;
    size_t* _mutex;
    size_t* _locked;
    size_t* _condvar;

  private:
    bool exit_is_called = false;
    void *ret_val_pthreadexit = NULL;
};