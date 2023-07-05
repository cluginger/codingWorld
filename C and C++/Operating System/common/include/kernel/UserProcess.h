#pragma once

#include "UserThread.h"
#include "Condition.h"
#include "umap.h"
#include "Mutex.h"
#include "ScopeLock.h"

#define START_INDEX_FIRST_THREAD 0
#define HEAP_END 1073741824 //1GB
//1 000 000 000 bytes (decimal) or 1073741824 bytes (binary)

class UserThread;

class UserProcess
{
  public:
    /**
     * Constructor
     * @param minixfs_filename filename of the file in minixfs to execute
     * @param fs_info filesysteminfo-object to be used
     * @param terminal_number the terminal to run in (default 0)
     *
     */
    UserProcess(ustl::string minixfs_filename, FileSystemInfo *fs_info, uint32 terminal_number = 0);

    //UserThread* getFirstThread(); == thread with TID = 0

    void pushDeadThreadToDeadMap(UserThread *dead_thread_to_add);

    virtual ~UserProcess();

    size_t getAndEnhanceTIDCounter();

    bool checkIfAllThreadsAreDead();

    void exit_(size_t exit_code);

    void setExitStatus(bool status);
    bool getExitStatus();

    void* getRetvalByID(size_t tid);
    size_t getNumOfExistingThreads();

    void eraseThreadFromMap(size_t tid);
    size_t getPID();
    virtual void Run(); // not used
    size_t cancelThread(size_t tid);
    size_t joinThread(size_t tid, void **ret_val);
    size_t detachThread(size_t tid);

    size_t fork();

    void setFD(int32_t fd);
    int32_t getFD();

    //size_t execv(char* path, char* argv[]);

    size_t heapStart, heapEndCurrent, heapEndTotal;
    Loader *process_loader;
    ustl::string filename_;
    FileSystemInfo *fs_info_;
    uint32 terminal_number_;

    Mutex heap_lock;
    Mutex exit_condition_lock;
    Mutex join_condition_lock;
    Mutex wake_from_sleep_lock;
    Mutex TID_lock;
    Mutex threadMap_lock;
    Mutex deadThreadMap_lock;
    Mutex retval_lock;
    Mutex number_existing_threads_lock;
    Mutex exit_status_lock;
    Mutex retvalMap_lock;

    Condition exit_condition;
    Condition join_condition;
    Condition wake_from_sleep_cond;
    //both need to be locked with threads_of_process
    ustl::map <size_t, UserThread*> threadMap;
    ustl::map <size_t, UserThread*> deadThreadMap;
    ustl::map <size_t, size_t> waiters_list_id;
    uint64 processTime = 0;

    bool exit_status = false;
    size_t exit_value;
    ustl::map <size_t, void*> retvalMap;
    // threadMap deadThreadMap
    size_t number_existing_threads = 0;


    //UserProcess constructor fork 
    UserProcess(const UserProcess &calling_process, UserThread &thread);

private:
    int32 fd_;
    size_t pid_;
    size_t tid_counter = START_INDEX_FIRST_THREAD;
    //everytime a threads gets pushed or erased form threads_of_process it will get enhanced/decreased !

    //for execv we dont want his variable to be true -> we wanna "override the main thread"

  
};

