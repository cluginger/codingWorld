#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


int main()
{
    char *path = "/usr/f1forkBasic.sweb";

    if(execv(path, NULL) != 0)
    {
        printf("execv doesnt work\n");
    }
    printf("this part should never be accessed\n");
    return 0;
}