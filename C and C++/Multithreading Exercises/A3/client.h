#ifndef ASS5_CLIENT_H
#define ASS5_CLIENT_H

#include "util.h"

/*
 * Initialization of global variables
 */
filedescriptors fds = { -1, -1, -1 };
mappings mmaps = { NULL, NULL, NULL };
pid_t process_id = -1;

/*
 * Various headers for used functions in server.c
 */
void initServer();
void initSharedMemoriesClient();
void initMmappingsClient();
void initLocks();
void closeMmappingsClient();

#endif //ASS5_CLIENT_H
