#define _GNU_SOURCE
#include <dlfcn.h>

#include <stdio.h>
#include <stdlib.h>

#include "alloc_debugger.h"

unsigned long long nb_alloc = 0;
unsigned long long size_alloc = 0;
unsigned long long nb_free = 0;

void *(*malloc_func)(size_t) = NULL;
void (*free_func)(void *) = NULL;

void *malloc(size_t size)
{
  nb_alloc++;
  size_alloc += size;

  if (!malloc_func)
    {
      malloc_func = (void *(*)(size_t))dlsym(RTLD_NEXT, "malloc");

      if (!malloc_func)
        {
          abort();
        }
    }
  
  return malloc_func(size);
}

void free(void *ptr)
{
  nb_free++;
  unsigned long long size_alloc = 0;

  if (!free_func)
    {
      free_func = (void (*)(void *))dlsym(RTLD_NEXT, "free");

      if (!free_func)
        {
          abort();
        }
    }

    free_func(ptr);
}

__attribute((destructor)) void finalize(void)
{
  printf("Allocate byte : %8.lld (%8.lld call)\n", size_alloc, nb_alloc);
  printf("Free byte     : %8s (%8.lld call)\n", "NULL", nb_free);
}
