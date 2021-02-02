#include <stdio.h>
#include <string.h>

#include "custom.h"

unsigned long long count = 0;

void custom_puts(char *str)
{
  puts(str);
  count += strlen(str);
}

void custom_nbchars(void)
{
  printf("nb chars: %lld\n", count);
}
