#include <stdio.h>
#include <pthread.h>

void* hello()
{
    printf("Hello World\n");
    return 0;
}

int main()
{
    pthread_t t1;


    int ret = pthread_create(NULL, NULL, &hello, NULL);

    printf("arg 1 == NULL -> ret should be -1 and is actually %d\n", ret);

    
    ret = pthread_create(&t1, NULL, NULL, NULL);

    printf("arg 3 == NULL -> ret should be -1 and is actually %d\n", ret);




    return 0;
}