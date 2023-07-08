/*
 * DO NOT TOUCH THIS FILE!
 */
#ifndef A2_DEFINITIONS_H
#define A2_DEFINITIONS_H
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <string.h>

#define UPPER_LIMIT_CLEANING_STUFF 100
#define UPPER_LIMIT_CUSTOMERS 2000
#define UPPER_LIMIT_TOILETS 1000

#define NUM_CLEANING_STAFF "num_cleaning_staff"
#define NUM_CUSTOMERS "num_customers"
#define NUM_TOILETS "num_toilets"

#define NUM_WASHING_BASINS 2

void customWait(ssize_t factor) {
  ssize_t microsec_to_sleep = 1000 + rand() % (factor * 1000);
  nanosleep((const struct timespec[]){{0, 1000L * microsec_to_sleep}}, NULL);
}

void checkAllowedRange(ssize_t value, char* name) {
  ssize_t upper_limit;
  ssize_t lower_limit;
  if (strncmp(name, NUM_CLEANING_STAFF, strlen(NUM_CLEANING_STAFF)) == 0) {
    upper_limit = UPPER_LIMIT_CLEANING_STUFF;
    lower_limit = 0;
  } else if (strncmp(name, NUM_CUSTOMERS, strlen(NUM_CUSTOMERS)) == 0) {
    upper_limit = UPPER_LIMIT_CUSTOMERS;
    lower_limit = 1;
  } else if (strncmp(name, NUM_TOILETS, strlen(NUM_TOILETS)) == 0) {
    upper_limit = UPPER_LIMIT_TOILETS;
    lower_limit = 1;
  } else {
    exit(-1);
  }

  if (value < lower_limit || value > upper_limit) {
    fprintf(stderr, "%s allowed range: [%zd,%zd].\n", name, lower_limit, upper_limit);
    exit(-1);
  }
}

void openPublicBathroom() {
  ssize_t microsec_to_sleep = 1000 + rand() % (10 * 1000);
  nanosleep((const struct timespec[]){{0, 1000L * microsec_to_sleep}}, NULL);
}

void coffeeBreak() {
  ssize_t microsec_to_sleep = 1000 + rand() % (50 * 1000);
  nanosleep((const struct timespec[]){{0, 1000L * microsec_to_sleep}}, NULL);
}

ssize_t isToiletClogged() {
  return rand() % 2;
}

#endif //A2_DEFINITIONS_H
