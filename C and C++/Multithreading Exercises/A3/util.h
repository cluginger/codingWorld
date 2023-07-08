#ifndef ASS5_UTIL_H
#define ASS5_UTIL_H

#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

//Number of numbers that have to be read
#define MAX_SORTNUMBERS 100

/*
 * Use those defines for the name required
 * to open shared objects
 */
#define SHM_NAME_REQUEST "request"
#define SHM_NAME_RESPONSE "response"
#define SHM_NAME_SEMAPHORES "semaphores"

/*
 * The encoding of the operation used when
 * requesting
 */
#define FIBONACCI 1
#define MAXIMUM   2
#define MINIMUM   3

/*
 * Can be used for testing
 */
#define TEST_STR "DEAD"
#define LENGTH_TEST_STR 0x4

/*
 * Important and useful predefined values
 * for shared objects
 */
#define FLAGS_SHM_READONLY O_RDONLY|O_CREAT
#define FLAGS_SHM_READWRITE O_RDWR|O_CREAT
#define MODERW S_IRUSR|S_IWUSR

/*
 * file descriptors of your shared objects
 */
typedef struct {
  int fd_shm_request_;
  int fd_shm_response_;
  int fd_shm_semaphores_;
} filedescriptors;

/*
 * shared objects for your locks
 */
typedef struct {
  sem_t request_sem;
  sem_t response_sem;
} shmlocks;

/*
 * the response struct
 * can be extended if wanted
 */
typedef struct {
  size_t result;
} shmresponse;

/*
 * the request struct
 * can be extended if wanted
 */
typedef struct {
  size_t operation;
  size_t number;
} shmrequest;

/*
 *the according address pointing to the mapped region
 */
typedef struct {
  shmrequest *mapped_region_request_;
  shmresponse *mapped_region_response_;
  shmlocks *mapped_region_semaphores_;
} mappings;

/*
 * global variables.You MUST USE them!!!!
 */
extern filedescriptors fds;
extern mappings mmaps;
extern pid_t process_id;

/*
 * various function signatures used by the
 * util.c file
 */
void checkResults();
void checkSHMServer();
ssize_t* readFile();
void checkSHMClient();
void checkMMAPClient();
void checkServer();
void checkCleanup();
int getCommand(char *buffer);

#endif //ASS5_UTIL_H
