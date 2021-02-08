#define _GNU_SOURCE
#include <dlfcn.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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
              //
              table.ptr[i] = ptr;
              table.size[i] = size;
              table.count++;

              //
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
              //
              size_free += table.size[i];

              //
              table.ptr[i] = NULL;
              table.size[i] = 0;
              table.count--;

              //
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

/* reverse:  reverse string s in place */
static void reverse(char s[])
{
  int i, j;
  char c;
 
  for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

/* itoa:  convert n to characters in s */
static void itoa(int n, char s[])
{
  int i, sign;
 
  if ((sign = n) < 0)  /* record sign */
    n = -n;          /* make n positive */
  i = 0;
  do {       /* generate digits in reverse order */
    s[i++] = n % 10 + '0';   /* get next digit */
  } while ((n /= 10) > 0);     /* delete it */
  if (sign < 0)
    s[i++] = '-';
  s[i] = '\0';
  reverse(s);
}

static void alloc_print(const char *name, unsigned long long a, unsigned long long b)
{
  char buff[9] = {'\0'};

  // Name
  write(1, name, sizeof(char) * strlen(name));

  // Size
  itoa(a, buff);
  write(1, buff, sizeof(buff));
  write(1, " Bytes (", 8);

  // Call
  if (b != 0)
    {
      itoa(b, buff);
      write(1, buff, sizeof(buff));
      write(1, " call)\n", 7);
    }
  else
    {
      write(1, "NOT call)\n", 10);
    }
}

__attribute((destructor)) void finalize(void)
{
  // Display general information
  alloc_print("Allocate : ", size_alloc, nb_alloc);
  alloc_print("Free     : ", size_free, nb_free);

  //
  int first = 1;

  // Display non free memory
  if (table.count)
    {
      for (int i = 0; i < MAX_ALLOC; i++)
        {
          if (table.ptr[i])
            {
              if (first)
                {
                  puts("WARNING: NON FREE MEMORY\n");
                  first = 0;
                }

              // Display
              alloc_print("Non Free : ", table.size[i], 0);

              // Update information
              table.ptr[i] = NULL;
              table.size[i] = 0;
              table.count--;
            }
        }
    }
}
