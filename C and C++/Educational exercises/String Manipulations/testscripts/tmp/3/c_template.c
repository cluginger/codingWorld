#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <inttypes.h>

int main()
{
    char str[] = "";
    int (*reversePrint) (char*);
    void *handle = dlopen("../../../ass-b1.so",RTLD_LAZY);
    reversePrint = dlsym(handle,"reversePrint");
    printf("\n%d\n",(*reversePrint)(str));
    dlclose(handle);
    return 0;
}