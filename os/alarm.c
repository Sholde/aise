#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

struct pid_alarm_t
{
  pid_t child;
  int count;
};

struct pid_alarm_t pa = {.child = 0, .count = 0};

//
void signal_alarm(int p)
{
  printf("Send SIGUSR1 to child\n");
  kill(pa.child, 10);
  alarm(3);
}

//
void signal_child(int p)
{
  printf("Child receive SIGUSR1 at %d\n", ++(pa.count));
}

//
void signal_exit(int p)
{
  printf("Parent receive SIGINT\n");
  kill(pa.child, 2);
  exit(1);
}

//
int main(void)
{
  //
  printf("Send message all 3 seconds\n");
  
  //
  pa.child = fork();

  //
  if (pa.child == 0)
    {
      signal(SIGUSR1, signal_child);
      while (1)
        {

        }
    }
  else
    {
      signal(SIGALRM, signal_alarm);
      signal(SIGINT, signal_exit);
      alarm(3);
      waitpid(pa.child, NULL, 0);
    }
  
  //
  return 0;
}
