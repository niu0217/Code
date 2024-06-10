/* ************************************************************************
> File Name:     udp_server.cpp
> Author:        niu0217
> Created Time:  Mon 10 Jun 2024 03:35:10 PM CST
> Description:
 ************************************************************************/

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main()
{
  int listenFd;
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;

  int port = 8888;
  char buffer[1024] = {0};

  // Creating socket file descriptor
  if ((listenFd = socket(AF_INET, SOCK_DGRAM, 0)) == 0)
  {
    perror("Socket creation failed");
    return 1;
  }

  servaddr.sin_family = AF_INET; // IPv4
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(port);

  // Bind the socket with the server address
  if (bind(listenFd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
  {
    perror("Bind failed");
    return 1;
  }

  while (1)
  {
    int len, n;
    len = sizeof(cliaddr); // len is value/resuslt

    // Receive the message from client
    n = recvfrom(listenFd, buffer, sizeof(buffer), 0, (struct sockaddr *)&cliaddr, (socklen_t *)&len);
    buffer[n] = '\0';

    std::cout << "Message from client: " << buffer << std::endl;
  }

  return 0;
}