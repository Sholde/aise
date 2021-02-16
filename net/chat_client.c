#include <stdio.h>
#include <string.h>

// getaddrinfo
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char **argv)
{
  if (argc != 3)
    return 1;
  
  struct addrinfo *res = NULL;

  struct addrinfo hints;
  memcpy(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  int ret = getaddrinfo(argv[1], argv[2], &hints, &res);

  if (ret)
    {
      perror("getaddrinfo");
      return 1;
    }
  
  return 0;
}
