#include "stdio.h"
#include "pthread.h"
#include "assert.h"

void* f(void* arg)
{
    sleep(2);
    return 0;
}
int main()
{
    pthread_t thread;

    assert(!pthread_create(&thread, NULL, &f, NULL));
    assert(!pthread_detach(thread));
    assert(pthread_join(thread, NULL));
    sleep(2);
    printf("Success");
    return 0;

}