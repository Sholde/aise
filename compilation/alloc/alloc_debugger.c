#define _GNU_SOURCE
#include <dlfcn.h>

#include <stdio.h>
#include <stdlib.h>

#include "alloc_debugger.h"

unsigned long long nb_alloc = 0;
unsigned long long size_alloc = 0;

unsigned long long nb_free = 0;
unsigned long long size_free = 0;

void *(*malloc_func)(size_t) = NULL;
void (*free_func)(void *) = NULL;

alloc_table_t table;

void *malloc(size_t size)
{
  // Allocation variable incrementation
  nb_alloc++;
  size_alloc += size;

  // First call
  if (!malloc_func)
    {
      malloc_func = (void *(*)(size_t))dlsym(RTLD_NEXT, "malloc");

      if (!malloc_func)
        {
          abort();
        }
    }

  // Call true malloc
  void *ptr = malloc_func(size);
  if (!ptr)
    return NULL;
  
  // Stock allocation information
  if (table.count < MAX_ALLOC)
    {
      for (int i = 0; i < MAX_ALLOC; i++)
        {
          if (table.ptr[i] == NULL)
            {
              table.count++;
              table.ptr[i] = ptr;
              table.size[i] = size;
              break;
            }
        }
    }

  // Return pointer
  return ptr;
}

void free(void *ptr)
{
  // Free variable incremantation
  nb_free++;

  // First call
  if (!free_func)
    {
      free_func = (void (*)(void *))dlsym(RTLD_NEXT, "free");

      if (!free_func)
        {
          abort();
        }
    }

  // Stock free information
  if (table.count)
    {
      for (int i = 0; i < MAX_ALLOC; i++)
        {
          if (table.ptr[i] == ptr)
            {
              table.count--;
              size_free += table.size[i];
              table.ptr[i] = NULL;
              table.size[i] = 0;
              break;
            }
        }
    }

  // Call true free
  free_func(ptr);
}

__attribute((constructor)) void init(void)
{
  table.count = 0;
  for (int i = 0; i < MAX_ALLOC; i++)
    {
      table.ptr[i] = NULL;
      table.size[i] = 0;
    }
}

__attribute((destructor)) void finalize(void)
{
  // Display general information
  printf("Allocate : %8.lld Bytes (%8.lld call)\n", size_alloc, nb_alloc);
  printf("Free     : %8.lld Bytes (%8.lld call)\n", size_free, nb_free);

  // Display non free memory
  if (table.count)
    {
      printf("WARNING: NON FREE MEMORY\n");
      for (int i = 0; i < MAX_ALLOC; i++)
        {
          if (table.ptr[i])
            {
              printf("Non Free : %8.lld Bytes (NOT call)\n", table.size[i]);
              table.ptr[i] = NULL;
              table.size[i] = 0;
              table.count--;
            }
        }
    }
}
