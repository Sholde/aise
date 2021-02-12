#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

int main(void)
{

  pid_t child = fork();

  if (child == 0)
    {
      printf("Child\n");
    }
  else
    {
      printf("Parent\n");
      wait(NULL);
    }
  
  return 0;
}
