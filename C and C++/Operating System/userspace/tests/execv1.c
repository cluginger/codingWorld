#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


int main()
{

    char *args[] = {"endprocess.c", (char*)NULL};

    char *path = "/usr/endprocess.sweb";

    if(execv(path, args) != 0)
    {
        printf("execv doesnt work\n");
    }
    printf("this part should never be accessed\n");
    return 0;
}