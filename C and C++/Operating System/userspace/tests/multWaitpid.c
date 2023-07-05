#include "pthread.h"
#include "stdio.h"
#include "assert.h"
#include "wait.h"

int main()
{
    for(int i = 0; i < 5; i++){
        pid_t pid = fork();
        if(pid == 0)
        {
            printf("still alive\n");
            sleep(1);
            printf("now dead\n");
            exit(-20);
        }
        else if(pid != 0)
        {
            int status = 0;
            printf("start waiting\n");
            size_t retval = waitpid(pid, &status, NULL);
            printf("proceed\n");
        }
        else{
            printf("ERROR\n");
        }
    }
    
    printf("SUCCESS\n");
    return 0;
}