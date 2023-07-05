#include "stdlib.h"

Entry* top = NULL;
Entry* bottom = NULL;
pthread_spinlock_t mallock_;

void *malloc(size_t size)
{
  Entry *entry;
  if (size == NULL)
    return NULL;

  if(top == NULL)
  {
    pthread_spin_init(&mallock_, 0);
    pthread_spin_lock(&mallock_);

    entry = sbrk(size + sizeof(Entry));
    if(entry == (void *) -1)
      goto leave;

    top = entry;
    entry->next = NULL;
    entry->size = size;
    entry->prev = NULL;
    entry->is_freed = 0;
    bottom = entry;

    pthread_spin_unlock(&mallock_);
    return (entry + 1);
  }
  else
  {
    pthread_spin_lock(&mallock_);
    entry = returnFreeEntry(size);
  }

  if (entry == NULL)
  {
    for (; bottom->next; bottom = (Entry *) bottom->next)
    {
      if (bottom->next == NULL)
        break;
    }

    entry = sbrk(size + sizeof(Entry));
    if (entry == (void *) -1)
      goto leave;

    bottom->next = (struct Entry*) entry;
    entry->prev = (struct Entry*) bottom;
    entry->size = size;
    bottom = entry;
    entry->next = NULL;
  }
  entry->is_freed = 0;
  pthread_spin_unlock(&mallock_);
  return (entry + 1);

leave:
  pthread_spin_unlock(&mallock_);
  return NULL;
}

void free(void *ptr)
{
  Entry* start;
  pthread_spin_lock(&mallock_);
  if(ptr == NULL)
   goto leave;

  for(start = top; start != NULL; start = (Entry*)start->next)
  {
    if(start == (Entry*)(ptr-1))
      break;
  }

  if(start == 0)
    goto leave;
  else
  {
    Entry* freeing_entry = (Entry*)(ptr - 1);
    if(freeing_entry->is_freed)
      goto leave;
    else
    {
      freeing_entry->is_freed = 1;
      if(freeing_entry->next == NULL)
      {
        if(freeing_entry != top)
          bottom = (Entry*) freeing_entry->prev;

        bottom->next = NULL;
        brk(freeing_entry);
        if(freeing_entry == top)
          top = NULL;
      }
    }
  }
leave:
  pthread_spin_unlock(&mallock_);
}

Entry* returnFreeEntry(size_t needed_size)
{
  for(Entry* entry = top; entry != NULL; entry = (Entry*)entry->next)
  {
    if(entry->is_freed && entry->size >= needed_size)
      return entry;
  }
  return NULL;
}

void *calloc(size_t nmemb, size_t size)
{
  return 0;
}

void *realloc(void *ptr, size_t size)
{
  return 0;
}

int atexit(void (*function)(void))
{
  return -1;
}