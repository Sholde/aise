// printf
#include <stdio.h>

// wait
#include <sys/wait.h>

// signal
#include <signal.h>

// fork
#include <unistd.h>

// rand
#include <stdlib.h>

pid_t child;
int count_cont = 0;
int count_pipe = 0;

void sig_handler(int sig)
{
  switch(sig)
    {
    case SIGCONT:
      count_cont++;
      break;
    case SIGPIPE:
      count_pipe++;
      break;
    default:
      printf("error\n");
      break;
    }
}

void sig_print(int sig)
{
  printf("%d SIGCONT, %d SIGPIPE\n", count_cont, count_pipe);
  exit(0);
}

void sig_stop(int sig)
{
  printf("Parent receive SIGINT\n");
  kill(child, SIGINT);
  printf("Parent stop child\n");
  exit(0);
}

int main(void)
{
  child = fork();

  if (child == 0)
    {
      signal(SIGCONT, sig_handler);
      signal(SIGPIPE, sig_handler);
      signal(SIGINT, sig_print);

      // Infinite loop
      while (1) {}
    }
  else
    {
      signal(SIGINT, sig_stop);
      for (int i = 0; i < 30; i++)
        {
          if (rand()%2)
            kill(child, SIGCONT);
          else
            kill(child, SIGPIPE);
          sleep(1);
        }
      kill(child, SIGINT);
      wait(NULL);
    }
  
  return 0;
}
