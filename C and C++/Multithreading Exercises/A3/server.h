#ifndef ASS5_SERVER_H
#define ASS5_SERVER_H

#include "util.h"

/*
 * Initialization of global variables
 */
filedescriptors fds = { -1, -1, -1 };
mappings mmaps = { NULL, NULL, NULL };
int process_id = -1;

/*
 * Various headers for used functions in server.c
 */
void initSharedMemoriesServer();
void initMmappingsServer();
void closeMmappingsServer();
size_t dispatcher(shmrequest *request); //TODO
size_t fibonacci(size_t number);
ssize_t minimum();
ssize_t maximum();

#endif //ASS5_SERVER_H
