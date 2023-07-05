#include "pthread.h"
#include "stdio.h"
#include "assert.h"


void* function(void* arg) {
    pthread_t *tid2 = (pthread_t*) arg;

    printf("function called");
    sleep(2);

    assert(!pthread_join(*tid2, NULL));
    return 0;
   

}
void* function2(void* arg) {
    pthread_t *tid = (pthread_t*) arg;
    printf("function2 called\n");
    sleep(2);

    assert(pthread_join(*tid, NULL));
    
    return 0;
}

int main()
{
    pthread_t tid;
    pthread_t tid2;
    assert(!pthread_create(&tid, NULL, function, &tid2));
    assert(!pthread_create(&tid2, NULL, function2, &tid));
    sleep(4);

    printf("Success\n");
    return 0;
}