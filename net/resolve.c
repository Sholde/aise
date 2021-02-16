#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
  if (argc != 2)
    return 1;
  
  struct hostent *host = gethostbyname(argv[1]);

  if (!host)
    {
      herror("gethostbyname");
      return 1;
    }
  
  printf("site: %s\n", host->h_name);

  char net_ty[3];
  net_ty[2] = '\0';

  if (host->h_addrtype == AF_INET)
    {
      net_ty[0] = 'v';
      net_ty[1] = '4';
    }
  else if (host->h_addrtype == AF_INET6)
    {
      net_ty[0] = 'v';
      net_ty[1] = '6';
    }
  else
    {
      return 2;
    }

  int i = 0;
  
  while (host->h_addr_list[i] != NULL)
    {
      printf("ip%s: %s\n", net_ty, inet_ntoa(*(struct in_addr *)(host->h_addr_list[i])));
      i++;
    }

  return 0;
}
