#include <stdio.h>
#include <unistd.h>
int main(void)
{
  if (setuid(0))
    {
      perror("setuid");
      return 1;
    }
  
  printf("uid %d\n", getuid());

  return 0;
}
