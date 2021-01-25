#include <stdio.h>
#include <signal.h>

int main(void)
{
  sigset_t blk;

  sigfillset(&blk);
  sigprocmask(SIG_BLOCK, &blk, NULL);

  printf("Kill me please\n");

  while (1)
    {

    }
  
  return 0;
}
