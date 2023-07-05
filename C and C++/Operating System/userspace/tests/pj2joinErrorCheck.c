#include "pthread.h"
#include "stdio.h"
#include "assert.h"

pthread_t tid;
pthread_t tid2;

void* function(void* arg) {
    return arg;
   

}
void* function2(void* arg) {
    //thread joined himself
    pthread_t* tid = (pthread_t*)arg;
    printf("function called\n");
    printf("sleep called\n");
    assert(pthread_join(*tid, NULL));
    
    return 0;
}

void* function3(void* arg) {
    //thread joined himself
    
    printf("function called\n");
    printf("sleep called\n");
    
    return 0;
}
int main()
{
    size_t args = 5;
    void* retval;
    assert(!pthread_create(&tid, NULL, function, (void*) args));
    assert(!pthread_join(tid, &retval));
    assert((size_t) retval == args);
    //we do double join here
    assert(pthread_join(tid, &retval) == -1);
    //wrong parameter
    assert(pthread_join(-1, NULL) == -1);
    assert(!pthread_create(&tid2, NULL, function, NULL));
    //NULL as retval
    assert(!pthread_join(tid2, NULL));
    pthread_t tid3;
    assert(!pthread_create(&tid3, NULL, function2, &tid3));
    assert(pthread_join(pthread_self(), NULL));

    pthread_t tid4;
    assert(!pthread_create(&tid4, NULL, function3, NULL));
    assert(!pthread_cancel(tid3));
    assert(!pthread_join(tid3, NULL));
    sleep(2);


    printf("Success\n");
    return 0;
}