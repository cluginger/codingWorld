#include "pthread.h"
#include "stdio.h"
#include "assert.h"
#include "wait.h"

int main()
{
    pid_t pid = fork();
    if(pid < 0)
    {
        printf("ERROR\n");
        
    }
    else if(pid == 0)
    {
        sleep(2);
    }
    else{
        size_t ret = waitpid(pid, NULL, NULL);
        assert(ret == pid);
        printf("child is still alive\n");
        printf("SUCCESS\n");
    }
    return 0;
}
