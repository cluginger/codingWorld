#include "util.h"

/*-----------------------------------------------------------------
 * wrapper function for launching the obServer thread.
 *
 * @param none
 *
 * @return none
 */
void checkServer() {
  //TODO add your additional checks here
  if (process_id == -1)
    exit(-3);

}
/*-----------------------------------------------------------------
 * checks the read- and write capabilities of the shared objects
 * initialized before.
 * You can extend our provided tests here if you want.
 *
 * @param none
 *
 * @return none
 */
void checkSHMrw(int fd) {
  if (fd == -1) {
    exit(-1);
  }

}
/*-----------------------------------------------------------------
 * checks the read-only capabilities of the shared objects
 * initialized before.
 * You can extend our provided tests here if you want.
 *
 * @param none
 *
 * @return none
 */
void checkSHMro(int fd) {
  if (fd == -1) {
    exit(-1);
  }
}

/*-----------------------------------------------------------------
 * wrapper function for testing the shared objects
 *
 * @param none
 *
 * @return none
 */
void checkSHMClient() {

  checkSHMrw(fds.fd_shm_request_);
  checkSHMrw(fds.fd_shm_semaphores_);
  checkSHMro(fds.fd_shm_response_);
}
/*-----------------------------------------------------------------
 * predefined function for testing.
 * wrapper function for testing the mmap capabilities.
 * You can extend our provided tests here if you want.
 *
 * @param none
 *
 * @return none
 */
void checkMMAPClient() {
  //TODO add some custom checks if needed
  if (mmaps.mapped_region_response_ == NULL
      || mmaps.mapped_region_request_ == NULL
      || mmaps.mapped_region_semaphores_ == NULL) {
    exit(-1);
  }
}

/*-----------------------------------------------------------------
 * predefined function that will read all the data from the file
 * 'sorted'. It will convert all numbers into signed size_t values
 * and fill them into an array allocated on the heap.
 * Usage is highly recommended.
 *
 * @param none
 *
 * @return pointer to the heap-allocated array of numbers read before
 */
ssize_t* readFile() {

  ssize_t *numbers_read = (ssize_t*) malloc(sizeof(size_t) * MAX_SORTNUMBERS);
  memset(numbers_read, 0, sizeof(size_t) * MAX_SORTNUMBERS);
  char tmp_readc[20] = { [0 ... 19] = 0 };
  char current_char = '\0';
  FILE *fstream = fopen("sorting", "r+");
  int relative_index_read = 0;
  int numbers_index_read = 0;

  while (current_char != EOF) {
    current_char = fgetc(fstream);
    if (current_char == ';') {
      numbers_read[numbers_index_read++] = strtoll(tmp_readc, NULL, 10);
      memset(tmp_readc, 0, 20);
      relative_index_read = 0;
      continue;
    } else {
      tmp_readc[relative_index_read++] = current_char;
    }
  }

  if (fclose(fstream) == -1)
    exit(-1);

  return numbers_read;
}

/*-----------------------------------------------------------------
 * legacy function for writing to a file. This function can be used
 * for your custom tests.
 *
 * @param pointer to the heap-allocated array of numbers
 *
 * @return none
 */
void writeFile(size_t *sorted_array) {
  FILE *fstream = fopen("sorted", "wr+");

  for (int i = 0; i < MAX_SORTNUMBERS; i++)
    fprintf(fstream, "%ld;", sorted_array[i]);

  fclose(fstream);
}

/*-----------------------------------------------------------------
 * predefined function for testing. This is just a wrapper for testing
 * the specific results
 *
 * @param none
 *
 * @return none
 */
void checkResults() {
  //optional insert your own tests here if wanted
  //DO NOT remove this call.
}
/*-----------------------------------------------------------------
 * predefined function for testing. Checks if everything was cleaned
 * up properly in both processes.
 * You can extend our provided tests here if you want.
 *
 * @param none
 *
 * @return none
 */
void checkCleanup() {
  //TODO optionally add some more checks

  if (shm_open(SHM_NAME_REQUEST, O_RDONLY, MODERW) != -1) {
    exit(-1);

  }
  if (shm_open(SHM_NAME_RESPONSE, O_RDONLY, MODERW) != -1) {
    exit(-1);

  }
  if (shm_open(SHM_NAME_SEMAPHORES, O_RDONLY, MODERW) != -1) {
    exit(-1);
  }
}

/*-----------------------------------------------------------------
 * reads from stdin and processes the string entered for an easier
 * handling of commands.
 *
 * @param char pointer to the buffer that will be processed
 *
 * @return int true/false when successfully read from the stdin
 */
int getCommand(char *buffer) {
  memset(buffer, 0, 10 * sizeof(char));
  if (fgets(buffer, 10 * sizeof(char), stdin) != NULL)
    return 1;
  else
    return 0;
}

void checkSHMServer() {
  checkSHMrw(fds.fd_shm_response_);
  checkSHMro(fds.fd_shm_request_);
}

