#include <stdio.h>
#include <unistd.h>

// memcpy
#include <string.h>

// atoi
#include <stdlib.h>

// gethostbyname
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// socket
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char **argv)
{
  // Check argument
  if (argc != 3)
    return 1;

  // Resolve Domain Name
  struct hostent *host = gethostbyname(argv[1]);

  if (!host)
    {
      herror("gethostbyname");
      return 1;
    }

  // Create Socket
  int sock = socket(AF_INET, SOCK_STREAM, 0);

  if (!sock)
    {
      perror("socket");
      return 2;
    }

  // Connect to the socket
  struct sockaddr_in server;

  memset(&server, 0, sizeof(struct sockaddr_in));

  server.sin_family = host->h_addrtype;
  server.sin_port = htons(atoi(argv[2]));

  memcpy(&server.sin_addr, host->h_addr_list[0], host->h_length);

  int ret = connect(sock, (struct sockaddr*)&server, sizeof(struct sockaddr_in));

  if( ret < 0)
    {
      perror("connect");
      return 0;
    }

  // Send to the socket the content of the buffer buff
  char buff[128];

  snprintf(buff, 128, "I'm here!\n");

  write(sock, buff, strlen(buff));

  // Read on the socket and print on standart output
  while((ret = read(sock, buff, 128)) != 0)
    {
      write(0, buff, ret);
    }

  // Clean
  shutdown(sock, SHUT_RDWR);

  close(sock);

  return 0;
}
