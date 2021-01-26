#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
  if (argc != 1)
    return 1;
  
  // Init pipe
  int pipefd[2];
  pipe(pipefd);

  // fork
  pid_t child = fork();

  if (child == 0)
    {
      // Replace stdout by pipe output
      dup2(pipefd[1], STDOUT_FILENO);
      close(pipefd[0]);
      close(pipefd[1]);

      //
      char *echo[] = {"echo", "8+4", NULL};
      execvp(echo[0], echo);
    }
  else
    {
      pid_t child_2 = fork();

      if (child_2 == 0)
        {
          //
          dup2(pipefd[0], STDIN_FILENO);
          close(pipefd[0]);
          close(pipefd[1]);

          //
          char *bc[] = {"bc", NULL};
          execvp(bc[0], bc);
        }
      else
        {
          //
          close(pipefd[0]);
          close(pipefd[1]);
          
          // Waiting child
          wait(NULL);
          wait(NULL);
        }
    }

  //
  return 0;
}
