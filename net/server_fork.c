#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
  // Check argument
  if (argc != 2)
    return printf("Need a port on argument!\n"), 1;

  // Prepare structure for getaddrinfo
  struct addrinfo *res = NULL;
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  // Resolve domain name
  int ret = getaddrinfo(NULL, argv[1], &hints, &res);

  // Check error
  if (ret < 0)
    {
      herror("getaddrinfo");
      return 1;
    }

  // Prepare socket creation
  struct addrinfo *tmp = NULL;
  int listen_sock = -1;
  int binded = 0;

  // Test all soket of the list
  for (tmp = res; tmp !=NULL; tmp = tmp->ai_next)
    {
      listen_sock = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);

      if (listen_sock < 0)
        {
          perror("socket");
          continue;
        }

      int ret = bind(listen_sock, tmp->ai_addr, tmp->ai_addrlen);
      
      if (ret < 0)
        {
          perror("bind");
          continue;
        }

      binded = 1;
      break;
    }

  // Check if it is bind
  if (!binded)
    {
      fprintf(stderr, "Failed to bind on 0.0.0.0:%s\n", argv[1]);
      return 1;
    }

  // Listen
  ret = listen(listen_sock, 2);

  if (ret < 0)
    {
      perror("listen");
      return 1;
    }

  struct sockaddr client;
  socklen_t addrlen;  

  // Server
  while (1)
    {
      int client_socket = accept(listen_sock, &client, &addrlen);

      if (client_socket < 0)
        {
          perror("accept");
          return 1;
        }

      pid_t client_server = fork();

      if (client_server == 0)
        {
          printf("New connection\n");
          write(client_socket, "Hello from server!\n", 20);
          close(client_socket);
        }
      else
        {
          close(client_socket);
        }
    }

  close(listen_sock);
  
  return 0;
}
