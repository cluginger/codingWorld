#include "pthread.h"
#include "stdio.h"
#include "assert.h"
#include "wait.h"

int main()
{
    pid_t pid = fork();
    if(pid == 0)
    {
        printf("still alive\n");
        sleep(2);
        printf("now dead\n");
        exit(100);
    }
    else if(pid != 0)
    {
        int status = 0;
        printf("start waiting\n");
        size_t retval = waitpid(-1, &status, NULL);
        printf("proceed\n");
        printf("%d %zu\n", status, retval);
    }
    else{
        printf("ERROR\n");
    }
    printf("SUCCESS");
    return 0;
}
