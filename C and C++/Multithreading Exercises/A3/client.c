#include "client.h"
//DONT REMOVE CHECK FUNCTIONS

/*-----------------------------------------------------------------
 *  Main routine that loops and makes requests and eventually
 *  makes it public to the server. Stops when the operation equals
 *  zero. Initialize everything needed in here. All checks in this
 *  function must not be relocated.
 *
 *  @param argc number of arguments given
 *  @param argv char* that points to the argument
 *
 *  @return predefined value
 **/
int main(int argc, char **argv) {
  //local variables, add new ones if needed
  size_t command = 0;
  char buffer[2];
  //Initialize shared memory
  initSharedMemoriesClient();
  checkSHMClient();

  //Initialize memory mappings
  initMmappingsClient();
  checkMMAPClient();

  //Initialize semaphore locks
  initLocks();
  //Initialize server infrastructure
  initServer();
  checkServer();




  do {


    printf("Input command[0-3]: ");
    //TODO process commands and values
    //TODO synchronization
    //TODO assigning/receiving to/from shared memory

    int check_command = getCommand(buffer);
    if(check_command == 0)
    {
      printf("could not read from stdin\n");
    }
    int first_number = atoi(buffer);
    if(first_number == 0)
    {
      command = 0;
      mmaps.mapped_region_request_->operation = 0;
      sem_post(&mmaps.mapped_region_semaphores_->request_sem);

    }
    else if (first_number == FIBONACCI) {
      command = FIBONACCI;
      mmaps.mapped_region_request_->operation = FIBONACCI;
      char fibo[1000];
      int check_command = getCommand(fibo);
      if(check_command == 0)
      {
        printf("could not read from stdin\n");
      }
      size_t second_number = atoi(fibo);
      mmaps.mapped_region_request_->number = second_number;



      sem_post(&mmaps.mapped_region_semaphores_->request_sem);

}
    else if (first_number == MAXIMUM)
    {
      command = MAXIMUM;
      mmaps.mapped_region_request_->operation = MAXIMUM;
      sem_post(&mmaps.mapped_region_semaphores_->request_sem);

    }
    else if (first_number == MINIMUM)
    {
      command = MINIMUM;
      mmaps.mapped_region_request_->operation = MINIMUM;
      sem_post(&mmaps.mapped_region_semaphores_->request_sem);

    }
      sem_wait(&mmaps.mapped_region_semaphores_->response_sem);




    checkResults();
  } while (command != 0);
  closeMmappingsClient();
  checkCleanup();
  return 0;

}

/*-----------------------------------------------------------------
 *  This function initializes the server, as the server does not
 *  run at this point. Be sure everything is defined before when
 *  calling needed syscalls. Use the predefined variables for the
 *  process id.
 *
 *  @param none
 *
 *  @return none
 **/
void initServer() {
  //TODO launch server and load the right executable
  //hint: use checks for debugging
  process_id = fork();
  if((process_id == 0))
  {
    char *const paramList[]= {"./server", NULL};
    execv("./server", paramList);
    //kill(0, SIGTERM);
  }

}

/*-----------------------------------------------------------------
 * initializes your shared objects on the server and resizes them
 *
 * @param none
 *
 * @return none
 */
void initSharedMemoriesClient() {
  //TODO create and load all shared objects needed
  //according to the given structs
  fds.fd_shm_request_ = shm_open(SHM_NAME_REQUEST, FLAGS_SHM_READWRITE, MODERW);
  fds.fd_shm_response_ = shm_open(SHM_NAME_RESPONSE, FLAGS_SHM_READONLY, MODERW);
  fds.fd_shm_semaphores_ = shm_open(SHM_NAME_SEMAPHORES, FLAGS_SHM_READWRITE, MODERW);
  ftruncate(fds.fd_shm_request_, sizeof(shmrequest));
  ftruncate(fds.fd_shm_semaphores_, sizeof(shmlocks));
}
/*-----------------------------------------------------------------
 * maps the shared objects to the virtual memory space of the server
 *
 * @param none
 *
 * @return none
 */
void initMmappingsClient() {
  //TODO Initialize all mappings of the shared objects

  mmaps.mapped_region_request_ = mmap(NULL, sizeof(shmrequest), PROT_READ | PROT_WRITE,
                                       MAP_SHARED, fds.fd_shm_request_, 0);



  mmaps.mapped_region_response_ = mmap(NULL, sizeof(shmresponse), PROT_READ,
                                       MAP_SHARED, fds.fd_shm_response_, 0);


  mmaps.mapped_region_semaphores_ = mmap(NULL, sizeof(shmlocks), PROT_READ | PROT_WRITE,
                                       MAP_SHARED, fds.fd_shm_semaphores_, 0);

}
/*-----------------------------------------------------------------
 * initializes the locks on the shared object
 *
 * @param none
 *
 * @return none
 */
void initLocks() {
  //TODO Initialize all semaphores on the appropriate mapped
  //address space
  sem_init(&mmaps.mapped_region_semaphores_->request_sem, 1, 0);
  sem_init(&mmaps.mapped_region_semaphores_->response_sem, 1, 0);

}
/*-----------------------------------------------------------------
 * removes all mappings and shared objects as seen from the server
 * this part is an essential function for closing this application
 * accordingly without leaving artifacts on your system!
 *
 * @param none
 *
 * @return none
 */
void closeMmappingsClient() {
  //TODO Clean up all resources allocated and created before!
  sem_destroy(&mmaps.mapped_region_semaphores_->request_sem);
  sem_destroy(&mmaps.mapped_region_semaphores_->response_sem);

  munmap(mmaps.mapped_region_request_, sizeof(shmrequest));
  munmap(mmaps.mapped_region_response_, sizeof(shmrequest));
  munmap(mmaps.mapped_region_semaphores_, sizeof(shmrequest));

  shm_unlink(SHM_NAME_REQUEST);
  shm_unlink(SHM_NAME_RESPONSE);
  shm_unlink(SHM_NAME_SEMAPHORES);
  }
