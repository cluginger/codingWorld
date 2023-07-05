#include "pthread.h"
#include "stdio.h"
#include "unistd.h"
#include "wait.h"
#include "assert.h"

#define SIZE_THREADS 5
#define SIZE_PROCESS 10

void noend()
{
  while(1);
}


void func1()
{
    int i = 10000000;
    while(i > 0)
    {
        i--;
    }
    pthread_exit(0);
}

int main()
{
    pthread_t threads[SIZE_THREADS];

    for (size_t i = 0; i < SIZE_THREADS; i++)
    {
        assert(pthread_create(&threads[i], NULL, (void*)noend, NULL) == 0 && "thread creation failed\n");
    }


    

    for(int i = 0; i < SIZE_PROCESS; i++){
        
       
        pid_t pid = fork();

        pthread_t t1;
        assert(pthread_create(&t1, NULL, (void*)func1, NULL) == 0 && "thread creation failed\n");
        pthread_cancel(t1);
        pthread_join(t1, NULL);
        
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

    for(int i = 0; i < SIZE_THREADS; i++)
    {
        pthread_cancel(threads[i]);
    }



    
    printf("SUCCESS\n");
    return 0;
}
