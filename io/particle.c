#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

#define NUM_PART 64

typedef struct particle_s
{
  int x[NUM_PART], y[NUM_PART];
} particle_t;

void init_particle(particle_t *parts)
{
  for (int i = 0; i < NUM_PART; i++)
    {
      parts->x[i] = i;
      parts->y[i] = i + 1;
    }
}

int main(int argc, char **argv)
{
  // Check arguments
  if (argc != 2)
    {
      fprintf(stderr, "Usage: %s [FILE NAME]\n", argv[0]);
      return 1;
    }

  //
  particle_t parts;
  
  // Fork
  pid_t child = fork();

  //
  if (child == 0)
    {
      //
      init_particle(&parts);
      
      //
      int fd = open(argv[1], O_CREAT | O_WRONLY, 0600);

      //
      if (fd < 0)
        {
          perror("fd");
          return 1;
        }

      //
      size_t to_write = sizeof(particle_t);
      size_t written = 0;

      // Write
      while (to_write != written)
        {
          int ret = write(fd, &parts + written, to_write + written);

          if (ret < 0)
            {
              if (errno == EINTR)
                {
                  continue;
                }
              
              perror("write");
              return 1;
            }

          written += ret;
        }

      //
      close(fd);
    }
  else
    {
      // Waiting child write on file
      wait(NULL);
      
      // Open file on READ ONLY
      int fd = open(argv[1], O_CREAT | O_RDONLY, 0600);

      //
      if (fd < 0)
        {
          perror("fd");
          return 1;
        }

      //
      size_t to_read = sizeof(particle_t);
      size_t is_read = 0;

      // Read
      while (to_read != is_read)
        {
          int ret = read(fd, &parts + is_read, to_read + is_read);

          if (ret < 0)
            {
              if (errno == EINTR)
                {
                  continue;
                }
              
              perror("read");
              return 1;
            }

          is_read += ret;
        }

      for (int i = 0; i < NUM_PART; i++)
        {
          if (parts.x[i] != i)
            printf("ERROR on x, %d != %d\n", parts.x[i], i);
          if (parts.y[i] != i + 1)
            printf("ERROR on y, %d != %d\n", parts.y[i], i + 1);
        }

      //
      close(fd);
    }

  //
  return 0;
}
