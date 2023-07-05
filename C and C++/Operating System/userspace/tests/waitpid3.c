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
    }

    else{
        sleep(2);
        size_t ret = waitpid(pid, NULL, NULL);
        assert(ret == -1);
        printf("child already terminated\n");
        printf("SUCCESS\n");
    }
    return 0;
}
