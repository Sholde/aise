#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
  //
  if (argc != 2)
    return 1;

  //
  int fd = open(argv[1], O_CREAT | O_APPEND | O_WRONLY, 0600);

  //
  if (fd < 1)
    {
      perror("open");
      return 1;
    }

  //
  dup2(fd, STDOUT_FILENO);
  close(fd);

  //
  for (int i = 0; i < 4096; i++)
    {
      printf("%d\n", i);
    }
  
  //
  return 0;
}
