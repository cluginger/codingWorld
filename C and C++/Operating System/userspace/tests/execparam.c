#include <stdio.h>
#include <stdlib.h>


int main()
{
    char *argv[] = {"hi", "ho", "ha", "hu", "he"};
    char *argv1[] = {"hi", "ho", "ha", "hu", "he", NULL};
    char* first = "toto";
    char* too_long_path = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

    
    if(execv(NULL, argv) == -1)
    {
        printf("first paramcheck works\n");
    }

    if(execv(first, NULL) == -1)
    {
        printf("second paramcheck works\n");
    }

    if(execv(too_long_path, NULL) == -1)
    {
        printf("third paramcheck works\n");
    }

    if(execv(first, argv1) == -1)
    {
        printf("fourth paramcheck works\n");
    }


    return 0;
}