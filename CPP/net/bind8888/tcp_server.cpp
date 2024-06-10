/* ************************************************************************
> File Name:     tcp_server.cpp
> Author:        niu0217
> Created Time:  Mon 10 Jun 2024 03:34:59 PM CST
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
  int connFd;
  struct sockaddr_in address;
  int addrlen = sizeof(address);
  int opt = 1;
  int port = 8888;

  // Creating socket file descriptor
  if ((listenFd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    perror("Socket creation failed");
    return 1;
  }

  // Forcefully attaching socket to the port 8888
  if (setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
  {
    perror("Setsockopt failed");
    return 1;
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
  address.sin_port = htons(port);

  // Forcefully attaching socket to the port 8888
  if (bind(listenFd, (struct sockaddr *)&address, sizeof(address)) < 0)
  {
    perror("Bind failed");
    return 1;
  }

  if (listen(listenFd, 3) < 0)
  {
    perror("Listen failed");
    return 1;
  }

  while (1)
  {
    if ((connFd = accept(listenFd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
      perror("Accept failed");
      return 1;
    }

    // Send message to the client
    const char *hello = "Hello from server";
    send(connFd, hello, strlen(hello), 0);
    std::cout << "Message sent to the client" << std::endl;

    // Close the socket and end the program
    close(connFd);
  }

  close(listenFd);
  return 0;
}