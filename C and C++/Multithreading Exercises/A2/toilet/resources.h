/*
 * DO NOT TOUCH THIS FILE!
 */
#ifndef A2_RESOURCES_H
#define A2_RESOURCES_H
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
  ssize_t door_number;
  ssize_t is_clogged;
} Toilet;

typedef struct {
  pthread_t customer;
  ssize_t customer_number;
} Customer;

typedef struct {
  pthread_t employee;
  ssize_t employee_id;
  ssize_t is_lazy;
} CleaningStaff;

typedef enum {
  CLOSED,
  OPENED,
  OPENED_AND_CLEANING_STAFF_NEEDED
} State;

#endif //A2_RESOURCES_H
