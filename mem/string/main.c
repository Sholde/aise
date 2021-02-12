#include "string.h"

int main(int argc, char **argv)
{
  string *s = str_create("Hello wrold!\n");

  if (!s)
    return 1;

  str_print(s);
  
  str_destroy(s);
  
  return 0;
}
