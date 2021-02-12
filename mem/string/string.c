#include <stdlib.h>
#include <unistd.h>

#include "string.h"

string *str_create(const char *str)
{
  string *s = aligned_alloc(64, sizeof(string));

  if (!s)
    return NULL;

  int i = 0;
  for (i = 0; i < STRING_MAX_SIZE && str[i] != '\0'; i++)
    {
      s->str[i] = str[i];
    }
  s->str[STRING_MAX_SIZE - 1] = '\0';
  s->size = i;

  return s;
}

void str_print(string *s)
{
  if (!s)
    return;

  write(1, s->str, s->size);
}

void str_copy(string *s1, string *s2)
{
  if (!s1 || !s2)
    return;

  // Copy size
  s2->size = s1->size;

  // Copy string
  for (int i = 0; i < s2->size; i++)
    {
      s2->str[i] = s1->str[i];
    }
}

void str_destroy(string *s)
{
  if (s)
    free(s);
}
