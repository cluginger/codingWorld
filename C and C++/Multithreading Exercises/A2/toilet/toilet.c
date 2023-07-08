#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "definitions.h"
#include "resources.h"
#include "vector.h"

int washing_basins[NUM_WASHING_BASINS];
vector free_toilets;
State public_bathroom_state;
volatile int running = 1;

/*
 * STUDENT TODO BEGIN:
 *  Global locks, semaphores and condition variables
 */
 pthread_cond_t conOpen, conOpen2;
 pthread_mutex_t mutex_open, mutex_1, mutex_2, mutex_3, mutex_4, mutex_5;

 sem_t washing_hands;
 sem_t go_to_toilet;

/*
 * STUDENT TODO END:
 */

/*
 * STUDENT TODO:
 *  signal all customers that the public bathroom has opened
 *  Could a condition variable help?
 */
void* doJanitor(void* param)
{
  printf("Janitor is opening public bathroom for customers...\n");
  openPublicBathroom();
  printf("Janitor just opened public bathroom for customers...\n");

  pthread_mutex_lock(&mutex_5);
  public_bathroom_state = OPENED;
  pthread_mutex_unlock(&mutex_5);

  pthread_mutex_lock(&mutex_open);
  pthread_cond_broadcast(&conOpen);
  pthread_mutex_unlock(&mutex_open);

  coffeeBreak();

  printf("Janitor is calling in cleaning staff!\n");

  pthread_mutex_lock(&mutex_5);
  public_bathroom_state = OPENED_AND_CLEANING_STAFF_NEEDED;
  pthread_mutex_unlock(&mutex_5);

  pthread_mutex_lock(&mutex_open);
  pthread_cond_broadcast(&conOpen2);
  pthread_mutex_unlock(&mutex_open);

  return NULL;
}

/* STUDENT TODO:
 *  make sure possible shared resources are locked correctly
 *  How do you make sure that every customer can get to a washing basin?
 */
void useWashingBasin(Customer* customer)
{
  sem_wait(&washing_hands);
  customWait(3);
  printf("Customer %zd goes to washing basins...\n", customer->customer_number);

  pthread_mutex_lock(&mutex_3);
  int i = 0;
  while (washing_basins[i] != 0)
  {
    i++;
  }
  washing_basins[i] = 1;
  pthread_mutex_unlock(&mutex_3);

  printf("Customer %zd washes hands thoroughly...\n", customer->customer_number);
  customWait(9);

  pthread_mutex_lock(&mutex_3);
  washing_basins[i] = 0;
  pthread_mutex_unlock(&mutex_3);
  printf("Customer %zd left washing basins...\n", customer->customer_number);
  sem_post(&washing_hands);
}

/* STUDENT TODO:
 *  make sure possible shared resources are locked correctly
 */
Toilet* goIntoCabin(Customer* customer)
{
  pthread_mutex_lock(&mutex_1);
  pthread_cond_wait(&conOpen, &mutex_1);
  pthread_mutex_unlock(&mutex_1);

  sem_wait(&go_to_toilet);

  Toilet* result = 0;
  printf("Customer %zd goes into cabin...\n", customer->customer_number);
  pthread_mutex_lock(&mutex_4);
  vector_iterator it = vector_begin(&free_toilets);
  for (; it != vector_end(&free_toilets); ++it)
  {
    Toilet* tmp = *it;
    if (!tmp->is_clogged)
    {
      result = *it;
      vector_erase(&free_toilets, it);
      break;
    }
  }
  pthread_mutex_unlock(&mutex_4);

  if (!result)
  {
    printf("Urgh, customer %zd did not find a free and clean toilet!\n", customer->customer_number);
  }

  return result;
}

void doBusiness()
{
  customWait(9);
}

/* STUDENT TODO:
 *  make sure possible shared resources are locked correctly
 */
void leaveCabin(Customer* customer, Toilet* toilet)
{
  pthread_mutex_lock(&mutex_4);
  toilet->is_clogged = isToiletClogged();
  vector_push_back(&free_toilets, toilet);
  pthread_mutex_unlock(&mutex_4);

  printf("Customer %zd leaves toilet cabin...\n", customer->customer_number);
  if (!toilet->is_clogged)
  {
    printf("Customer %zd has manners :-)\n", customer->customer_number);
    sem_post(&go_to_toilet);
  }
  else
  {
    printf("Customer %zd is a pig :-(\n", customer->customer_number);
  }
}

void* useToilet(void* customer)
{
  Customer* current_customer = (Customer*)customer;
  Toilet* toilet = goIntoCabin(current_customer);
  doBusiness();
  leaveCabin(current_customer, toilet);
  useWashingBasin(current_customer);

  return NULL;
}

void cleanToilet(Toilet* toilet, ssize_t isLazy)
{
  customWait(isLazy ? 14 : 7);
  toilet->is_clogged = 0;
}

/* STUDENT TODO:
 *  make sure possible shared resources are locked correctly
 */
void* doCleaning(void* cleaningStaff)
{
  pthread_mutex_lock(&mutex_2);
  pthread_cond_wait(&conOpen2, &mutex_2);
  pthread_mutex_unlock(&mutex_2);

  CleaningStaff* current_staff = (CleaningStaff*)cleaningStaff;
  ssize_t id = (ssize_t)current_staff->employee_id;

  while (running)
  {
    printf("Cleaning staff %zd is looking for a toilet to be cleaned...\n", id);

    Toilet* toilet = 0;
    pthread_mutex_lock(&mutex_4);
    vector_iterator it = vector_begin(&free_toilets);
    for (; it != vector_end(&free_toilets); ++it)
    {
      Toilet* tmp = *it;
      if (tmp->is_clogged)
      {
        toilet = *it;
        vector_erase(&free_toilets, it);
        break;
      }
    }
    pthread_mutex_unlock(&mutex_4);

    if (!toilet)
    {
      printf("Cleaning staff %zd has nothing to do... taking a short nap! *zZzZzZzZz*\n", id);
      nanosleep((const struct timespec[]){{0, 1000L * 1000L * 50L}}, NULL);
      continue;
    }

    printf("Cleaning staff %zd is cleaning toilet %zd...\n", id, toilet->door_number);
    cleanToilet(toilet, current_staff->is_lazy);

    printf("Cleaning staff %zd has cleaned toilet %zd\n", id, toilet->door_number);

    pthread_mutex_lock(&mutex_4);
    vector_push_back(&free_toilets, toilet);
    pthread_mutex_unlock(&mutex_4);

    sem_post(&go_to_toilet);
  }
  return NULL;
}

int main(int argc, char* argv[])
{
  /*
   * ONLY TOUCH THIS METHOD BETWEEN THE TODO COMMENTS!
   */
  srand(time(NULL));
  if (argc != 4)
  {
    fprintf(stderr, "Usage: %s <num_cleaning_staff> <num_customers> <num_toilets>\n", argv[0]);
    exit(-1);
  }

  ssize_t num_cleaning_staff = atoi(argv[1]);
  checkAllowedRange(num_cleaning_staff, NUM_CLEANING_STAFF);

  ssize_t num_customers = atoi(argv[2]);
  checkAllowedRange(num_customers, NUM_CUSTOMERS);

  ssize_t num_toilets = atoi(argv[3]);
  checkAllowedRange(num_toilets, NUM_TOILETS);

  CleaningStaff* staff = malloc(num_cleaning_staff * sizeof(CleaningStaff));
  Customer* customers = malloc(num_customers * sizeof(Customer));

  if (!staff || !customers)
  {
    free(staff);
    free(customers);
    fprintf(stderr, "Could not allocate memory!\n");
    exit(-1);
  }

  printf("Public toilet is opening!\n");

  /*
   * STUDENT TODO BEGIN:
   *  Initialize the mutexes, semaphores and condition variables
   */
   pthread_mutex_init(&mutex_open, NULL);
   pthread_mutex_init(&mutex_1, NULL);
   pthread_mutex_init(&mutex_2, NULL);
   pthread_mutex_init(&mutex_3, NULL);
   pthread_mutex_init(&mutex_4, NULL);
   pthread_mutex_init(&mutex_5, NULL);
   pthread_cond_init(&conOpen, NULL);
   pthread_cond_init(&conOpen2, NULL);
   sem_init(&washing_hands, 0, NUM_WASHING_BASINS);
   sem_init(&go_to_toilet,0, num_toilets);
  /*
   * STUDENT TODO END
   */

  vector_init(&free_toilets);
  public_bathroom_state = CLOSED;

  for (ssize_t i = 0; i < num_toilets; i++)
  {
    Toilet* toilet = malloc(sizeof(Toilet));
    toilet->door_number = i;
    toilet->is_clogged = 0;
    vector_push_back(&free_toilets, toilet);
  }

  for (ssize_t i = 0; i < num_cleaning_staff; i++)
  {
    staff[i].employee_id = i;
    staff[i].is_lazy = rand() % 2;
  }

  for (ssize_t i = 0; i < num_customers; i++)
  {
    customers[i].customer_number = i;
  }

  for (ssize_t i = 0; i < num_cleaning_staff; i++)
  {
    pthread_create(&staff[i].employee, NULL, doCleaning, (void*)&staff[i]);
  }

  for (ssize_t i = 0; i < num_customers; i++)
  {
    pthread_create(&customers[i].customer, NULL, useToilet, (void*)&customers[i]);
  }

  pthread_t janitor_tid;
  pthread_create(&janitor_tid, NULL, doJanitor, NULL);

  for (ssize_t i = 0; i < num_customers; i++)
  {
    pthread_join(customers[i].customer, NULL);
  }
  printf("No more customers!\n");

  printf("Waiting for the Janitor to finish!\n");
  pthread_join(janitor_tid, NULL);

  running = 0;
  for (ssize_t i = 0; i < num_cleaning_staff; i++)
  {
    pthread_join(staff[i].employee, NULL);
  }

  printf("Public toilet is closed!\n");

  /*
   * STUDENT TODO BEGIN:
   *  Deinitialize the mutexes, semaphores and condition variables
   */
   pthread_mutex_destroy(&mutex_open);
   pthread_mutex_destroy(&mutex_1);
   pthread_mutex_destroy(&mutex_2);
   pthread_mutex_destroy(&mutex_3);
   pthread_mutex_destroy(&mutex_4);
   pthread_mutex_destroy(&mutex_5);
   pthread_cond_destroy(&conOpen);
   pthread_cond_destroy(&conOpen2);
  /*
   * STUDENT TODO END
   */

  free(staff);
  free(customers);

  vector_iterator it = vector_begin(&free_toilets);
  while (it != vector_end(&free_toilets))
  {
    free(*it);
    vector_erase(&free_toilets, it);
  }
  vector_destroy(&free_toilets);

  return 0;
}

