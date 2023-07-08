#include "server.h"
//DONT REMOVE CHECK FUNCTIONS

/*-----------------------------------------------------------------
 *  Main routine that loops and forwards requests and eventually
 *  makes it public to the client. stops when the operation equals
 *  zero. Initialize everything needed in here.
 *
 *  @param argc number of arguments given
 *  @param argv char* that points to the argument
 *
 *  @return predefined value
 **/
int main(int argc, char **argv)
{
  printf("\n[SERVER]STARTED SERVER \n");


  //Initialize shared memory objects here
  initSharedMemoriesServer();
  checkSHMServer();
  //Initialize mappings to the virtual address space
  initMmappingsServer();


  do
  {

    //TODO add synchronization
    sem_wait(&mmaps.mapped_region_semaphores_->request_sem);
    if(mmaps.mapped_region_request_->operation != 0)
    {
        mmaps.mapped_region_response_->result = dispatcher(
        mmaps.mapped_region_request_);
      }



    sem_post(&mmaps.mapped_region_semaphores_->response_sem);

  } while (mmaps.mapped_region_request_->operation != 0);

  closeMmappingsServer();
  return -3;
}
/*-----------------------------------------------------------------
 * initializes your shared objects on the server and resizes them
 *
 * @param none
 *
 * @return none
 */
void initSharedMemoriesServer()
{
  //TODO Open your shared resources made in the client.
  fds.fd_shm_response_ = shm_open(SHM_NAME_RESPONSE, FLAGS_SHM_READWRITE, MODERW);
  fds.fd_shm_request_ = shm_open(SHM_NAME_REQUEST, FLAGS_SHM_READONLY, MODERW);
  fds.fd_shm_semaphores_ = shm_open(SHM_NAME_SEMAPHORES, FLAGS_SHM_READWRITE, MODERW);
  ftruncate(fds.fd_shm_response_, sizeof(shmrequest));
  ftruncate(fds.fd_shm_semaphores_, sizeof(shmlocks));
}
/*-----------------------------------------------------------------
 * maps the shared objects to the virtual memory space of the server
 *
 * @param none
 *
 * @return none
 */
void initMmappingsServer()
{
  //TODO Map all shared resources opened in this process to the
  //virtual address space of this process

  mmaps.mapped_region_response_ = mmap(NULL, sizeof(shmrequest), PROT_READ | PROT_WRITE,
                                       MAP_SHARED, fds.fd_shm_response_, 0);



  mmaps.mapped_region_request_ = mmap(NULL, sizeof(shmresponse), PROT_READ,
                                       MAP_SHARED, fds.fd_shm_request_, 0);


  mmaps.mapped_region_semaphores_ = mmap(NULL, sizeof(shmlocks), PROT_READ | PROT_WRITE,
                                       MAP_SHARED, fds.fd_shm_semaphores_, 0);
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
void closeMmappingsServer()
{
  //TODO Clean up all resources opened and created before
  munmap(mmaps.mapped_region_request_, sizeof(shmrequest));
  munmap(mmaps.mapped_region_response_, sizeof(shmrequest));
  munmap(mmaps.mapped_region_semaphores_, sizeof(shmrequest));

  shm_unlink(SHM_NAME_REQUEST);
  shm_unlink(SHM_NAME_RESPONSE);
  shm_unlink(SHM_NAME_SEMAPHORES);
}
/*-----------------------------------------------------------------
 * dispatches the operation from the request/client
 *
 * @param shmrequest the pointer to the request object
 *
 * @return size_t the result of the function
 */
size_t dispatcher(shmrequest *request)
{
  printf("[SERVER]DISPATCHER: %ld\n", request->operation);

  //TODO dispatch the operation to the respective function
  //provided. Use the defines in the header file.

  size_t result;

  if(request->operation == FIBONACCI)
  {
    result = fibonacci(request->number);
    return result;
  }
  else if(request->operation == MAXIMUM)
  {
    result = maximum();
    return result;
  }
  else if(request->operation == MINIMUM)
  {
    result = minimum();
    return result;
  }
  else
  {
    return 0;
  }



}
/*-----------------------------------------------------------------
 * calculates the fibonacci sequence
 *
 * @param size_t number of steps for the calculation
 *
 * @return size_t the final result of the calculation
 */
size_t fibonacci(size_t number)
{
  //TODO Implement the calculation of the fibonacci sequence.
  size_t first_element = 0;
  size_t store_first;
  size_t next_element = 1;

  size_t final_result;

  if(number == 1 || number == 0)
  {
    final_result = 0;
    return final_result;
  }
  if(number == 2)
  {
    final_result = 1;
    return final_result;
  }

  for(int i = 2; i < number; i++)
  {

      final_result = first_element + next_element;

      store_first = first_element;
      first_element = next_element;
      next_element += store_first;

  }

  return final_result ;
}
/*-----------------------------------------------------------------
 * finds the maximum of the data given in the sorting file
 * hint: use the limits.h header
 *
 * @param none
 *
 * @return the maximum value of the sequence
 */
ssize_t maximum()
{
  //TODO Search for the highest number in the heap-allocated array
  ssize_t max;
  ssize_t* sorted_array = readFile();
  max = *sorted_array;

  for(int i = 0; i < MAX_SORTNUMBERS; i++)
  {
    if(sorted_array[i] > max)
    {
      max = sorted_array[i];
    }
  }

  return max;
}
/*-----------------------------------------------------------------
 * finds the minimum of the data given in the sorting file
 * hint: use the limits.h header
 *
 * @param none
 *
 * @return the maximum value of the sequence
 */
ssize_t minimum()
{
  //TODO Search for the lowest number in the heap-allocated array
  ssize_t min;
  ssize_t* sorted_array = readFile();
  min = *sorted_array;

  for(int i = 0; i < MAX_SORTNUMBERS; i++)
  {
    if(sorted_array[i] < min)
    {
      min = sorted_array[i];
    }
  }

  return min;

}
